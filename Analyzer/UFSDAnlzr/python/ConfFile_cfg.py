import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

# import of standard configurations for PIXEL
#process.load('Configuration.StandardSequences.Services_cff')
#process.load('FWCore.MessageService.MessageLogger_cfi')
#process.load('Configuration.EventContent.EventContent_cff')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_hlt_relval', '')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.verbosity = cms.untracked.PSet( input = cms.untracked.int32(0) )

process.source = cms.Source("NewEventStreamFileReader",
  fileNames = cms.untracked.vstring(
    'file:/eos/cms/store/group/dpg_ctpps/comm_ctpps/TotemTiming/Minidaq/519/run312519_ls0002_streamA_StorageManager.dat'
  )
)
  

# raw-to-digi conversion
process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")

process.load("UFSDAnlzr.CfiFile_cfi.py")
process.demo = cms.EDAnalyzer('UFSDAnlzr',
 tagDigi = cms.InputTag("ctppsTotemTimingRawToDigi", "TotemTiming"),
)

process.TFileService = cms.Service("TFileService",
     fileName = cms.string('out.root')
)

process.p = cms.Path(
    process.ctppsRawToDigi *
    process.demo
    )



