/****************************************************************************
*
* This is a part of CTPPS offline software.
* Authors:
*   Jan Kašpar (jan.kaspar@gmail.com)
*   Nicola Minafra
*   Laurent Forthomme
*
****************************************************************************/

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Run.h"

#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/Provenance/interface/EventRange.h"
#include "DataFormats/CTPPSDigi/interface/TotemVFATStatus.h"
#include "DataFormats/CTPPSDigi/interface/TotemFEDInfo.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSReco/interface/TotemRPLocalTrack.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"

#include "DataFormats/CTPPSDetId/interface/TotemTimingDetId.h"
#include "DataFormats/CTPPSDigi/interface/TotemTimingDigi.h"

#include "DataFormats/CTPPSReco/interface/CTPPSDiamondRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondLocalTrack.h"

#include <string>

//----------------------------------------------------------------------------------------------------

class TotemTimingDQMSource : public DQMEDAnalyzer
{
  public:
    TotemTimingDQMSource( const edm::ParameterSet& );
    ~TotemTimingDQMSource() override;

  protected:
    void dqmBeginRun( const edm::Run&, const edm::EventSetup& ) override;
    void bookHistograms( DQMStore::IBooker&, const edm::Run&, const edm::EventSetup& ) override;
    void analyze( const edm::Event&, const edm::EventSetup& ) override;
    void beginLuminosityBlock( const edm::LuminosityBlock&, const edm::EventSetup& ) override;
    void endLuminosityBlock( const edm::LuminosityBlock&, const edm::EventSetup& ) override;
    void endRun( const edm::Run&, const edm::EventSetup& ) override;

  private:
    // Constants
    static const double SEC_PER_LUMI_SECTION;                   // Number of seconds per lumisection: used to compute hit rates in Hz
    static const double DISPLAY_RESOLUTION_FOR_HITS_MM;         // Bin width of histograms showing hits and tracks (in mm)
    static const double INV_DISPLAY_RESOLUTION_FOR_HITS_MM;
    static const double SAMPIC_SAMPLING_PERIOD_NS;                        // ns per HPTDC bin
    static const int CTPPS_NUM_OF_ARMS;
    static const int TOTEM_TIMING_STATION_ID;
    static const int TOTEM_TIMING_MIN_RP_ID;
    static const int TOTEM_TIMING_MAX_RP_ID;
    static const int CTPPS_NEAR_RP_ID;
    static const int CTPPS_FAR_RP_ID;
    static const int TOTEM_TIMING_NUM_OF_PLANES;
    static const int TOTEM_TIMING_NUM_OF_CHANNELS;
    static const int TOTEM_TIMING_FED_ID_45;
    static const int TOTEM_TIMING_FED_ID_56;

    edm::EDGetTokenT< edm::DetSetVector<TotemRPLocalTrack> > tokenLocalTrack_;
    edm::EDGetTokenT< edm::DetSetVector<TotemTimingDigi> > tokenDigi_;
    edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondRecHit> > tokenDiamondHit_;
    edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondLocalTrack> > tokenDiamondTrack_;
    edm::EDGetTokenT< std::vector<TotemFEDInfo> > tokenFEDInfo_;

    double minimumStripAngleForTomography_;
    double maximumStripAngleForTomography_;
    unsigned int verbosity_;

    /// plots related to the whole system
    struct GlobalPlots
    {
      MonitorElement* h_trackCorr_hor = nullptr;

      GlobalPlots() {}
      GlobalPlots( DQMStore::IBooker& ibooker );
    };

    GlobalPlots globalPlot_;

    /// plots related to one Diamond detector package
    struct PotPlots
    {
      MonitorElement* activity_per_bx = nullptr;
      MonitorElement* activity_per_bx_fed = nullptr;

      MonitorElement* hitDistribution2d = nullptr;
      MonitorElement* hitDistribution2d_lumisection = nullptr;
      
      MonitorElement* digiDistribution = nullptr;
      MonitorElement* digiSent = nullptr;
      MonitorElement* digiAll = nullptr;
      MonitorElement* digiSentPercentage = nullptr;

      MonitorElement* activePlanes = nullptr;

      MonitorElement* trackDistribution = nullptr;

      MonitorElement* stripTomography210 = nullptr;
      MonitorElement* stripTomography220 = nullptr;

      MonitorElement* leadingEdge = nullptr;
      MonitorElement* amplitude = nullptr;
      
      std::set<unsigned int> planesWithHits;
      
      PotPlots() {};
      PotPlots( DQMStore::IBooker& ibooker, unsigned int id );
    };

