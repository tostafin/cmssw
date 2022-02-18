import FWCore.ParameterSet.Config as cms
import CalibPPS.Configuration.PCL as config 
from CalibPPS.AlignmentGlobal.ppsAlignmentWorker_cfi import ppsAlignmentWorker as ppsAlignmentWorker_

ppsAlignmentWorker = ppsAlignmentWorker_.clone()

if(config.useLegacyProducer != True):
    ppsAlignmentWorker.tagTracks = cms.InputTag("ctppsLocalTrackLiteProducerAlCaRecoProducer")

MEtoEDMConvertPPSAlignment = cms.EDProducer('MEtoEDMConverter',
    Name=cms.untracked.string('MEtoEDMConverter'),
    Verbosity=cms.untracked.int32(0),
    Frequency=cms.untracked.int32(50),
    MEPathToSave=cms.untracked.string('AlCaReco/PPSAlignment'),
    deleteAfterCopy=cms.untracked.bool(True)
)

taskALCARECOPromptCalibProdPPSAlignment = cms.Task(
    ppsAlignmentWorker,
    MEtoEDMConvertPPSAlignment
)
