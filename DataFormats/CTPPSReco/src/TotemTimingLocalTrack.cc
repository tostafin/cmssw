/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Nicola Minafra (nicola.minafra@cern.ch)
 *   Mateusz Szpyrka (mateusz.szpyrka@cern.ch)
 *
 ****************************************************************************/


#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/CTPPSReco/interface/TotemTimingLocalTrack.h"

//----------------------------------------------------------------------------------------------------

//====================================================================================================
// TotemTimingLocalTrack class implementation
//====================================================================================================

//--- constructors

TotemTimingLocalTrack::TotemTimingLocalTrack() : CTPPSTimingLocalTrack() {}

TotemTimingLocalTrack::TotemTimingLocalTrack(
    const math::XYZPoint& pos0,
    const math::XYZPoint& pos0_sigma,
    float t,
    float t_sigma) : 
  CTPPSTimingLocalTrack(pos0, pos0_sigma, t, t_sigma) {}
