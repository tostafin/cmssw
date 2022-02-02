import FWCore.ParameterSet.Config as cms

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt

ALCARECOPPSCalMaxTracksFilter = hlt.hltHighLevel.clone(
  eventSetupPathsKey = 'PPSCalMaxTracks',
)

from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import *

totemRPRawToDigi.rawDataTag = 'hltPPSCalibrationRaw'
ctppsDiamondRawToDigi.rawDataTag = 'hltPPSCalibrationRaw'
totemTimingRawToDigi.rawDataTag = 'hltPPSCalibrationRaw'
ctppsPixelDigis.inputLabel = 'hltPPSCalibrationRaw'

ctppsRawToDigiTask = cms.Task(
  totemRPRawToDigi,
  ctppsDiamondRawToDigi,
  totemTimingRawToDigi,
  ctppsPixelDigis
)

ALCARECOPPSCalMaxTracksRaw2Digi = cms.Sequence(ctppsRawToDigiTask)

from RecoPPS.Configuration.recoCTPPS_cff import *

seqALCARECOPPSCalMaxTracksReco = cms.Sequence( ALCARECOPPSCalMaxTracksFilter  + ALCARECOPPSCalMaxTracksRaw2Digi + recoCTPPS )
