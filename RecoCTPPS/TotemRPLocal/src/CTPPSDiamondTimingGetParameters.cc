/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *   Nicola Minafra (nicola.minafra@cern.ch)
 *
 ****************************************************************************/

#include "RecoCTPPS/TotemRPLocal/interface/CTPPSDiamondTimingGetParameters.h"

#include <cmath>
#include <cstdio>
#include <algorithm>

/*
 * Internal ID: arm*1000 + plane*100 + channel
 */

//---------------------------------------------------------------------------------------------------

CTPPSDiamondTimingGetParameters::CTPPSDiamondTimingGetParameters( const edm::ParameterSet& iConfig ) 
{
  unsigned int arm(0), plane(0), channel(0);
  // Sector 45
  t0_map_[ arm*1000 + plane*100 + channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  
  plane = 1;
  channel = 0;
  t0_map_[ arm*1000 + plane*100 + channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  
  plane = 2;
  channel = 0;
  t0_map_[ arm*1000 + plane*100 + channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  
  plane = 3;
  channel = 0;
  t0_map_[ arm*1000 + plane*100 + channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  
  
  // Sector 56
  arm = 1;
  plane = 0;
  channel = 0;
  t0_map_[ arm*1000 + plane*100 + channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  
  plane = 1;
  channel = 0;
  t0_map_[ arm*1000 + plane*100 + channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  
  plane = 2;
  channel = 0;
  t0_map_[ arm*1000 + plane*100 + channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  
  plane = 3;
  channel = 0;
  t0_map_[ arm*1000 + plane*100 + channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;

}

//----------------------------------------------------------------------------------------------------

CTPPSDiamondTimingGetParameters::~CTPPSDiamondTimingGetParameters()
{}

//----------------------------------------------------------------------------------------------------

float CTPPSDiamondTimingGetParameters::getT0( const CTPPSDiamondDetId& detId )
{
  float t0 = .0;
  unsigned int internalID = 1000*detId.arm() + 100*detId.plane() + detId.channel();
  if ( t0_map_.find( internalID ) != t0_map_.end() ) t0 = t0_map_[ internalID ];
  return t0;
}

//----------------------------------------------------------------------------------------------------

std::vector<float> CTPPSDiamondTimingGetParameters::getTOTParameters( const CTPPSDiamondDetId& detId )
{
  std::vector<float> params;
  
  return params;
}

//----------------------------------------------------------------------------------------------------

float CTPPSDiamondTimingGetParameters::getPrecision( const CTPPSDiamondDetId& detId )
{
  float t0 = .0;
  return t0;
}

