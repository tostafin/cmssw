import FWCore.ParameterSet.Config as cms

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt

ALCARECOPPSCalMaxTracksFilterExpress = hlt.hltHighLevel.clone(
  TriggerResultsTag = cms.InputTag("TriggerResults","","HLTX"),
  HLTPaths = ['HLT_PPSMaxTracksPerArm1_v1']
)

from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import totemRPRawToDigi as _totemRPRawToDigi
from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import ctppsDiamondRawToDigi as _ctppsDiamondRawToDigi
from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import totemTimingRawToDigi as _totemTimingRawToDigi
from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import ctppsPixelDigis as _ctppsPixelDigis

#totemRPRawToDigi = _totemRPRawToDigi.clone(rawDataTag = 'hltPPSCalibrationRaw')
#ctppsDiamondRawToDigi  = _ctppsDiamondRawToDigi.clone(rawDataTag = 'hltPPSCalibrationRaw')
#totemTimingRawToDigi = _totemTimingRawToDigi.clone(rawDataTag = 'hltPPSCalibrationRaw')
ctppsPixelDigis = _ctppsPixelDigis.clone(inputLabel = 'hltPPSCalibrationRaw')

ctppsRawToDigiTask = cms.Task(
  #totemRPRawToDigi,
  #ctppsDiamondRawToDigi,
  # totemTimingRawToDigi,
  ctppsPixelDigis
)

# commenting out totemRPRawToDigi, as uncommented version gives following error
'''
Begin processing the 1st record. Run 322022, Event 1760583922, LumiSection 1025 on stream 0 at 01-Feb-2022 13:27:17.613 CET
----- Begin Fatal Exception 01-Feb-2022 13:27:18 CET-----------------------
An exception of category 'NoRecord' occurred while
   [0] Processing  Event run: 322022 lumi: 1025 event: 1760583922 stream: 0
   [1] Running path 'ALCARECOoutput_step'
   [2] Prefetching for module PoolOutputModule/'ALCARECOoutput'
   [3] Calling method for module TotemVFATRawToDigi/'ctppsDiamondRawToDigi'
Exception Message:
No "TotemReadoutRcd" record found in the EventSetup.

 Please add an ESSource or ESProducer that delivers such a record.
'''


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
