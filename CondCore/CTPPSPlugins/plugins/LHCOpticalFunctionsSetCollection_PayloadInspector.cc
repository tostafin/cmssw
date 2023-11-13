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

  typedef OpticalFunction_History<LHCOpticalFunctionsSetCollection,
                                  CTPPSOptics::RP::RP3,
                                  CTPPSOptics::Function::evx>
      OpticalFunction_History_RP3_evx;

  typedef OpticalFunction_History<LHCOpticalFunctionsSetCollection,
                                  CTPPSOptics::RP::RP3,
                                  CTPPSOptics::Function::eLx>
      OpticalFunction_History_RP3_eLx;

  typedef OpticalFunction_History<LHCOpticalFunctionsSetCollection,
                                  CTPPSOptics::RP::RP3,
                                  CTPPSOptics::Function::e14>
      OpticalFunction_History_RP3_e14;

  typedef OpticalFunction_History<LHCOpticalFunctionsSetCollection,
                                  CTPPSOptics::RP::RP3,
                                  CTPPSOptics::Function::exd>
      OpticalFunction_History_RP3_exd;

  typedef OpticalFunction_History<LHCOpticalFunctionsSetCollection,
                                  CTPPSOptics::RP::RP3,
                                  CTPPSOptics::Function::evpx>
      OpticalFunction_History_RP3_evpx;

  typedef OpticalFunction_History<LHCOpticalFunctionsSetCollection,
                                  CTPPSOptics::RP::RP3,
                                  CTPPSOptics::Function::eLpx>
      OpticalFunction_History_RP3_eLpx;

  typedef OpticalFunction_History<LHCOpticalFunctionsSetCollection,
                                  CTPPSOptics::RP::RP3,
                                  CTPPSOptics::Function::e24>
      OpticalFunction_History_RP3_e24;

  typedef OpticalFunction_History<LHCOpticalFunctionsSetCollection,
                                  CTPPSOptics::RP::RP3,
                                  CTPPSOptics::Function::expd>
      OpticalFunction_History_RP3_expd;

  typedef OpticalFunction_History<LHCOpticalFunctionsSetCollection,
                                  CTPPSOptics::RP::RP3,
                                  CTPPSOptics::Function::e32>
      OpticalFunction_History_RP3_e32;

  typedef OpticalFunction_History<LHCOpticalFunctionsSetCollection,
                                  CTPPSOptics::RP::RP3,
                                  CTPPSOptics::Function::evy>
      OpticalFunction_History_RP3_evy;

  typedef OpticalFunction_History<LHCOpticalFunctionsSetCollection,
                                  CTPPSOptics::RP::RP3,
                                  CTPPSOptics::Function::eLy>
      OpticalFunction_History_RP3_eLy;

  typedef OpticalFunction_History<LHCOpticalFunctionsSetCollection,
                                  CTPPSOptics::RP::RP3,
                                  CTPPSOptics::Function::eyd>
      OpticalFunction_History_RP3_eyd;

  typedef OpticalFunction_History<LHCOpticalFunctionsSetCollection,
                                  CTPPSOptics::RP::RP3,
                                  CTPPSOptics::Function::e42>
      OpticalFunction_History_RP3_e42;

  typedef OpticalFunction_History<LHCOpticalFunctionsSetCollection,
                                  CTPPSOptics::RP::RP3,
                                  CTPPSOptics::Function::evpy>
      OpticalFunction_History_RP3_evpy;

  typedef OpticalFunction_History<LHCOpticalFunctionsSetCollection,
                                  CTPPSOptics::RP::RP3,
                                  CTPPSOptics::Function::eLpy>
      OpticalFunction_History_RP3_eLpy;

  typedef OpticalFunction_History<LHCOpticalFunctionsSetCollection,
                                  CTPPSOptics::RP::RP3,
                                  CTPPSOptics::Function::eypd>
      OpticalFunction_History_RP3_eypd;


  typedef OpticalFunctionsInfo<LHCOpticalFunctionsSetCollection, CTPPSOptics::RP::RP123> OpticalFunctionsInfoPrint_RP123;

  typedef OpticalFunctionRPs_History<LHCOpticalFunctionsSetCollection,
                                     cond::payloadInspector::MULTI_IOV,
                                     1,
                                     CTPPSOptics::Function::eLx>
      OpticalFunctionsRPs;

}  // namespace

PAYLOAD_INSPECTOR_MODULE(LHCOpticalFunctionsSetCollection) {
  PAYLOAD_INSPECTOR_CLASS(OpticalFunction_History_RP3_evx);
  PAYLOAD_INSPECTOR_CLASS(OpticalFunction_History_RP3_eLx);
  PAYLOAD_INSPECTOR_CLASS(OpticalFunction_History_RP3_e14);
  PAYLOAD_INSPECTOR_CLASS(OpticalFunction_History_RP3_exd);
  PAYLOAD_INSPECTOR_CLASS(OpticalFunction_History_RP3_evpx);
  PAYLOAD_INSPECTOR_CLASS(OpticalFunction_History_RP3_eLpx);
  PAYLOAD_INSPECTOR_CLASS(OpticalFunction_History_RP3_e24);
  PAYLOAD_INSPECTOR_CLASS(OpticalFunction_History_RP3_expd);
  PAYLOAD_INSPECTOR_CLASS(OpticalFunction_History_RP3_e32);
  PAYLOAD_INSPECTOR_CLASS(OpticalFunction_History_RP3_evy);
  PAYLOAD_INSPECTOR_CLASS(OpticalFunction_History_RP3_eLy);
  PAYLOAD_INSPECTOR_CLASS(OpticalFunction_History_RP3_eyd);
  PAYLOAD_INSPECTOR_CLASS(OpticalFunction_History_RP3_e42);
  PAYLOAD_INSPECTOR_CLASS(OpticalFunction_History_RP3_evpy);
  PAYLOAD_INSPECTOR_CLASS(OpticalFunction_History_RP3_eLpy);
  PAYLOAD_INSPECTOR_CLASS(OpticalFunction_History_RP3_eypd);

  PAYLOAD_INSPECTOR_CLASS(OpticalFunctionsRPs);
  PAYLOAD_INSPECTOR_CLASS(OpticalFunctionsInfoPrint_RP123);
}