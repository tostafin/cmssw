// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/CTPPSDigi/interface/TotemTimingDigi.h"
#include "DataFormats/CTPPSDetId/interface/TotemTimingDetId.h"

#include "DataFormats/CTPPSReco/interface/TotemTimingRecHit.h"
#include "DataFormats/CTPPSReco/interface/TotemTimingLocalTrack.h"


#include <map>

#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"

class SimpleAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit SimpleAnalyzer(const edm::ParameterSet&);
      ~SimpleAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      void initTrackHistogram(const TotemTimingDetId&);
      void initEventTrackPlot(const int eventNo, const TotemTimingDetId& detId);

      void eventTrackPlotAddHit(int eventNo, const TotemTimingRecHit& hit, const TotemTimingDetId& detId);
      void eventTrackPlotAddTrack(int eventNo, const TotemTimingLocalTrack& track, const TotemTimingDetId& detId);

      // ---------- constants ---------------------------
      static const double SAMPIC_MAX_NUMBER_OF_SAMPLES;
      static const double SAMPIC_ADC_V;
      static const double SAMPIC_SAMPLING_PERIOD_NS;

      // ---------- objects to retrieve ---------------------------
      edm::EDGetTokenT< edm::DetSetVector<TotemTimingDigi> > tokenDigi_;
      edm::EDGetTokenT< edm::DetSetVector<TotemTimingRecHit> > tokenRecHit_;
      edm::EDGetTokenT< edm::DetSetVector<TotemTimingLocalTrack> > tokenLocalTrack_;


      // ---------- directories ---------------------------
      std::map< TotemTimingDetId, TFileDirectory > maindir_map_;
      std::map< int, TFileDirectory > eventDirMap;

      // ---------- histograms ---------------------------
      std::map< TotemTimingDetId, TH1F*> trackXHistoMap;
      std::map< TotemTimingDetId, TH1F*> trackYHistoMap;
      std::map< TotemTimingDetId, TH1F*> hitXHistoMap;
      std::map< TotemTimingDetId, TH1F*> hitYHistoMap;
      std::map< int, TH2F*> eventTrackMap;

      // ---------- graphs ---------------------------
      std::map< TotemTimingDetId, TGraph*> samples_graph_map_;
      std::map< TotemTimingDetId, unsigned int> samples_graphCtr_map_;
};

//
// constants, enums and typedefs
//
const double    SimpleAnalyzer::SAMPIC_MAX_NUMBER_OF_SAMPLES = 64;
const double    SimpleAnalyzer::SAMPIC_ADC_V = 1./256;
const double    SimpleAnalyzer::SAMPIC_SAMPLING_PERIOD_NS = 1./7.8;

//
// static data member definitions
//

//
// constructors and destructor
//
SimpleAnalyzer::SimpleAnalyzer(const edm::ParameterSet& iConfig)
 :
 tokenDigi_            ( consumes< edm::DetSetVector<TotemTimingDigi> >      ( iConfig.getParameter<edm::InputTag>( "tagDigi" ) ) ),
 tokenRecHit_          ( consumes< edm::DetSetVector<TotemTimingRecHit> >      ( iConfig.getParameter<edm::InputTag>( "tagRecHit" ) ) ),
 tokenLocalTrack_      ( consumes< edm::DetSetVector<TotemTimingLocalTrack> >      ( iConfig.getParameter<edm::InputTag>( "tagLocalTrack" ) ) )
{
  usesResource("TFileService");
}


SimpleAnalyzer::~SimpleAnalyzer()
{}


//
// member functions
//

