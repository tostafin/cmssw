/****************************************************************************
 *
 * This is a part of PPS PI software.
 *
 ****************************************************************************/

#include "CondCore/Utilities/interface/PayloadInspectorModule.h"
#include "CondCore/Utilities/interface/PayloadInspector.h"
#include "CondCore/CondDB/interface/Time.h"
#include "CondCore/CondDB/interface/PayloadReader.h"
#include "CondFormats/PPSObjects/interface/LHCOpticalFunctionsSetCollection.h"
#include "CondFormats/PPSObjects/interface/LHCOpticalFunctionsSet.h"
#include "CondCore/CTPPSPlugins/interface/LHCOpticalFunctionsSetCollectionHelper.h"

namespace {
  typedef OpticalFunctionsInfo<LHCOpticalFunctionsSetCollection, CTPPSOptics::RP::RP123>
      OpticalFunctionsInfoPrint_RP123;

}  // namespace

PAYLOAD_INSPECTOR_MODULE(LHCOpticalFunctionsSetCollection) {
  PAYLOAD_INSPECTOR_CLASS(OpticalFunctionsInfoPrint_RP123);
}