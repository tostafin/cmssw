import FWCore.ParameterSet.Config as cms

import sys

process = cms.Process('test')

# minimum logging
process.MessageLogger = cms.Service("MessageLogger",
    cerr = cms.untracked.PSet(
        enable = cms.untracked.bool(False)
    ),
    cout = cms.untracked.PSet(
        enable = cms.untracked.bool(True),
        threshold = cms.untracked.string('INFO')
    )
)


iov = int(7133383367994638336  if len(sys.argv) < 3 else sys.argv[2])
print("iov:", iov)

process.source = cms.Source('EmptyIOVSource',
    timetype = cms.string('timestamp'),
    firstValue = cms.uint64(iov  ),
    lastValue = cms.uint64(iov ),
    interval = cms.uint64(1)
)

# load info from database
process.load('CondCore.CondDB.CondDB_cfi')
process.CondDB.connect = 'sqlite_file:../python/lhcinfo_pop_test.db' # SQLite input

process.PoolDBESSource = cms.ESSource('PoolDBESSource',
    process.CondDB,
    DumpStats = cms.untracked.bool(True),
    toGet = cms.VPSet(
        cms.PSet(
            record = cms.string('LHCInfoPerLSRcd'),
            tag = cms.string('perLS_3_end')
        )
    )
)

process.LHCInfoPerLSAnalyzer = cms.EDAnalyzer('LHCInfoPerLSAnalyzer')

process.path = cms.Path(
    process.LHCInfoPerLSAnalyzer
)