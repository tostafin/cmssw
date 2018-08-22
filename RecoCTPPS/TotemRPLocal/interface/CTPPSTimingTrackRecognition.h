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
template<class TRACK_TYPE, class HIT_TYPE>
class CTPPSTimingTrackRecognition
{
  public:

    CTPPSTimingTrackRecognition(const edm::ParameterSet& parameters);

    ~CTPPSTimingTrackRecognition();


    // Class API:

    // Resets internal state of a class instance.
    void clear();


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
    float tolerance;
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



    /* Retrieves the bounds of a single dimension range in which all hits are contained.
     *
     * @hits: hits collection to retrieve the range from
     * @getHitCenter: function extracting hit's center in considered dimension
     * @getHitRangeWidth: function extracting hit's width in considered dimension
     * @rangeBegin: buffer to which the range's lower bound is written
     * @rangeEnd: buffer to which the range's upper bound is written
     */
    bool getHitSpatialRange(
        const HitVector& hits,
        float (*getHitCenter)(const HIT_TYPE&),
        float (*getHitRangeWidth)(const HIT_TYPE&),
        float& rangeBegin,
        float& rangeEnd
      );
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template<class TRACK_TYPE, class HIT_TYPE>
CTPPSTimingTrackRecognition<TRACK_TYPE, HIT_TYPE>::CTPPSTimingTrackRecognition(const edm::ParameterSet& iConfig) :
    threshold               ( iConfig.getParameter<double>( "threshold" ) ),
    thresholdFromMaximum    ( iConfig.getParameter<double>( "thresholdFromMaximum" ) ),
    resolution              ( iConfig.getParameter<double>( "resolution" ) ),
    sigma                   ( iConfig.getParameter<double>( "sigma" ) ),
    tolerance               ( iConfig.getParameter<double>( "tolerance" ) ),
    pixelEfficiencyFunction ( "hit_TF1_CTPPS", iConfig.getParameter<std::string>( "pixelEfficiencyFunction" ).c_str() ) {
}


template<class TRACK_TYPE, class HIT_TYPE>
CTPPSTimingTrackRecognition<TRACK_TYPE, HIT_TYPE>::~CTPPSTimingTrackRecognition() {};


template<class TRACK_TYPE, class HIT_TYPE>
void CTPPSTimingTrackRecognition<TRACK_TYPE, HIT_TYPE>::clear() {
  hitVectorMap.clear();
}


template<class TRACK_TYPE, class HIT_TYPE>
void CTPPSTimingTrackRecognition<TRACK_TYPE, HIT_TYPE>::producePartialTracks(
    const HitVector& hits,
    const DimensionParameters& param,
    float (*getHitCenter)(const HIT_TYPE&),
    float (*getHitRangeWidth)(const HIT_TYPE&),
    void (*setTrackCenter)(TRACK_TYPE&, float),
    void (*setTrackSigma)(TRACK_TYPE&, float),
    std::vector<TRACK_TYPE>& result
  ) {

  int numberOfTracks = 0;
  const double invResolution = 1./param.resolution;

  std::vector<float> hitProfile((param.rangeEnd - param.rangeBegin) * invResolution, 0.);
  auto hitFunction = param.hitFunction;

  // Creates hit profile
  for(auto const& hit : hits) {

    float center = getHitCenter(hit);
    float rangeWidth = getHitRangeWidth(hit);

    hitFunction.SetParameters(center, rangeWidth, param.sigma);

    for(unsigned int i = 0; i < hitProfile.size(); ++i) {
      hitProfile[i] += hitFunction.Eval(param.rangeBegin + i*param.resolution);
    }
  }

  // Guard to make sure that no track is lost
  hitProfile.push_back(param.threshold - 1.0);

  bool underThreshold = true;
  float rangeMaximum = -1.0;
  bool trackRangeFound = false;
  int trackRangeBegin = 0;
  int trackRangeEnd;

  // Searches for tracks in the hit profile
  for(unsigned int i = 0; i < hitProfile.size(); i++) {

    if(hitProfile[i] > rangeMaximum)
      rangeMaximum = hitProfile[i];

    // Going above the threshold
    if(underThreshold && hitProfile[i] > param.threshold) {
      underThreshold = false;
      trackRangeBegin = i;
      rangeMaximum = hitProfile[i];
    }

    // Going under the threshold
    else if(!underThreshold && hitProfile[i] <= param.threshold){
      underThreshold = true;
      trackRangeEnd = i;
      trackRangeFound = true;
    }

    // Finds all tracks within the track range
    if(trackRangeFound) {

      float trackThreshold = rangeMaximum - param.thresholdFromMaximum;
      int trackBegin;
      bool underTrackThreshold = true;

      for(int j = trackRangeBegin; j <= trackRangeEnd; j++) {

        if(underTrackThreshold && hitProfile[j] > trackThreshold) {
          underTrackThreshold = false;
          trackBegin = j;
        }

        else if(!underTrackThreshold && hitProfile[j] <= trackThreshold) {
          underTrackThreshold = true;
          TRACK_TYPE track;
          float leftMargin = param.rangeBegin + param.resolution * trackBegin;
          float rightMargin = param.rangeBegin + param.resolution * j;
          setTrackCenter(track, (leftMargin + rightMargin) / 2.0);
          setTrackSigma(track, (rightMargin - leftMargin) / 2.0);
          result.push_back(track);
          numberOfTracks++;
        }
      }

      trackRangeFound = false;
    }
  }
}


template<class TRACK_TYPE, class HIT_TYPE>
bool CTPPSTimingTrackRecognition<TRACK_TYPE, HIT_TYPE>::getHitSpatialRange(
    const HitVector& hits,
    float (*getHitCenter)(const HIT_TYPE&),
    float (*getHitRangeWidth)(const HIT_TYPE&),
    float& rangeBegin,
    float& rangeEnd
  ) {

  bool initialized = false;

  for(unsigned int i = 0; i < hits.size(); i++) {

    if(initialized) {
      float bottomVal = getHitCenter(hits[i]) - getHitRangeWidth(hits[i]) / 2.0;
      float topVal = getHitCenter(hits[i]) + getHitRangeWidth(hits[i]) / 2.0;

      if(bottomVal < rangeBegin)
        rangeBegin = bottomVal;

      if(topVal > rangeEnd)
        rangeEnd = topVal;
    }

    else {
      rangeBegin = getHitCenter(hits[i]) - getHitRangeWidth(hits[i]) / 2.0;
      rangeEnd = getHitCenter(hits[i]) + getHitRangeWidth(hits[i]) / 2.0;
      initialized = true;
    }
  }

  return initialized;
}


#endif
