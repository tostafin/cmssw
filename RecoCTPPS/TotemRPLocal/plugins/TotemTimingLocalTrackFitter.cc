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

    int maxPlaneActiveChannels;

    edm::EDGetTokenT<edm::DetSetVector<TotemTimingRecHit> > recHitsToken_;
    TotemTimingTrackRecognition trk_algo_45_rp0;
    TotemTimingTrackRecognition trk_algo_56_rp0;
    TotemTimingTrackRecognition trk_algo_45_rp1;
    TotemTimingTrackRecognition trk_algo_56_rp1;
};

TotemTimingLocalTrackFitter::TotemTimingLocalTrackFitter( const edm::ParameterSet& iConfig ) :
  maxPlaneActiveChannels( iConfig.getParameter<int>( "maxPlaneActiveChannels" ) ),
  recHitsToken_( consumes<edm::DetSetVector<TotemTimingRecHit> >( iConfig.getParameter<edm::InputTag>( "recHitsTag" ) ) ),
  trk_algo_45_rp0 ( iConfig ),
  trk_algo_56_rp0 ( iConfig ),
  trk_algo_45_rp1 ( iConfig ),
  trk_algo_56_rp1 ( iConfig )
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

  const TotemTimingDetId id_45_rp0( 0, 1, 0, 0, 0 );
  const TotemTimingDetId id_56_rp0( 1, 1, 0, 0, 0 );
  const TotemTimingDetId id_45_rp1( 0, 1, 1, 0, 0 );
  const TotemTimingDetId id_56_rp1( 1, 1, 1, 0, 0 );

  pOut->find_or_insert( id_45_rp0 ); // tracks in 4-5
  pOut->find_or_insert( id_45_rp1 ); // tracks in 4-5
  pOut->find_or_insert( id_56_rp0 ); // tracks in 4-5
  pOut->find_or_insert( id_56_rp1 ); // tracks in 4-5

  //edm::DetSet<TotemTimingLocalTrack>& tracks56 = pOut->find_or_insert( id_56 ); // tracks in 5-6

  // workaround to retrieve the detset for 4-5 without losing the reference
  //edm::DetSet<TotemTimingLocalTrack>& tracks45 = pOut->operator[]( id_45 );

  // feed hits to the track producers
  for ( const auto& vec : *recHits ) {

    if (((int)vec.size()) > maxPlaneActiveChannels)
      continue;

    const TotemTimingDetId detId(vec.detId());

    // count number of hits for each plane
    //int planeHitCount[] = {0, 0, 0, 0};
    //for ( const auto& hit : vec )
      //planeHitCount[detId.plane()]++;

    for ( const auto& hit : vec ) {



      //std::cout << "arm=" << detId.arm() << ", rp=" << detId.rp();

      if(detId.arm() == 0 && detId.rp() == 0)
        trk_algo_45_rp0.addHit( hit );

      else if(detId.arm() == 0 && detId.rp() == 1)
        trk_algo_45_rp1.addHit( hit );

      else if(detId.arm() == 1 && detId.rp() == 0)
        trk_algo_56_rp0.addHit( hit );

      else if(detId.arm() == 1 && detId.rp() == 1)
        trk_algo_56_rp1.addHit( hit );

      else
        edm::LogWarning("TotemTimingLocalTrackFitter") << "Invalid detId for rechit: arm=" << detId.arm() << ", rp=" << detId.rp();
    }
  }

  // retrieve the tracks for both arms
  trk_algo_45_rp0.produceTracks( pOut->operator[](id_45_rp0) );
  trk_algo_56_rp0.produceTracks( pOut->operator[](id_56_rp0) );
  trk_algo_45_rp1.produceTracks( pOut->operator[](id_45_rp1) );
  trk_algo_56_rp1.produceTracks( pOut->operator[](id_56_rp1) );

  iEvent.put( std::move( pOut ) );

  // remove all hits from the track producers to prepare for the next event
  trk_algo_45_rp0.clear();
  trk_algo_45_rp1.clear();
  trk_algo_56_rp0.clear();
  trk_algo_56_rp1.clear();
}

void
TotemTimingLocalTrackFitter::fillDescriptions( edm::ConfigurationDescriptions& descr )
{
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>( "recHitsTag", edm::InputTag( "totemTimingRecHits" ) )
    ->setComment( "input rechits collection to retrieve" );
  desc.add<int>( "verbosity", 0 )
    ->setComment( "general verbosity of this module" );

  desc.add<double>( "threshold", 1.5 )
    ->setComment( "minimal number of rechits to be observed before launching the track recognition algorithm" );
  desc.add<double>( "thresholdFromMaximum", 0.5 )
    ->setComment( "threshold relative to hit profile function local maximum for determining the width of the track" );
  desc.add<double>( "resolution", 0.01 /* mm */ )
    ->setComment( "spatial resolution on the horizontal coordinate (in mm)" );
  desc.add<double>( "sigma", 0. )
    ->setComment( "pixel efficiency function parameter determining the smoothness of the step" );
  desc.add<double>( "tolerance", 0. /* mm */)
    ->setComment( "tolerance used for checking if the track contains certain hit" );
  desc.add<int>( "maxPlaneActiveChannels", 3 /* mm */)
    ->setComment( "threshold for discriminating noisy planes" );


  desc.add<std::string>( "pixelEfficiencyFunction", "(TMath::Erf((x-[0]+0.5*[1])/([2]/4)+2)+1)*TMath::Erfc((x-[0]-0.5*[1])/([2]/4)-2)/4" )
    ->setComment( "efficiency function for single pixel\n"
                  "can be defined as:\n"
                  " * Precise: (TMath::Erf((x-[0]+0.5*[1])/([2]/4)+2)+1)*TMath::Erfc((x-[0]-0.5*[1])/([2]/4)-2)/4\n"
                  " * Fast: (x>[0]-0.5*[1])*(x<[0]+0.5*[1])+((x-[0]+0.5*[1]+[2])/[2])*(x>[0]-0.5*[1]-[2])*(x<[0]-0.5*[1])+(2-(x-[0]-0.5*[1]+[2])/[2])*(x>[0]+0.5*[1])*(x<[0]+0.5*[1]+[2])\n"
                  " * Legacy: (1/(1+exp(-(x-[0]+0.5*[1])/[2])))*(1/(1+exp((x-[0]-0.5*[1])/[2])))\n"
                  "with:\n"
                  "  [0]: centre of pad\n"
                  "  [1]: width of pad\n"
                  "  [2]: sigma: distance between efficiency ~100 -> 0 outside width" );

  desc.add<double>( "yPosition", 0.0 )
    ->setComment( "vertical offset of the outcoming track centre" );
  desc.add<double>( "yWidth", 0.0 )
    ->setComment( "vertical track width" );

  descr.add( "totemTimingLocalTracks", desc );
}

DEFINE_FWK_MODULE( TotemTimingLocalTrackFitter );
