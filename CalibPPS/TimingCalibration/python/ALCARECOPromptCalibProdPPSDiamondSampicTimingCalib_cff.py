import FWCore.ParameterSet.Config as cms
import CalibPPS.Configuration.PCL as config

from RecoPPS.Configuration.recoCTPPS_cff import totemTimingRecHits, diamondSampicLocalTracks
from CalibPPS.TimingCalibration.PPSDiamondSampicTimingCalibrationPCLWorker_cfi import PPSDiamondSampicTimingCalibrationPCLWorker
from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer

MEtoEDMConvertPPSDiamondSampicTimingCalib = cms.EDProducer('MEtoEDMConverter',
    Name = cms.untracked.string('MEtoEDMConverter'),
    Verbosity = cms.untracked.int32(0),
    Frequency = cms.untracked.int32(50),
    MEPathToSave = cms.untracked.string('AlCaReco/PPSDiamondSampicTimingCalibrationPCL')
)

if(config.useLegacyProducer != True):
    totemTimingRecHits.digiTag=cms.InputTag("totemTimingRawToDigiAlCaRecoProducer","TotemTiming")
    diamondSampicLocalTracks.recHitsTag=cms.InputTag("totemTimingRecHitsAlCaRecoProducer")
diamondSampicLocalReconstructionTask=cms.Task(
		totemTimingRecHits,
		diamondSampicLocalTracks)
		
taskALCARECOPromptCalibProdPPSDiamondSampicTimingCalib = cms.Task(
    diamondSampicLocalReconstructionTask,
    PPSDiamondSampicTimingCalibrationPCLWorker,
    MEtoEDMConvertPPSDiamondSampicTimingCalib
)
