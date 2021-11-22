
import FWCore.ParameterSet.Config as cms

# load era modifier to run on 2018 data
from Configuration.Eras.Modifier_ctpps_2018_cff import ctpps_2018

process = cms.Process( 'ppsAlCaReco',ctpps_2018)

process.load('Calibration.PPSAlCaRecoProducer.PPSAlCaRecoProducer_cff') 
process.load('Calibration.PPSAlCaRecoProducer.PPSAlCaReco_EventContent_cff') 

process.recoOutputALCAPPS = cms.OutputModule( 'PoolOutputModule',
    fileName = cms.untracked.string( 'outputALCAPPS_RECO_prompt.root' ),
    outputCommands = process.alcarecoPPSEventContent.outputCommands
)

process.reco = cms.Path( process.alcaPPSreco_prompt )
process.recoOutput = cms.EndPath( process.recoOutputALCAPPS )

process.HLTSchedule = cms.Schedule( *(process.recoOutput, process.reco))

process.options = cms.PSet(
    wantSummary = cms.untracked.bool( True ),
)

# load GT
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run3_data_express')
# process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run3_data_prompt') # --> No LHCInfo, temporarily using the express GT

process.source = cms.Source( 'PoolSource',
    fileNames = cms.untracked.vstring(
        'file:outputALCAPPS_single.root',
    ),
    inputCommands = cms.untracked.vstring(
        'keep *'
    )
)

# limit the number of events to be processed
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32( -1 )
)
