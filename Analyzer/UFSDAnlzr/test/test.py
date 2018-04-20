import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

# import of standard configurations for PIXEL
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_hlt_relval', '')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.verbosity = cms.untracked.PSet( input = cms.untracked.int32(0) )

process.source = cms.Source("NewEventStreamFileReader",
  fileNames = cms.untracked.vstring(
'/store/t0streamer/Minidaq/A/000/314/787/run314787_ls0002_streamA_StorageManager.dat',
  )
)
  

# raw-to-digi conversion
process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")

#process.load("Analyzer.UFSDAnlzr.CfiFile_cfi")
process.UFSDAnlzr = cms.EDAnalyzer('UFSDAnlzr',
 tagDigi = cms.InputTag("totemTimingRawToDigi", "TotemTiming"),
)

process.TFileService = cms.Service("TFileService",
     fileName = cms.string('out_LHC.root')
)

process.p = cms.Path(
    process.ctppsRawToDigi *
    process.UFSDAnlzr
    )


# output configuration
process.output = cms.OutputModule("PoolOutputModule",
  fileName = cms.untracked.string("file:./sampic_LHC.root"),
  outputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_*RawToDigi_*_*',
  )
)

process.outpath = cms.EndPath(process.output)

