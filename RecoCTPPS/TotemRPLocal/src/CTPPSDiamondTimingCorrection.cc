/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *   Nicola Minafra (nicola.minafra@cern.ch)
 *
 ****************************************************************************/

#include "RecoCTPPS/TotemRPLocal/interface/CTPPSDiamondTimingCorrection.h"

#include <cmath>
#include <cstdio>
#include <algorithm>

//----------------------------------------------------------------------------------------------------

CTPPSDiamondTimingCorrection::CTPPSDiamondTimingCorrection( const edm::ParameterSet& iConfig ) :
  startFromT_          ( iConfig.getParameter<double>( "startFromT" ) ),
  stopAtT_             ( iConfig.getParameter<double>( "stopAtT" ) ),
  tot_f_( "tot_TF1_CTPPS", iConfig.getParameter<std::string>( "totCorrectionFunction" ).c_str(), startFromT_, stopAtT_ )
{}

//----------------------------------------------------------------------------------------------------

CTPPSDiamondTimingCorrection::~CTPPSDiamondTimingCorrection()
{}

//----------------------------------------------------------------------------------------------------

float
CTPPSDiamondTimingCorrection::correctTiming( const CTPPSDiamondDetId& detId, const CTPPSDiamondRecHit& recHit )
{
  CTPPSDiamondRecHit recHitLocal = recHit;  
  // Set parameters in the TF1
  
  // Compute the corrected timing
  return params_.getT0( detId ) + recHitLocal.getT() + tot_f_.Eval( recHitLocal.getToT() );
}

//----------------------------------------------------------------------------------------------------

float CTPPSDiamondTimingCorrection::getPrecision( const CTPPSDiamondDetId& detId )
{
  return 0.1;
}

