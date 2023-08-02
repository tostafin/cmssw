import sys

import FWCore.ParameterSet.Config as cms

process = cms.Process('test')

# minimum logging
process.MessageLogger = cms.Service("MessageLogger",
    cerr = cms.untracked.PSet(
        enable = cms.untracked.bool(False)
    ),
    cout = cms.untracked.PSet(
        enable = cms.untracked.bool(False),
        # threshold = cms.untracked.string('INFO')
    )
)

# process.source = cms.Source('EmptyIOVSource',
#     timetype = cms.string('timestamp'),
#     firstValue = cms.uint64(7127126554127106048),
#     lastValue = cms.uint64(7127126554127106048),
#     interval = cms.uint64(1)
# )

# process.source = cms.Source('EmptyIOVSource',
#     timetype = cms.string('runnumber'),
#     firstValue = cms.uint64(355905),
#     lastValue = cms.uint64(355912),
#     interval = cms.uint64(1)
# )


# iov = int(7133429435813855232 if len(sys.argv) < 3 else sys.argv[2])
iov = int(7133428598295232512 if len(sys.argv) < 3 else sys.argv[2])
# print("iov:", iov)

process.source = cms.Source('EmptyIOVSource',
    timetype = cms.string('timestamp'),
    firstValue = cms.uint64(iov  ),
    lastValue = cms.uint64(iov ),
    interval = cms.uint64(1)
)

# load info from database
process.load('CondCore.CondDB.CondDB_cfi')
process.CondDB.connect = 'sqlite_file:../python/lhcinfo_pop_test_9019.db' # SQLite input

process.PoolDBESSource = cms.ESSource('PoolDBESSource',
    process.CondDB,
    DumpStats = cms.untracked.bool(True),
    toGet = cms.VPSet(
        cms.PSet(
            record = cms.string('LHCInfoRcd'),
            tag = cms.string('LHCInfoEndFill_Run3_validation_v1')
        )
    )
)

process.LHCInfoAnalyzer = cms.EDAnalyzer('LHCInfoAnalyzer')

process.path = cms.Path(
    process.LHCInfoAnalyzer
)