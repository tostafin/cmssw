import FWCore.ParameterSet.Config as cms

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt

ALCARECOPPSCalMaxTracksFilterExpress = hlt.hltHighLevel.clone(
  TriggerResultsTag = cms.InputTag("TriggerResults","","HLTX"),
  HLTPaths = ['HLT_PPSMaxTracksPerArm1_v1']
)

from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import totemTriggerRawToDigi as _totemTriggerRawToDigi
from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import totemRPRawToDigi as _totemRPRawToDigi
from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import ctppsDiamondRawToDigi as _ctppsDiamondRawToDigi
from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import totemTimingRawToDigi as _totemTimingRawToDigi
from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import ctppsPixelDigis as _ctppsPixelDigis

totemTriggerRawToDigiAlCaRecoProducerExpress = _totemTriggerRawToDigi.clone(rawDataTag = cms.InputTag('hltPPSCalibrationRaw'))
totemRPRawToDigiAlCaRecoProducerExpress = _totemRPRawToDigi.clone(rawDataTag = cms.InputTag('hltPPSCalibrationRaw'))
ctppsDiamondRawToDigiAlCaRecoProducerExpress  = _ctppsDiamondRawToDigi.clone(rawDataTag = cms.InputTag('hltPPSCalibrationRaw'))
totemTimingRawToDigiAlCaRecoProducerExpress = _totemTimingRawToDigi.clone(rawDataTag = cms.InputTag('hltPPSCalibrationRaw'))
ctppsPixelDigisAlCaRecoProducerExpress = _ctppsPixelDigis.clone(inputLabel = cms.InputTag('hltPPSCalibrationRaw'))

ctppsRawToDigiTaskAlCaRecoProducerExpress = cms.Task(
  totemTriggerRawToDigiAlCaRecoProducerExpress,
  totemRPRawToDigiAlCaRecoProducerExpress,
  ctppsDiamondRawToDigiAlCaRecoProducerExpress,
  totemTimingRawToDigiAlCaRecoProducerExpress,
  ctppsPixelDigisAlCaRecoProducerExpress
)

ALCARECOPPSCalMaxTracksRaw2DigiExpress = cms.Sequence(ctppsRawToDigiTaskAlCaRecoProducerExpress)

from RecoPPS.Configuration.recoCTPPS_cff import recoCTPPS

seqALCARECOPPSCalMaxTracksRecoExpress = cms.Sequence( ALCARECOPPSCalMaxTracksFilterExpress + ALCARECOPPSCalMaxTracksRaw2DigiExpress + recoCTPPS)
