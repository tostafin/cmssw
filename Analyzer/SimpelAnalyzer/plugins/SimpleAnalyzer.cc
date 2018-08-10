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
#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"
#include "DataFormats/CTPPSDetId/interface/TotemTimingDetId.h"

#include "DataFormats/CTPPSReco/interface/TotemTimingRecHit.h"
#include "DataFormats/CTPPSReco/interface/TotemTimingLocalTrack.h"
#include "DataFormats/CTPPSReco/interface/TotemRPLocalTrack.h"


#include <map>

#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TGraphErrors.h"

class SimpleAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit SimpleAnalyzer(const edm::ParameterSet&);
      ~SimpleAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      void initEventPlots(int eventNo, const CTPPSDetId& detId);

      void initTrackHistogram(const TotemTimingDetId&);
      void initEventTrackPlot(const int eventNo, const TotemTimingDetId& detId);
      void initTrackScatterPlot(const CTPPSDetId& detId);

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
      edm::EDGetTokenT< edm::DetSetVector<TotemRPLocalTrack> > tokenStripTrack_;


      // ---------- directories ---------------------------
      std::map< TotemTimingDetId, TFileDirectory > maindir_map_;
      std::map< int, TFileDirectory > eventDirMap;
      std::map< int, TFileDirectory > scatterPlotDirMap;

      // ---------- histograms ---------------------------
      std::map< TotemTimingDetId, TH1F*> trackXHistoMap;
      std::map< TotemTimingDetId, TH1F*> trackYHistoMap;
      std::map< TotemTimingDetId, TH1F*> hitXHistoMap;
      std::map< TotemTimingDetId, TH1F*> hitYHistoMap;
      std::map< int, TH2F*> eventTrackMap;
      std::map< int, TH2F*> scatterPlotMap;
      std::map< int, TH2F*> matchedStripTracks;
      std::map< int, TH2F*> unmatchedStripTracks;

      // ---------- graphs ---------------------------
      std::map< TotemTimingDetId, TGraph*> samples_graph_map_;
      std::map< TotemTimingDetId, unsigned int> samples_graphCtr_map_;
      std::map< int, TGraphErrors* > trackGraphErrorsMap;

      std::map< TotemTimingDetId, TGraphErrors*> trackXvsX;
      std::map< TotemTimingDetId, TH1F*> stripTrackUnmatchedX;
      std::map< TotemTimingDetId, TH1F*> timingTrackUnmatchedX;
      std::map< TotemTimingDetId, TGraphErrors*> trackYvsY;
      std::map< TotemTimingDetId, TH1F*> stripTrackUnmatchedY;
      std::map< TotemTimingDetId, TH1F*> timingTrackUnmatchedY;
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
 tokenLocalTrack_      ( consumes< edm::DetSetVector<TotemTimingLocalTrack> >      ( iConfig.getParameter<edm::InputTag>( "tagLocalTrack" ) ) ),
 tokenStripTrack_      ( consumes< edm::DetSetVector<TotemRPLocalTrack> >      ( iConfig.getParameter<edm::InputTag>( "tagStripTrack" ) ) )
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
    trackYHistoMap[ detId ] = maindir_map_[ detId ].make<TH1F>(trackYHistName.c_str(), trackYHistName.c_str(), 40, 45, 65 );

    std::string hitXHistName;
    hitXHistName.insert(0, "xHitDistribution");
    hitXHistoMap[ detId ] = maindir_map_[ detId ].make<TH1F>(hitXHistName.c_str(), hitXHistName.c_str(), 18, 0, 9 );

    std::string hitYHistName;
    hitYHistName.insert(0, "yHitDistribution");
    hitYHistoMap[ detId ] = maindir_map_[ detId ].make<TH1F>(hitYHistName.c_str(), hitYHistName.c_str(), 40, 45, 65 );
  }
}

int getArmHash(int eventNo, const CTPPSDetId& detId) {
  return 1000*eventNo + 100*detId.station() + 10*detId.arm() + detId.rp();
}

