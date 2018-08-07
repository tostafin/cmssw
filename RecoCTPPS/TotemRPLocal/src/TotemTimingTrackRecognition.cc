/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *   Nicola Minafra (nicola.minafra@cern.ch)
 *   Mateusz Szpyrka (mateusz.szpyrka@cern.ch)
 *
 ****************************************************************************/

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Math/interface/Point3D.h"


#include "DataFormats/CTPPSReco/interface/TotemTimingRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSTimingLocalTrack.h"
#include "DataFormats/CTPPSReco/interface/TotemTimingLocalTrack.h"

#include "RecoCTPPS/TotemRPLocal/interface/TotemTimingTrackRecognition.h"

#include <string>
#include <cmath>
#include <set>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "TF1.h"


/* General parameters in the set:
 * "threshold": float
 * "thresholdFromMaximum": float
 * "resolution": float
 * "sigma": float
 * "pixelEfficiencyFunction": TF1
 * Parameters specific to TotemTimingLocalTrack:
 * "startFromX": float
 * "stopAtX": float
 * "startFromY": float
 * "stopAtY": float
 */
TotemTimingTrackRecognition::TotemTimingTrackRecognition(const edm::ParameterSet& iConfig) :
    threshold               ( iConfig.getParameter<double>( "threshold" ) ),
    thresholdFromMaximum    ( iConfig.getParameter<double>( "thresholdFromMaximum" ) ),
    resolution              ( iConfig.getParameter<double>( "resolution" ) ),
    sigma                   ( iConfig.getParameter<double>( "sigma" ) ),
    tolerance               ( iConfig.getParameter<double>( "tolerance" ) ),
    pixelEfficiencyFunction ( "hit_TF1_CTPPS", iConfig.getParameter<std::string>( "pixelEfficiencyFunction" ).c_str() ) {
}

TotemTimingTrackRecognition::~TotemTimingTrackRecognition() {};


void TotemTimingTrackRecognition::clear() {
  hitVectorMap.clear();
}


void TotemTimingTrackRecognition::addHit(const TotemTimingRecHit& recHit) {
  hitVectorMap[getHitKey(recHit)].push_back(recHit);
}


int TotemTimingTrackRecognition::produceTracks(edm::DetSet<TotemTimingLocalTrack>& tracks) {

  int numberOfTracks = 0;
  DimensionParameters param;

  param.threshold = threshold;
  param.thresholdFromMaximum = thresholdFromMaximum;
  param.resolution = resolution;
  param.sigma = sigma;
  param.hitFunction = pixelEfficiencyFunction;


  for(auto hitBatch: hitVectorMap) {

    auto hits = hitBatch.second;

    std::vector<TotemTimingLocalTrack> xPartTracks, yPartTracks;
    auto getX = [](const CTPPSTimingRecHit& hit){ return hit.getX(); };
    auto getXWidth = [](const CTPPSTimingRecHit& hit){ return hit.getXWidth(); };
    auto setX = [](TotemTimingLocalTrack& track, float x){ track.setPosition(math::XYZPoint(x, 0., 0.)); };
    auto setXSigma = [](TotemTimingLocalTrack& track, float sigma){ track.setPositionSigma(math::XYZPoint(sigma, 0., 0.)); };
    auto getY = [](const CTPPSTimingRecHit& hit){ return hit.getY(); };
    auto getYWidth = [](const CTPPSTimingRecHit& hit){ return hit.getYWidth(); };
    auto setY = [](TotemTimingLocalTrack& track, float y){ track.setPosition(math::XYZPoint(0., y, 0.)); };
    auto setYSigma = [](TotemTimingLocalTrack& track, float sigma){ track.setPositionSigma(math::XYZPoint(0., sigma, 0.)); };

    float xRangeBegin, xRangeEnd, yRangeBegin, yRangeEnd;
    getHitSpatialRange(hits, getX, getXWidth, xRangeBegin, xRangeEnd);
    getHitSpatialRange(hits, getY, getYWidth, yRangeBegin, yRangeEnd);

    param.rangeBegin = xRangeBegin;
    param.rangeEnd = xRangeEnd;
    producePartialTracks(hits, param, getX, getXWidth, setX, setXSigma, xPartTracks);

    param.rangeBegin = yRangeBegin;
    param.rangeEnd = yRangeEnd;
    producePartialTracks(hits, param, getY, getYWidth, setY, setYSigma, yPartTracks);

    if(xPartTracks.size() == 0 && yPartTracks.size() == 0)
      continue;

    //TODO: create default tracks
    //if(xPartTracks.size() == 0)
    //  xPartTracks.push_back(xDefaultPartTrack);

    //if(yPartTracks.size() == 0)
    //  yPartTracks.push_back(yDefaultPartTrack);

    auto getZ = [](const CTPPSTimingRecHit& hit){ return hit.getZ(); };
    auto getZWidth = [](const CTPPSTimingRecHit& hit){ return hit.getZWidth(); };
    float zRangeBegin, zRangeEnd;
    getHitSpatialRange(hits, getZ, getZWidth, zRangeBegin, zRangeEnd);

    // TODO: unify threshold among different dimensions
    int validHitsNumber = (int)(param.threshold + 1.0);

    for(const auto& xTrack: xPartTracks) {
      for(const auto& yTrack: yPartTracks) {

        math::XYZPoint position(
          xTrack.getX0(),
          yTrack.getY0(),
          (zRangeBegin + zRangeEnd) / 2.0
        );
        math::XYZPoint positionSigma(
          xTrack.getX0Sigma(),
          yTrack.getY0Sigma(),
          (zRangeEnd - zRangeBegin) / 2.0
        );

        TotemTimingLocalTrack newTrack;
        newTrack.setPosition(position);
        newTrack.setPositionSigma(positionSigma);
        // TODO: setting validity / time / numHits / numPlanes

        int hitCounter = 0;
        for(auto hit: hits) {
          if(newTrack.containsHit(hit, tolerance))
            hitCounter++;
        }

        if(hitCounter >= validHitsNumber)
          tracks.push_back(newTrack);
      }
    }
  }

  return numberOfTracks;
}


int TotemTimingTrackRecognition::getHitKey(const CTPPSTimingRecHit& obj) {
  return 0;
}


int TotemTimingTrackRecognition::getHitKey(const CTPPSDiamondRecHit& hit) {
  return hit.getOOTIndex();
}


void TotemTimingTrackRecognition::producePartialTracks(
    const HitVector& hits,
    const DimensionParameters& param,
    float (*getHitCenter)(const CTPPSTimingRecHit&),
    float (*getHitRangeWidth)(const CTPPSTimingRecHit&),
    void (*setTrackCenter)(TotemTimingLocalTrack&, float),
    void (*setTrackSigma)(TotemTimingLocalTrack&, float),
    std::vector<TotemTimingLocalTrack>& result
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
      rangeMaximum = -1.0;
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
          TotemTimingLocalTrack track;
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


bool TotemTimingTrackRecognition::getHitSpatialRange(
    const HitVector& hits,
    float (*getHitCenter)(const CTPPSTimingRecHit&),
    float (*getHitRangeWidth)(const CTPPSTimingRecHit&),
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
