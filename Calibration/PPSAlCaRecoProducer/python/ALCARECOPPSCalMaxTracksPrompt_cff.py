import FWCore.ParameterSet.Config as cms

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt

ALCARECOPPSCalMaxTracksFilterPrompt = hlt.hltHighLevel.clone(
  TriggerResultsTag = cms.InputTag("TriggerResults","","HLTX"),
  HLTPaths = ['HLT_PPSMaxTracksPerRP4_v1']
)

from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import totemTriggerRawToDigi as _totemTriggerRawToDigi
from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import totemRPRawToDigi as _totemRPRawToDigi
from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import ctppsDiamondRawToDigi as _ctppsDiamondRawToDigi
from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import totemTimingRawToDigi as _totemTimingRawToDigi
from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import ctppsPixelDigis as _ctppsPixelDigis

totemTriggerRawToDigiAlCaRecoProducerPrompt = _totemTriggerRawToDigi.clone(rawDataTag = cms.InputTag('hltPPSCalibrationRaw'))
totemRPRawToDigiAlCaRecoProducerPrompt = _totemRPRawToDigi.clone(rawDataTag = cms.InputTag('hltPPSCalibrationRaw'))
ctppsDiamondRawToDigiAlCaRecoProducerPrompt  = _ctppsDiamondRawToDigi.clone(rawDataTag = cms.InputTag('hltPPSCalibrationRaw'))
totemTimingRawToDigiAlCaRecoProducerPrompt = _totemTimingRawToDigi.clone(rawDataTag = cms.InputTag('hltPPSCalibrationRaw'))
ctppsPixelDigisAlCaRecoProducerPrompt = _ctppsPixelDigis.clone(inputLabel = cms.InputTag('hltPPSCalibrationRaw'))

ctppsRawToDigiTaskAlCaRecoProducerPrompt = cms.Task(
  totemTriggerRawToDigiAlCaRecoProducerPrompt,
  totemRPRawToDigiAlCaRecoProducerPrompt,
  ctppsDiamondRawToDigiAlCaRecoProducerPrompt,
  totemTimingRawToDigiAlCaRecoProducerPrompt,
  ctppsPixelDigisAlCaRecoProducerPrompt
)

ALCARECOPPSCalMaxTracksRaw2DigiPrompt = cms.Sequence(ctppsRawToDigiTaskAlCaRecoProducerPrompt)

from RecoPPS.Configuration.recoCTPPS_cff import recoCTPPS

seqALCARECOPPSCalMaxTracksRecoPrompt = cms.Sequence( ALCARECOPPSCalMaxTracksFilterPrompt + ALCARECOPPSCalMaxTracksRaw2DigiPrompt + recoCTPPS)