void SimpleAnalyzer::initEventPlots(int eventNo, const CTPPSDetId& detId) {

  if(trackGraphErrorsMap.find(getArmHash(eventNo, detId)) != trackGraphErrorsMap.end())
    return;

  edm::Service<TFileService> fs;
  std::string dirName = "event_" + std::to_string(eventNo);
  eventDirMap[eventNo] = fs->mkdir(dirName);

  trackGraphErrorsMap[getArmHash(eventNo, detId)] = eventDirMap[eventNo].make<TGraphErrors>(0);
  std::string title = "tracks_arm" + std::to_string(detId.arm()) + "_st" + std::to_string(detId.station()) + "_rp" + std::to_string(detId.rp());

  trackGraphErrorsMap[getArmHash(eventNo, detId)]->SetTitle(title.c_str());
  trackGraphErrorsMap[getArmHash(eventNo, detId)]->SetName(title.c_str());
}

void SimpleAnalyzer::initTrackScatterPlot(const CTPPSDetId& detId) {
  edm::Service<TFileService> fs;
  if (scatterPlotDirMap.find(detId) == scatterPlotDirMap.end())
  {
    std::string dirName = "tracks_arm" + std::to_string(detId.arm());

    // create directory for the detector, if not already done
    scatterPlotDirMap[ detId.arm() ] = fs->mkdir( dirName );

    //scatterPlotMap[ detId.arm() ] = scatterPlotDirMap[ detId.arm() ].make<TH2F>("strip track distribution", "strip track distribution", 50, -15, 15, 100, -70, 70 );
    //matchedStripTracks[ detId.arm() ] = scatterPlotDirMap[ detId.arm() ].make<TH2F>("matched strip track distribution", "matched strip track distribution", 50, -15, 15, 100, -70, 70 );
    //unmatchedStripTracks[ detId.arm() ] = scatterPlotDirMap[ detId.arm() ].make<TH2F>("unmatched strip track distribution", "unmatched strip track distribution", 50, -15, 15, 100, -70, 70 );
  }
}

int getArmRPHash(int eventNo, const TotemTimingDetId& detId) {
  return eventNo * 100 + detId.arm()*10 + detId.rp();
}


void SimpleAnalyzer::initEventTrackPlot(const int eventNo, const TotemTimingDetId& detId) {
  edm::Service<TFileService> fs;
  std::string dirName = "event_" + std::to_string(eventNo);

  // create directory for the detector, if not already done
  eventDirMap[eventNo] = fs->mkdir( dirName );

  float yOffset = (detId.rp() == 0 ? 45. : -65.);
  // create all histograms
  std::string plotName = "arm" + std::to_string(detId.arm()) + "_rp" + std::to_string(detId.rp()) + "_hits";
  eventTrackMap[getArmRPHash(eventNo, detId)] = eventDirMap[eventNo].make<TH2F>(plotName.c_str(), plotName.c_str(), 1500, -5, 51, 500, yOffset, yOffset + 20);


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
      for(unsigned int value = 0; value < 1; value++)
        plot->Fill(leftX + i*X_RESOLUTION + X_RESOLUTION/2.0, bottomY + j*Y_RESOLUTION + Y_RESOLUTION/2.0);
}

