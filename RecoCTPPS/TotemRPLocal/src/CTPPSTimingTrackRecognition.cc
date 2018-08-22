/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Nicola Minafra (nicola.minafra@cern.ch)
 *   Mateusz Szpyrka (mateusz.szpyrka@cern.ch)
 *
 ****************************************************************************/

 #ifndef RecoCTPPS_TotemRPLocal_CTPPSTimingTrackRecognition_CC
 #define RecoCTPPS_TotemRPLocal_CTPPSTimingTrackRecognition_CC

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"

#include "DataFormats/CTPPSReco/interface/TotemTimingRecHit.h"
#include "DataFormats/CTPPSReco/interface/TotemTimingLocalTrack.h"

#include "RecoCTPPS/TotemRPLocal/interface/CTPPSTimingTrackRecognition.h"

#include <string>
#include <cmath>
#include <set>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "TF1.h"


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
