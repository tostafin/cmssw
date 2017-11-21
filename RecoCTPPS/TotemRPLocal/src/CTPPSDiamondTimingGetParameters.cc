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
  // HPTDC delays (in bits: delay in ns * 1024/25=40.96) //TODO from mapping!
  unsigned int arm(0), plane(0), channel(0);
  // Sector 45
  //DB 0x40 HPTDC 0x18
  t0_shiftmap_[ arm*1000 + plane*100 + channel ] = 30.5*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 30.5*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 30.5*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 30.5*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 30.5*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 30.5*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 30.5*40.96;
  //DB 0x40 HPTDC 0x17
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 30.3*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 30.3*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 30.3*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 30.3*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 30.3*40.96;
  
  plane = 1;
  channel = 0;
  //DB 0x40 HPTDC 0x16
  t0_shiftmap_[ arm*1000 + plane*100 + channel ] = 17.5*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 17.5*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 17.5*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 17.5*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 17.5*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 17.5*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 17.5*40.96;
  //DB 0x40 HPTDC 0x15
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 30.5*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 30.5*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 30.5*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 30.5*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 30.5*40.96;
  
  plane = 2;
  channel = 0;
  //DB 0x41 HPTDC 0x18
  t0_shiftmap_[ arm*1000 + plane*100 + channel ] = 35.0*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 35.0*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 35.0*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 35.0*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 35.0*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 35.0*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 35.0*40.96;
  //DB 0x41 HPTDC 0x17
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 34.6*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 34.6*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 34.6*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 34.6*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 34.6*40.96;
  
  plane = 3;
  channel = 0;
  //DB 0x41 HPTDC 0x16
  t0_shiftmap_[ arm*1000 + plane*100 + channel ] = 22.1*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 22.1*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 22.1*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 22.1*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 22.1*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 22.1*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 22.1*40.96;
  //DB 0x41 HPTDC 0x15
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 35.0*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 35.0*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 35.0*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 35.0*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 35.0*40.96;
  
  
  // Sector 56
  arm = 1;
  plane = 0;
  channel = 0;
  //DB 0x50 HPTDC 0x18
  t0_shiftmap_[ arm*1000 + plane*100 + channel ] = 23.3*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 23.3*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 23.3*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 23.3*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 23.3*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 23.3*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 23.3*40.96;
  //DB 0x50 HPTDC 0x17
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 23.2*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 23.2*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 23.2*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 23.2*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 23.2*40.96;
  // relocated channels from DB 0x50 HPTDC 0x16...
  t0_shiftmap_[ 1*1000 + 1*100 + 0 ] = 23.2*40.96;
  t0_shiftmap_[ 1*1000 + 1*100 + 1 ] = 23.2*40.96;
  
  plane = 1;
  //DB 0x50 HPTDC 0x16
  t0_shiftmap_[ arm*1000 + 1*100 + 6 ] = 10.6*40.96;
  //DB 0x50 HPTDC 0x15
  channel = 7;
  t0_shiftmap_[ arm*1000 + plane*100 + channel ] = 23.4*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 23.4*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 23.4*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 23.4*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 23.4*40.96;
  // relocated channels from DB 0x50 HPTDC 0x16...
  t0_shiftmap_[ 1*1000 + 1*100 + 2 ] = 23.4*40.96;
  t0_shiftmap_[ 1*1000 + 1*100 + 3 ] = 23.4*40.96;
  
  plane = 2;
  channel = 0;
  //DB 0x51 HPTDC 0x18
  t0_shiftmap_[ arm*1000 + plane*100 + channel ] = 39.2*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 39.2*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 39.2*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 39.2*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 39.2*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 39.2*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 39.2*40.96;
  //DB 0x51 HPTDC 0x17
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 32.6*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 32.6*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 32.6*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 32.6*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 32.6*40.96;
  // relocated channels from DB 0x50 HPTDC 0x16...
  t0_shiftmap_[ 1*1000 + 1*100 + 4 ] = 32.6*40.96;
  t0_shiftmap_[ 1*1000 + 1*100 + 5 ] = 32.6*40.96;
  
  plane = 3;
  channel = 0;
  //DB 0x51 HPTDC 0x16
  t0_shiftmap_[ arm*1000 + plane*100 + channel ] = 20.2*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 20.2*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 20.2*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 20.2*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 20.2*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 20.2*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 20.2*40.96;
  //DB 0x51 HPTDC 0x15
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 33.2*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 33.2*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 33.2*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 33.2*40.96;
  t0_shiftmap_[ arm*1000 + plane*100 + ++channel ] = 33.2*40.96;
  
