/****************************************************************************
 *
 * This is a part of PPS offline software.
 * Authors:
 *   Edoardo Bossini
 *   Piotr Maciej Cwiklicki
 *   Laurent Forthomme
 *
 ****************************************************************************/

#include "DQMServices/Core/interface/DQMGlobalEDAnalyzer.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Utilities/interface/EDMException.h"
#include "Geometry/VeryForwardGeometryBuilder/interface/CTPPSGeometry.h"
#include "Geometry/Records/interface/VeryForwardRealGeometryRecord.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondRecHit.h"

#include "CalibPPS/TimingCalibration/interface/TimingCalibrationStruct.h"

#include <TFile.h>
#include <TH2.h>
#include <cstdint>

using PlaneKey = std::tuple<uint8_t, uint8_t, uint8_t>;
std::unordered_map<PlaneKey, std::pair<unsigned int, double>> doublePeakLsAndOffset;

//------------------------------------------------------------------------------

class PPSTimingCalibrationPCLValidWorker : public DQMGlobalEDAnalyzer<TimingCalibrationHistograms> {
public:
  explicit PPSTimingCalibrationPCLValidWorker(const edm::ParameterSet&);

  void dqmBeginRun(const edm::Run&, const edm::EventSetup&, TimingCalibrationHistograms&) const override;
  void dqmAnalyze(const edm::Event&, const edm::EventSetup&, const TimingCalibrationHistograms&) const override;

  static void fillDescriptions(edm::ConfigurationDescriptions&);

private:

  void extractDoublePeakLsAndOffset(const PlaneKey& key, const TH2F* tVsLs) const;
  void bookHistograms(DQMStore::IBooker&,
                      const edm::Run&,
                      const edm::EventSetup&,
                      TimingCalibrationHistograms&) const override;

  template <typename T>
  bool searchForProduct(edm::Event const& iEvent,
                        const std::vector<edm::EDGetTokenT<T>>& tokens,
                        const std::vector<edm::InputTag>& tags,
                        edm::Handle<T>& handle) const;

  std::unordered_map<PlaneKey, std::pair<uint16_t, float>> doublePeakLsPerPlane;
  const std::vector<edm::InputTag> RecHitTags_;
  std::vector<edm::EDGetTokenT<edm::DetSetVector<CTPPSDiamondRecHit>>> diamondRecHitTokens_;
  const edm::ESGetToken<CTPPSGeometry, VeryForwardRealGeometryRecord> geomEsToken_;

  const std::string dqmDir_;
  const std::string tVsLsFileName_;
  static constexpr double double_peak_t_max_diff_ = 3.;
};

//------------------------------------------------------------------------------

PPSTimingCalibrationPCLValidWorker::PPSTimingCalibrationPCLValidWorker(const edm::ParameterSet& iConfig)
    : RecHitTags_(iConfig.getParameter<std::vector<edm::InputTag>>("diamondRecHitTags")),
      geomEsToken_(esConsumes<edm::Transition::BeginRun>()),
      dqmDir_(iConfig.getParameter<std::string>("dqmDir")),
      tVsLsFileName_(iConfig.getParameter<std::string>("tVsLsFileName")) {
  for (auto& tag : RecHitTags_)
    diamondRecHitTokens_.push_back(consumes<edm::DetSetVector<CTPPSDiamondRecHit>>(tag));
}

