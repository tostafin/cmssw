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
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0008_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0009_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0010_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0011_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0012_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0013_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0014_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0015_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0016_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0017_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0018_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0019_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0020_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0021_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0022_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0023_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0024_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0025_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0026_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0027_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0028_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0029_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0030_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0031_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0032_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0033_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0034_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0035_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0036_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0037_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0038_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0039_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0040_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0041_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0042_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0043_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0044_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0045_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0046_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0047_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0048_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0049_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0050_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0051_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0052_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0053_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0054_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0055_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0056_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0057_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0058_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0059_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0060_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0061_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0062_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0063_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0064_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0065_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0066_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0067_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0068_streamExpress_StorageManager.dat',
'/store/t0streamer/Data/Express/000/314/221/run314221_ls0069_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0070_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0071_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0072_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0073_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0074_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0075_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0076_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0077_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0078_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0079_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0080_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0081_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0082_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0083_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0084_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0085_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0086_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0087_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0088_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0089_streamExpress_StorageManager.dat',
#'/store/t0streamer/Data/Express/000/314/221/run314221_ls0090_streamExpress_StorageManager.dat',
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

