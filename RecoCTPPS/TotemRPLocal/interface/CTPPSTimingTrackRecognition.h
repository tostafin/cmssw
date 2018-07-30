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
class CTPPSTimingTrackRecognition
{
  public:

    /* Parameters set naming convention:
     * TODO
     */
    CTPPSTimingTrackRecognition( const edm::ParameterSet& ) {

    }

    ~CTPPSTimingTrackRecognition();

    // Class public interface:

    // Resets internal state of the class instance.
    void clear() {
      hitVectorMap.clear();
    }

    // Adds new hit to the set from which the tracks are reconstructed.
    void addHit(const CTPPSTimingRecHit& recHit) {
      hitVectorMap[getHitKey(recHit)].push_back(recHit);
    }

    /// Produce a collection of tracks for the current station, given its hits collection
    int produceTracks( edm::DetSet<CTPPSDiamondLocalTrack>& tracks );



    int produceTotemTimingTracks(edm::DetSet<CTPPSDiamondLocalTrack>& tracks) {

      int numberOfTracks;
      DimensionParameters xParam, yParam;

      // TODO: reading parameters for both dimensions

      for(auto hitBatch: hitVectorMap) {

        auto hits = hitBatch.first;
        std::vector<PartialTrack> xPartTracks, yPartTracks;
        auto getX = [](CTPPSTimingRecHit& hit){ return hit.getX; };
        auto getXWidth = [](CTPPSTimingRecHit& hit){ return hit.getXWidth; };
        auto getY = [](CTPPSTimingRecHit& hit){ return hit.getY; };
        auto getYWidth = [](CTPPSTimingRecHit& hit){ return hit.getYWidth; };
        producePartialTracks(hits, xParam, getX, getXWidth, xPartTracks);
        producePartialTracks(hits, yParam, getY, getYWidth, yPartTracks);

        if(xPartTracks.size() == 0 && yPartTracks.size() == 0)
          continue;

        if(xPartTracks.size() == 0) {
          PartialTrack partTrackBuffer;
          getDefaultPartialTrack(hits, getX, getXWidth, partTrackBuffer);
          xPartTracks.push_back(partTrackBuffer);
        }

        if(yPartTracks.size() == 0) {
          PartialTrack partTrackBuffer;
          getDefaultPartialTrack(hits, getY, getYWidth, partTrackBuffer);
          yPartTracks.push_back(partTrackBuffer);
        }

        PartialTrack zTrack;
        auto getZ = [](CTPPSTimingRecHit& hit){ return hit.getZ; };
        auto getZWidth = [](CTPPSTimingRecHit& hit){ return hit.getZWidth; };
        getDefaultPartialTrack(hits, getZ, getZWidth, zTrack);

        // TODO: unify threshold among different dimensions
        int validHitsNumber = (int)(xParam.threshold + 1.0);

        for(auto xTrack: xPartTracks) {
          for(auto yTrack: yPartTracks) {

            int commonHitsNumber = countTrackIntersectionSize(xTrack, yTrack);
            if(commonHitsNumber >= validHitsNumber) {
              math::XYZPoint position(
                (xTrack.End - xTrack.Begin) / 2.0;
                (yTrack.End - yTrack.Begin) / 2.0;
                (zTrack.End - zTrack.Begin) / 2.0;
              );
              math::XYZPoint positionSigma(
                (xTrack.End + xTrack.Begin) / 2.0;
                (yTrack.End + yTrack.Begin) / 2.0;
                (zTrack.End + zTrack.Begin) / 2.0;
              );

              // TODO: setting validity / time / numHits / numPlanes

              CTPPSTimingLocalTrack newTrack;
              newTrack.setPosition(position);
              newTrack.setPositionSigma(positionSigma);
              tracks.push_back(newTrack);
              numberOfTracks+;
              a = 0
            }
          }
        }
      }

      return numberOfTracks;
    }



  private:

    typedef std::vector<CTPPSTimingRecHit> HitVector;
    typedef std::unordered_map<int, HitVector> HitVectorMap;

    // Stores CTPPSTimingRecHit vectors grouped by OOTIndex
    HitVectorMap hitVectorMap;


    // Funtions used to extract hit's key by wchich the hits are grouped in hitVectorMap
    template <class CTPPSTimingRecHit>
    int getHitKey(const CTPPSTimingRecHit& obj) {
      return 0;
    }

    template <>
    int getHitKey(const CTPPSDiamondRecHit& hit) {
      return hit.getOOTIndex();
    }


