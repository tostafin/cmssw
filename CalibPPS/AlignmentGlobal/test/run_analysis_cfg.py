
########## Configuration ##########
# if set to True, a file with logs will be produced.
produce_logs = True

# if set to True, the harvester will produce an extra ROOT file with some debug plots. 
# Works only for one-run input.
harvester_debug = True

# Use results with the slope fixed (True) or without (False) when producing the final results
slope_fixed_final_results = True

# Path for a ROOT file with the histograms.
input_distributions = 'file:dqm_run_distributions_test.root'

# Specifies a method used to load conditions. Can be set to:
#   - "ESSource" - configs loaded directly via ESSource
#   - "local_sqlite" - configs retrieved from an SQLite file (input_conditions)
#   - "db" - configs retrieved from the Conditions DB.
conditions_input = "ESSource"            # test config
conditions_input_reference = "db"  # reference config

# Input database. Used only if conditions_input(_reference) is set to "local_sqlite".
input_conditions = 'sqlite_file:alignment_config.db'
input_conditions_reference = f'alignment_config_reference.db'

# Database tag. Used only if conditions_input(_reference) is set to "local_sqlite" or "db".
input_db_tag = 'PPSAlignmentConfiguration_v1_express'
input_db_tag_reference = 'PPSAlignmentConfig_reference_Run3_v1_express'

# Reference dataset path. Only used if conditions_input(_reference) is set to "ESSource".
reference_dataset_path = ""

# If set to True, the results will be also written to an SQLite file.
write_sqlite_results = True

# Output database. Used only if write_sqlite_results is set to True.
output_conditions = 'sqlite_file:alignment_results.db'

# Database tag. Used only if write_sqlite_results is set to True.
output_db_tag = 'CTPPSRPAlignment_real_pcl'
###################################

import sys
import os
import FWCore.ParameterSet.Config as cms

process = cms.Process('testDistributions')

process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("DQMServices.Core.DQMStore_cfi")
process.load("CalibPPS.AlignmentGlobal.ppsAlignmentHarvester_cfi")

process.ppsAlignmentHarvester.x_ali_rel_final_slope_fixed = cms.bool(slope_fixed_final_results)
process.ppsAlignmentHarvester.y_ali_final_slope_fixed = cms.bool(slope_fixed_final_results)

process.ppsAlignmentHarvester.debug = cms.bool(harvester_debug)

# Message Logger
if produce_logs:
    process.MessageLogger = cms.Service("MessageLogger",
        destinations = cms.untracked.vstring('run_analysis',
            'cout'
        ),
        run_analysis = cms.untracked.PSet(
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

# load DQM framework
process.load("DQMServices.Components.DQMEnvironment_cfi")
process.dqmEnv.subSystemFolder = "CalibPPS"
process.dqmSaver.convention = 'Offline'
process.dqmSaver.workflow = "/CalibPPS/AlignmentGlobal/CMSSW_12_1_0_pre1"
process.dqmSaver.saveByRun = -1
process.dqmSaver.saveAtJobEnd = True
process.dqmSaver.forceRunNumber = 999999

# Source (histograms)
process.source = cms.Source("DQMRootSource",
    fileNames = cms.untracked.vstring(input_distributions),
)

# Event Setup (test)
if conditions_input == 'ESSource':
    from config_cff import ppsAlignmentConfigESSource as ppsAlignmentConfigESSourceTest
    process.ppsAlignmentConfigESSourceTest = ppsAlignmentConfigESSourceTest
elif conditions_input == 'local_sqlite':
    process.load("CondCore.CondDB.CondDB_cfi")
    process.CondDB.connect = input_conditions
    process.PoolDBESSource = cms.ESSource("PoolDBESSource",
        process.CondDB,
        toGet = cms.VPSet(cms.PSet(
            record = cms.string('PPSAlignmentConfigurationRcd'),
            tag = cms.string(input_db_tag)
        )),
        appendToDataLabel = cms.string('')
    )
elif conditions_input == 'db':
    process.load("CondCore.CondDB.CondDB_cfi")
    process.CondDB.connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
    process.PoolDBESSource = cms.ESSource("PoolDBESSource",
        process.CondDB,
        toGet = cms.VPSet(cms.PSet(
            record = cms.string('PPSAlignmentConfigurationRcd'),
            tag = cms.string(input_db_tag)
        )),
        appendToDataLabel = cms.string('')
    )
else:
    raise ValueError(conditions_input + ' is wrong conditions_input')

# Event Setup (reference)
if conditions_input_reference == 'ESSource':
    # sys.path.append(os.path.relpath(cdict['reference_folder_path']))
    from config_reference_cff import ppsAlignmentConfigESSource as ppsAlignmentConfigESSourceReference
    ppsAlignmentConfigESSourceReference.matching = cms.PSet(
        reference_dataset = cms.string(reference_dataset_path)
    )
    process.ppsAlignmentConfigESSourceReference = ppsAlignmentConfigESSourceReference
elif conditions_input_reference == 'local_sqlite':
    process.load("CondCore.CondDB.CondDB_cfi")
    process.CondDB.connect = input_conditions_reference
    process.PoolDBESSourceReference = cms.ESSource("PoolDBESSource",
        process.CondDB,
        toGet = cms.VPSet(cms.PSet(
            record = cms.string('PPSAlignmentConfigurationRcd'),
            tag = cms.string(input_db_tag_reference)
        )),
        appendToDataLabel = cms.string('reference')
    )
elif conditions_input_reference == 'db':
    process.load("CondCore.CondDB.CondDB_cfi")
    process.CondDB.connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
    process.PoolDBESSourceReference = cms.ESSource("PoolDBESSource",
        process.CondDB,
        toGet = cms.VPSet(cms.PSet(
            record = cms.string('PPSAlignmentConfigurationRcd'),
            tag = cms.string(input_db_tag_reference)
        )),
        appendToDataLabel = cms.string('reference')
    )
else:
    raise ValueError(conditions_input_reference + ' is wrong conditions_input_reference')

# SQLite results
if write_sqlite_results:
    process.load("CondCore.CondDB.CondDB_cfi")
    process.CondDB.connect = output_conditions
    process.PoolDBOutputService = cms.Service("PoolDBOutputService",
        process.CondDB,
        timetype = cms.untracked.string('runnumber'),
        toPut = cms.VPSet(cms.PSet(
            record = cms.string('RPRealAlignmentRecord'),
            tag = cms.string('MyTag'),
            label = cms.string('MyLabel')
        ))
    )

    # DB object maker parameters
    process.ppsAlignmentHarvester.write_sqlite_results = cms.bool(True)

process.path = cms.Path(
    process.ppsAlignmentHarvester
)

process.end_path = cms.EndPath(
    process.dqmSaver
)

process.schedule = cms.Schedule(
    process.path,
    process.end_path
)