void SimpleAnalyzer::initTrackHistogram(const TotemTimingDetId& detId) {
  edm::Service<TFileService> fs;
  if (maindir_map_.find(detId) == maindir_map_.end())
  {
    std::string dirName;
    detId.armName(dirName, TotemTimingDetId::nPath);

    // create directory for the detector, if not already done
    maindir_map_[ detId ] = fs->mkdir( dirName );

    // create all histograms
    std::string trackXHistName;
    trackXHistName.insert(0, "xTrackDistribution");
    trackXHistoMap[ detId ] = maindir_map_[ detId ].make<TH1F>(trackXHistName.c_str(), trackXHistName.c_str(), 18, 0, 9 );

    std::string trackYHistName;
    trackYHistName.insert(0, "yTrackDistribution");
    trackYHistoMap[ detId ] = maindir_map_[ detId ].make<TH1F>(trackYHistName.c_str(), trackYHistName.c_str(), 20, 45, 65 );

    std::string hitXHistName;
    hitXHistName.insert(0, "xHitDistribution");
    hitXHistoMap[ detId ] = maindir_map_[ detId ].make<TH1F>(hitXHistName.c_str(), hitXHistName.c_str(), 100, 0, 9 );

    std::string hitYHistName;
    hitYHistName.insert(0, "yHitDistribution");
    hitYHistoMap[ detId ] = maindir_map_[ detId ].make<TH1F>(hitYHistName.c_str(), hitYHistName.c_str(), 800, 45, 65 );
  }
}

int getArmRPHash(int eventNo, const TotemTimingDetId& detId) {
  return eventNo * 100 + detId.arm()*10 + detId.rp();
}


void SimpleAnalyzer::initEventTrackPlot(const int eventNo, const TotemTimingDetId& detId) {
  edm::Service<TFileService> fs;

  //std::string dirName = "event_" + std::to_string(eventNo) + "_track_plot";
  std::string dirName = "event_" + std::to_string(eventNo) + "_track_plots";

  // create directory for the detector, if not already done
  eventDirMap[eventNo] = fs->mkdir( dirName );

  float yOffset = (detId.rp() == 0 ? 45. : -65.);
  // create all histograms
  std::string plotName = "arm" + std::to_string(detId.arm()) + "_rp" + std::to_string(detId.rp()) + "_hits";
  eventTrackMap[getArmRPHash(eventNo, detId)] = eventDirMap[eventNo].make<TH2F>(plotName.c_str(), plotName.c_str(), 1500, 0, 51, 500, yOffset, yOffset + 20);


  // drawing vertical plane separators
  auto plot = eventTrackMap[getArmRPHash(eventNo, detId)];

  float X_OFFSET = 10.0;

  auto xAxis = plot->GetXaxis();
  float X_RESOLUTION = (xAxis->GetXmax() - xAxis->GetXmin()) / (xAxis->GetNbins());
  auto yAxis = plot->GetYaxis();
  float Y_RESOLUTION = (yAxis->GetXmax() - yAxis->GetXmin()) / (yAxis->GetNbins());

  int xBins = 0.2 / X_RESOLUTION;
  int yBins = 20 / Y_RESOLUTION;

  for(int i = 0; i < xBins; i++)
    for(int j = 0; j < yBins; j++)
      for(int k = 1; k < 5; k++)
        plot->Fill(0.5 + k*X_OFFSET + i*X_RESOLUTION + X_RESOLUTION/2.0, yOffset + j*Y_RESOLUTION + Y_RESOLUTION/2.0);
}


void SimpleAnalyzer::eventTrackPlotAddHit(int eventNo, const TotemTimingRecHit& hit, const TotemTimingDetId& detId) {

  if(eventTrackMap.find(getArmRPHash(eventNo, detId)) == eventTrackMap.end())
    initEventTrackPlot(eventNo, detId);

  auto plot = eventTrackMap[getArmRPHash(eventNo, detId)];

  float X_OFFSET = 10.0;

  auto xAxis = plot->GetXaxis();
  float X_RESOLUTION = (xAxis->GetXmax() - xAxis->GetXmin()) / (xAxis->GetNbins());
  auto yAxis = plot->GetYaxis();
  float Y_RESOLUTION = (yAxis->GetXmax() - yAxis->GetXmin()) / (yAxis->GetNbins());

  float leftX = (1.0 + hit.getX() - hit.getXWidth()/2.0) + X_OFFSET * ((float) detId.plane());
  float bottomY = hit.getY() - hit.getYWidth()/2.0;

  int xBins = hit.getXWidth() / X_RESOLUTION;
  int yBins = hit.getYWidth() / Y_RESOLUTION;

  for(int i = 0; i < xBins; i++)
    for(int j = 0; j < yBins; j++)
      for(unsigned int value = 0; value < detId.channel()+1; value++)
        plot->Fill(leftX + i*X_RESOLUTION + X_RESOLUTION/2.0, bottomY + j*Y_RESOLUTION + Y_RESOLUTION/2.0);
}

