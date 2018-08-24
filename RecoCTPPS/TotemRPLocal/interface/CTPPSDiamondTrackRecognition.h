/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *   Nicola Minafra (nicola.minafra@cern.ch)
 *
 ****************************************************************************/

#ifndef RecoCTPPS_TotemRPLocal_CTPPSDiamondTrackRecognition
#define RecoCTPPS_TotemRPLocal_CTPPSDiamondTrackRecognition

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"

#include "DataFormats/CTPPSReco/interface/CTPPSDiamondRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondLocalTrack.h"

#include "RecoCTPPS/TotemRPLocal/interface/CTPPSTimingTrackRecognition.h"

#include <vector>
#include <unordered_map>
#include "TF1.h"

/**
 * \brief Class performing smart reconstruction for CTPPS Diamond Detectors.
 * \date Jan 2017
**/
class CTPPSDiamondTrackRecognition : public CTPPSTimingTrackRecognition<CTPPSDiamondLocalTrack, CTPPSDiamondRecHit>
{
  public:
    CTPPSDiamondTrackRecognition( const edm::ParameterSet& );

    void clear();

    /// Feed a new hit to the tracks recognition algorithm
    void addHit( const CTPPSDiamondRecHit& recHit );

    /// Produce a collection of tracks for the current station, given its hits collection
    int produceTracks( edm::DetSet<CTPPSDiamondLocalTrack>& tracks );

  private:

    std::unordered_map<int,int> mhMap_;
};

#include "RecoCTPPS/TotemRPLocal/src/CTPPSDiamondTrackRecognition.cc"


#endif
