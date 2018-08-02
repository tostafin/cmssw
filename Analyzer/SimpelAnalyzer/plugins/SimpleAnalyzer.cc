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

      void initHistograms( const TotemTimingDetId&);

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

      // ---------- histograms ---------------------------
      std::map< TotemTimingDetId, TH1F*> yHisto_map_;
      std::map< TotemTimingDetId, TH1F*> tHisto_map_;

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

void
SimpleAnalyzer::initHistograms(const TotemTimingDetId& detId)
{
  //now do what ever initialization is needed
  edm::Service<TFileService> fs;
  if (maindir_map_.find(detId) == maindir_map_.end())
  {
    std::string dirName;
    detId.rpName(dirName, TotemTimingDetId::nPath);
    std::string chName;
    detId.channelName(chName, TotemTimingDetId::nFull);

    // create directory for the detector, if not already done
    maindir_map_[ detId ] = fs->mkdir( dirName );

    // create all histograms
    std::string yHisto_name(chName);
    yHisto_name.insert(0, "yDistribution_");
    yHisto_map_[ detId ] = maindir_map_[ detId ].make<TH1F>(yHisto_name.c_str(), yHisto_name.c_str(), 120, -60, 60 );

    std::string tHisto_name(chName);
    tHisto_name.insert(0, "tDistribution_");
    tHisto_map_[ detId ] = maindir_map_[ detId ].make<TH1F>(tHisto_name.c_str(), tHisto_name.c_str(), 100, -20, 20 );

    std::string samples_graph_name(chName);
    samples_graph_name.insert(0, "samples_graph_");
    samples_graph_map_[ detId ] = maindir_map_[ detId ].make<TGraph>();
    samples_graph_map_[ detId ]->SetName(samples_graph_name.c_str());
    samples_graph_map_[ detId ]->SetTitle(samples_graph_name.c_str());
    samples_graphCtr_map_[ detId ] = 0;
  }

}

// ------------ method called for each event  ------------
void
SimpleAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  edm::Handle< edm::DetSetVector<TotemTimingDigi> > timingDigi;
  edm::Handle< edm::DetSetVector<TotemTimingRecHit> > timingRecHit;
  edm::Handle< edm::DetSetVector<TotemTimingLocalTrack> > timingLocalTrack;
  iEvent.getByToken( tokenDigi_, timingDigi );
  iEvent.getByToken( tokenRecHit_, timingRecHit );
  iEvent.getByToken( tokenLocalTrack_, timingLocalTrack );

  for (const auto& digis : *timingDigi)
  {
    const TotemTimingDetId detId( digis.detId() );
    if (maindir_map_.find(detId) == maindir_map_.end())
      initHistograms( detId );
    for (const auto& digi : digis )
    {
      // Do stuff on DIGIs
      int i=0;
      for ( auto sampIt = digi.getSamplesBegin(); sampIt != digi.getSamplesEnd(); ++sampIt)
        samples_graph_map_[ detId ]->SetPoint(samples_graphCtr_map_[ detId ]++, SAMPIC_SAMPLING_PERIOD_NS*(i++), *sampIt  );
    }
  }

  for (const auto& recHits : *timingRecHit)
  {
    const TotemTimingDetId detId( recHits.detId() );
    if (maindir_map_.find(detId) == maindir_map_.end())
      initHistograms( detId );
    for (const auto& recHit : recHits )
    {
      // Do stuff on recHits
      yHisto_map_[ detId ]->Fill( recHit.getY() );
      tHisto_map_[ detId ]->Fill( recHit.getT() );

    }
  }

  int iter = 0;
  for (const auto& trackSet : *timingLocalTrack)
  {
    for(auto track: trackSet) {
      std::cout << iter << ": x = " << track.getX0() << ", y = " << track.getY0() << std::endl;
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
