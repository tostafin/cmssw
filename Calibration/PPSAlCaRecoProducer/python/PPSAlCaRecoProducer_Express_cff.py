import FWCore.ParameterSet.Config as cms

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
hltFilter = copy.deepcopy(hltHighLevel)
hltFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLTX")
hltFilter.HLTPaths = ['HLT_PPSMaxTracksPerArm1_v1']

from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import *
from RecoPPS.Configuration.recoCTPPS_cff import *

totemTriggerRawToDigi.rawDataTag = cms.InputTag('hltPPSExpCalibrationRaw')
totemRPRawToDigi.rawDataTag = cms.InputTag('hltPPSExpCalibrationRaw')
ctppsDiamondRawToDigi.rawDataTag = cms.InputTag('hltPPSExpCalibrationRaw')
totemTimingRawToDigi.rawDataTag = cms.InputTag('hltPPSExpCalibrationRaw')
ctppsPixelDigis.inputLabel = cms.InputTag('hltPPSExpCalibrationRaw')

ctppsRawToDigiTask = cms.Task(
  totemTriggerRawToDigi,
  totemRPRawToDigi,
  ctppsDiamondRawToDigi,
  totemTimingRawToDigi,
  ctppsPixelDigis
)
ctppsRawToDigi = cms.Sequence(ctppsRawToDigiTask)

alcaPPSreco = cms.Sequence( hltFilter + ctppsRawToDigi + recoCTPPS )