    std::unordered_map<unsigned int, PotPlots> potPlots_;

    /// plots related to one Diamond plane
    struct PlanePlots
    {
      MonitorElement* digiDistribution = nullptr;

      MonitorElement* hitProfile = nullptr;
      MonitorElement* hit_multiplicity = nullptr;

      MonitorElement* stripTomography210 = nullptr;
      MonitorElement* stripTomography220 = nullptr;

      PlanePlots() {}
      PlanePlots( DQMStore::IBooker& ibooker, unsigned int id );
    };

    std::unordered_map<unsigned int, PlanePlots> planePlots_;

    /// plots related to one Diamond channel
    struct ChannelPlots
    {
      MonitorElement* activity_per_bx = nullptr;
      
      MonitorElement* leadingEdge = nullptr;
      MonitorElement* amplitude = nullptr;

      MonitorElement* hit_rate = nullptr;
      unsigned long hitsCounterPerLumisection;
      
      ChannelPlots() : hitsCounterPerLumisection( 0 ) {}
      ChannelPlots( DQMStore::IBooker &ibooker, unsigned int id );
    };

    std::unordered_map<unsigned int, ChannelPlots> channelPlots_;
};

//----------------------------------------------------------------------------------------------------

// Values for all constants
const double    TotemTimingDQMSource::SEC_PER_LUMI_SECTION = 23.31;
const double    TotemTimingDQMSource::DISPLAY_RESOLUTION_FOR_HITS_MM = 0.1;
const double    TotemTimingDQMSource::INV_DISPLAY_RESOLUTION_FOR_HITS_MM = 1./DISPLAY_RESOLUTION_FOR_HITS_MM;
const double    TotemTimingDQMSource::SAMPIC_SAMPLING_PERIOD_NS = 1./7.8e9;
const int       TotemTimingDQMSource::CTPPS_NUM_OF_ARMS = 2;
const int       TotemTimingDQMSource::TOTEM_TIMING_STATION_ID = 2;
const int       TotemTimingDQMSource::TOTEM_TIMING_MIN_RP_ID = 0;
const int       TotemTimingDQMSource::TOTEM_TIMING_MAX_RP_ID = 1;
const int       TotemTimingDQMSource::CTPPS_NEAR_RP_ID = 2;
const int       TotemTimingDQMSource::CTPPS_FAR_RP_ID = 3;
const int       TotemTimingDQMSource::TOTEM_TIMING_NUM_OF_PLANES = 4;
const int       TotemTimingDQMSource::TOTEM_TIMING_NUM_OF_CHANNELS = 12;
const int       TotemTimingDQMSource::TOTEM_TIMING_FED_ID_45 = FEDNumbering::MAXTotemRPTimingVerticalFEDID;
const int       TotemTimingDQMSource::TOTEM_TIMING_FED_ID_56 = FEDNumbering::MINTotemRPTimingVerticalFEDID;

//----------------------------------------------------------------------------------------------------

TotemTimingDQMSource::GlobalPlots::GlobalPlots( DQMStore::IBooker& ibooker )
{
  ibooker.setCurrentFolder( "CTPPS" );

  h_trackCorr_hor = ibooker.book2D( "track correlation all vertical", "rp, all, ver", 12, -0.5, 11.5, 12, -0.5, 11.5 );
  TH2F* hist = h_trackCorr_hor->getTH2F();
  TAxis* xa = hist->GetXaxis(), *ya = hist->GetYaxis();
  xa->SetBinLabel( 12, "45, 210, far, top" ); ya->SetBinLabel( 1, "45, 210, far, top" );
  xa->SetBinLabel( 11, "45, 220, timing, top" );  ya->SetBinLabel( 2, "45, 220, timing, top" );
  xa->SetBinLabel( 10, "45, 220, far, top" );  ya->SetBinLabel( 3, "45, 220, far, top" );
  xa->SetBinLabel( 9, "56, 210, far, top" ); ya->SetBinLabel( 4, "56, 210, far, top" );
  xa->SetBinLabel( 8, "56, 220, timing, top" );  ya->SetBinLabel( 5, "56, 220, timing, top" );
  xa->SetBinLabel( 7, "56, 220, far, top" );  ya->SetBinLabel( 6, "56, 220, far, top" );
  
  xa->SetBinLabel( 6, "45, 210, far, bot" ); ya->SetBinLabel( 7, "45, 210, far, bot" );
  xa->SetBinLabel( 5, "45, 220, timing, bot" );  ya->SetBinLabel( 8, "45, 220, timing, bot" );
  xa->SetBinLabel( 4, "45, 220, far, bot" );  ya->SetBinLabel( 9, "45, 220, far, bot" );
  xa->SetBinLabel( 3, "56, 210, far, bot" ); ya->SetBinLabel( 10, "56, 210, far, bot" );
  xa->SetBinLabel( 2, "56, 220, timing, bot" );  ya->SetBinLabel( 11, "56, 220, timing, bot" );
  xa->SetBinLabel( 1, "56, 220, far, bot" );  ya->SetBinLabel( 12, "56, 220, far, bot" );
}

