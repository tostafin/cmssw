import FWCore.ParameterSet.Config as cms

# HLT filter
import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
ALCARECOPPSCalMaxTracksFilter = hlt.hltHighLevel.clone(
  eventSetupPathsKey = 'PPSCalMaxTracks',
)

# EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff contains these ESSource modules:
#   - totemDAQMappingESSourceXML_TrackingStrip
#   - totemDAQMappingESSourceXML_TimingDiamond 
#   - totemDAQMappingESSourceXML_TotemTiming 

from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import totemDAQMappingESSourceXML_TimingDiamond 

# in this minimal config we are not adding RECO to execution path
# however if following import line is commented out (or removed) we have a crash and error message:
#  `An entry in sequence pathALCARECOPromptCalibProdPPSDiamondSampicTimingCalib has no label`
# does it mean that `PPSDiamondSampicTimingCalib` producer needs some objects from recoCTPPS_cff ?
# before this PR AlCa Reco producer made by Laurent (PPSCalTrackBasedSel) was used, now it is disabled
# this mean that CalibPPS/TimingCalibration/python/ALCARECOPPSCalTrackBasedSel_cff.py it not imported now
# but ALCARECOPPSCalTrackBasedSel_cff.py has inside `from RecoPPS.Configuration.recoCTPPS_cff import *`
# this may mean that `PPSDiamondSampicTimingCalib` has a hidden dependency on `PPSCalTrackBasedSel`
# maybe all necessary imports should be added to `PPSDiamondSampicTimingCalib`
from RecoPPS.Configuration.recoCTPPS_cff import *

# raw2digi step
from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import ctppsDiamondRawToDigi as _ctppsDiamondRawToDigi
ctppsDiamondRawToDigiAlCaRecoProducer  = _ctppsDiamondRawToDigi.clone(rawDataTag = cms.InputTag('hltPPSCalibrationRaw'))

# rechits step
from RecoPPS.Local.ctppsDiamondRecHits_cfi import ctppsDiamondRecHits as _ctppsDiamondRecHits
ctppsDiamondRecHitsAlCaRecoProducer = _ctppsDiamondRecHits.clone(digiTag = cms.InputTag("ctppsDiamondRawToDigiAlCaRecoProducer","TimingDiamond"))

# generation of DIGIs seem to be working, try following line
seqALCARECOPPSCalMaxTracksReco = cms.Sequence( ALCARECOPPSCalMaxTracksFilter + ctppsDiamondRawToDigiAlCaRecoProducer )

# generation of Diamond rec hits fails, uncomment following line to check
#seqALCARECOPPSCalMaxTracksReco = cms.Sequence( ALCARECOPPSCalMaxTracksFilter + ctppsDiamondRawToDigiAlCaRecoProducer + ctppsDiamondRecHitsAlCaRecoProducer)
