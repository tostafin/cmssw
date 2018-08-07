/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *   Nicola Minafra (nicola.minafra@cern.ch)
 *   Mateusz Szpyrka (mateusz.szpyrka@cern.ch)
 *
 ****************************************************************************/

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/Common/interface/DetSetVector.h"

#include "DataFormats/CTPPSDetId/interface/TotemTimingDetId.h"
#include "DataFormats/CTPPSReco/interface/TotemTimingRecHit.h"
#include "DataFormats/CTPPSReco/interface/TotemTimingLocalTrack.h"

#include "RecoCTPPS/TotemRPLocal/interface/TotemTimingTrackRecognition.h"

class TotemTimingLocalTrackFitter : public edm::stream::EDProducer<>
{
  public:
    explicit TotemTimingLocalTrackFitter( const edm::ParameterSet& );
    ~TotemTimingLocalTrackFitter() override;

    static void fillDescriptions( edm::ConfigurationDescriptions& );

  private:
    void produce( edm::Event&, const edm::EventSetup& ) override;

    edm::EDGetTokenT<edm::DetSetVector<TotemTimingRecHit> > recHitsToken_;
    TotemTimingTrackRecognition trk_algo_45_;
    TotemTimingTrackRecognition trk_algo_56_;
};

TotemTimingLocalTrackFitter::TotemTimingLocalTrackFitter( const edm::ParameterSet& iConfig ) :
  recHitsToken_( consumes<edm::DetSetVector<TotemTimingRecHit> >( iConfig.getParameter<edm::InputTag>( "recHitsTag" ) ) ),
  trk_algo_45_ ( iConfig.getParameter<edm::ParameterSet>( "trackingAlgorithmParams" ) ),
  trk_algo_56_ ( iConfig.getParameter<edm::ParameterSet>( "trackingAlgorithmParams" ) )
{
  produces<edm::DetSetVector<TotemTimingLocalTrack> >();
}

TotemTimingLocalTrackFitter::~TotemTimingLocalTrackFitter()
{}

void
TotemTimingLocalTrackFitter::produce( edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  std::unique_ptr<edm::DetSetVector<TotemTimingLocalTrack> > pOut( new edm::DetSetVector<TotemTimingLocalTrack> );

  edm::Handle<edm::DetSetVector<TotemTimingRecHit> > recHits;
  iEvent.getByToken( recHitsToken_, recHits );

  const TotemTimingDetId id_45( 0, 1, 6, 0, 0 ), id_56( 1, 1, 6, 0, 0 );

  pOut->find_or_insert( id_45 ); // tracks in 4-5
  edm::DetSet<TotemTimingLocalTrack>& tracks56 = pOut->find_or_insert( id_56 ); // tracks in 5-6

  // workaround to retrieve the detset for 4-5 without losing the reference
  edm::DetSet<TotemTimingLocalTrack>& tracks45 = pOut->operator[]( id_45 );

  // feed hits to the track producers
  for ( const auto& vec : *recHits ) {
    const TotemTimingDetId detid( vec.detId() );
    for ( const auto& hit : vec ) {

      switch ( detid.arm() ) {
        case 0: { trk_algo_45_.addHit( hit ); } break;
        case 1: { trk_algo_56_.addHit( hit ); } break;
        default:
          edm::LogWarning("TotemTimingLocalTrackFitter") << "Invalid arm for rechit: " << detid.arm();
          break;
      }
    }
  }

  // retrieve the tracks for both arms
  trk_algo_45_.produceTracks( tracks45 );
  trk_algo_56_.produceTracks( tracks56 );

  iEvent.put( std::move( pOut ) );

  // remove all hits from the track producers to prepare for the next event
  trk_algo_45_.clear();
  trk_algo_56_.clear();
}

void
TotemTimingLocalTrackFitter::fillDescriptions( edm::ConfigurationDescriptions& descr )
{
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>( "recHitsTag", edm::InputTag( "totemTimingRecHits" ) )
    ->setComment( "input rechits collection to retrieve" );
  desc.add<int>( "verbosity", 0 )
    ->setComment( "general verbosity of this module" );

  edm::ParameterSetDescription trackingAlgoParams;
  trackingAlgoParams.add<double>( "threshold", 1.5 )
    ->setComment( "minimal number of rechits to be observed before launching the track recognition algorithm" );
  trackingAlgoParams.add<double>( "thresholdFromMaximum", 0.5 )
    ->setComment( "threshold relative to hit profile function local maximum for determining the width of the track" );
  trackingAlgoParams.add<double>( "resolution", 0.01 /* mm */ )
    ->setComment( "spatial resolution on the horizontal coordinate (in mm)" );
  trackingAlgoParams.add<double>( "sigma", 0. )
    ->setComment( "pixel efficiency function parameter determining the smoothness of the step" );
  trackingAlgoParams.add<double>( "tolerance", 0.1 /* mm */)
    ->setComment( "tolerance used for checking if the track contains certain hit" );


  trackingAlgoParams.add<std::string>( "pixelEfficiencyFunction", "(TMath::Erf((x-[0]+0.5*[1])/([2]/4)+2)+1)*TMath::Erfc((x-[0]-0.5*[1])/([2]/4)-2)/4" )
    ->setComment( "efficiency function for single pixel\n"
                  "can be defined as:\n"
                  " * Precise: (TMath::Erf((x-[0]+0.5*[1])/([2]/4)+2)+1)*TMath::Erfc((x-[0]-0.5*[1])/([2]/4)-2)/4\n"
                  " * Fast: (x>[0]-0.5*[1])*(x<[0]+0.5*[1])+((x-[0]+0.5*[1]+[2])/[2])*(x>[0]-0.5*[1]-[2])*(x<[0]-0.5*[1])+(2-(x-[0]-0.5*[1]+[2])/[2])*(x>[0]+0.5*[1])*(x<[0]+0.5*[1]+[2])\n"
                  " * Legacy: (1/(1+exp(-(x-[0]+0.5*[1])/[2])))*(1/(1+exp((x-[0]-0.5*[1])/[2])))\n"
                  "with:\n"
                  "  [0]: centre of pad\n"
                  "  [1]: width of pad\n"
                  "  [2]: sigma: distance between efficiency ~100 -> 0 outside width" );

  trackingAlgoParams.add<double>( "yPosition", 0.0 )
    ->setComment( "vertical offset of the outcoming track centre" );
  trackingAlgoParams.add<double>( "yWidth", 0.0 )
    ->setComment( "vertical track width" );

  desc.add<edm::ParameterSetDescription>( "trackingAlgorithmParams", trackingAlgoParams )
    ->setComment( "list of parameters associated to the track recognition algorithm" );

  descr.add( "totemTimingLocalTracks", desc );
}

DEFINE_FWK_MODULE( TotemTimingLocalTrackFitter );
