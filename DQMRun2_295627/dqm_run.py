import FWCore.ParameterSet.Config as cms
import string

process = cms.Process('RECODQM')

process.maxEvents = cms.untracked.PSet(
input = cms.untracked.int32(-1)
)

process.verbosity = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# diamonds mapping
process.totemDAQMappingESSourceXML_TimingDiamond = cms.ESSource("TotemDAQMappingESSourceXML",
  verbosity = cms.untracked.uint32(0),
  subSystem = cms.untracked.string("TimingDiamond"),
  configuration = cms.VPSet(
    # before diamonds inserted in DAQ
    cms.PSet(
      validityRange = cms.EventRange("1:min - 283819:max"),
      mappingFileNames = cms.vstring(),
      maskFileNames = cms.vstring()
    ),
    # after diamonds inserted in DAQ
    cms.PSet(
      validityRange = cms.EventRange("283820:min - 999999999:max"),
      mappingFileNames = cms.vstring("CondFormats/CTPPSReadoutObjects/xml/mapping_timing_diamond.xml"),
      maskFileNames = cms.vstring()
    )
  )
)

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
    statistics = cms.untracked.vstring(),
    destinations = cms.untracked.vstring('cerr'),
    cerr = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    )
)

# load DQM framework
process.load("DQM.Integration.config.environment_cfi")
process.dqmEnv.subSystemFolder = "CTPPS"
process.dqmEnv.eventInfoFolder = "EventInfo"
process.dqmSaver.path = ""
process.dqmSaver.tag = "CTPPS"


process.source = cms.Source("NewEventStreamFileReader",
    fileNames = cms.untracked.vstring(
      *(
'/store/t0streamer/Minidaq/A/000/295/627/run295627_ls0001_streamA_StorageManager.dat',
'/store/t0streamer/Minidaq/A/000/295/627/run295627_ls0002_streamA_StorageManager.dat',
'/store/t0streamer/Minidaq/A/000/295/627/run295627_ls0003_streamA_StorageManager.dat',
'/store/t0streamer/Minidaq/A/000/295/627/run295627_ls0004_streamA_StorageManager.dat',
'/store/t0streamer/Minidaq/A/000/295/627/run295627_ls0005_streamA_StorageManager.dat',
'/store/t0streamer/Minidaq/A/000/295/627/run295627_ls0006_streamA_StorageManager.dat',
'/store/t0streamer/Minidaq/A/000/295/627/run295627_ls0007_streamA_StorageManager.dat',
'/store/t0streamer/Minidaq/A/000/295/627/run295627_ls0008_streamA_StorageManager.dat',
'/store/t0streamer/Minidaq/A/000/295/627/run295627_ls0009_streamA_StorageManager.dat',
'/store/t0streamer/Minidaq/A/000/295/627/run295627_ls0010_streamA_StorageManager.dat',
'/store/t0streamer/Minidaq/A/000/295/627/run295627_ls0011_streamA_StorageManager.dat',
'/store/t0streamer/Minidaq/A/000/295/627/run295627_ls0012_streamA_StorageManager.dat',
'/store/t0streamer/Minidaq/A/000/295/627/run295627_ls0013_streamA_StorageManager.dat',
'/store/t0streamer/Minidaq/A/000/295/627/run295627_ls0014_streamA_StorageManager.dat',
)

    )
)

# raw-to-digi conversion
process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")

# local RP reconstruction chain with standard settings
process.load("RecoCTPPS.Configuration.recoCTPPS_cff")

# rechits production
process.load('Geometry.VeryForwardGeometry.geometryRP_cfi')
process.load('RecoCTPPS.TotemRPLocal.ctppsDiamondRecHits_cfi')

# local tracks fitter
process.load('RecoCTPPS.TotemRPLocal.ctppsDiamondLocalTracks_cfi')
#process.ctppsDiamondLocalTracks.trackingAlgorithmParams.threshold = cms.double(1.5)
#process.ctppsDiamondLocalTracks.trackingAlgorithmParams.sigma = cms.double(0.1)
#process.ctppsDiamondLocalTracks.trackingAlgorithmParams.resolution = cms.double(0.05) # in mm
#process.ctppsDiamondLocalTracks.trackingAlgorithmParams.pixel_efficiency_function = cms.string("(TMath::Erf((x-[0]+0.5*[1])/([2]/4)+2)+1)*TMath::Erfc((x-[0]-0.5*[1])/([2]/4)-2)/4")

# CTPPS DQM modules
process.load("DQM.CTPPS.ctppsDQM_cff")
process.ctppsDiamondDQMSource.excludeMultipleHits = cms.bool(True);

process.path = cms.Path(
  process.ctppsRawToDigi *
  process.recoCTPPS *
  process.ctppsDiamondRawToDigi *
  process.ctppsDiamondRecHits *
  process.ctppsDiamondLocalTracks *
  process.ctppsDQM 
)

process.end_path = cms.EndPath(
  process.dqmEnv +
  process.dqmSaver
)

process.schedule = cms.Schedule(
  process.path,
  process.end_path
)


## output configuration
#process.output = cms.OutputModule("PoolOutputModule",
  #fileName = cms.untracked.string("file:./reco_digi.root"),
  #outputCommands = cms.untracked.vstring(
    #'drop *',
    #'keep *_*RawToDigi_*_*',
  #)
#)

#process.outpath = cms.EndPath(process.output)


