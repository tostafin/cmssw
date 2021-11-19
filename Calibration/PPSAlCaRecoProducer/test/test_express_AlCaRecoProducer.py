
import FWCore.ParameterSet.Config as cms

# load era modifier to run on 2018 data
from Configuration.Eras.Modifier_ctpps_2018_cff import ctpps_2018

process = cms.Process( 'ppsAlCaReco',ctpps_2018)

process.load('Calibration.PPSAlCaRecoProducer.PPSAlCaRecoProducer_Express_cff') 
process.load('Calibration.PPSAlCaRecoProducer.PPSAlCaReco_EventContent_cff') 

process.recoOutputALCAPPS = cms.OutputModule( 'PoolOutputModule',
    fileName = cms.untracked.string( 'outputALCAPPS_RECO.root' ),
    outputCommands = process.alcarecoPPSEventContent.outputCommands
)

process.recoOutput = cms.EndPath( process.recoOutputALCAPPS )
process.reco = cms.Path( process.alcaPPSreco )

process.HLTSchedule = cms.Schedule( *(process.recoOutput, process.reco))

process.options = cms.PSet(
    wantSummary = cms.untracked.bool( True ),
)

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