void SimpleAnalyzer::eventTrackPlotAddTrack(int eventNo, const TotemTimingLocalTrack& track, const TotemTimingDetId& detId) {

  if(eventTrackMap.find(getArmRPHash(eventNo, detId)) == eventTrackMap.end())
    initEventTrackPlot(eventNo, detId);

  auto plot = eventTrackMap[getArmRPHash(eventNo, detId)];

  float X_OFFSET = 10.0;

  auto xAxis = plot->GetXaxis();
  float X_RESOLUTION = (xAxis->GetXmax() - xAxis->GetXmin()) / (xAxis->GetNbins());
  auto yAxis = plot->GetYaxis();
  float Y_RESOLUTION = (yAxis->GetXmax() - yAxis->GetXmin()) / (yAxis->GetNbins());

  float leftX = (1.0 + track.getX0() - track.getX0Sigma()) + X_OFFSET * 4.0;
  float bottomY = track.getY0() - track.getY0Sigma();

  int xBins = track.getX0Sigma() * 2.0 / X_RESOLUTION;
  int yBins = track.getY0Sigma() * 2.0 / Y_RESOLUTION;

  for(int i = 0; i < xBins; i++)
    for(int j = 0; j < yBins; j++)
      for(int val = 0; val < 10; val++)
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
  edm::Handle< edm::DetSetVector<TotemRPLocalTrack> > stripLocalTrack;
  iEvent.getByToken( tokenDigi_, timingDigi );
  iEvent.getByToken( tokenRecHit_, timingRecHit );
  iEvent.getByToken( tokenLocalTrack_, timingLocalTrack );
  iEvent.getByToken( tokenStripTrack_, stripLocalTrack );

  unsigned int validStripStation = 0;
  for(const auto& timingTrackSet: *timingLocalTrack) {
    for(const auto& stripTrackSet: *stripLocalTrack) {

      CTPPSDetId timingId(timingTrackSet.detId());
      CTPPSDetId stripId(stripTrackSet.detId());

      if(stripId.station() != validStripStation)
        continue;

      if(timingId.arm() != stripId.arm() ||
          timingId.rp() % 2 != stripId.rp() % 2)
        continue;

      TotemTimingDetId tmpId(0, 0, 0, 0, 0);
      tmpId.setArm(timingId.arm());
      tmpId.setRP(timingId.rp() % 2);

      if(timingTrackSet.size() == 0) {
        for(const auto& stripTrack: stripTrackSet) {
          unmatchedStripTracks[tmpId]->Fill(stripTrack.getX0(), stripTrack.getY0());
          stripTrackUnmatchedX[tmpId]->Fill(stripTrack.getX0());
          stripTrackUnmatchedY[tmpId]->Fill(stripTrack.getY0());
        }
      }

      if(stripTrackSet.size() == 0) {
        for(const auto& timingTrack: timingTrackSet) {
          timingTrackUnmatchedX[tmpId]->Fill(timingTrack.getX0());
          timingTrackUnmatchedY[tmpId]->Fill(timingTrack.getY0());
        }
      }

      for(const auto& timingTrack: timingTrackSet)
        for(const auto& stripTrack: stripTrackSet) {
          matchedStripTracks[tmpId]->Fill(stripTrack.getX0(), stripTrack.getY0());
          int n = trackXvsX[tmpId]->GetN();
          trackXvsX[tmpId]->SetPoint(n, timingTrack.getX0(), stripTrack.getX0());
          //trackXvsX[tmpId]->SetPointError(n, timingTrack.getX0Sigma(), stripTrack.getX0Sigma());
          trackYvsY[tmpId]->SetPoint(n, timingTrack.getY0(), stripTrack.getY0());
          //trackYvsY[tmpId]->SetPointError(n, timingTrack.getY0Sigma(), stripTrack.getY0Sigma());
        }
    }
  }



  bool multipleTracks = false;

  for (const auto& trackSet : *timingLocalTrack)
    if(trackSet.size() > 1)
      multipleTracks = true;

  int timingTrackCounter = 0;

  for (const auto& trackSet : *timingLocalTrack) {
    timingTrackCounter += trackSet.size();

    const TotemTimingDetId detId( trackSet.detId() );
    if (maindir_map_.find(detId.getArmId()) == maindir_map_.end())
      initTrackHistogram( detId.getArmId() );

    if(scatterPlotDirMap.find(detId.arm()) == scatterPlotDirMap.end())
      initTrackScatterPlot(detId);




    for (const auto& track : trackSet ) {
      trackXHistoMap[detId.getArmId()]->Fill(track.getX0());
      trackYHistoMap[detId.getArmId()]->Fill(track.getY0());

      if(eventCounter <= 10 || multipleTracks) {
        eventTrackPlotAddTrack(eventCounter, track, detId);

        initEventPlots(eventCounter, detId);
        int n = trackGraphErrorsMap[getArmHash(eventCounter, detId)]->GetN();
        trackGraphErrorsMap[getArmHash(eventCounter, detId)]->Set(n+1);
        trackGraphErrorsMap[getArmHash(eventCounter, detId)]->SetPoint(n, track.getX0(), track.getY0());
        trackGraphErrorsMap[getArmHash(eventCounter, detId)]->SetPointError(n, track.getX0Sigma(), track.getY0Sigma());
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

  int stripTrackCounter = 0;

  for (const auto& trackSet : *stripLocalTrack) {
    stripTrackCounter += trackSet.size();

    CTPPSDetId detId( trackSet.detId() );

    if(eventCounter > 10)
      break;

    for(const auto& track: trackSet) {
      initEventPlots(eventCounter, detId);
      int n = trackGraphErrorsMap[getArmHash(eventCounter, detId)]->GetN();
      trackGraphErrorsMap[getArmHash(eventCounter, detId)]->Set(n+1);
      trackGraphErrorsMap[getArmHash(eventCounter, detId)]->SetPoint(n, track.getX0(), track.getY0());
      trackGraphErrorsMap[getArmHash(eventCounter, detId)]->SetPointError(n, track.getX0Sigma(), track.getY0Sigma());
    }
  }

}



// ------------ method called once each job just before starting event loop  ------------
void
SimpleAnalyzer::beginJob()
{
  edm::Service<TFileService> fs;
  auto trackDir = fs->mkdir("TotemTiming_vs_TotemRP");

  for(int i = 0; i < 4; i++) {
    int arm = i / 2;
    int rp = i % 2;
    std::string str_pot = (rp == 0 ? "top" : "bottom");
    std::string str_arm = std::to_string(arm);
    TotemTimingDetId tmpId(arm, 0, rp, 0, 0);

    trackXvsX[tmpId] = trackDir.make<TGraphErrors>(0);
    std::string xName = "X_vs_X_" + str_arm + str_pot;
    trackXvsX[tmpId]->SetName(xName.c_str());
    trackXvsX[tmpId]->SetTitle(xName.c_str());

    xName = "strip_unmatched_X_" + str_arm + str_pot;
    stripTrackUnmatchedX[tmpId] = trackDir.make<TH1F>(xName.c_str(), xName.c_str(), 40, -20, 20);

    xName = "timing_unmatched_X_" + str_arm + str_pot;
    timingTrackUnmatchedX[tmpId] = trackDir.make<TH1F>(xName.c_str(), xName.c_str(), 20, 0, 9);

    trackYvsY[tmpId] = trackDir.make<TGraphErrors>(0);
    std::string yName = "Y_vs_Y_" + str_arm + str_pot;
    trackYvsY[tmpId]->SetName(yName.c_str());
    trackYvsY[tmpId]->SetTitle(yName.c_str());

    float yOffsetStrip = (tmpId.rp() == 0 ? 5. : -35.);
    float yRangeStrip = (tmpId.rp() == 0 ? 40. : 30.);
    float yOffsetTiming = (tmpId.rp() == 0 ? 45. : -65.);

    yName = "strip_unmatched_Y_" + str_arm + str_pot;
    stripTrackUnmatchedY[tmpId] = trackDir.make<TH1F>(yName.c_str(), yName.c_str(), ((int)yRangeStrip) * 2, yOffsetStrip, yOffsetStrip + yRangeStrip);

    yName = "timing_unmatched_Y_" + str_arm + str_pot;
    timingTrackUnmatchedY[tmpId] = trackDir.make<TH1F>(yName.c_str(), yName.c_str(), 40, yOffsetTiming, yOffsetTiming + 20);

    std::string name = "matchedStripTrackDistribution_" + str_arm + str_pot;
    matchedStripTracks[tmpId] = trackDir.make<TH2F>(name.c_str(), name.c_str(), 100, -15, 15, 100, -70, 70 );
    name = "unmatchedStripTrackDistribution_" + str_arm + str_pot;
    unmatchedStripTracks[tmpId] = trackDir.make<TH2F>(name.c_str(), name.c_str(), 100, -15, 15, 100, -70, 70 );
  }
}

// ------------ method called once each job just after ending the event loop  ------------
void
SimpleAnalyzer::endJob() {

  for(int i = 0; i < 4; i++) {
    int arm = i / 2;
    int rp = i % 2;
    std::string str_pot = (rp == 0 ? "top" : "bottom");
    std::string str_arm = std::to_string(arm);
    TotemTimingDetId tmpId(arm, 0, rp, 0, 0);

    std::cout << str_arm + str_pot + ": " << std::endl;
    std::cout << "Xmatched: " << trackXvsX[tmpId]->GetN() << std::endl;
    std::cout << "Ymatched: " << trackYvsY[tmpId]->GetN() << std::endl;
  }
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
