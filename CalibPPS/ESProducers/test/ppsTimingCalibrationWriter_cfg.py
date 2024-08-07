import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

process = cms.Process('test')
options = VarParsing()

options.register('runNumber',
                     -1,
                      VarParsing.multiplicity.singleton,
                      VarParsing.varType.int,
                      "Run number")

options.register('cmsswJsonCalibOutputPath',
                     '',
                      VarParsing.multiplicity.singleton,
                      VarParsing.varType.string,
                      "Path to the JSON Calib Output in the CMSSW repo")

options.register('record',
                     '',
                      VarParsing.multiplicity.singleton,
                      VarParsing.varType.string,
                      "Record")

options.register('tag',
                     '',
                      VarParsing.multiplicity.singleton,
                      VarParsing.varType.string,
                      "Tag")

options.parseArguments()

process.source = cms.Source('EmptyIOVSource',
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(options.runNumber),
    lastValue = cms.uint64(options.runNumber),
    interval = cms.uint64(1)
)

from CondFormats.PPSObjects.PPSTimingDetEnum_cff import PPSTimingDetEnum

# load calibrations from JSON file
process.load('CalibPPS.ESProducers.ppsTimingCalibrationESSource_cfi')
process.ppsTimingCalibrationESSource.calibrationFile = cms.FileInPath(options.cmsswJsonCalibOutputPath)
process.ppsTimingCalibrationESSource.subDetector = PPSTimingDetEnum.PPS_DIAMOND

# output service for database
process.load('CondCore.CondDB.CondDB_cfi')
process.CondDB.connect = f'sqlite_file:ppsDiamondTiming_calibration{options.runNumber}.sqlite' # SQLite output

process.PoolDBOutputService = cms.Service('PoolDBOutputService',
    process.CondDB,
    timetype = cms.untracked.string('runnumber'),
    toPut = cms.VPSet(
        cms.PSet(
            record = cms.string(options.record),
            tag = cms.string(options.tag),
        )
    )
)

process.ppsTimingCalibrationWriter = cms.EDAnalyzer('PPSTimingCalibrationWriter')

process.path = cms.Path(
    process.ppsTimingCalibrationWriter
)
