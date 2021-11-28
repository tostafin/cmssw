import FWCore.ParameterSet.Config as cms

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt

ALCARECOPPSCalMaxTracksFilterExpress = hlt.hltHighLevel.clone(
  TriggerResultsTag = cms.InputTag("TriggerResults","","HLTX"),
  HLTPaths = ['HLT_PPSMaxTracksPerArm1_v1']
)

from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import *
from RecoPPS.Configuration.recoCTPPS_cff import *

totemTriggerRawToDigi.rawDataTag = cms.InputTag('hltPPSCalibrationRaw')
totemRPRawToDigi.rawDataTag = cms.InputTag('hltPPSCalibrationRaw')
ctppsDiamondRawToDigi.rawDataTag = cms.InputTag('hltPPSCalibrationRaw')
totemTimingRawToDigi.rawDataTag = cms.InputTag('hltPPSCalibrationRaw')
ctppsPixelDigis.inputLabel = cms.InputTag('hltPPSCalibrationRaw')

ctppsRawToDigiTask = cms.Task(
  totemTriggerRawToDigi,
  totemRPRawToDigi,
  ctppsDiamondRawToDigi,
  totemTimingRawToDigi,
  ctppsPixelDigis
)
ctppsRawToDigi = cms.Sequence(ctppsRawToDigiTask)

seqALCARECOPPSCalMaxTracksRecoExpress = cms.Sequence( ALCARECOPPSCalMaxTracksFilterExpress + ctppsRawToDigi + recoCTPPS )