//----------------------------------------------------------------------------------------------------


TotemTimingDQMSource::PotPlots::PotPlots( DQMStore::IBooker& ibooker, unsigned int id )
{
  std::string path, title;
  TotemTimingDetId( id ).rpName( path, TotemTimingDetId::nPath );
  ibooker.setCurrentFolder( path );

  TotemTimingDetId( id ).rpName( title, TotemTimingDetId::nFull );

  activity_per_bx = ibooker.book1D( "activity per BX CMS", title+" Activity per BX;Event.BX", 3600, -1.5, 3598. + 0.5 );
  activity_per_bx_fed = ibooker.book1D( "activity per BX FED", title+" Activity per BX;Event.BX", 3600, -1.5, 3598. + 0.5 );

  hitDistribution2d = ibooker.book2D( "hits in planes", title+" hits in planes;plane number;x (mm)", 10, -0.5, 4.5, 19.*INV_DISPLAY_RESOLUTION_FOR_HITS_MM, -1, 18 );
  hitDistribution2d_lumisection = ibooker.book2D( "hits in planes lumisection", title+" hits in planes in the last lumisection;plane number;x (mm)", 10, -0.5, 4.5, 19.*INV_DISPLAY_RESOLUTION_FOR_HITS_MM, -1, 18 );
  
  digiDistribution = ibooker.book2D( "digi distribution", title+" digi distribution;plane;channel", 10, -0.5, 4.5, 12, 0, 12);
  digiSent = ibooker.book2D( "digis sent", title+" digi sent (sampic);sampic;channel", 14, -0.5, 6.5, 16, 0, 16);
  digiAll = ibooker.book2D( "all digis", title+" all digis(sampic);sampic;channel", 14, -0.5, 6.5, 16, 0, 16);
  digiSentPercentage = ibooker.book2D( "sent digis percentage", title+" sent digis percentage(sampic);sampic;channel", 14, -0.5, 6.5, 16, 0, 16);
  
  activePlanes = ibooker.book1D( "active planes", title+" active planes (per event);number of active planes", 6, -0.5, 5.5 );

  trackDistribution = ibooker.book1D( "tracks", title+" tracks;x (mm)", 19.*INV_DISPLAY_RESOLUTION_FOR_HITS_MM, -1, 18 );

  stripTomography210 = ibooker.book2D( "tomography 210", title+" tomography with strips 210 (all planes);x + 25*plane(mm);y (mm)", 100, 0, 100, 12, -2, 10 );
  stripTomography220 = ibooker.book2D( "tomography 220", title+" tomography with strips 220 (all planes);x + 25*plane(mm);y (mm)", 100, 0, 100, 12, -2, 10 );

  leadingEdge = ibooker.book1D( "leading edge", title+" leading edge; leading edge (ns)", 125, 0, 125 );
  amplitude = ibooker.book1D( "amplitude", title+" amplitude; amplitude (V)", 100, 0, 2 );
}

//----------------------------------------------------------------------------------------------------

TotemTimingDQMSource::PlanePlots::PlanePlots( DQMStore::IBooker& ibooker, unsigned int id )
{
  std::string path, title;
  TotemTimingDetId( id ).planeName( path, TotemTimingDetId::nPath );
  ibooker.setCurrentFolder( path );

  TotemTimingDetId( id ).planeName( title, TotemTimingDetId::nFull );

  digiDistribution = ibooker.book2D( "digi distribution", title+" digi distribution;plane;channel", 10, -0.5, 4.5, 12, 0, 12);
  
  hitProfile = ibooker.book1D( "hit profile", title+" hit profile;x (mm)", 19.*INV_DISPLAY_RESOLUTION_FOR_HITS_MM, -1, 18 );
  hit_multiplicity = ibooker.book1D( "channels per plane", title+" channels per plane; ch per plane", 13, -0.5, 12.5 );

  stripTomography210 = ibooker.book2D( "tomography 210", title+" tomography with strips 210;x (mm);y (mm)", 100, 0, 100, 12, -2, 10 );
  stripTomography220 = ibooker.book2D( "tomography 220", title+" tomography with strips 220;x (mm);y (mm)", 100, 0, 100, 12, -2, 10 );
}

