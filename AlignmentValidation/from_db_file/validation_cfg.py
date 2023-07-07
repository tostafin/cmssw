import FWCore.ParameterSet.Config as cms
from Configuration.Eras.Era_Run3_cff import Run3

process = cms.Process('RECODQM', Run3)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100_000) )
process.verbosity = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
    cerr = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
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
        '/store/data/Run2023D/ZeroBias/AOD/PromptReco-v1/000/369/927/00000/18962b9c-1e4c-4612-9d46-f249b5dff72f.root'
    ),
)

from Configuration.AlCa.GlobalTag import GlobalTag
from Configuration.AlCa.autoCond import autoCond
process.GlobalTag = GlobalTag(process.GlobalTag, autoCond['run3_data_prompt'], '')

# local RP reconstruction chain with standard settings
process.load("RecoPPS.Configuration.recoCTPPS_cff")

# alignment calibration from sqlite
process.load('CondCore.CondDB.CondDB_cfi')
process.CondDB.connect = 'sqlite_file:alignment_results.db'  # SQLite input
process.PoolDBESSource = cms.ESSource('PoolDBESSource',
    process.CondDB,
    DumpStats=cms.untracked.bool(True),
    toGet=cms.VPSet(
        cms.PSet(
            record=cms.string('RPRealAlignmentRecord'),
            tag=cms.string('MyTag'),
            label=cms.untracked.string('MyLabel')
        )
    )
)

# CTPPS DQM modules
process.load("DQM.CTPPS.ctppsDQM_cff")
process.ctppsDiamondDQMSource.plotOnline = True
process.ctppsDiamondDQMSource.plotOffline = False

process.path = cms.Path(
    process.recoCTPPS *
    process.ctppsDQMOnlineSource *
    process.ctppsDQMOnlineHarvest
)

process.end_path = cms.EndPath(
    process.dqmEnv +
    process.dqmSaver
)

process.schedule = cms.Schedule(
    process.path,
    process.end_path
)
