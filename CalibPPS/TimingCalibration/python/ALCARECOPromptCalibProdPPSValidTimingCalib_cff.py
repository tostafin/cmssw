import FWCore.ParameterSet.Config as cms

from CalibPPS.TimingCalibration.ppsTimingCalibrationPCLValidWorker_cfi import ppsTimingCalibrationPCLValidWorker

MEtoEDMConvertPPSTimingCalib = cms.EDProducer('MEtoEDMConverter',
    Name = cms.untracked.string('MEtoEDMConverter'),
    Verbosity = cms.untracked.int32(0),
    Frequency = cms.untracked.int32(50),
    MEPathToSave = cms.untracked.string('AlCaReco/PPSTimingCalibrationPCL'),
    deleteAfterCopy = cms.untracked.bool(True),
)

ppsTimingCalibrationPCLValidWorker.diamondRecHitTags=cms.VInputTag(cms.InputTag("ctppsDiamondRecHitsAlCaRecoProducer"),
                                         			  cms.InputTag("ctppsDiamondRecHits"))

taskALCARECOPromptCalibProdPPSTimingCalib = cms.Task(
    ppsTimingCalibrationPCLValidWorker,
    MEtoEDMConvertPPSTimingCalib
)