    /* Structure representing parameters set for single dimension.
     * Intended to use when producing partial tracks.
     */
    struct DimensionParameters {
      string name;
      float threshold;
      float thresholdFromMaximum;
      float resolution;
      float sigma;
      float rangeBegin;
      float rangeEnd;
      TF1 hitFunction;
    };

    /* Structure describing single "partial track".
     * Partial track is a single dimensional component of a complete track.
     *
     * @trackBegin: starting point of the track within considered dimension
     * @trackEnd: ending point of the track within considered dimension
     * @hitComponents: indeces of all hits from source hit vector
     *      which produced the partial track
     * @timeComponents: time values of all hits that produced the track
     */
    struct PartialTrack {
      float trackBegin;
      float trackEnd;
      std::set<int> hitComponents;

      bool containsHit(const float& hitCenter, const float& hitRangeWidth) {
        float trackWidth = trackEnd - trackBegin;
        float trackCenter = (trackBegin + trackEnd) / 2.0;
        return ((fabs(trackCenter - hitCenter)) < ((trackWidth + hitRangeWidth) / 2.0));
      }
    };

    /* Counts the size of
     *
     */
    int countTrackIntersectionSize(const PartialTrack &track1, const PartialTrack& track2) {
      auto it1 = track1.hitComponents.begin();
      auto it2 = track2.hitComponents.begin();

      int result = 0;

      while(it1 != track1.hitComponents.end() && it2 != track2.hitComponents.end()) {
        if(it1 < it2)
          it1++;

        else if(it1 > it2)
          it2++;

        else {
          result++;
          it1++;
          it2++;
        }
      }

      return result;
    }

    /* Produces all partial tracks from given set with regard to single dimension.
     *
     * @hits: vector of hits from which the tracks are created
     * @param: describes all parameters used by 1D track recognition algorithm
     * @getHitCenter: functor extracting hit's center in the dimension
     *      that the partial tracks relate to
     * @getHitRangeWidth: analogical to getHitCenter, but extracts hit's width
     *      in specific dimension
     * @result: vector to which produced tracks are appended
     */
    void producePartialTracks(
        const HitVector& hits,
        const dimensionParameters& param,
        float (*getHitCenter)(const &CTPPSTimingRecHit),
        float (*getHitRangeWidth)(const &CTPPSTimingRecHit),
        std::vector<PartialTrack>& result
      ) {

      int numberOfTracks = 0;
      const double invResolution = 1./param.resolution;

      std::vector<float> hitProfile((param.rangeEnd - param.rangeBegin) * invResolution, 0.);

      // Creates hit profile
      for(auto const& hit : hits) {

        float center = getHitCenter(hit);
        float rangeWidth = getHitRangeWidth(hit);
        param.hitFunction.SetParameters(center, rangeWidth, param.sigma);

        for(int i = 0; i < hitProfile.size(); ++i) {
          hitProfile[i] += param.hitFunction.Eval(param.rangeBegin + i*param.resolution);
        }
      }

      // Guard to make sure that no track is lost
      hitProfile.push_back(param.threshold - 1.0);

      bool underThreshold = true;
      float rangeMaximum;
      bool trackRangeFound = false;
      int trackRangeBegin;
      int trackRangeEnd;

      // Searches for tracks in the hit profile
      for(int i = 0; i < hitProfile.size(); i++) {

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
              pt.trackBegin = param.rangeBegin + param.resolution * trackBegin;
              pt.trackEnd = param.rangeBegin + param.resolution * j;
              result.push_back(pt);
              numberOfTracks++;
            }
          }

          trackRangeFound = false;
        }
      }
    }

    /* Creates a partial track that consists of all hits in the hit set.
     *
     * @hits: vector of hits from which the track is created
     * @getHitCenter: functor extracting hit's center in the dimension
     *      that the track relates to
     * @getHitRangeWidth: analogical to getHitCenter, but extracts hit's width
     *      in specific dimension
     * @result: track buffer to which found values are saved
     *
     * @return: true if the track was successfully produced. false otherwise
     */
    bool getDefaultPartialTrack(
        const HitVector& hits,
        float (*getHitCenter)(const &CTPPSTimingRecHit),
        float (*getHitRangeWidth)(const &CTPPSTimingRecHit),
        PartialTrack& result
      ) {

      float minVal;
      float maxVal;
      bool initialized = false;

      for(int i = 0; i < hits.size(); i++) {

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

      return initialized;
    }
};

#endif
