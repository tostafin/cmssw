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

#include "DataFormats/CTPPSReco/interface/TotemTimingRecHit.h"
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

    std::vector<PartialTrack> xPartTracks, yPartTracks;
    auto getX = [](const CTPPSTimingRecHit& hit){ return hit.getX(); };
    auto getXWidth = [](const CTPPSTimingRecHit& hit){ return hit.getXWidth(); };
    auto getY = [](const CTPPSTimingRecHit& hit){ return hit.getY(); };
    auto getYWidth = [](const CTPPSTimingRecHit& hit){ return hit.getYWidth(); };

    PartialTrack xDefaultPartTrack, yDefaultPartTrack;
    getDefaultPartialTrack(hits, getX, getXWidth, xDefaultPartTrack);
    getDefaultPartialTrack(hits, getY, getYWidth, yDefaultPartTrack);

    param.rangeBegin = xDefaultPartTrack.begin;
    param.rangeEnd = xDefaultPartTrack.end;
    producePartialTracks(hits, param, getX, getXWidth, xPartTracks);

    param.rangeBegin = yDefaultPartTrack.begin;
    param.rangeEnd = yDefaultPartTrack.end;
    producePartialTracks(hits, param, getY, getYWidth, yPartTracks);

    if(xPartTracks.size() == 0 && yPartTracks.size() == 0)
      continue;

    if(xPartTracks.size() == 0)
      xPartTracks.push_back(xDefaultPartTrack);

    if(yPartTracks.size() == 0)
      yPartTracks.push_back(yDefaultPartTrack);

    PartialTrack zTrack;
    auto getZ = [](const CTPPSTimingRecHit& hit){ return hit.getZ(); };
    auto getZWidth = [](const CTPPSTimingRecHit& hit){ return hit.getZWidth(); };
    getDefaultPartialTrack(hits, getZ, getZWidth, zTrack);

    // TODO: unify threshold among different dimensions
    int validHitsNumber = (int)(param.threshold + 1.0);

    std::cout << "yy: " << xPartTracks.size() << " " << yPartTracks.size() << std::endl;

    for(auto xTrack: xPartTracks) {
      for(auto yTrack: yPartTracks) {

        int commonHitsNumber = countTrackIntersectionSize(xTrack, yTrack);
        std::cout << "int: " << commonHitsNumber << std::endl;
        if(commonHitsNumber >= validHitsNumber) {
          math::XYZPoint position(
            (xTrack.end - xTrack.begin) / 2.0,
            (yTrack.end - yTrack.begin) / 2.0,
            (zTrack.end - zTrack.begin) / 2.0
          );
          math::XYZPoint positionSigma(
            (xTrack.end + xTrack.begin) / 2.0,
            (yTrack.end + yTrack.begin) / 2.0,
            (zTrack.end + zTrack.begin) / 2.0
          );

          // TODO: setting validity / time / numHits / numPlanes

          TotemTimingLocalTrack newTrack;
          newTrack.setPosition(position);
          newTrack.setPositionSigma(positionSigma);
          tracks.push_back(newTrack);
        }
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


int TotemTimingTrackRecognition::countTrackIntersectionSize(const PartialTrack &track1, const PartialTrack& track2) {
  auto it1 = track1.hitComponents.begin();
  auto it2 = track2.hitComponents.begin();

  int result = 0;

  while(it1 != track1.hitComponents.end() && it2 != track2.hitComponents.end()) {
    if(*it1 < *it2)
      it1++;

    else if(*it1 > *it2)
      it2++;

    else {
      result++;
      it1++;
      it2++;
    }
  }

  return result;
}


void TotemTimingTrackRecognition::producePartialTracks(
    const HitVector& hits,
    const DimensionParameters& param,
    float (*getHitCenter)(const CTPPSTimingRecHit&),
    float (*getHitRangeWidth)(const CTPPSTimingRecHit&),
    std::vector<PartialTrack>& result
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

  {
    float tmp = 0;
    for(auto a : hitProfile) {
      if(tmp < a)
        tmp = a;
    }

    std::cout << "! " << tmp << std::endl;
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
      trackRangeEnd = true;
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
          PartialTrack pt;
          pt.begin = param.rangeBegin + param.resolution * trackBegin;
          pt.end = param.rangeBegin + param.resolution * j;
          result.push_back(pt);
          numberOfTracks++;
        }
      }

      trackRangeFound = false;
    }
  }
}


bool TotemTimingTrackRecognition::getDefaultPartialTrack(
    const HitVector& hits,
    float (*getHitCenter)(const CTPPSTimingRecHit&),
    float (*getHitRangeWidth)(const CTPPSTimingRecHit&),
    PartialTrack& result
  ) {

  float minVal;
  float maxVal;
  bool initialized = false;
  result.hitComponents.clear();

  for(unsigned int i = 0; i < hits.size(); i++) {

    result.hitComponents.insert(i);

    if(initialized) {
      float newMinVal = getHitCenter(hits[i]) - getHitRangeWidth(hits[i]) / 2.0;
      float newMaxVal = getHitCenter(hits[i]) + getHitRangeWidth(hits[i]) / 2.0;

      if(minVal > newMinVal)
        minVal = newMinVal;

      if(maxVal < newMaxVal)
        maxVal = newMaxVal;
    }

    else {
      minVal = getHitCenter(hits[i]) - getHitRangeWidth(hits[i]) / 2.0;
      maxVal = getHitCenter(hits[i]) + getHitRangeWidth(hits[i]) / 2.0;
      initialized = true;
    }
  }

  if(initialized) {
    result.begin = minVal;
    result.end = maxVal;
  }

  return initialized;
}
