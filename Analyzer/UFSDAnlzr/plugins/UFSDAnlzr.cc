// -*- C++ -*-
//
// Package:    Analyzer/UFSDAnlzr
// Class:      UFSDAnlzr
//
/**\class UFSDAnlzr UFSDAnlzr.cc Analyzer/UFSDAnlzr/plugins/UFSDAnlzr.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Nicola Minafra
//         Created:  Sat, 24 Mar 2018 17:06:55 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/DetSetVector.h"


#include "FWCore/ParameterSet/interface/ParameterSet.h"
 #include "FWCore/Utilities/interface/InputTag.h"
 #include "DataFormats/TrackReco/interface/Track.h"
 #include "DataFormats/TrackReco/interface/TrackFwd.h"
 #include "FWCore/ServiceRegistry/interface/Service.h"
 #include "CommonTools/UtilAlgos/interface/TFileService.h"
 #include "TH1.h"
 #include "TGraph.h"
 
 #include "DataFormats/CTPPSDigi/interface/TotemTimingDigi.h"
 #include "DataFormats/CTPPSDetId/interface/TotemTimingDetId.h"
 
#include <map>

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.


using reco::TrackCollection;

class UFSDAnlzr : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit UFSDAnlzr(const edm::ParameterSet&);
      ~UFSDAnlzr();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      uint16_t timestampAConverter( const uint16_t& binary) const;

      // ---------- constants ---------------------------
      static const double SAMPIC_MAX_NUMBER_OF_SAMPLES;
      static const double SAMPIC_ADC_V;
      
      // ---------- directories ---------------------------
      TFileDirectory subdir_45_bot_[4], subdir_45_top_[4], subdir_56_bot_[4], subdir_56_top_[4];
      
      // ----------member data ---------------------------
      unsigned int verbosity_;
      const double samplingPeriod_;
      
      edm::EDGetTokenT< edm::DetSetVector<TotemTimingDigi> > tokenDigi_;
       
      std::map<TotemTimingDetId, TH1F*> allSamples_map_;
      std::map<TotemTimingDetId, TH1F*> triggerCellTimeInstant_map_;
      std::map<TotemTimingDetId, TGraph*> graphADC_map_;
      std::map<TotemTimingDetId, int> graphADC_counterMap_;
};

//
// constants, enums and typedefs
//
const double    UFSDAnlzr::SAMPIC_MAX_NUMBER_OF_SAMPLES = 64;
const double    UFSDAnlzr::SAMPIC_ADC_V = 1./256;

//
// static data member definitions
//

//
// constructors and destructor
//
UFSDAnlzr::UFSDAnlzr(const edm::ParameterSet& iConfig)
 :
  verbosity_            ( iConfig.getUntrackedParameter<unsigned int>( "verbosity", 0 ) ),
  samplingPeriod_       ( 1./iConfig.getUntrackedParameter<double>( "samplingFrequency", 7.8 ) ),
  tokenDigi_            ( consumes< edm::DetSetVector<TotemTimingDigi> >      ( iConfig.getParameter<edm::InputTag>( "tagDigi" ) ) )

{
   //now do what ever initialization is needed
   usesResource("TFileService");
//    edm::Service<TFileService> fs;

}


UFSDAnlzr::~UFSDAnlzr()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
UFSDAnlzr::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle< edm::DetSetVector<TotemTimingDigi> > timingDigi;
  iEvent.getByToken( tokenDigi_, timingDigi );
       
  // check validity
  bool valid = true;
//   valid &= timingDigi.isValid();

  if ( !valid ) {
    if ( verbosity_ ) {
      edm::LogProblem("TimingAnalyzer")
        << "ERROR in UFSDAnlzr::analyze > some of the required inputs are not valid. Skipping this event." << std::endl
        << "    timingDigi.isValid = " << timingDigi.isValid() << std::endl;
    }
    return;
  }
  
  edm::Service<TFileService> fs;
  
  for (int i=0; i<4; ++i) {
    subdir_45_top_[i] = fs->mkdir( "45_top_"+std::to_string(i) );
    subdir_45_bot_[i] = fs->mkdir( "45_bot_"+std::to_string(i) );
    subdir_56_top_[i] = fs->mkdir( "56_top_"+std::to_string(i) );
    subdir_56_bot_[i] = fs->mkdir( "56_bot_"+std::to_string(i) );
  }
  
  for (const auto& digis : *timingDigi) {
    const TotemTimingDetId detId( digis.detId() );
    TFileDirectory* subdir_rp = nullptr;
    if ( detId.arm() == 0 ) 
    {
      if ( detId.rp() == 0 )
        subdir_rp = & subdir_45_top_[ detId.plane() ];
      else if ( detId.rp() == 1 )
        subdir_rp = & subdir_45_bot_[ detId.plane() ];
    }
    else if ( detId.arm() == 1 ) 
    {
      if ( detId.rp() == 0 )
        subdir_rp = & subdir_56_top_[ detId.plane() ];
      else if ( detId.rp() == 1 )
        subdir_rp = & subdir_56_bot_[ detId.plane() ];
    }
    if ( subdir_rp == nullptr ) {
      std::cout<<detId<<std::endl;
      continue;
    }
    
    std::string name;
    detId.channelName(name);
    name.insert(0,"allSamples");
    std::string name2;
    detId.channelName(name2);
    name2.insert(0,"triggerCellTimeInstant");
    if ( allSamples_map_.find( detId ) == allSamples_map_.end() ) {
      allSamples_map_[ detId ] = subdir_rp->make<TH1F>(name.c_str(), name.c_str(), 100, 0, 1 );
      triggerCellTimeInstant_map_[ detId ] = subdir_rp->make<TH1F>(name2.c_str(), name2.c_str(), 100, -50, 50 );
      graphADC_map_[ detId ] = subdir_rp->make<TGraph>();
      graphADC_map_[ detId ]->SetName( name.c_str() );
      graphADC_map_[ detId ]->SetTitle( name.c_str() );
      graphADC_counterMap_[ detId ] = 0;
    }
    for (const auto& digi : digis ) {
      // Time of samples
      int cell0TimeClock;
      double cell0TimeInstant;  // Time of first cell
      double triggerCellTimeInstant;    // Time of triggered cell
      
//       uint16_t timestampA = timestampAConverter( digi.getTimestampA() );
      unsigned int timestamp = digi.getCellInfo() <= 32 ? digi.getTimestampA() : digi.getTimestampB();
      
      cell0TimeClock =  timestamp + ( ( digi.getFPGATimestamp() - timestamp ) & 0xFFFFFFF000 ) - digi.getEventInfo().getL1ATimestamp() + digi.getEventInfo().getL1ALatency(); 
      if ( std::abs( cell0TimeClock ) > 1 ) std::cout << "Strange cell0TimeClock: " << cell0TimeClock << std::endl;
      cell0TimeInstant = samplingPeriod_ * SAMPIC_MAX_NUMBER_OF_SAMPLES * cell0TimeClock;
      
      if ( digi.getCellInfo() < 30 )    
        triggerCellTimeInstant = cell0TimeInstant + samplingPeriod_ * (int) digi.getCellInfo();
      else
        triggerCellTimeInstant = cell0TimeInstant - samplingPeriod_ * ( SAMPIC_MAX_NUMBER_OF_SAMPLES - (int) digi.getCellInfo() );   
      // End time of samples
      
      int i=0;
      for ( auto sampIt = digi.getSamplesBegin(); sampIt != digi.getSamplesEnd(); ++sampIt) {
        triggerCellTimeInstant_map_[ detId ]->Fill( triggerCellTimeInstant );
        allSamples_map_[ detId ]->Fill( SAMPIC_ADC_V * *sampIt );
        graphADC_map_[ detId ]->SetPoint( (graphADC_counterMap_[ detId ])++, triggerCellTimeInstant + samplingPeriod_*(i++), SAMPIC_ADC_V * *sampIt );
      }
    }
  }
 
}


// ------------ method called once each job just before starting event loop  ------------
void
UFSDAnlzr::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
UFSDAnlzr::endJob()
{
  std::cout << "RP\tPlane\tCh\t\tMean\t\tRMS" << std::endl;
  for (const auto& histo:allSamples_map_ )
  {
    if ( histo.first.arm() == 0 ) std::cout << "45 ";
    else if ( histo.first.arm() == 1 ) std::cout << "56 ";
    if ( histo.first.rp() == 0 ) std::cout << "top";
    else if ( histo.first.rp() == 1 ) std::cout << "bot";
    std::cout << "\t" << histo.first.plane() << "\t" << histo.first.channel() << "\t\t" << std::setprecision(4) << std::setfill(' ') << std::setw(6) << histo.second->GetMean() << "\t\t" <<  histo.second->GetRMS() << std::endl;
  }
  for (const auto& graph:graphADC_map_ )
  {
    graph.second->GetYaxis()->SetRangeUser(0,1);
  }
  
  
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
UFSDAnlzr::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
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

uint16_t UFSDAnlzr::timestampAConverter(const uint16_t& binary) const
{
  uint16_t gray;
  gray = binary & 0x800;
  for (unsigned short int i = 1; i < 12; ++i)
    gray |= ( binary ^ ( binary >> 1 ) ) & (0x0001 << ( 11 - i ) );

  gray = 0xFFF - gray;
  
  uint16_t binaryOut = 0;
  binaryOut = gray & 0x800;
  for (unsigned short int i = 1; i < 12; ++i)
    binaryOut |= ( gray ^ ( binaryOut >> 1 ) ) & (0x0001 << ( 11 - i ) );
  
  return binaryOut;
}

//define this as a plug-in
DEFINE_FWK_MODULE(UFSDAnlzr);