//----------------------------------------------------------------------------------------------------

TotemTimingDQMSource::ChannelPlots::ChannelPlots( DQMStore::IBooker& ibooker, unsigned int id ) : hitsCounterPerLumisection(0)
{
  std::string path, title;
  TotemTimingDetId( id ).channelName( path, TotemTimingDetId::nPath );
  ibooker.setCurrentFolder( path );

  TotemTimingDetId( id ).channelName( title, TotemTimingDetId::nFull );


  activity_per_bx = ibooker.book1D( "activity per BX", title+" Activity per BX;Event.BX", 1000, -1.5, 998. + 0.5 );
  

  hit_rate = ibooker.book1D( "hit rate", title+"hit rate;rate (Hz)", 40, 0, 20);
}

//----------------------------------------------------------------------------------------------------

TotemTimingDQMSource::TotemTimingDQMSource( const edm::ParameterSet& ps ) :
  tokenLocalTrack_  ( consumes< edm::DetSetVector<TotemRPLocalTrack> >     ( ps.getParameter<edm::InputTag>( "tagLocalTrack" ) ) ),
  tokenDigi_        ( consumes< edm::DetSetVector<TotemTimingDigi> >      ( ps.getParameter<edm::InputTag>( "tagDigi" ) ) ),
  tokenDiamondHit_  ( consumes< edm::DetSetVector<CTPPSDiamondRecHit> >    ( ps.getParameter<edm::InputTag>( "tagDiamondRecHits" ) ) ),
  tokenDiamondTrack_( consumes< edm::DetSetVector<CTPPSDiamondLocalTrack> >( ps.getParameter<edm::InputTag>( "tagDiamondLocalTracks" ) ) ),
  tokenFEDInfo_     ( consumes< std::vector<TotemFEDInfo> >                ( ps.getParameter<edm::InputTag>( "tagFEDInfo" ) ) ),
  minimumStripAngleForTomography_( ps.getParameter<double>( "minimumStripAngleForTomography" ) ),
  maximumStripAngleForTomography_( ps.getParameter<double>( "maximumStripAngleForTomography" ) ),
  verbosity_                     ( ps.getUntrackedParameter<unsigned int>( "verbosity", 0 ) )
{
}

//----------------------------------------------------------------------------------------------------

TotemTimingDQMSource::~TotemTimingDQMSource()
{}

//----------------------------------------------------------------------------------------------------

void
TotemTimingDQMSource::dqmBeginRun( const edm::Run& iRun, const edm::EventSetup& )
{
}


//----------------------------------------------------------------------------------------------------

void
TotemTimingDQMSource::bookHistograms( DQMStore::IBooker& ibooker, const edm::Run&, const edm::EventSetup& )
{
  ibooker.cd();
  ibooker.setCurrentFolder( "CTPPS" );

  globalPlot_= GlobalPlots( ibooker );

  for ( unsigned short arm = 0; arm < CTPPS_NUM_OF_ARMS; ++arm ) {
    for (unsigned short rp = TOTEM_TIMING_MIN_RP_ID; rp <= TOTEM_TIMING_MAX_RP_ID; ++rp) { 
      const TotemTimingDetId rpId( arm, TOTEM_TIMING_STATION_ID, rp );
      potPlots_[rpId] = PotPlots( ibooker, rpId );
      for ( unsigned short pl = 0; pl < TOTEM_TIMING_NUM_OF_PLANES; ++pl ) {
        const TotemTimingDetId plId( arm, TOTEM_TIMING_STATION_ID, rp, pl );
        planePlots_[plId] = PlanePlots( ibooker, plId);
        for ( unsigned short ch = 0; ch < TOTEM_TIMING_NUM_OF_CHANNELS; ++ch ) {
          const TotemTimingDetId chId( arm, TOTEM_TIMING_STATION_ID, rp, pl, ch );
          channelPlots_[chId] = ChannelPlots( ibooker, chId );
        }  
      }
    }
  }
}


//----------------------------------------------------------------------------------------------------

void
TotemTimingDQMSource::beginLuminosityBlock( const edm::LuminosityBlock&, const edm::EventSetup& ) 
{
  for ( auto& plot : potPlots_ ) 
    plot.second.hitDistribution2d_lumisection->Reset();
}

//----------------------------------------------------------------------------------------------------