void PPSTimingCalibrationPCLValidWorker::dqmBeginRun(const edm::Run& iRun, const edm::EventSetup& iSetup, TimingCalibrationHistograms& iHists) const {
  std::cout << tVsLsFileName_ << '\n';
  auto* efficiencyFile = new TFile(tVsLsFileName_.data(), "READ");
  if (!efficiencyFile->IsOpen()) {
    throw edm::Exception(edm::errors::FileOpenError) << "Can't open tVsLs file: " << tVsLsFileName_;
  }
  const auto& geom = iSetup.getData(geomEsToken_);
    for (auto it = geom.beginSensor(); it != geom.endSensor(); ++it) {
    if (!CTPPSDiamondDetId::check(it->first))
      continue;
    const CTPPSDiamondDetId detid(it->first);
    PlaneKey key{detid.arm(), detid.station(), detid.plane()};
    std::string plane_name;
    detid.planeName(plane_name);
    std::string namecycle = "DQMData/Run " + std::to_string(iRun.run()) + "/AlCaReco/Run summary/PPSTimingCalibrationPCL/tvsls_" + plane_name;
    auto* tVsLs = (TH2F*) efficiencyFile->Get(namecycle.data());
    extractDoublePeakLsAndOffset(key, tVsLs);
  }
  efficiencyFile->Close();
  delete efficiencyFile;
  std::cout << "Success!\n";
}

//------------------------------------------------------------------------------

void PPSTimingCalibrationPCLValidWorker::bookHistograms(DQMStore::IBooker& iBooker,
                                                   const edm::Run& iRun,
                                                   const edm::EventSetup& iSetup,
                                                   TimingCalibrationHistograms& iHists) const {
  iBooker.cd();
  iBooker.setCurrentFolder(dqmDir_);
  std::string plane_name;
  std::string ch_name;

  const auto& geom = iSetup.getData(geomEsToken_);
  for (auto it = geom.beginSensor(); it != geom.endSensor(); ++it) {
    if (!CTPPSDiamondDetId::check(it->first))
      continue;
    const CTPPSDiamondDetId detid(it->first);

    detid.channelName(ch_name);
    iHists.leadingTime[detid.rawId()] = iBooker.book1D("t_" + ch_name, ch_name + ";t (ns);Entries", 1200, -60., 60.);
    iHists.toT[detid.rawId()] = iBooker.book1D("tot_" + ch_name, ch_name + ";ToT (ns);Entries", 160, -20., 20.);
    iHists.leadingTimeVsToT[detid.rawId()] =
        iBooker.book2D("tvstot_" + ch_name, ch_name + ";ToT (ns);t (ns)", 240, 0., 60., 450, -20., 25.);
    // if (iHists.leadingTimeVsLs.count({detid.arm(), detid.station(), detid.plane()}) == 0) {
    //   detid.planeName(plane_name);
    //   iHists.leadingTimeVsLs[{detid.arm(), detid.station(), detid.plane()}] = iBooker.book2D("tvsls_" + plane_name, plane_name + ";LS;t (ns)", 3000, 1, 3000, 500, 0, 20);
    // }
  }
}

//------------------------------------------------------------------------------

void PPSTimingCalibrationPCLValidWorker::extractDoublePeakLsAndOffset(const PlaneKey& key, const TH2F* tVsLs) const {
  unsigned int double_peak_ls = 1;
  unsigned int n_bins_x = tVsLs->GetNbinsX();
  unsigned int n_bins_y = tVsLs->GetNbinsX();
  double prev_t_with_max_count = -1.;
  double time_shift = 0.;
  for (unsigned int bin_x = 1; bin_x <= n_bins_x; ++bin_x) {
    double t_with_max_count = 1;
    double max_t_count = 0;
    for (unsigned int bin_y = 1; bin_y <= n_bins_y; ++bin_y) {
      double t_count = tVsLs->GetBinContent(bin_x, bin_y);
      if (t_count > max_t_count) {
        max_t_count = t_count;
        t_with_max_count = tVsLs->GetYaxis()->GetBinCenter(bin_y);
      }
    }
    std::cout << "Max count for LS = " << bin_x << " and t = " << t_with_max_count << ": " << max_t_count << ". Prev max t: " << prev_t_with_max_count << '\n';
    if (max_t_count != 0.) {
      double time_diff = t_with_max_count - prev_t_with_max_count;
      if (prev_t_with_max_count != -1. && abs(time_diff) > double_peak_t_max_diff_) {
        double_peak_ls = bin_x;
        time_shift = time_diff;
        break;
      }
      prev_t_with_max_count = t_with_max_count;
    }
  }
  doublePeakLsAndOffset[key] = {double_peak_ls, time_shift};
}

