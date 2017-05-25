import FWCore.ParameterSet.Config as cms

process = cms.Process("CTPPSRawToDigiTestDiamondsOnly")

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
    statistics = cms.untracked.vstring(),
    destinations = cms.untracked.vstring('cerr'),
    cerr = cms.untracked.PSet( threshold = cms.untracked.string('DEBUG') )
)

# raw data source
process.source = cms.Source("NewEventStreamFileReader",
  fileNames = cms.untracked.vstring(
    #'file:/afs/cern.ch/user/j/jkaspar/public/run273062_ls0001-2_stream.root'
        '/store/t0streamer/Data/Physics/000/294/737/run294737_ls0011_streamPhysics_StorageManager.dat'
  )
)

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(100)
)

# raw-to-digi conversion
process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")
 
process.p = cms.Path(
  process.ctppsDiamondRawToDigi
)

# output configuration
process.output = cms.OutputModule("PoolOutputModule",
  fileName = cms.untracked.string("file:./reco_diamond_digi.root"),
  outputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_ctppsDiamondRawToDigi_*_*',
 )
)

process.outpath = cms.EndPath(process.output)
