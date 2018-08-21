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
#include "DataFormats/CTPPSReco/interface/TotemRPRecHit.h"
#include "DataFormats/CTPPSReco/interface/TotemTimingLocalTrack.h"
#include "DataFormats/CTPPSReco/interface/TotemRPLocalTrack.h"


#include <map>
#include <cmath>

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
      edm::EDGetTokenT< edm::DetSetVector<TotemRPRecHit> > tokenRPHit_;
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
      std::map< int, TH2F*> matchedStripTracks;
      std::map< int, TH2F*> unmatchedStripTracks;
      std::map< int, TH2F*> allStripTracks;
      std::map< int, TH2F*> detectorEfficiency;

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

      std::map< TotemTimingDetId, TH2F*> matchedStripPlaneHits;
      std::map< TotemTimingDetId, TH2F*> unmatchedStripPlaneHits;
      std::map< TotemTimingDetId, TH2F*> efficiencyPlaneHits;

      std::map< TotemTimingDetId, TH2F*> matchedStripChannelHits;
      std::map< TotemTimingDetId, TH2F*> unmatchedStripChannelHits;
      std::map< TotemTimingDetId, TH2F*> efficiencyChannelHits;

      std::map< TotemTimingDetId, TH1F*> matchedStripChannelHitsX;
      std::map< TotemTimingDetId, TH1F*> matchedStripChannelHitsY;
      std::map< TotemTimingDetId, TH1F*> efficiencyChannelHitsX;
      std::map< TotemTimingDetId, TH1F*> efficiencyChannelHitsY;


      struct TrackMatchParameters {
        float xMainCoeff;
        float xOffset;
        float xTolerance;
        float yMainCoeff;
        float yOffset;
        float yTolerance;

        TrackMatchParameters(double xmc = 0, double xo = 0, double xt = 0, double ymc = 0, double yo = 0, double yt = 0) {
          xMainCoeff = xmc;
          xOffset = xo;
          xTolerance = xt;
          yMainCoeff = ymc;
          yOffset = yo;
          yTolerance = yt;
        }
      };

      std::map< TotemTimingDetId, TrackMatchParameters > trackMatchParamsMap;
      bool trackMatch(const TotemTimingLocalTrack& timingTrack, const TotemRPLocalTrack& stripTrack, const CTPPSDetId& detId);
      bool hitMatch(const TotemTimingRecHit& timingHit, const TotemRPLocalTrack& stripTrack, const CTPPSDetId& detId);
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
 tokenRPHit_          ( consumes< edm::DetSetVector<TotemRPRecHit> >      ( iConfig.getParameter<edm::InputTag>( "tagRPHit" ) ) ),
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
  edm::Handle< edm::DetSetVector<TotemRPRecHit> > stripRecHit;
  edm::Handle< edm::DetSetVector<TotemTimingLocalTrack> > timingLocalTrack;
  edm::Handle< edm::DetSetVector<TotemRPLocalTrack> > stripLocalTrack;
  iEvent.getByToken( tokenDigi_, timingDigi );
  iEvent.getByToken( tokenRecHit_, timingRecHit );
  iEvent.getByToken( tokenRPHit_, stripRecHit );
  iEvent.getByToken( tokenLocalTrack_, timingLocalTrack );
  iEvent.getByToken( tokenStripTrack_, stripLocalTrack );

  unsigned int validStripStation = 0;
  for(const auto& timingTrackSet: *timingLocalTrack) {
    for(const auto& stripTrackSet: *stripLocalTrack) {

      CTPPSDetId timingId(timingTrackSet.detId());
      CTPPSDetId stripId(stripTrackSet.detId());

      if(stripId.station() != validStripStation)
        continue;

      if((timingId.arm() != stripId.arm()) ||
          ((timingId.rp() % 2) != (stripId.rp() % 2)))
        continue;

      TotemTimingDetId tmpId(0, 0, 0, 0, 0);
      tmpId.setArm(timingId.arm());
      tmpId.setRP(timingId.rp() % 2);

/*
      if(timingTrackSet.size() == 0) {
        for(const auto& stripTrack: stripTrackSet) {
          unmatchedStripTracks[tmpId]->Fill(stripTrack.getX0(), stripTrack.getY0());
          stripTrackUnmatchedX[tmpId]->Fill(stripTrack.getX0());
          stripTrackUnmatchedY[tmpId]->Fill(stripTrack.getY0());
        }
      }
*/
/*
      if(stripTrackSet.size() == 0) {
        for(const auto& timingTrack: timingTrackSet) {
          timingTrackUnmatchedX[tmpId]->Fill(timingTrack.getX0());
          timingTrackUnmatchedY[tmpId]->Fill(timingTrack.getY0());
        }
      }
*/

      for(const auto& timingTrack: timingTrackSet) {
        trackXHistoMap[tmpId]->Fill(timingTrack.getX0());
        trackYHistoMap[tmpId]->Fill(timingTrack.getY0());
      }
/*
      for(const auto& timingTrack: timingTrackSet) {

        trackXHistoMap[tmpId]->Fill(timingTrack.getX0());
        trackYHistoMap[tmpId]->Fill(timingTrack.getY0());

        for(const auto& stripTrack: stripTrackSet) {
          matchedStripTracks[tmpId]->Fill(stripTrack.getX0(), stripTrack.getY0());
          int n = trackXvsX[tmpId]->GetN();
          trackXvsX[tmpId]->SetPoint(n, timingTrack.getX0(), stripTrack.getX0());
          //trackXvsX[tmpId]->SetPointError(n, timingTrack.getX0Sigma(), stripTrack.getX0Sigma());
          trackYvsY[tmpId]->SetPoint(n, timingTrack.getY0(), stripTrack.getY0());
          //trackYvsY[tmpId]->SetPointError(n, timingTrack.getY0Sigma(), stripTrack.getY0Sigma());
        }
      }
*/
      for(const auto& stripTrack: stripTrackSet) {

        bool matched = false;

        for(const auto& timingTrack: timingTrackSet) {

          if(trackMatch(timingTrack, stripTrack, tmpId)) {
            int n = trackXvsX[tmpId]->GetN();
            trackXvsX[tmpId]->SetPoint(n, timingTrack.getX0(), stripTrack.getX0());
            trackYvsY[tmpId]->SetPoint(n, timingTrack.getY0(), stripTrack.getY0());
            matched = true;
          }
        }

        if(matched)
          matchedStripTracks[tmpId]->Fill(stripTrack.getX0(), stripTrack.getY0());
        else {
          unmatchedStripTracks[tmpId]->Fill(stripTrack.getX0(), stripTrack.getY0());
          stripTrackUnmatchedX[tmpId]->Fill(stripTrack.getX0());
          stripTrackUnmatchedY[tmpId]->Fill(stripTrack.getY0());
        }
      }

      for(const auto& timingTrack: timingTrackSet) {

        bool matched = false;

        for(const auto& stripTrack: stripTrackSet) {

          if(trackMatch(timingTrack, stripTrack, tmpId)) {
            matched = true;
            break;
          }
        }

        if(!matched) {
          timingTrackUnmatchedX[tmpId]->Fill(timingTrack.getX0());
          timingTrackUnmatchedY[tmpId]->Fill(timingTrack.getY0());
        }
      }

    }
  }

  /*

  bool multipleTracks = false;

  for (const auto& trackSet : *timingLocalTrack)
    if(trackSet.size() > 1)
      multipleTracks = true;

  int timingTrackCounter = 0;

  for (const auto& trackSet : *timingLocalTrack) {
    timingTrackCounter += trackSet.size();

    const TotemTimingDetId detId( trackSet.detId() );

    for (const auto& track : trackSet ) {

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

    TotemTimingDetId tmpId(0, 0, 0, 0, 0);
    tmpId.setArm(detId.arm());
    tmpId.setRP(detId.rp() % 2);

    for (const auto& recHit : recHits ) {

      hitXHistoMap[tmpId]->Fill(recHit.getX());
      hitYHistoMap[tmpId]->Fill(recHit.getY());

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
*/

  for(const auto& stripTrackSet: *stripLocalTrack) {
    for(const auto& stripTrack: stripTrackSet) {

      CTPPSDetId stripId(stripTrackSet.detId());

      if(stripId.station() != validStripStation)
        continue;

      TotemTimingDetId tmpId(0, 0, 0, 0, 0);
      tmpId.setArm(stripId.arm());
      tmpId.setRP(stripId.rp() % 2);
      allStripTracks[tmpId]->Fill(stripTrack.getX0(), stripTrack.getY0());

      for(unsigned int planeNo = 0; planeNo < 4; planeNo++) {

        bool matched = false;
        tmpId.setPlane(planeNo);

        for(const auto& timingHitSet: *timingRecHit) {

          TotemTimingDetId timingId(timingHitSet.detId());

          if(stripId.station() != validStripStation)
            continue;

          if((timingId.arm() != stripId.arm()) ||
              ((timingId.rp() % 2) != (stripId.rp() % 2)))
            continue;

          if(timingId.plane() != planeNo)
            continue;

          for(const auto& timingHit: timingHitSet) {
            if(hitMatch(timingHit, stripTrack, tmpId)) {
              matched = true;
              break;
            }
          }
          if(matched)
            break;
        }

        if(matched) {
          //std::cout << "match " << tmpId.arm() << ", " << tmpId.rp() << ", " << tmpId.plane() << std::endl;
          matchedStripPlaneHits[tmpId]->Fill(stripTrack.getX0(), stripTrack.getY0());
        }
        else {
          //std::cout << "unmatch " << tmpId.arm() << ", " << tmpId.rp() << ", " << tmpId.plane() << std::endl;
          unmatchedStripPlaneHits[tmpId]->Fill(stripTrack.getX0(), stripTrack.getY0());
        }

        for(unsigned int channelNo = 0; channelNo < 12; channelNo++) {
          matched = false;
          tmpId.setChannel(channelNo);

          for(const auto& timingHitSet: *timingRecHit) {
            TotemTimingDetId timingId(timingHitSet.detId());

            if(stripId.station() != validStripStation)
              continue;

            if((timingId.arm() != stripId.arm()) ||
                ((timingId.rp() % 2) != (stripId.rp() % 2)))
              continue;

            if(timingId.plane() != planeNo)
              continue;

            if(timingId.channel() != channelNo)
              continue;

            for(const auto& timingHit: timingHitSet) {
              if(hitMatch(timingHit, stripTrack, tmpId)) {
                matched = true;
                break;
              }
            }
            if(matched)
              break;
          }
          if(matched)
            matchedStripChannelHits[tmpId]->Fill(stripTrack.getX0(), stripTrack.getY0());
          else
            unmatchedStripChannelHits[tmpId]->Fill(stripTrack.getX0(), stripTrack.getY0());
        }
        tmpId.setChannel(0);
      }
    }
  }
}

