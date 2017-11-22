/****************************************************************************
*
* This is a part of CTPPS offline software.
* Authors:
*   Laurent Forthomme (laurent.forthomme@cern.ch)
*
****************************************************************************/

#include "RecoCTPPS/TotemRPLocal/interface/CTPPSDiamondRecHitProducerAlgorithm.h"

//----------------------------------------------------------------------------------------------------

CTPPSDiamondRecHitProducerAlgorithm::CTPPSDiamondRecHitProducerAlgorithm( const edm::ParameterSet& iConfig ) :
  ts_to_ns_( iConfig.getParameter<double>( "timeSliceNs" ) ),
//   t_shift_( iConfig.getParameter<int>( "timeShift" ) ),      //TODO remove?
  plane_inversion_( iConfig.getParameter<int>( "planeInversion" ) ),      //TODO remove for PR
  coarse_correction_( iConfig.getParameter<int>( "coarseCorrection" ) ),      //TODO remove ?
  TOTCorrections_ ( iConfig )
{}

void
CTPPSDiamondRecHitProducerAlgorithm::build( const CTPPSGeometry* geom, const edm::DetSetVector<CTPPSDiamondDigi>& input, edm::DetSetVector<CTPPSDiamondRecHit>& output )
{
  for ( edm::DetSetVector<CTPPSDiamondDigi>::const_iterator vec = input.begin(); vec != input.end(); ++vec ) {
//     const CTPPSDiamondDetId detid( vec->detId() );           //TODO uncomment
    
    //FIXME patch for runs before 300670, not needed after rereco!
    CTPPSDiamondDetId detid( vec->detId() ); 
    if ( plane_inversion_>0 ) {
      if (detid.arm() == 0) {
        switch ( detid.plane() ) {
          case 0:
                     detid.setPlane(2);
                     break;
          case 1:
                     detid.setPlane(3);
                     break;
          case 2:
                     detid.setPlane(0);
                     break;
          case 3:
                     detid.setPlane(1);
                     break;    
          default:
                     std::cout << "ERROR!!! Strange plane number!" << std::endl;
        }
      }
    }
    //FIXME end of patch for runs before 300670


    if ( detid.channel() > 20 ) continue;              // VFAT-like information, to be ignored by CTPPSDiamondRecHitProducer

    const DetGeomDesc* det = geom->getSensor( detid );
    const float x_pos = det->translation().x(),
                x_width = 2.0 * det->params().at( 0 ), // parameters stand for half the size
                y_pos = det->translation().y(),
                y_width = 2.0 * det->params().at( 1 ),
                z_pos = det->translation().z(),
                z_width = 2.0 * det->params().at( 2 );  // TODO ?
                

    edm::DetSet<CTPPSDiamondRecHit>& rec_hits = output.find_or_insert( detid );

    for ( edm::DetSet<CTPPSDiamondDigi>::const_iterator digi = vec->begin(); digi != vec->end(); ++digi ) {
      if ( digi->getLeadingEdge()==0 and digi->getTrailingEdge()==0 ) { continue; }
      
      //FIXME patch for runs before 300670, not needed after rereco!
      if ( detid.arm() == 0 and detid.plane() == 0 and detid.channel() == 0 ) {
        std::cout << "###############################################################" << std::endl;
        std::cout << "###############################################################" << std::endl;
        std::cout << "###############################################################" << std::endl;
        std::cout << "###############################################################" << std::endl;
        std::cout << "###############################################################" << std::endl;
        std::cout << "###################   Forgot plane inversion?   ###############" << std::endl;
        std::cout << "###############################################################" << std::endl;
        std::cout << "###############################################################" << std::endl;
        std::cout << "###############################################################" << std::endl;
        std::cout << "###############################################################" << std::endl;
        std::cout << "In your python add: \nprocess.ctppsDiamondRecHits.planeInversion = cms.int32(1)" << std::endl;
    }
    //FIXME end of patch for runs before 300670
    
//     if ( detid.plane() == 3 ) continue;
      
      int t_shift_tmp = 0;
      if ( coarse_correction_ > 0 ) t_shift_tmp = TOTCorrections_.getCoarseAlignment(detid);
//       std::cout<<"\t\tHPTDC Corrections: " << t_shift_tmp << " for ch" <<  detid << std::endl;

      const int t = digi->getLeadingEdge();
      const int t0 = ( t-t_shift_tmp ) % 1024;
      int time_slice = ( t-t_shift_tmp ) / 1024;
      
      if ( t==0 ) time_slice= CTPPSDIAMONDRECHIT_WITHOUT_LEADING_TIMESLICE;

      int tot = 0;
      if ( t!=0 && digi->getTrailingEdge()!=0 ) tot = ( (int) digi->getTrailingEdge() ) - t;

      float t_corrected = TOTCorrections_.correctTiming(detid, ts_to_ns_ * t0, ts_to_ns_ * tot);
      
      rec_hits.push_back( CTPPSDiamondRecHit( x_pos, x_width, y_pos, y_width, z_pos, z_width, // spatial information
                                              ( t_corrected ),
                                              ( ts_to_ns_ * tot),
                                              TOTCorrections_.getPrecision(detid, tot ),
                                              time_slice,
                                              digi->getHPTDCErrorFlags(),
                                              digi->getMultipleHit() ) );
    }
  }
}
