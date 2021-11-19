import FWCore.ParameterSet.Config as cms

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
hltFilter = copy.deepcopy(hltHighLevel)
hltFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLTX")
hltFilter.HLTPaths = ['HLT_PPSMaxTracksPerRP4_v1']


from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import *
from RecoPPS.Configuration.recoCTPPS_cff import *

totemTriggerRawToDigi.rawDataTag = cms.InputTag('hltPPSPrCalibrationRaw')
totemRPRawToDigi.rawDataTag = cms.InputTag('hltPPSPrCalibrationRaw')
ctppsDiamondRawToDigi.rawDataTag = cms.InputTag('hltPPSPrCalibrationRaw')
totemTimingRawToDigi.rawDataTag = cms.InputTag('hltPPSPrCalibrationRaw')
ctppsPixelDigis.inputLabel = cms.InputTag('hltPPSPrCalibrationRaw')

ctppsRawToDigiTask = cms.Task(
  totemTriggerRawToDigi,
  totemRPRawToDigi,
  ctppsDiamondRawToDigi,
  totemTimingRawToDigi,
  ctppsPixelDigis
)
ctppsRawToDigi = cms.Sequence(ctppsRawToDigiTask)

alcaPPSreco = cms.Sequence( hltFilter + ctppsRawToDigi + recoCTPPS )