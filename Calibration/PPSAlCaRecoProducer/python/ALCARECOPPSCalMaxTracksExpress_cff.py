import FWCore.ParameterSet.Config as cms

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt

ALCARECOPPSCalMaxTracksFilterExpress = hlt.hltHighLevel.clone(
  TriggerResultsTag = cms.InputTag("TriggerResults","","HLTX"),
  HLTPaths = ['HLT_PPSMaxTracksPerArm1_v1']
)

from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import *

totemRPRawToDigi.rawDataTag = cms.InputTag('hltPPSCalibrationRaw')
ctppsDiamondRawToDigi.rawDataTag = cms.InputTag('hltPPSCalibrationRaw')
totemTimingRawToDigi.rawDataTag = cms.InputTag('hltPPSCalibrationRaw')
ctppsPixelDigis.inputLabel = cms.InputTag('hltPPSCalibrationRaw')

ctppsRawToDigiTask = cms.Task(
  totemRPRawToDigi,
  ctppsDiamondRawToDigi,
  totemTimingRawToDigi,
  ctppsPixelDigis
)

ALCARECOPPSCalMaxTracksRaw2DigiExpress = cms.Sequence(ctppsRawToDigiTask)

from RecoPPS.Configuration.recoCTPPS_cff import *

seqALCARECOPPSCalMaxTracksRecoExpress = cms.Sequence( ALCARECOPPSCalMaxTracksFilterExpress  + ALCARECOPPSCalMaxTracksRaw2DigiExpress + recoCTPPS )
