########## Configuration ##########
# if set to True, a file with logs will be produced.
produce_logs = False

# Specifies a method used to load conditions. Can be set to:
#   - "ESSource" - configs loaded directly via ESSource
#   - "local_sqlite" - configs retrieved from an SQLite file (input_conditions)
#   - "db" - configs retrieved from the Conditions DB.
conditions_input = "ESSource"

# Input SQLite file. Used only if conditions_input is set to "local_sqlite".
input_sqlite = 'sqlite_file:alignment_config.db'

# Database tag. Used only if conditions_input is set to "local_sqlite" or "db".
db_tag = 'PPSAlignmentConfiguration_v1_express'

# Path for a ROOT file with the histograms
output_distributions = 'dqm_run_distributions_test.root'
###################################

import FWCore.ParameterSet.Config as cms

from input_files_cff import input_files

process = cms.Process('testDistributions')

process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("CalibPPS.AlignmentGlobal.ppsAlignmentWorker_cfi")
process.load("DQMServices.Core.DQMStore_cfi")

# Update the product labels for the worker inputs
process.ppsAlignmentWorker.tracksTags = cms.VInputTag(
    cms.InputTag("ctppsLocalTrackLiteProducer"),
    cms.InputTag("ctppsLocalTrackLiteProducerAlCaRecoProducer")
)

# Message Logger
if produce_logs:
    process.MessageLogger = cms.Service("MessageLogger",
        destinations = cms.untracked.vstring('run_distributions',
            'cout'
        ),
        run_distributions = cms.untracked.PSet(
            threshold = cms.untracked.string("INFO")
        ),
        cout = cms.untracked.PSet(
            threshold = cms.untracked.string('WARNING')
        )
    )
else:
    process.MessageLogger = cms.Service("MessageLogger",
        destinations = cms.untracked.vstring('cout'),
        cout = cms.untracked.PSet(
            threshold = cms.untracked.string('WARNING')
        )
    )

# Source
process.source = cms.Source("PoolSource",
    fileNames = input_files
)
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100_000))

# Event Setup
if conditions_input == 'ESSource':
    from config_cff import ppsAlignmentConfigESSource
    process.ppsAlignmentConfigESSource = ppsAlignmentConfigESSource
elif conditions_input == 'local_sqlite':
    process.load("CondCore.CondDB.CondDB_cfi")
    process.CondDB.connect = input_sqlite
    process.PoolDBESSource = cms.ESSource("PoolDBESSource",
        process.CondDB,
        DumbStat = cms.untracked.bool(True),
        toGet = cms.VPSet(cms.PSet(
            record = cms.string('PPSAlignmentConfigurationRcd'),
            tag = cms.string(db_tag)
        ))
    )
elif conditions_input == 'db':
    process.load("CondCore.CondDB.CondDB_cfi")
    process.CondDB.connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
    process.PoolDBESSource = cms.ESSource("PoolDBESSource",
        process.CondDB,
        toGet = cms.VPSet(cms.PSet(
            record = cms.string('PPSAlignmentConfigurationRcd'),
            tag = cms.string(db_tag)
        ))
    )
else:
    raise ValueError(conditions_input + ' is wrong conditions_input')

# Output for the histograms
process.dqmOutput = cms.OutputModule("DQMRootOutputModule",
    fileName = cms.untracked.string(output_distributions)
)

process.options.numberOfThreads = cms.untracked.uint32(8)

process.path = cms.Path(
    process.ppsAlignmentWorker
)

process.end_path = cms.EndPath(
    process.dqmOutput
)

process.schedule = cms.Schedule(
    process.path,
    process.end_path
)
