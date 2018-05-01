import FWCore.ParameterSet.Config as cms
import string

process = cms.Process('RECODQM')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.verbosity = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
    statistics = cms.untracked.vstring(),
    destinations = cms.untracked.vstring('cerr'),
    cerr = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    )
)

    # import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# load DQM framework
process.load("DQM.Integration.config.environment_cfi")
process.dqmEnv.subSystemFolder = "CTPPS"
process.dqmEnv.eventInfoFolder = "EventInfo"
process.dqmSaver.path = ""
process.dqmSaver.tag = "CTPPS"

# raw data source
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    '/store/data/Commissioning2018/ZeroBias10/AOD/PromptReco-v1/000/314/276/00000/0099E2CC-0743-E811-9CF7-FA163E7270C7.root',
  )
)


from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = process.GlobalTag = GlobalTag(process.GlobalTag, '101X_dataRun2_Prompt_Candidate_2018_04_04_13_35_31', '')

process.load('Geometry.VeryForwardGeometry.geometryRPFromDD_2018_cfi')

# CTPPS DQM modules
process.load("DQM.CTPPS.ctppsDQM_cff")
process.ctppsDiamondDQMSource.excludeMultipleHits = cms.bool(True);

process.path = cms.Path(
    process.ctppsDQM
    )


process.end_path = cms.EndPath(
    process.dqmEnv +
    process.dqmSaver
)

process.schedule = cms.Schedule(
    process.path,
    process.end_path
)
