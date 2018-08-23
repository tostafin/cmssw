/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *   Nicola Minafra (nicola.minafra@cern.ch)
 *   Mateusz Szpyrka (mateusz.szpyrka@cern.ch)
 *
 ****************************************************************************/

#ifndef RecoCTPPS_TotemRPLocal_TotemTimingTrackRecognition
#define RecoCTPPS_TotemRPLocal_TotemTimingTrackRecognition

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"

#include "DataFormats/CTPPSReco/interface/CTPPSTimingRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSTimingLocalTrack.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondLocalTrack.h"
#include "DataFormats/CTPPSReco/interface/TotemTimingLocalTrack.h"
#include "RecoCTPPS/TotemRPLocal/interface/CTPPSTimingTrackRecognition.h"


#include <string>
#include <cmath>
#include <set>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "TF1.h"

/**
 * Class intended to perform general CTPPS timing detectors track recognition,
 * as well as construction of specialized classes (for now CTPPSDiamond and TotemTiming local tracks).
**/

class TotemTimingTrackRecognition : public CTPPSTimingTrackRecognition<TotemTimingLocalTrack, TotemTimingRecHit>
{
  public:

    TotemTimingTrackRecognition(const edm::ParameterSet& parameters);

    // Adds new hit to the set from which the tracks are reconstructed.
    void addHit(const TotemTimingRecHit& recHit);

    /// Produces a collection of tracks for the current station, given its hits collection
    int produceTracks(edm::DetSet<TotemTimingLocalTrack>& tracks);
};

#include "RecoCTPPS/TotemRPLocal/src/TotemTimingTrackRecognition.cc"

#endif