void SimpleAnalyzer::eventTrackPlotAddTrack(int eventNo, const TotemTimingLocalTrack& track, const TotemTimingDetId& detId) {

  if(eventTrackMap.find(getArmRPHash(eventNo, detId)) == eventTrackMap.end())
    initEventTrackPlot(eventNo, detId);

  auto plot = eventTrackMap[getArmRPHash(eventNo, detId)];

  std::cout << "dodaje tracka " << track.getX0() << "," << track.getY0() << " do pota " << detId.rp() << ", arm " << detId.arm() << std::endl;

  float X_OFFSET = 10.0;

  auto xAxis = plot->GetXaxis();
  float X_RESOLUTION = (xAxis->GetXmax() - xAxis->GetXmin()) / (xAxis->GetNbins());
  auto yAxis = plot->GetYaxis();
  float Y_RESOLUTION = (yAxis->GetXmax() - yAxis->GetXmin()) / (yAxis->GetNbins());

  float leftX = (1.0 + track.getX0() - track.getX0Sigma()) + X_OFFSET * 4.0;
  float bottomY = track.getY0() - track.getY0Sigma();

  std::cout << "leftX:" << leftX << " bottomY:" << bottomY << std::endl;

  int xBins = track.getX0Sigma() * 2.0 / X_RESOLUTION;
  int yBins = track.getY0Sigma() * 2.0 / Y_RESOLUTION;

  std::cout << "xBins:" << xBins << " yBins:" << yBins << std::endl;

  for(int i = 0; i < xBins; i++)
    for(int j = 0; j < yBins; j++)
      plot->Fill(leftX + i*X_RESOLUTION + X_RESOLUTION/2.0, bottomY + j*Y_RESOLUTION + Y_RESOLUTION/2.0);
}


// ------------ method called for each event  ------------
int eventCounter = 0;

void
SimpleAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  eventCounter++;

  edm::Handle< edm::DetSetVector<TotemTimingDigi> > timingDigi;
  edm::Handle< edm::DetSetVector<TotemTimingRecHit> > timingRecHit;
  edm::Handle< edm::DetSetVector<TotemTimingLocalTrack> > timingLocalTrack;
  iEvent.getByToken( tokenDigi_, timingDigi );
  iEvent.getByToken( tokenRecHit_, timingRecHit );
  iEvent.getByToken( tokenLocalTrack_, timingLocalTrack );

  bool multipleTracks = false;

  for (const auto& trackSet : *timingLocalTrack) {
    const TotemTimingDetId detId( trackSet.detId() );
    if (maindir_map_.find(detId.getArmId()) == maindir_map_.end())
      initTrackHistogram( detId.getArmId() );

    if (trackSet.size() > 1)
      multipleTracks = true;

    for (const auto& track : trackSet ) {
      trackXHistoMap[detId.getArmId()]->Fill(track.getX0());
      trackYHistoMap[detId.getArmId()]->Fill(track.getY0());

      if(eventCounter <= 10 || multipleTracks) {
        TotemTimingDetId tmpId(detId);
        int potId = (track.getY0() > 0 ? 0 : 1);
        tmpId.setRP(potId);
        eventTrackPlotAddTrack(eventCounter, track, tmpId);
      }
    }
  }

  for (const auto& recHits : *timingRecHit) {
    const TotemTimingDetId detId( recHits.detId() );
    if (maindir_map_.find(detId.getArmId()) == maindir_map_.end())
      initTrackHistogram( detId.getArmId() );

    for (const auto& recHit : recHits ) {
      // Do stuff on recHits
      hitXHistoMap[ detId.getArmId() ]->Fill( recHit.getX() );
      hitYHistoMap[ detId.getArmId() ]->Fill( recHit.getY() );

      if(eventCounter <= 10 || multipleTracks)
        eventTrackPlotAddHit(eventCounter, recHit, detId);
    }
  }


}


// ------------ method called once each job just before starting event loop  ------------
void
SimpleAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
SimpleAnalyzer::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
SimpleAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(SimpleAnalyzer);
