import FWCore.ParameterSet.Config as cms

process = cms.Process('test')

process.source = cms.Source('EmptyIOVSource',
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    interval = cms.uint64(1)
)

# output service for database
process.load('CondCore.CondDB.CondDB_cfi')
process.CondDB.connect = 'sqlite_file:LHCInfoPerFill_endFill_Run3_mc_v1.db' # SQLite output

process.PoolDBOutputService = cms.Service('PoolDBOutputService',
    process.CondDB,
    timetype = cms.untracked.string('runnumber'),
    toPut = cms.VPSet(
        cms.PSet(
            record = cms.string('LHCInfoPerFillRcd'),
            tag = cms.string('LHCInfoPerFill_endFill_Run3_mc_v1'),
        )
    )
)

process.LHCInfoPerFillWriter = cms.EDAnalyzer('LHCInfoPerFillWriter')

process.path = cms.Path(
    process.LHCInfoPerFillWriter
)