//   unsigned int arm(0), plane(0), channel(0);
//   // Sector 45
//   t0_map_[ arm*1000 + plane*100 + channel ] = .0;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.583;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.697;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.874;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.895;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.935;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.06;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.997;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.759;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.666;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.574;
//   
//   plane = 1;
//   channel = 0;
//   t0_map_[ arm*1000 + plane*100 + channel ] = 3.811;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.850;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.756;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.079;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.028;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.013;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.03;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.9076;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.878;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.528;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.449;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 2.899;
//   
//   plane = 2;
//   channel = 0;
//   t0_map_[ arm*1000 + plane*100 + channel ] = 3.988;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.678;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.686;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.864;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.055;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.034;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.022;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.166;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.9;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.739;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.541;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.534;
//   
//   plane = 3;
//   channel = 0;
//   t0_map_[ arm*1000 + plane*100 + channel ] = 3.437;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.623;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.779;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.981;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.213;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.381;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.299;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.055;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.894;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.712;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.598;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.475;
//   
//   
//   // Sector 56
//   arm = 1;
//   plane = 0;
//   channel = 0;
//   t0_map_[ arm*1000 + plane*100 + channel ] = 3.488;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.071;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.333;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.596;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.929;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.826;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.906;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.980;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.739;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.616;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.384;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.163;
//   
//   plane = 1;
//   channel = 0;
//   t0_map_[ arm*1000 + plane*100 + channel ] = 3.328;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.624;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.524;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.854;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.217;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.149;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.871;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.910;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.766;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = .0;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.305;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.084;
//   
//   plane = 2;
//   channel = 0;
//   t0_map_[ arm*1000 + plane*100 + channel ] = 3.065;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.24;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.487;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.631;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.856;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.941;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.930;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.039;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.809;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.675;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.456;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.317;
//   
//   plane = 3;
//   channel = 0;
//   t0_map_[ arm*1000 + plane*100 + channel ] = 3.559;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.763;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.88;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.178;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.474;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.697;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.603;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.138;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 4.031;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.825;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.672;
//   t0_map_[ arm*1000 + plane*100 + ++channel ] = 3.476;
  
  
// RUN 304671
// t0_map_[ 2 ] = 3.57602; // Error 0.0455118
// t0_map_[ 3 ] = 3.64872; // Error 0.0228649
// t0_map_[ 4 ] = 3.8505; // Error 0.0247964
// t0_map_[ 5 ] = 3.90304; // Error 0.0179811
// t0_map_[ 6 ] = 3.79516; // Error 0.0489422
// t0_map_[ 7 ] = 4.10264; // Error 0.0217118
// t0_map_[ 8 ] = 3.96682; // Error 0.0238202
// t0_map_[ 9 ] = 3.71115; // Error 0.0317986
// t0_map_[ 10 ] = 3.71078; // Error 0.0326722
// t0_map_[ 11 ] = 3.48429; // Error 0.054113
// t0_map_[ 100 ] = 3.50671; // Error 0.0717523
// t0_map_[ 101 ] = 3.80301; // Error 0.0440468
// t0_map_[ 102 ] = 3.74651; // Error 0.038688
// t0_map_[ 103 ] = 4.03062; // Error 0.0261744
// t0_map_[ 104 ] = 4.0197; // Error 0.0196944
// t0_map_[ 105 ] = 4.08355; // Error 0.0185428
// t0_map_[ 106 ] = 3.96135; // Error 0.0482007
// t0_map_[ 107 ] = 3.8816; // Error 0.0198546
// t0_map_[ 108 ] = 3.84039; // Error 0.0293206
// t0_map_[ 109 ] = 3.51244; // Error 0.0335613
// t0_map_[ 110 ] = 3.51055; // Error 0.0405147
// t0_map_[ 111 ] = 3.19638; // Error 0.0448481
// t0_map_[ 200 ] = 3.72317; // Error 0.0346538
// t0_map_[ 201 ] = 3.6321; // Error 0.0385127
// t0_map_[ 202 ] = 3.73219; // Error 0.0336628
// t0_map_[ 203 ] = 3.8608; // Error 0.0315378
// t0_map_[ 204 ] = 4.06665; // Error 0.0320542
// t0_map_[ 205 ] = 3.93027; // Error 0.0436962
// t0_map_[ 206 ] = 3.89521; // Error 0.0433479
// t0_map_[ 207 ] = 4.13858; // Error 0.0225973
// t0_map_[ 208 ] = 3.88456; // Error 0.0229812
// t0_map_[ 209 ] = 3.73411; // Error 0.0299461
// t0_map_[ 210 ] = 3.53615; // Error 0.0292733
// t0_map_[ 211 ] = 3.33516; // Error 0.0528799
// t0_map_[ 300 ] = 3.52948; // Error 0.036174
// t0_map_[ 301 ] = 3.59349; // Error 0.0353451
// t0_map_[ 302 ] = 3.8017; // Error 0.0380583
// t0_map_[ 303 ] = 3.98994; // Error 0.0318515
// t0_map_[ 304 ] = 4.19239; // Error 0.0298538
// t0_map_[ 305 ] = 4.41547; // Error 0.0240458
// t0_map_[ 306 ] = 4.28181; // Error 0.023196
// t0_map_[ 307 ] = 4.08096; // Error 0.0280373
// t0_map_[ 308 ] = 3.94154; // Error 0.0336058
// t0_map_[ 309 ] = 3.66839; // Error 0.0318278
// t0_map_[ 310 ] = 3.64187; // Error 0.0377913
// t0_map_[ 311 ] = 3.43144; // Error 0.0336024
// t0_map_[ 1000 ] = 3.43635; // Error 0.0417095
// t0_map_[ 1001 ] = 3.23791; // Error 0.0435441
// t0_map_[ 1002 ] = 3.54437; // Error 0.0356389
// t0_map_[ 1003 ] = 3.69284; // Error 0.0408672
// t0_map_[ 1004 ] = 3.98175; // Error 0.0373612
// t0_map_[ 1005 ] = 3.77837; // Error 0.0304807
// t0_map_[ 1006 ] = 3.83084; // Error 0.0391116
// t0_map_[ 1007 ] = 3.98229; // Error 0.0364559
// t0_map_[ 1008 ] = 3.81232; // Error 0.0438167
// t0_map_[ 1009 ] = 3.61724; // Error 0.0433108
// t0_map_[ 1010 ] = 3.34721; // Error 0.064158
// t0_map_[ 1011 ] = 3.32186; // Error 0.0490847
// t0_map_[ 1100 ] = 3.41388; // Error 0.0398518
// t0_map_[ 1101 ] = 3.7112; // Error 0.055313
// t0_map_[ 1102 ] = 3.55277; // Error 0.0514148
// t0_map_[ 1103 ] = 3.96009; // Error 0.0307133
// t0_map_[ 1104 ] = 4.20889; // Error 0.0440291
// t0_map_[ 1105 ] = 4.12029; // Error 0.027644
// t0_map_[ 1106 ] = 3.94222; // Error 0.0528941
// t0_map_[ 1107 ] = 3.90987; // Error 0.0300517
// t0_map_[ 1108 ] = 3.77947; // Error 0.0342847
// t0_map_[ 1110 ] = 3.51489; // Error 0.0413121
// t0_map_[ 1111 ] = 3.11137; // Error 0.0616709
// t0_map_[ 1200 ] = 2.96434; // Error 0.0248282
// t0_map_[ 1201 ] = 3.3798; // Error 0.0431965
// t0_map_[ 1202 ] = 3.50679; // Error 0.0412058
// t0_map_[ 1203 ] = 3.72358; // Error 0.043767
// t0_map_[ 1204 ] = 3.88598; // Error 0.0332882
// t0_map_[ 1205 ] = 3.94446; // Error 0.0464478
// t0_map_[ 1206 ] = 4.0269; // Error 0.0422306
// t0_map_[ 1207 ] = 4.03087; // Error 0.0364537
// t0_map_[ 1208 ] = 3.94154; // Error 0.0371679
// t0_map_[ 1209 ] = 3.66506; // Error 0.0351456
// t0_map_[ 1210 ] = 3.50653; // Error 0.0340036
// t0_map_[ 1211 ] = 3.52671; // Error 0.0427835
// t0_map_[ 1300 ] = 3.67202; // Error 0.0413316
// t0_map_[ 1301 ] = 3.80974; // Error 0.0432389
// t0_map_[ 1302 ] = 3.98367; // Error 0.0288777
// t0_map_[ 1303 ] = 4.22741; // Error 0.0297701
// t0_map_[ 1304 ] = 4.49806; // Error 0.0362891
// t0_map_[ 1305 ] = 4.75214; // Error 0.0276374
// t0_map_[ 1306 ] = 4.61467; // Error 0.0285102
// t0_map_[ 1307 ] = 4.29445; // Error 0.0417305
// t0_map_[ 1308 ] = 4.16042; // Error 0.0410321
// t0_map_[ 1309 ] = 3.87497; // Error 0.0318946
// t0_map_[ 1310 ] = 3.78044; // Error 0.0328614
// t0_map_[ 1311 ] = 3.55032; // Error 0.0336349

  
  //RUN 303819
