/****************************************************************************
 *
 * This is a part of TOTEM offline software.
 * Author:
 *   Laurent Forthomme
 *
 ****************************************************************************/

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"

#include "DataFormats/CTPPSDetId/interface/TotemT2DetId.h"
#include "DataFormats/TotemReco/interface/TotemT2Digi.h"
#include "DataFormats/TotemReco/interface/TotemT2RecHit.h"

#include "Geometry/Records/interface/TotemGeometryRcd.h"
#include "DQM/CTPPS/interface/TotemT2Segmentation.h"

#include <string>

class TotemT2DQMSource : public DQMEDAnalyzer {
public:
  TotemT2DQMSource(const edm::ParameterSet&);
  ~TotemT2DQMSource() override;

protected:
  void dqmBeginRun(const edm::Run&, const edm::EventSetup&) override;
  void bookHistograms(DQMStore::IBooker&, const edm::Run&, const edm::EventSetup&) override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;

private:
  edm::ESGetToken<TotemGeometry, TotemGeometryRcd> geometryToken_;
  edm::EDGetTokenT<edm::DetSetVector<TotemT2Digi>> digiToken_;
  edm::EDGetTokenT<edmNew::DetSetVector<TotemT2RecHit>> rechitToken_;

  std::unique_ptr<TotemT2Segmentation> segm_;
  MonitorElement* m_digis_mult_[2][TotemT2DetId::maxPlane + 1];
  MonitorElement* m_rechits_mult_[2][TotemT2DetId::maxPlane + 1];

  std::unordered_map<unsigned int, MonitorElement*> numberOfActivePlanes_;
  std::unordered_map<unsigned int, MonitorElement*> activePlanes_;
  void fillActivePlanes(std::unordered_map<unsigned int, std::set<unsigned int>>&, const TotemT2DetId);
  std::string changePathToParentDir(std::string);
};

TotemT2DQMSource::TotemT2DQMSource(const edm::ParameterSet& iConfig)
    : geometryToken_(esConsumes<TotemGeometry, TotemGeometryRcd, edm::Transition::BeginRun>()),
      digiToken_(consumes<edm::DetSetVector<TotemT2Digi>>(iConfig.getParameter<edm::InputTag>("digisTag"))),
      rechitToken_(consumes<edmNew::DetSetVector<TotemT2RecHit>>(iConfig.getParameter<edm::InputTag>("rechitsTag"))) {}

TotemT2DQMSource::~TotemT2DQMSource() {}

void TotemT2DQMSource::dqmBeginRun(const edm::Run&, const edm::EventSetup&) {}

void TotemT2DQMSource::bookHistograms(DQMStore::IBooker& ibooker, const edm::Run&, const edm::EventSetup& iSetup) {
  ibooker.cd();
  ibooker.setCurrentFolder("CTPPS/TotemT2");

  const size_t summary_nbinsx = 25, summary_nbinsy = 25;

  for (unsigned int arm = 0; arm <= CTPPSDetId::maxArm; ++arm){
    std::string title, path;
    for (unsigned int pl = 0; pl <= TotemT2DetId::maxPlane; ++pl) {
      const TotemT2DetId detid(arm, pl, 0);
      detid.planeName(title, TotemT2DetId::nFull);
      detid.planeName(path, TotemT2DetId::nPath);
      ibooker.setCurrentFolder(path);
      m_digis_mult_[arm][pl] = ibooker.book2DD("digis multiplicity",
                                               title + " digis multiplicity;x;y",
                                               summary_nbinsx,
                                               0.,
                                               summary_nbinsx,
                                               summary_nbinsy,
                                               0.,
                                               summary_nbinsy);
      m_rechits_mult_[arm][pl] = ibooker.book2DD("rechits multiplicity",
                                                 title + " rechits multiplicity;x;y",
                                                 summary_nbinsx,
                                                 0.,
                                                 summary_nbinsx,
                                                 summary_nbinsy,
                                                 0.,
                                                 summary_nbinsy);
    }
    ibooker.setCurrentFolder(changePathToParentDir(path));

    activePlanes_[arm] = ibooker.book1D("active planes (arm " + std::to_string(arm) + ")",
                                    "which planes are active;plane number",
                                    8,
                                    -0.5,
                                    7.5);
                                    
    numberOfActivePlanes_[arm] = ibooker.book1D("number of active planes (even planes)",
                                    "how many planes are active;number of active planes",
                                    6,
                                    -0.5,
                                    5.5);

    numberOfActivePlanes_[arm+2] = ibooker.book1D("number of active planes (odd planes)",
                                    "how many planes are active;number of active planes",
                                    6,
                                    -0.5,
                                    5.5);

  }
  // build a segmentation helper for the size of histograms previously booked
  segm_ = std::make_unique<TotemT2Segmentation>(iSetup.getData(geometryToken_), summary_nbinsx, summary_nbinsy);
}

void TotemT2DQMSource::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  // fill digis information
  for (const auto& ds_digis : iEvent.get(digiToken_)) {
    const TotemT2DetId detid(ds_digis.detId());
    for (const auto& digi : ds_digis) {
      segm_->fill(m_digis_mult_[detid.arm()][detid.plane()]->getTH2D(), detid);
      (void)digi;  //FIXME make use of them
    }
  }

  // fill rechits information
  std::unordered_map<unsigned int, std::set<unsigned int>> planes;
  for (const auto& ds_rechits : iEvent.get(rechitToken_)) {
    const TotemT2DetId detid(ds_rechits.detId());
    for (const auto& rechit : ds_rechits) {
      segm_->fill(m_rechits_mult_[detid.arm()][detid.plane()]->getTH2D(), detid);
      (void)rechit;  //FIXME make use of them

      fillActivePlanes(planes, detid);
    }
  }
  for (const auto& plt : numberOfActivePlanes_) {
    plt.second->Fill(planes[plt.first].size());
  }
}

void TotemT2DQMSource::fillActivePlanes(std::unordered_map<unsigned int, std::set<unsigned int>>& planes, const TotemT2DetId detid){
  unsigned short pl = detid.plane();    
  if(detid.arm() == 0){
    if(pl % 2 == 0){
      planes[0].insert(pl);
    }else{
      planes[2].insert(pl);
    }
  }else{
    if(pl % 2 == 0){
      planes[1].insert(pl);
    }else{
      planes[3].insert(pl);
    }
  }

  activePlanes_[detid.arm()]->Fill(pl);
}

std::string TotemT2DQMSource::changePathToParentDir(std::string path) {
  std::string toReplace = "/plane ";
  path = path.substr(0, path.find(toReplace));
  return path;
}


DEFINE_FWK_MODULE(TotemT2DQMSource);
