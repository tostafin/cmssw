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
  tot_f_( "tot_TF1_CTPPS", iConfig.getParameter<std::string>( "totCorrectionFunction" ).c_str(), startFromT_, stopAtT_ ),
  params_              ( iConfig )
{}

//----------------------------------------------------------------------------------------------------

CTPPSDiamondTimingCorrection::~CTPPSDiamondTimingCorrection()
{}

//----------------------------------------------------------------------------------------------------

float
CTPPSDiamondTimingCorrection::correctTiming( const CTPPSDiamondDetId& detId, const float& t, const float& tot )
{
  // Set parameters in the TF1
  
  // Compute the corrected timing
//   std::cout<<"#### Correcting " << detId << " with t0 = " << params_.getT0( detId ) << std::endl;
//   return params_.getT0( detId ) - t + tot_f_.Eval( tot );
  float t_corrected = t;
  if ( t != .0 ) t_corrected -= params_.getT0( detId );
//   if ( tot != 0 ) t_corrected += tot_f_.Eval( tot );
  return t_corrected;
}

//----------------------------------------------------------------------------------------------------

float CTPPSDiamondTimingCorrection::getPrecision( const CTPPSDiamondDetId& detId, const bool withTot )
{
  float precision=.0;
  if (withTot) precision=0.1;
  else precision=0.3;
  return precision;
}