/*  t0_map_[ 2 ] = 3.36546; // Error 0.0175403
  t0_map_[ 3 ] = 3.25237; // Error 0.0109004
  t0_map_[ 4 ] = 3.47265; // Error 0.011581
  t0_map_[ 5 ] = 3.46521; // Error 0.0100821
  t0_map_[ 6 ] = 3.50606; // Error 0.0206782
  t0_map_[ 7 ] = 3.56075; // Error 0.01111
  t0_map_[ 8 ] = 3.56896; // Error 0.0126556
  t0_map_[ 9 ] = 3.3968; // Error 0.0128481
  t0_map_[ 10 ] = 3.3877; // Error 0.0142863
  t0_map_[ 11 ] = 3.26704; // Error 0.0189305
  t0_map_[ 100 ] = 3.41297; // Error 0.0210314
  t0_map_[ 101 ] = 3.57827; // Error 0.0179465
  t0_map_[ 102 ] = 3.51045; // Error 0.0189757
  t0_map_[ 103 ] = 3.70164; // Error 0.0143039
  t0_map_[ 104 ] = 3.60979; // Error 0.0119364
  t0_map_[ 105 ] = 3.64357; // Error 0.0114905
  t0_map_[ 106 ] = 3.59941; // Error 0.0180807
  t0_map_[ 107 ] = 3.42926; // Error 0.00963743
  t0_map_[ 108 ] = 3.52404; // Error 0.0147582
  t0_map_[ 109 ] = 3.29721; // Error 0.0164086
  t0_map_[ 110 ] = 3.20308; // Error 0.0206057
  t0_map_[ 111 ] = 3.02639; // Error 0.0243816
  t0_map_[ 200 ] = 3.05459; // Error 0.00969234
  t0_map_[ 201 ] = 3.04344; // Error 0.0192004
  t0_map_[ 202 ] = 3.13864; // Error 0.0162744
  t0_map_[ 203 ] = 3.23921; // Error 0.0193291
  t0_map_[ 204 ] = 3.48959; // Error 0.018154
  t0_map_[ 205 ] = 3.24585; // Error 0.0193022
  t0_map_[ 206 ] = 3.21797; // Error 0.0192195
  t0_map_[ 207 ] = 3.4791; // Error 0.0133909
  t0_map_[ 208 ] = 3.2076; // Error 0.0136413
  t0_map_[ 209 ] = 3.16771; // Error 0.017115
  t0_map_[ 210 ] = 3.03529; // Error 0.0166197
  t0_map_[ 211 ] = 2.80677; // Error 0.019381
  t0_map_[ 300 ] = 2.94856; // Error 0.022304
  t0_map_[ 301 ] = 3.02152; // Error 0.0175081
  t0_map_[ 302 ] = 3.08345; // Error 0.0159063
  t0_map_[ 303 ] = 3.13509; // Error 0.012837
  t0_map_[ 304 ] = 3.29174; // Error 0.0124571
  t0_map_[ 305 ] = 3.42459; // Error 0.00969436
  t0_map_[ 306 ] = 3.33247; // Error 0.00910894
  t0_map_[ 307 ] = 2.93724; // Error 0.0104307
  t0_map_[ 308 ] = 2.78202; // Error 0.00968245
  t0_map_[ 309 ] = 2.68829; // Error 0.00989351
  t0_map_[ 310 ] = 2.6684; // Error 0.0104314
  t0_map_[ 311 ] = 2.69153; // Error 0.0129868
  t0_map_[ 1000 ] = 3.2279; // Error 0.0136047
  t0_map_[ 1001 ] = 3.0926; // Error 0.0241552
  t0_map_[ 1002 ] = 3.38067; // Error 0.0187801
  t0_map_[ 1003 ] = 3.45149; // Error 0.0180626
  t0_map_[ 1004 ] = 3.68917; // Error 0.0152679
  t0_map_[ 1005 ] = 3.41513; // Error 0.0141884
  t0_map_[ 1006 ] = 3.51453; // Error 0.0219245
  t0_map_[ 1007 ] = 3.61419; // Error 0.0147357
  t0_map_[ 1008 ] = 3.55953; // Error 0.0166114
  t0_map_[ 1009 ] = 3.44038; // Error 0.0186723
  t0_map_[ 1010 ] = 3.35514; // Error 0.019943
  t0_map_[ 1011 ] = 3.30838; // Error 0.0209364
  t0_map_[ 1100 ] = 3.48244; // Error 0.0168207
  t0_map_[ 1101 ] = 3.47658; // Error 0.0227097
  t0_map_[ 1102 ] = 3.40438; // Error 0.0238473
  t0_map_[ 1103 ] = 3.62737; // Error 0.0150576
  t0_map_[ 1104 ] = 3.5468; // Error 0.0182278
  t0_map_[ 1105 ] = 3.3584; // Error 0.0142769
  t0_map_[ 1106 ] = 3.58414; // Error 0.0238871
  t0_map_[ 1107 ] = 3.52763; // Error 0.0129347
  t0_map_[ 1108 ] = 3.53681; // Error 0.0153503
  t0_map_[ 1110 ] = 3.25373; // Error 0.0198788
  t0_map_[ 1111 ] = 3.08169; // Error 0.0220912
  t0_map_[ 1200 ] = 2.50937; // Error 0.0309937
  t0_map_[ 1201 ] = 2.7407; // Error 0.0206007
  t0_map_[ 1202 ] = 2.98591; // Error 0.0209495
  t0_map_[ 1203 ] = 3.06696; // Error 0.0237149
  t0_map_[ 1204 ] = 3.32831; // Error 0.0174071
  t0_map_[ 1205 ] = 3.25849; // Error 0.0213271
  t0_map_[ 1206 ] = 3.29396; // Error 0.0233389
  t0_map_[ 1207 ] = 3.51572; // Error 0.0181191
  t0_map_[ 1208 ] = 3.34301; // Error 0.0184914
  t0_map_[ 1209 ] = 3.11785; // Error 0.0216768
  t0_map_[ 1210 ] = 2.91079; // Error 0.0190869
  t0_map_[ 1211 ] = 2.82178; // Error 0.0200577
  t0_map_[ 1300 ] = 3.00877; // Error 0.0151646
  t0_map_[ 1301 ] = 3.20146; // Error 0.0152648
  t0_map_[ 1302 ] = 3.24181; // Error 0.0157437
  t0_map_[ 1303 ] = 3.38376; // Error 0.0152635
  t0_map_[ 1304 ] = 3.55449; // Error 0.0173552
  t0_map_[ 1305 ] = 3.69092; // Error 0.0112569
  t0_map_[ 1306 ] = 3.55062; // Error 0.0101994
  t0_map_[ 1307 ] = 3.09631; // Error 0.0157718
  t0_map_[ 1308 ] = 2.99225; // Error 0.0127903
  t0_map_[ 1309 ] = 2.87877; // Error 0.0134417
  t0_map_[ 1310 ] = 2.82755; // Error 0.0123306
  t0_map_[ 1311 ] = 2.7775; // Error 0.014653
  */
  //RUN 298854  Remember to check mapping in the unpacker
  t0_map_[ 0 ] = 6.01235; // Error 0.151432
  t0_map_[ 1 ] = 6.08194; // Error 0.0641096
  t0_map_[ 2 ] = 6.0827; // Error 0.162013
  t0_map_[ 3 ] = 6.19935; // Error 0.190524
  t0_map_[ 4 ] = 6.25321; // Error 0.0582182
  t0_map_[ 5 ] = 6.11795; // Error 0.341836
  t0_map_[ 6 ] = 6.3495; // Error 1.01097
  t0_map_[ 7 ] = 6.3557; // Error 0.121655
  t0_map_[ 8 ] = 6.16326; // Error 0.0496252
  t0_map_[ 9 ] = 6.08234; // Error 0.0838216
  t0_map_[ 10 ] = 6.14038; // Error 0.0619995
  t0_map_[ 11 ] = 6.14856; // Error 0.161696
  t0_map_[ 100 ] = 5.6114; // Error 0.116892
  t0_map_[ 101 ] = 5.5515; // Error 0.069573
  t0_map_[ 102 ] = 5.61321; // Error 0.0601495
  t0_map_[ 103 ] = 5.53484; // Error 0.973463
  t0_map_[ 104 ] = 5.77223; // Error 0.055002
  t0_map_[ 105 ] = 5.94299; // Error 0.0600058
  t0_map_[ 106 ] = 5.99172; // Error 0.06662
  t0_map_[ 107 ] = 4.42724; // Error 8.18213
  t0_map_[ 108 ] = 6.13513; // Error 0.495912
  t0_map_[ 109 ] = 5.94754; // Error 0.306876
  t0_map_[ 110 ] = 5.64968; // Error 0.0648766
  t0_map_[ 111 ] = 5.57648; // Error 0.0749048
  t0_map_[ 202 ] = 9.81644; // Error 0.296687
  t0_map_[ 203 ] = 9.50484; // Error 0.0506287
  t0_map_[ 204 ] = 9.69385; // Error 0.0584468
  t0_map_[ 205 ] = 9.85867; // Error 1.76453
  t0_map_[ 206 ] = 9.6978; // Error 0.288865
  t0_map_[ 207 ] = 9.75963; // Error 0.24286
  t0_map_[ 208 ] = 10.053; // Error 0.629051
  t0_map_[ 209 ] = 9.4357; // Error 0.117598
  t0_map_[ 210 ] = 9.74203; // Error 0.214481
  t0_map_[ 211 ] = 9.74852; // Error 0.254561
  t0_map_[ 300 ] = 10.4573; // Error 2.69445
  t0_map_[ 301 ] = 18.0452; // Error 6.22035
  t0_map_[ 302 ] = 13.3031; // Error 8.5117
  t0_map_[ 303 ] = 10.1526; // Error 0.929877
  t0_map_[ 304 ] = 10.0136; // Error 0.51339
  t0_map_[ 305 ] = 13.0241; // Error 9.18401
  t0_map_[ 306 ] = 11.4742; // Error 18.6155
  t0_map_[ 307 ] = 9.60585; // Error 0.0549525
  t0_map_[ 308 ] = 9.77363; // Error 0.131681
  t0_map_[ 309 ] = 9.66151; // Error 0.191514
  t0_map_[ 310 ] = 10.2762; // Error 2.22953
  t0_map_[ 311 ] = 9.52279; // Error 0.200529
  t0_map_[ 1000 ] = 4.46838; // Error 0.417697
  t0_map_[ 1001 ] = 5.60251; // Error 1.17508
  t0_map_[ 1002 ] = 6.52498; // Error 2.48427
  t0_map_[ 1003 ] = 5.02021; // Error 0.734911
  t0_map_[ 1004 ] = 4.93366; // Error 0.845514
  t0_map_[ 1005 ] = 4.25399; // Error 0.612517
  t0_map_[ 1006 ] = -2.73044; // Error 53.0539
  t0_map_[ 1007 ] = 6.72497; // Error 0.0624481
  t0_map_[ 1008 ] = 6.74585; // Error 0.0620446
  t0_map_[ 1009 ] = 6.72302; // Error 0.0824845
  t0_map_[ 1010 ] = 6.63056; // Error 0.0416726
  t0_map_[ 1011 ] = 6.46663; // Error 0.0723292
  t0_map_[ 1100 ] = 8.87916; // Error 4.83496
  t0_map_[ 1101 ] = 8.87916; // Error 4.83496
  t0_map_[ 1102 ] = 8.87916; // Error 4.83496
  t0_map_[ 1103 ] = 8.87916; // Error 4.83496
  t0_map_[ 1104 ] = 3.96008; // Error 37.3671
  t0_map_[ 1105 ] = 3.374; // Error 5.0125
  t0_map_[ 1106 ] = 6.94742; // Error 0.119408
  t0_map_[ 1107 ] = 6.4352; // Error 1.38068
  t0_map_[ 1108 ] = 5.81835; // Error 2.41683
  t0_map_[ 1109 ] = 6.28201; // Error 0.274251
  t0_map_[ 1110 ] = 5.95009; // Error 0.0785828
  t0_map_[ 1111 ] = 5.88698; // Error 0.111537

  


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

int CTPPSDiamondTimingGetParameters::getCoarseAlignment( const CTPPSDiamondDetId& detId )
{
  int t0 = 0;
  unsigned int internalID = 1000*detId.arm() + 100*detId.plane() + detId.channel();
  if ( t0_shiftmap_.find( internalID ) != t0_shiftmap_.end() ) t0 = t0_shiftmap_[ internalID ];
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

