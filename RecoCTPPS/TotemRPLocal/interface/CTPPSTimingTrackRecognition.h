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

#include <set>
#include <vector>
#include <unordered_map>
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
    CTPPSDiamondTrackRecognition( const edm::ParameterSet& ) {

    }

    ~CTPPSDiamondTrackRecognition();

    // Class public interface:

    // Resets internal state of the class instance.
    void clear() {
      hitVectorMap.clear();
    }

    // Adds new hit to the set from which the tracks are reconstructed.
    void addHit( const CTPPSTimingRecHit& recHit ) {
      hitVectorMap[recHit.getOOTIndex].push_back(recHit);
    }

    /// Produce a collection of tracks for the current station, given its hits collection
    virtual int produceTracks( edm::DetSet<CTPPSDiamondLocalTrack>& tracks ) = 0;

  private:

    typedef std::vector<CTPPSTimingRecHit> HitVector;
    typedef std::unordered_map<int, HitVector> HitVectorMap;

    // Stores CTPPSTimingRecHit vectors grouped by OOTIndex
    HitVectorMap hitVectorMap;

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
     * @channelComponents: ids of all channels that detected the hits which
     *      produced the partial track
     */
    struct PartialTrack {
      float trackBegin;
      float trackEnd;
      std::set<detId> channelComponents;
    };

    /* Produces all partial tracks from <<hitSet>> with regard to single dimension.
     *
     * @param: describes all parameters used by 1D track recognition algorithm
     * @getHitCenter: functor extracting hit's center in the dimension
     *      that the partial tracks relate to
     * @getHitRangeWidth: analogical to getHitCenter, but extracts hit's width
     *      in specific dimension
     *
     * @return: number of produced partial tracks
     */
    int producePartialTracks(
        const dimensionParameters& param,
        float (*getHitCenter)(const &CTPPSTimingRecHit),
        float (*getHitRangeWidth)(const &CTPPSTimingRecHit),
        std::vector<PartialTrack>& resultVector
      ) {

      int number_of_tracks = 0;
      const double invResolution = 1./param.resolution;

      for(auto const& mapEntry : hitVectorMap) {

        float

        std::vector<float> hitProfile((param.rangeEnd - param.rangeBegin) * invResolution, 0.);

        for (auto const& hit : mapEntry.second) {

          float center = getHitCenter(hit);
          float rangeWidth = getHitRangeWidth(hit);

          param.hitFunction.SetParameters(center, rangeWidth, param.sigma);
          for (unsigned int i = 0; i < hit_profile.size(); ++i) {
            hitProfile[i] += param.hitFunction.Eval(param.rangeBegin + i*param.resolution);
          }
        }

    }


};

#endif