//------------------------------------------------------------------------------

void PPSTimingCalibrationPCLValidWorker::dqmAnalyze(const edm::Event& iEvent,
                                               const edm::EventSetup& iSetup,
                                               const TimingCalibrationHistograms& iHists) const {
  edm::Handle<edm::DetSetVector<CTPPSDiamondRecHit>> dsv_rechits;
  // then extract the rechits information for later processing
  searchForProduct(iEvent, diamondRecHitTokens_, RecHitTags_, dsv_rechits);

  // ensure timing detectors rechits are found in the event content
  if (dsv_rechits->empty()) {
    edm::LogWarning("PPSTimingCalibrationPCLValidWorker:dqmAnalyze") << "No rechits retrieved from the event content.";
    return;
  }
  for (const auto& ds_rechits : *dsv_rechits) {
    const CTPPSDiamondDetId detid(ds_rechits.detId());
    PlaneKey key{detid.arm(), detid.station(), detid.plane()};
    // if (iHists.leadingTimeVsLs.count(key) == 0) {
    //   edm::LogWarning("PPSTimingCalibrationPCLValidWorker:dqmAnalyze")
    //       << "Pad with detId=" << detid << " is not set to be monitored.";
    //   continue;
    // }
    auto [double_peak_ls, offset] = doublePeakLsAndOffset[key];
    if (iEvent.luminosityBlock() >= double_peak_ls) {
      for (const auto& rechit : ds_rechits) {
        // skip invalid rechits
        if (rechit.time() == 0. || rechit.toT() < 0.)
          continue;
        iHists.leadingTime.at(detid.rawId())->Fill(rechit.time() - offset);
        iHists.toT.at(detid.rawId())->Fill(rechit.toT());
        iHists.leadingTimeVsToT.at(detid.rawId())->Fill(rechit.toT(), rechit.time() - offset);
      }
    } else {
      for (const auto& rechit : ds_rechits) {
        // skip invalid rechits
        if (rechit.time() == 0. || rechit.toT() < 0.)
          continue;
        iHists.leadingTime.at(detid.rawId())->Fill(rechit.time());
        iHists.toT.at(detid.rawId())->Fill(rechit.toT());
        iHists.leadingTimeVsToT.at(detid.rawId())->Fill(rechit.toT(), rechit.time());
      }
    }
  }
}

//------------------------------------------------------------------------------

void PPSTimingCalibrationPCLValidWorker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<std::vector<edm::InputTag>>("diamondRecHitTags", {edm::InputTag("ctppsDiamondRecHits")})
      ->setComment("input tag for the PPS diamond detectors rechits");
  desc.add<std::string>("dqmDir", "AlCaReco/PPSTimingCalibrationPCL")
      ->setComment("output path for the various DQM plots");
  desc.add<std::string>("tVsLsFileName", "DQM_V0001_R000385281__CalibPPS__TimingCalibration__CMSSW_13_3_0.root")
      ->setComment("t vs LS ROOT filename");

  descriptions.addWithDefaultLabel(desc);
}

template <typename T>
bool PPSTimingCalibrationPCLValidWorker::searchForProduct(edm::Event const& iEvent,
                                                     const std::vector<edm::EDGetTokenT<T>>& tokens,
                                                     const std::vector<edm::InputTag>& tags,
                                                     edm::Handle<T>& handle) const {
  bool foundProduct = false;
  for (unsigned int i = 0; i < tokens.size(); i++)
    if (auto h = iEvent.getHandle(tokens[i])) {
      handle = h;
      foundProduct = true;
      edm::LogInfo("searchForProduct") << "Found a product with " << tags[i];
      break;
    }

  if (!foundProduct)
    throw edm::Exception(edm::errors::ProductNotFound) << "Could not find a product with any of the selected labels.";

  return foundProduct;
}

DEFINE_FWK_MODULE(PPSTimingCalibrationPCLValidWorker);
