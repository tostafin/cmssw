import FWCore.ParameterSet.Config as cms

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt

ALCARECOPPSCalMaxTracksFilter = hlt.hltHighLevel.clone(
  TriggerResultsTag = cms.InputTag("TriggerResults","","HLTX"),
  eventSetupPathsKey = 'PPSCalMaxTracks',
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

ALCARECOPPSCalMaxTracksRaw2Digi = cms.Sequence(ctppsRawToDigiTask)

from RecoPPS.Configuration.recoCTPPS_cff import *

seqALCARECOPPSCalMaxTracksReco = cms.Sequence( ALCARECOPPSCalMaxTracksFilter  + ALCARECOPPSCalMaxTracksRaw2Digi + recoCTPPS )