bool SimpleAnalyzer::trackMatch(const TotemTimingLocalTrack& timingTrack, const TotemRPLocalTrack& stripTrack, const CTPPSDetId& detId){

  TotemTimingDetId tmpId(0, 0, 0, 0, 0);
  tmpId.setArm(detId.arm());
  tmpId.setRP(detId.rp());

  TrackMatchParameters params = trackMatchParamsMap[tmpId];

  float topTimingX = params.xMainCoeff * (timingTrack.getX0() + timingTrack.getX0Sigma()) + params.xOffset;
  float bottomTimingX = params.xMainCoeff * (timingTrack.getX0() - timingTrack.getX0Sigma()) + params.xOffset;
  float topTimingY = params.yMainCoeff * (timingTrack.getY0() + timingTrack.getY0Sigma()) + params.yOffset;
  float bottomTimingY = params.yMainCoeff * (timingTrack.getY0() - timingTrack.getY0Sigma()) + params.yOffset;

  float topStripX = stripTrack.getX0() + stripTrack.getX0Sigma();
  float bottomStripX = stripTrack.getX0() - stripTrack.getX0Sigma();
  float topStripY = stripTrack.getY0() + stripTrack.getY0Sigma();
  float bottomStripY = stripTrack.getY0() - stripTrack.getY0Sigma();

  float xDiff = std::fabs((topTimingX + bottomTimingX)/2.0 - (topStripX + bottomStripX)/2.0);
  float yDiff = std::fabs((topTimingY + bottomTimingY)/2.0 - (topStripY + bottomStripY)/2.0);

  float xTolerance = (topTimingX - bottomTimingX)/2.0 + (topStripX - bottomStripX)/2.0 + params.xTolerance;
  float yTolerance = (topTimingY - bottomTimingY)/2.0 + (topStripY - bottomStripY)/2.0 + params.yTolerance;

  //std::cout << "timing: (" << timingTrack.getX0() << ", " << timingTrack.getY0() << ") +- (" << timingTrack.getX0Sigma() << ", " << timingTrack.getY0Sigma() << ")" << std::endl;
  //std::cout << "timing projected: x (" << bottomTimingX << ", " << topTimingX << "), y (" << bottomTimingY << ", " << topTimingY << ")" << std::endl;
  //std::cout << "strip: (" << stripTrack.getX0() << ", " << stripTrack.getY0() << ") +- (" << stripTrack.getX0Sigma() << ", " << stripTrack.getY0Sigma() << ")" << std::endl;
  //std::cout << std::endl;

  return (xDiff < xTolerance) && (yDiff < yTolerance);
}
bool SimpleAnalyzer::hitMatch(const TotemTimingRecHit& timingHit, const TotemRPLocalTrack& stripTrack, const CTPPSDetId& detId){

  TotemTimingDetId tmpId(0, 0, 0, 0, 0);
  tmpId.setArm(detId.arm());
  tmpId.setRP(detId.rp());

  TrackMatchParameters params = trackMatchParamsMap[tmpId];

  float topTimingX = params.xMainCoeff * (timingHit.getX() + timingHit.getXWidth()) + params.xOffset;
  float bottomTimingX = params.xMainCoeff * (timingHit.getX() - timingHit.getXWidth()) + params.xOffset;
  float topTimingY = params.yMainCoeff * (timingHit.getY() + timingHit.getYWidth()) + params.yOffset;
  float bottomTimingY = params.yMainCoeff * (timingHit.getY() - timingHit.getYWidth()) + params.yOffset;

  float topStripX = stripTrack.getX0() + stripTrack.getX0Sigma();
  float bottomStripX = stripTrack.getX0() - stripTrack.getX0Sigma();
  float topStripY = stripTrack.getY0() + stripTrack.getY0Sigma();
  float bottomStripY = stripTrack.getY0() - stripTrack.getY0Sigma();

  float xDiff = std::fabs((topTimingX + bottomTimingX)/2.0 - (topStripX + bottomStripX)/2.0);
  float yDiff = std::fabs((topTimingY + bottomTimingY)/2.0 - (topStripY + bottomStripY)/2.0);

  float xTolerance = (topTimingX - bottomTimingX)/2.0 + (topStripX - bottomStripX)/2.0 + params.xTolerance;
  float yTolerance = (topTimingY - bottomTimingY)/2.0 + (topStripY - bottomStripY)/2.0 + params.yTolerance;

  //std::cout << "timing: (" << timingTrack.getX0() << ", " << timingTrack.getY0() << ") +- (" << timingTrack.getX0Sigma() << ", " << timingTrack.getY0Sigma() << ")" << std::endl;
  //std::cout << "timing projected: x (" << bottomTimingX << ", " << topTimingX << "), y (" << bottomTimingY << ", " << topTimingY << ")" << std::endl;
  //std::cout << "strip: (" << stripTrack.getX0() << ", " << stripTrack.getY0() << ") +- (" << stripTrack.getX0Sigma() << ", " << stripTrack.getY0Sigma() << ")" << std::endl;
  //std::cout << std::endl;

  return (xDiff < xTolerance) && (yDiff < yTolerance);
}

