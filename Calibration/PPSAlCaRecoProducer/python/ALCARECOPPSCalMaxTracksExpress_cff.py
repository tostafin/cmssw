import FWCore.ParameterSet.Config as cms

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt

ALCARECOPPSCalMaxTracksFilterExpress = hlt.hltHighLevel.clone(
  TriggerResultsTag = cms.InputTag("TriggerResults","","HLTX"),
  HLTPaths = ['HLT_PPSMaxTracksPerArm1_v1']
)

from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import totemRPRawToDigi as _totemRPRawToDigi
# from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import ctppsDiamondRawToDigi as _ctppsDiamondRawToDigi
# from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import totemTimingRawToDigi as _totemTimingRawToDigi
from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import ctppsPixelDigis as _ctppsPixelDigis

totemRPRawToDigi = _totemRPRawToDigi.clone(rawDataTag = 'hltPPSCalibrationRaw')
# ctppsDiamondRawToDigi  = _ctppsDiamondRawToDigi.clone(rawDataTag = 'hltPPSCalibrationRaw')
# totemTimingRawToDigi = _totemTimingRawToDigi.clone(rawDataTag = 'hltPPSCalibrationRaw')
ctppsPixelDigis = _ctppsPixelDigis.clone(inputLabel = 'hltPPSCalibrationRaw')

ctppsRawToDigiTask = cms.Task(
  totemRPRawToDigi,
  # ctppsDiamondRawToDigi,
  # totemTimingRawToDigi,
  ctppsPixelDigis
)

ALCARECOPPSCalMaxTracksRaw2DigiExpress = cms.Sequence(ctppsRawToDigiTask)

from RecoPPS.Local.totemRPLocalReconstruction_cff import *
from RecoPPS.Local.ctppsDiamondLocalReconstruction_cff import *
from RecoPPS.Local.totemTimingLocalReconstruction_cff import *
from RecoPPS.Local.ctppsPixelLocalReconstruction_cff import *

from RecoPPS.Local.ctppsLocalTrackLiteProducer_cff import ctppsLocalTrackLiteProducer

from RecoPPS.ProtonReconstruction.ctppsProtons_cff import *

from Geometry.VeryForwardGeometry.geometryRPFromDB_cfi import *
from CalibPPS.ESProducers.ppsTopology_cff import *

recoCTPPSTask = cms.Task(
    # totemRPLocalReconstructionTask ,
    # ctppsDiamondLocalReconstructionTask ,
    # diamondSampicLocalReconstructionTask ,
    ctppsPixelLocalReconstructionTask ,
    ctppsLocalTrackLiteProducer ,
    ctppsProtons
)

recoCTPPS = cms.Sequence(recoCTPPSTask)

seqALCARECOPPSCalMaxTracksRecoExpress = cms.Sequence( ALCARECOPPSCalMaxTracksFilterExpress  + ALCARECOPPSCalMaxTracksRaw2DigiExpress + recoCTPPS )
