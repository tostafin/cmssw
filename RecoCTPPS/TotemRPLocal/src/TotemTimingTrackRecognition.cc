#ifndef RecoCTPPS_TotemRPLocal_TotemTimingTrackRecognition_CC
#define RecoCTPPS_TotemRPLocal_TotemTimingTrackRecognition_CC

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

TotemTimingTrackRecognition::TotemTimingTrackRecognition(const edm::ParameterSet& parameters) :
    CTPPSTimingTrackRecognition<TotemTimingLocalTrack, TotemTimingRecHit>(parameters)
  {};

void TotemTimingTrackRecognition::addHit(const TotemTimingRecHit& recHit) {
  if(recHit.getT() != TotemTimingRecHit::NO_T_AVAILABLE)
    hitVectorMap[0].push_back(recHit);
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
   auto getX = [](const TotemTimingRecHit& hit){ return hit.getX(); };
   auto getXWidth = [](const TotemTimingRecHit& hit){ return hit.getXWidth(); };
   auto setX = [](TotemTimingLocalTrack& track, float x){ track.setPosition(math::XYZPoint(x, 0., 0.)); };
   auto setXSigma = [](TotemTimingLocalTrack& track, float sigma){ track.setPositionSigma(math::XYZPoint(sigma, 0., 0.)); };
   auto getY = [](const TotemTimingRecHit& hit){ return hit.getY(); };
   auto getYWidth = [](const TotemTimingRecHit& hit){ return hit.getYWidth(); };
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

   //TODO: create default tracks (not sure if necessary)

   auto getZ = [](const TotemTimingRecHit& hit){ return hit.getZ(); };
   auto getZWidth = [](const TotemTimingRecHit& hit){ return hit.getZWidth(); };
   float zRangeBegin, zRangeEnd;
   getHitSpatialRange(hits, getZ, getZWidth, zRangeBegin, zRangeEnd);

   int validHitsNumber = (int)(threshold + 1.0);

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
         if(newTrack.containsHit(hit, tolerance)) {
           hitCounter++;
         }
       }

       if(hitCounter >= validHitsNumber)
         tracks.push_back(newTrack);
     }
   }
 }

 return numberOfTracks;
}

#endif