// ------------ method called once each job just before starting event loop  ------------
void
SimpleAnalyzer::beginJob()
{
  trackMatchParamsMap[TotemTimingDetId(0, 0, 0, 0, 0)] = TrackMatchParameters(0.81, -1.16, 100.0, 0.95, -36.81, 100.0);
  trackMatchParamsMap[TotemTimingDetId(0, 0, 1, 0, 0)] = TrackMatchParameters(1.17, -1.82, 100.0, 0.95, 36.48, 100.0);
  trackMatchParamsMap[TotemTimingDetId(1, 0, 0, 0, 0)] = TrackMatchParameters(0.25, -1.0, 100.0, 0.91, -34.14, 100.0);
  trackMatchParamsMap[TotemTimingDetId(1, 0, 1, 0, 0)] = TrackMatchParameters(0.53, -1.35, 100.0, 0.94, 35.46, 100.0);


  edm::Service<TFileService> fs;
  std::string topDir = "TotemTiming_vs_TotemRP";


  for(int i = 0; i < 4; i++) {

    int arm = i / 2;
    int rp = i % 2;

    std::string str_pot = (rp == 0 ? "top" : "bottom");
    std::string str_arm = std::to_string(arm);

    std::string dirName = topDir + "/" + str_pot + str_arm;
    auto trackDir = fs->mkdir(dirName);

    TotemTimingDetId tmpId(arm, 0, rp, 0, 0);

    trackXvsX[tmpId] = trackDir.make<TGraphErrors>(0);
    std::string name = "X_vs_X";
    trackXvsX[tmpId]->SetName(name.c_str());
    trackXvsX[tmpId]->SetTitle(name.c_str());

    name = "strip_unmatched_X";
    stripTrackUnmatchedX[tmpId] = trackDir.make<TH1F>(name.c_str(), name.c_str(), 40, -20, 20);

    name = "timing_unmatched_X";
    timingTrackUnmatchedX[tmpId] = trackDir.make<TH1F>(name.c_str(), name.c_str(), 20, 0, 9);

    trackYvsY[tmpId] = trackDir.make<TGraphErrors>(0);
    name = "Y_vs_Y";
    trackYvsY[tmpId]->SetName(name.c_str());
    trackYvsY[tmpId]->SetTitle(name.c_str());

    float yOffsetStrip = (tmpId.rp() == 0 ? 5. : -35.);
    float yRangeStrip = (tmpId.rp() == 0 ? 40. : 30.);
    float yOffsetTiming = (tmpId.rp() == 0 ? 45. : -65.);

    name = "strip_unmatched_Y";
    stripTrackUnmatchedY[tmpId] = trackDir.make<TH1F>(name.c_str(), name.c_str(), ((int)yRangeStrip) * 2, yOffsetStrip, yOffsetStrip + yRangeStrip);

    name = "timing_unmatched_Y";
    timingTrackUnmatchedY[tmpId] = trackDir.make<TH1F>(name.c_str(), name.c_str(), 40, yOffsetTiming, yOffsetTiming + 20);

    name = "stripTrackDistribution";
    allStripTracks[tmpId] = trackDir.make<TH2F>(name.c_str(), name.c_str(), 600, -15, 15, 2800, -70, 70 );
    name = "matchedStripTrackDistribution";
    matchedStripTracks[tmpId] = trackDir.make<TH2F>(name.c_str(), name.c_str(), 600, -15, 15, 2800, -70, 70 );
    name = "unmatchedStripTrackDistribution";
    unmatchedStripTracks[tmpId] = trackDir.make<TH2F>(name.c_str(), name.c_str(), 600, -15, 15, 2800, -70, 70 );
    name = "detectorEfficiency";
    detectorEfficiency[tmpId] = trackDir.make<TH2F>(name.c_str(), name.c_str(), 600, -15, 15, 2800, -70, 70 );

    name = "timingTrackDistributionX";
    trackXHistoMap[tmpId] = trackDir.make<TH1F>(name.c_str(), name.c_str(), 40, 0, 9 );
    name = "timingTrackDistributionY";
    trackYHistoMap[tmpId] = trackDir.make<TH1F>(name.c_str(), name.c_str(), 60, yOffsetTiming, yOffsetTiming + 20);
    name = "timingRecHitDistributionX";
    hitXHistoMap[tmpId] = trackDir.make<TH1F>(name.c_str(), name.c_str(), 40, 0, 9 );
    name = "timingRecHitDistributionY";
    hitYHistoMap[tmpId] = trackDir.make<TH1F>(name.c_str(), name.c_str(), 60, yOffsetTiming, yOffsetTiming + 20);

    std::string planeDirName = dirName + "/planeEfficiency";
    auto planeDir = fs->mkdir(planeDirName);

    for(int planeNo = 0; planeNo < 4; planeNo++) {

      tmpId.setChannel(0);
      std::string certainPlaneDirName = planeDirName + "/plane" + std::to_string(planeNo);
      auto certainPlaneDir = fs->mkdir(certainPlaneDirName);

      tmpId.setPlane(planeNo);

      name = "plane" + std::to_string(planeNo) + "stripTracksMatchedWithTimingHits";
      matchedStripPlaneHits[tmpId] = certainPlaneDir.make<TH2F>(name.c_str(), name.c_str(), 600, -15, 15, 2800, -70, 70 );

      name = "plane" + std::to_string(planeNo) + "stripTracksNotMatchedWithTimingHits";
      unmatchedStripPlaneHits[tmpId] = certainPlaneDir.make<TH2F>(name.c_str(), name.c_str(), 600, -15, 15, 2800, -70, 70 );

      name = "plane" + std::to_string(planeNo) + "efficiency";
      efficiencyPlaneHits[tmpId] = certainPlaneDir.make<TH2F>(name.c_str(), name.c_str(), 600, -15, 15, 2800, -70, 70 );

      for(int channelNo = 0; channelNo < 12; channelNo++) {
        std::string channelDirName = certainPlaneDirName + "/channel" + std::to_string(channelNo);
        auto channelDir = fs->mkdir(channelDirName);

        tmpId.setChannel(channelNo);
        name = "stripTracksMatchedWithTimingHits";
        matchedStripChannelHits[tmpId] = channelDir.make<TH2F>(name.c_str(), name.c_str(), 500, -5, 10, 2000, -25, 25 );
        name = "stripTracksNotMatchedWithTimingHits";
        unmatchedStripChannelHits[tmpId] = channelDir.make<TH2F>(name.c_str(), name.c_str(), 500, -5, 10, 2000, -25, 25 );
        name = "efficiency";
        efficiencyChannelHits[tmpId] = channelDir.make<TH2F>(name.c_str(), name.c_str(), 500, -5, 10, 2000, -25, 25 );

        name = "stripTracksMatchedDistributionX";
        matchedStripChannelHitsX[tmpId] = channelDir.make<TH1F>(name.c_str(), name.c_str(), 500, -5, 10 );
        name = "stripTracksMatchedDistributionY";
        matchedStripChannelHitsY[tmpId] = channelDir.make<TH1F>(name.c_str(), name.c_str(), 2000, -25, 25 );
        name = "efficiencyX";
        efficiencyChannelHitsX[tmpId] = channelDir.make<TH1F>(name.c_str(), name.c_str(), 500, -5, 10 );
        name = "efficiencyY";
        efficiencyChannelHitsY[tmpId] = channelDir.make<TH1F>(name.c_str(), name.c_str(), 2000, -25, 25 );
      }
    }
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


  for(int i = 0; i < 4; i++) {

    int arm = i / 2;
    int rp = i % 2;
    TotemTimingDetId tmpId(arm, 0, rp, 0, 0);

    auto totalPlot = (TH2F*)matchedStripTracks[tmpId]->Clone();
    totalPlot->Add(unmatchedStripTracks[tmpId]);
    detectorEfficiency[tmpId]->Add(matchedStripTracks[tmpId]);
    detectorEfficiency[tmpId]->Divide(totalPlot);

    for(int planeNo = 0; planeNo < 4; planeNo++) {

      tmpId.setPlane(planeNo);
      tmpId.setChannel(0);

      efficiencyPlaneHits[tmpId]->Add(matchedStripPlaneHits[tmpId]);
      auto totalPlot = (TH2F*)matchedStripPlaneHits[tmpId]->Clone();
      totalPlot->Add(unmatchedStripPlaneHits[tmpId]);
      efficiencyPlaneHits[tmpId]->Divide(totalPlot);

      for(int channelNo = 0; channelNo < 12; channelNo++) {
        tmpId.setChannel(channelNo);

        matchedStripChannelHitsX[tmpId]->Add(matchedStripChannelHits[tmpId]->ProjectionX());
        matchedStripChannelHitsY[tmpId]->Add(matchedStripChannelHits[tmpId]->ProjectionY());

        efficiencyChannelHits[tmpId]->Add(matchedStripChannelHits[tmpId]);
        auto totalPlot = (TH2F*)matchedStripChannelHits[tmpId]->Clone();
        totalPlot->Add(unmatchedStripChannelHits[tmpId]);
        efficiencyChannelHits[tmpId]->Divide(totalPlot);
/*
        efficiencyChannelHitsX[tmpId]->Add(matchedStripChannelHits[tmpId]->ProjectionX());
        efficiencyChannelHitsY[tmpId]->Add(matchedStripChannelHits[tmpId]->ProjectionY());


        auto totalPlotCut = (TH2F*)totalPlot->Clone();
        for(int binIter = 1; binIter <= totalPlot->GetNcells(); binIter++) {
          int matchedBinContent = matchedStripChannelHits[tmpId]->GetBinContent(binIter);
          if(matchedBinContent == 0)
            totalPlotCut->SetBinContent(binIter, 0);
        }

        auto xTotal = totalPlotCut->ProjectionX();
        auto yTotal = totalPlotCut->ProjectionY();
*/
        efficiencyChannelHitsX[tmpId]->Add(efficiencyChannelHits[tmpId]->ProjectionX());
        efficiencyChannelHitsY[tmpId]->Add(efficiencyChannelHits[tmpId]->ProjectionY());
      }
    }
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
