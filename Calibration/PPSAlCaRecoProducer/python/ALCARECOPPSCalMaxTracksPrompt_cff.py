import FWCore.ParameterSet.Config as cms

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt

ALCARECOPPSCalMaxTracksFilterPrompt = hlt.hltHighLevel.clone(
  TriggerResultsTag = cms.InputTag("TriggerResults","","HLTX"),
  HLTPaths = ['HLT_PPSMaxTracksPerRP4_v1']
)

from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import *
from RecoPPS.Configuration.recoCTPPS_cff import *

_totemTriggerRawToDigi = totemTriggerRawToDigi.clone(rawDataTag = cms.InputTag('hltPPSCalibrationRaw'))
_totemTriggerRawToDigi.setLabel("totemTriggerRawToDigi")

_totemRPRawToDigi = totemRPRawToDigi.clone(rawDataTag = cms.InputTag('hltPPSCalibrationRaw'))
_totemRPRawToDigi.setLabel("totemRPRawToDigi")

_ctppsDiamondRawToDigi = ctppsDiamondRawToDigi.clone(rawDataTag = cms.InputTag('hltPPSCalibrationRaw'))
_ctppsDiamondRawToDigi.setLabel("ctppsDiamondRawToDigi")

_totemTimingRawToDigi = totemTimingRawToDigi.clone(rawDataTag = cms.InputTag('hltPPSCalibrationRaw'))
_totemTimingRawToDigi.setLabel("totemTimingRawToDigi")

_ctppsPixelDigis = ctppsPixelDigis.clone(inputLabel = cms.InputTag('hltPPSCalibrationRaw'))
_ctppsPixelDigis.setLabel("CTPPSPixelRawToDigi")

_ctppsRawToDigiTask = cms.Task(
  _totemTriggerRawToDigi,
  _totemRPRawToDigi,
  _ctppsDiamondRawToDigi,
  _totemTimingRawToDigi,
  _ctppsPixelDigis
)

_ctppsRawToDigi = cms.Sequence(_ctppsRawToDigiTask)

seqALCARECOPPSCalMaxTracksRecoPrompt = cms.Sequence( ALCARECOPPSCalMaxTracksFilterPrompt + _ctppsRawToDigi + recoCTPPS )