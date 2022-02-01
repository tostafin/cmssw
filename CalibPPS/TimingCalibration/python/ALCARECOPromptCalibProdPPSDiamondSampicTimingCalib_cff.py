import FWCore.ParameterSet.Config as cms

#from RecoPPS.Local.totemTimingLocalReconstruction_cff import diamondSampicLocalReconstructionTask
from CalibPPS.TimingCalibration.PPSDiamondSampicTimingCalibrationPCLWorker_cfi import PPSDiamondSampicTimingCalibrationPCLWorker
#from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer

MEtoEDMConvertPPSDiamondSampicTimingCalib = cms.EDProducer('MEtoEDMConverter',
    Name = cms.untracked.string('MEtoEDMConverter'),
    Verbosity = cms.untracked.int32(0),
    Frequency = cms.untracked.int32(50),
    MEPathToSave = cms.untracked.string('AlCaReco/PPSDiamondSampicTimingCalibrationPCL')
)

# reco hit production
from RecoPPS.Local.totemTimingRecHits_cfi import totemTimingRecHits

#Diamond Sampic reconstruction flow
from RecoPPS.Local.diamondSampicLocalTracks_cfi import diamondSampicLocalTracks
diamondSampicLocalTracks.recHitsTag=cms.InputTag("totemTimingRecHits")
diamondSampicLocalReconstructionTask = cms.Task(
		totemTimingRecHits,
		diamondSampicLocalTracks)

taskALCARECOPromptCalibProdPPSDiamondSampicTimingCalib = cms.Task(
    diamondSampicLocalReconstructionTask,
    PPSDiamondSampicTimingCalibrationPCLWorker,
    MEtoEDMConvertPPSDiamondSampicTimingCalib
)
