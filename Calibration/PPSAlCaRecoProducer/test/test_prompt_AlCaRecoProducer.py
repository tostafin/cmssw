import FWCore.ParameterSet.Config as cms

# load era modifier to run on 2018 data
from Configuration.Eras.Modifier_ctpps_2018_cff import ctpps_2018

process = cms.Process( 'ppsAlCaReco',ctpps_2018)

process.load('Calibration.PPSAlCaRecoProducer.PPSAlCaRecoProducer_cff') 

# fix FEDRawDataCollection name --> is it going to be like this in Run 3?
# process.totemTriggerRawToDigi.rawDataTag = cms.InputTag('hltPPSExpCalibrationRaw')
# process.totemRPRawToDigi.rawDataTag = cms.InputTag('hltPPSExpCalibrationRaw')
# process.ctppsDiamondRawToDigi.rawDataTag = cms.InputTag('hltPPSExpCalibrationRaw')
# process.totemTimingRawToDigi.rawDataTag = cms.InputTag('hltPPSExpCalibrationRaw')
# process.ctppsPixelDigis.inputLabel = cms.InputTag('hltPPSExpCalibrationRaw')

process.totemTriggerRawToDigi.rawDataTag = cms.InputTag('hltPPSPrCalibrationRaw')
process.totemRPRawToDigi.rawDataTag = cms.InputTag('hltPPSPrCalibrationRaw')
process.ctppsDiamondRawToDigi.rawDataTag = cms.InputTag('hltPPSPrCalibrationRaw')
process.totemTimingRawToDigi.rawDataTag = cms.InputTag('hltPPSPrCalibrationRaw')
process.ctppsPixelDigis.inputLabel = cms.InputTag('hltPPSPrCalibrationRaw')

# this is needed when running on the express stream, otherwise the run will 
# crash when only the prompt HLT passed and the express object is empty
# Andrea: not sure if this can be done when running in global...
# process.options = cms.PSet(
#     SkipEvent = cms.untracked.vstring('ProductNotFound')
# )

process.recoOutputALCAPPS = cms.OutputModule( 'PoolOutputModule',
    fileName = cms.untracked.string( 'outputALCAPPS_RECO.root' ),
    outputCommands = process.alcarecoPPSEventContent.outputCommands
)

# from RecoPPS.Configuration.RecoCTPPS_EventContent_cff import RecoCTPPSAOD
# process.recoOutputALCAPPS.outputCommands.extend(RecoCTPPSAOD.outputCommands)

process.recoOutput = cms.EndPath( process.recoOutputALCAPPS )
process.reco = cms.Path( process.alcaPPSreco )

process.HLTSchedule = cms.Schedule( *(process.recoOutput, process.reco))

# load GT
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run3_data_express')
# process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run3_data_prompt') # --> No LHCInfo 

process.source = cms.Source( 'PoolSource',
    fileNames = cms.untracked.vstring(
        'file:outputALCAPPS.root',
    ),
    inputCommands = cms.untracked.vstring(
        'keep *'
    )
)

# limit the number of events to be processed
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32( -1 )
)
