/****************************************************************************
*
* This is a part of PPS offline software.
* Authors:
*   Laurent Forthomme (laurent.forthomme@cern.ch)
*
****************************************************************************/

#include "FWCore/Utilities/interface/isFinite.h"
#include "RecoPPS/Local/interface/TotemT2RecHitProducerAlgorithm.h"

#include "DataFormats/Common/interface/DetSetNew.h"
#include "DataFormats/CTPPSDetId/interface/TotemT2DetId.h"

void TotemT2RecHitProducerAlgorithm::build(const TotemGeometry& geom,
                                           const edmNew::DetSetVector<TotemT2Digi>& input,
                                           edmNew::DetSetVector<TotemT2RecHit>& output) {
  const int verbosity=0;
  int nDigis=0;
  int nDetSets=0;
  for (const auto& vec : input) {
    nDigis++;
    const TotemT2DetId detid(vec.detId());
//    const int sector = detid.arm(), plane = detid.plane(), channel = detid.channel();

//    // retrieve the timing calibration part for this channel
//    const auto& ch_params = (apply_calib_) ? calib_->parameters(sector, 0, plane, channel) : std::vector<double>{};
//    // default values for offset + time precision if calibration object not found
//    const double ch_t_offset = (apply_calib_) ? calib_->timeOffset(sector, 0, plane, channel) : 0.;
//    const double ch_t_precis = (apply_calib_) ? calib_->timePrecision(sector, 0, plane, channel) : 0.;

    // prepare the output collection filler
    edmNew::DetSetVector<TotemT2RecHit>::FastFiller filler(output, detid);

    for (const auto& digi : vec) {
      const int t_lead = digi.leadingEdge(), t_trail = digi.trailingEdge();
//      if (t_lead == 0 && t_trail == 0)  // skip invalid digis
      if (!(digi.hasLE()||digi.hasTE()))  // skip invalid digis
        continue;
      double tot = 0.;
//      if (t_lead != 0 && t_trail != 0) {
      if (digi.hasLE() && digi.hasTE()) {
        tot = (t_trail - t_lead) * ts_to_ns_;  // in ns
 //       if (calib_fct_ && apply_calib_) {      // compute the time-walk correction
 //         ch_t_twc = calib_fct_->evaluate(std::vector<double>{tot}, ch_params);
 //         if (edm::isNotFinite(ch_t_twc))
 //          ch_t_twc = 0.;
 //       }
      }
      nDetSets++;
      double ch_t_precis=ts_to_ns_/2.0; //without a calibration, assume LE/TE precision is +-0.5

      // retrieve the geometry element associated to this DetID
      const auto& tile = geom.tile(detid);

      // store to the output collection
      filler.emplace_back(tile.centre(), t_lead * ts_to_ns_ , ch_t_precis, tot);
      if (verbosity>2)
       edm::LogWarning("Totem")<<"T2 RecHits produced (T2Digis: #DetSetVector/#DetSet): ("<<nDigis
	       <<"/"<<nDetSets<<"), "<<"T2 tile centre: "<<tile.centre()<<" TE/LE/ToT/precision, in ns: "
	       <<(t_trail*ts_to_ns_)<<"/"<<(t_lead*ts_to_ns_)<<"/"<<tot<<"/"<<ch_t_precis<<std::endl;
    }
  }
}
