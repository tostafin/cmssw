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
  verbosity = cms.untracked.uint32(10),
  #subSystem = cms.untracked.string("TimingDiamond"),
  #configuration = cms.VPSet(
    ## before diamonds inserted in DAQ
    #cms.PSet(
      #validityRange = cms.EventRange("1:min - 283819:max"),
      #mappingFileNames = cms.vstring(),
      #maskFileNames = cms.vstring()
    #),
    ## after diamonds inserted in DAQ
    #cms.PSet(
      #validityRange = cms.EventRange("283820:min - 291200:max"),
      #mappingFileNames = cms.vstring("CondFormats/CTPPSReadoutObjects/xml/mapping_timing_diamond_test.xml"),
      #maskFileNames = cms.vstring()
    #),
    ## 2017 mapping with 4 Digitizer boards
    #cms.PSet(
      #validityRange = cms.EventRange("291201:min - 999999999:max"),
      #mappingFileNames = cms.vstring("CondFormats/CTPPSReadoutObjects/xml/mapping_timing_diamond_test.xml"),
      #maskFileNames = cms.vstring()
    #)
  #)
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


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      *(
        '/store/data/Run2017A/ZeroBias/RAW/v1/000/295/977/00000/0E36C551-FF48-E711-A5F3-02163E01186D.root',
        '/store/data/Run2017A/ZeroBias/RAW/v1/000/295/977/00000/106C194D-FF48-E711-94C3-02163E01A1C1.root',
        '/store/data/Run2017A/ZeroBias/RAW/v1/000/295/977/00000/1240AF4F-FF48-E711-96BF-02163E01A4C2.root',
        '/store/data/Run2017A/ZeroBias/RAW/v1/000/295/977/00000/1416385D-FF48-E711-BAC1-02163E01A5AE.root',
        '/store/data/Run2017A/ZeroBias/RAW/v1/000/295/977/00000/18A22F4E-FF48-E711-A636-02163E01A2FA.root',
        '/store/data/Run2017A/ZeroBias/RAW/v1/000/295/977/00000/36CA7C51-FF48-E711-9745-02163E0134EE.root',
        '/store/data/Run2017A/ZeroBias/RAW/v1/000/295/977/00000/40242A4F-FF48-E711-B36E-02163E01A6AA.root',
        '/store/data/Run2017A/ZeroBias/RAW/v1/000/295/977/00000/4852BBF8-F748-E711-932A-02163E0136F8.root',
        '/store/data/Run2017A/ZeroBias/RAW/v1/000/295/977/00000/527A2050-FF48-E711-87F7-02163E011CFD.root',
        '/store/data/Run2017A/ZeroBias/RAW/v1/000/295/977/00000/649F5750-FF48-E711-830C-02163E0141B5.root',
        '/store/data/Run2017A/ZeroBias/RAW/v1/000/295/977/00000/64B6E633-0749-E711-BB39-02163E0144F5.root',
        '/store/data/Run2017A/ZeroBias/RAW/v1/000/295/977/00000/66212A4F-FF48-E711-A678-02163E01A6AA.root',
        '/store/data/Run2017A/ZeroBias/RAW/v1/000/295/977/00000/7063474F-FF48-E711-94CD-02163E01A27C.root',
        '/store/data/Run2017A/ZeroBias/RAW/v1/000/295/977/00000/8C85924D-FF48-E711-B29C-02163E019E38.root',
        '/store/data/Run2017A/ZeroBias/RAW/v1/000/295/977/00000/9649B84F-FF48-E711-827C-02163E01453A.root',
        '/store/data/Run2017A/ZeroBias/RAW/v1/000/295/977/00000/B40BB257-FF48-E711-B116-02163E01A5E6.root',
        '/store/data/Run2017A/ZeroBias/RAW/v1/000/295/977/00000/DC130F84-FF48-E711-AB12-02163E011C45.root',
        '/store/data/Run2017A/ZeroBias/RAW/v1/000/295/977/00000/F03C8161-FF48-E711-A662-02163E011EA7.root',
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
