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
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.583;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.697;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.874;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.895;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.935;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.06;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.997;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.759;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.666;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.574;
  
  plane = 1;
  channel = 0;
  t0_map_[ arm*1000 + plane*100 + channel ] = 3.811;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.850;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.756;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.079;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.028;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.013;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.03;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.9076;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.878;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.528;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.449;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 2.899;
  
  plane = 2;
  channel = 0;
  t0_map_[ arm*1000 + plane*100 + channel ] = 3.988;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.678;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.686;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.864;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.055;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.034;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.022;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.166;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.9;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.739;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.541;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.534;
  
  plane = 3;
  channel = 0;
  t0_map_[ arm*1000 + plane*100 + channel ] = 3.437;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.623;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.779;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.981;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.213;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.381;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.299;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.055;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.894;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.712;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.598;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.475;
  
  
  // Sector 56
  arm = 1;
  plane = 0;
  channel = 0;
  t0_map_[ arm*1000 + plane*100 + channel ] = 3.488;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.071;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.333;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.596;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.929;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.826;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.906;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.980;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.739;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.616;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.384;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.163;
  
  plane = 1;
  channel = 0;
  t0_map_[ arm*1000 + plane*100 + channel ] = 3.328;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.624;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.524;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.854;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.217;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.149;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.871;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.910;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.766;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.305;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.084;
  
  plane = 2;
  channel = 0;
  t0_map_[ arm*1000 + plane*100 + channel ] = 3.065;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.24;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.487;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.631;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.856;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.941;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.930;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.039;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.809;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.675;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.456;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.317;
  
  plane = 3;
  channel = 0;
  t0_map_[ arm*1000 + plane*100 + channel ] = 3.559;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.763;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.88;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.178;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.474;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.697;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.603;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.138;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.031;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.825;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.672;
  t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.476;

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
  float t0 = 0.1;
  return t0;
}

