/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *   Nicola Minafra (nicola.minafra@cern.ch)
 *   Mateusz Szpyrka (mateusz.szpyrka@cern.ch)
 *
 ****************************************************************************/

#ifndef RecoCTPPS_TotemRPLocal_CTPPSTimingTrackRecognition
#define RecoCTPPS_TotemRPLocal_CTPPSTimingTrackRecognition

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"

#include "DataFormats/CTPPSReco/interface/CTPPSTimingRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSTimingLocalTrack.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondLocalTrack.h"
#include "DataFormats/CTPPSReco/interface/TotemTimingLocalTrack.h"

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
template<typename TRACK_TYPE, typename HIT_TYPE>
class CTPPSTimingTrackRecognition
{
  public:

    CTPPSTimingTrackRecognition(const edm::ParameterSet& parameters);

    ~CTPPSTimingTrackRecognition();


    // Class API:

    // Resets internal state of a class instance.
    virtual void clear();


    // Adds new hit to the set from which the tracks are reconstructed.
    virtual void addHit(const HIT_TYPE& recHit) = 0;


    // Produces a collection of tracks, given its hits collection
    virtual int produceTracks(edm::DetSet<TRACK_TYPE>& tracks) = 0;



  protected:

    // Algorithm parameters:
    float threshold;
    float thresholdFromMaximum;
    float resolution;
    float sigma;
    TF1 pixelEfficiencyFunction;


    typedef std::vector<HIT_TYPE> HitVector;
    typedef std::unordered_map<int, HitVector> HitVectorMap;


    /* Stores RecHit vectors that should be processed separately while reconstructing tracks. */
    HitVectorMap hitVectorMap;


    /* Structure representing parameters set for single dimension.
     * Intended to use when producing partial tracks.
     */
    struct DimensionParameters {
      float threshold;
      float thresholdFromMaximum;
      float resolution;
      float sigma;
      float rangeBegin;
      float rangeEnd;
      TF1 hitFunction;
    };


    /* Structure representing 3D range in space.
     */
    struct SpatialRange {
      float xBegin;
      float xEnd;
      float yBegin;
      float yEnd;
      float zBegin;
      float zEnd;
    };


    /* Produces all partial tracks from given set with regard to single dimension.
     *
     * @hits: vector of hits from which the tracks are created
     * @param: describes all parameters used by 1D track recognition algorithm
     * @getHitCenter: function extracting hit's center in the dimension
     *      that the partial tracks relate to
     * @getHitRangeWidth: analogical to getHitCenter, but extracts hit's width
     *      in specific dimension
     * @setTrackCenter: function used to set track's position in considered dimension
     * @setTrackSigma: function used to set track's sigma in considered dimension
     * @result: vector to which produced tracks are appended
     */
    void producePartialTracks(
        const HitVector& hits,
        const DimensionParameters& param,
        float (*getHitCenter)(const HIT_TYPE&),
        float (*getHitRangeWidth)(const HIT_TYPE&),
        void (*setTrackCenter)(TRACK_TYPE&, float),
        void (*setTrackSigma)(TRACK_TYPE&, float),
        std::vector<TRACK_TYPE>& result
      );



    /* Retrieves the bounds of a 3D range in which all hits from given collection are contained.
     *
     * @hits: hits collection to retrieve the range from
     */
    SpatialRange getHitSpatialRange(const HitVector& hits);
};

// Implementation for all member methods
#include "RecoCTPPS/TotemRPLocal/src/CTPPSTimingTrackRecognition.cc"

#endif