void
TotemTimingDQMSource::analyze( const edm::Event& event, const edm::EventSetup& )
{
  // get event data
  edm::Handle< edm::DetSetVector<TotemRPLocalTrack> > stripTracks;
  event.getByToken( tokenLocalTrack_, stripTracks );

  edm::Handle< edm::DetSetVector<TotemTimingDigi> > timingDigis;
  event.getByToken( tokenDigi_, timingDigis );

  edm::Handle< std::vector<TotemFEDInfo> > fedInfo;
  event.getByToken( tokenFEDInfo_, fedInfo );

  edm::Handle< edm::DetSetVector<CTPPSDiamondRecHit> > diamondRecHits;
  event.getByToken( tokenDiamondHit_, diamondRecHits );

  edm::Handle< edm::DetSetVector<CTPPSDiamondLocalTrack> > diamondLocalTracks;
  event.getByToken( tokenDiamondTrack_, diamondLocalTracks );

  // check validity
  bool valid = true;
  valid &= timingDigis.isValid();
  valid &= fedInfo.isValid();

  if ( !valid ) {
    if ( verbosity_ ) {
      edm::LogProblem("TotemTimingDQMSource")
        << "ERROR in TotemDQMModuleRP::analyze > some of the required inputs are not valid. Skipping this event.\n"
        << "    timingDigis.isValid = " << timingDigis.isValid() << "\n"
        << "    fedInfo.isValid = " << fedInfo.isValid();
    }

    return;
  }

  //------------------------------
  // RP Plots
  //------------------------------  

  //------------------------------
  // Correlation Plots
  //------------------------------
  //TODO

  // Using TotemTimingDigi
  std::set<uint8_t> boardSet;
  for ( const auto& digis : *timingDigis ) {
    const TotemTimingDetId detId( digis.detId() );
    TotemTimingDetId detId_pot( digis.detId() );

    for ( const auto& digi : digis ) {
      detId_pot.setPlane( 0 );
      detId_pot.setChannel( 0 );
      if ( potPlots_.find( detId_pot ) != potPlots_.end() )
      {
        potPlots_[detId_pot].digiDistribution->Fill( detId.plane(), detId.channel() );
        float boardId =  digi.getEventInfo().getHardwareBoardId() + 0.5*digi.getEventInfo().getHardwareSampicId();
        potPlots_[detId_pot].digiSent->Fill( boardId, digi.getHardwareChannelId() );
        if ( boardSet.find( digi.getEventInfo().getHardwareId() ) == boardSet.end() )
        {
          // This guarantees that every board is counted only once
          boardSet.insert( digi.getEventInfo().getHardwareId() );
          std::bitset<16> chMap( digi.getEventInfo().getChannelMap() );
          for ( int i=0; i<16; ++i)
          {
            if ( chMap.test(i) ) potPlots_[detId_pot].digiAll->Fill( boardId, i );
          }
        }
        
        potPlots_[detId_pot].activity_per_bx->Fill( event.bunchCrossing() );
        
        for ( const auto& optorx : *fedInfo ) 
        {
          if ( ( detId.arm() == 1 && optorx.getFEDId() == TOTEM_TIMING_FED_ID_56 ) || ( detId.arm() == 0 && optorx.getFEDId() == TOTEM_TIMING_FED_ID_45 ) ) 
            potPlots_[detId_pot].activity_per_bx_fed->Fill( optorx.getBX() );
        }
        
        potPlots_[detId_pot].planesWithHits.insert( detId.plane() );
        
      }
    }
  }
  for ( auto& plt : potPlots_ ) {
    plt.second.activePlanes->Fill( plt.second.planesWithHits.size() );
    plt.second.planesWithHits.clear();
  }
  

}

//----------------------------------------------------------------------------------------------------

void
TotemTimingDQMSource::endLuminosityBlock( const edm::LuminosityBlock&, const edm::EventSetup& ) 
{
  // Efficeincy of Data Transmission from SAMPIC
  for ( auto& plot : potPlots_ ) {
    TH2F *hitHistoTmp = plot.second.digiSentPercentage->getTH2F();
    TH2F *histoSent = plot.second.digiSent->getTH2F();
    TH2F *histoAll = plot.second.digiAll->getTH2F();
    
    hitHistoTmp->Divide( histoSent, histoAll );
    hitHistoTmp->Scale(100);
  }
    
}

//----------------------------------------------------------------------------------------------------

void
TotemTimingDQMSource::endRun( const edm::Run&, const edm::EventSetup& )
{}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE( TotemTimingDQMSource );
