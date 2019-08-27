import FWCore.ParameterSet.Config as cms
import string

from Configuration.StandardSequences.Eras import eras
process = cms.Process('CTPPSDQM',eras.Run2_2018)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.verbosity = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
    statistics = cms.untracked.vstring(),
    destinations = cms.untracked.vstring('cerr'),
    cerr = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    )
)
    
    # import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# load DQM framework
process.load("DQM.Integration.config.environment_cfi")
process.dqmEnv.subSystemFolder = "CTPPS"
process.dqmEnv.eventInfoFolder = "EventInfo"
process.dqmSaver.path = ""
process.dqmSaver.tag = "CTPPS"

# raw data source
process.source = cms.Source("PoolSource",
    # replace '*.root',',' with the source file you want to use
    fileNames = cms.untracked.vstring( *(
'/store/data/Run2018D/ZeroBias/RAW/v1/000/324/021/00000/CED9C85E-7F4B-C545-BACC-9ECF0459EC04.root',
'/store/data/Run2018D/ZeroBias/RAW/v1/000/324/021/00000/47B83E15-4E53-A546-B34B-9708D5A2CDC6.root',
'/store/data/Run2018D/ZeroBias/RAW/v1/000/324/021/00000/2C1FD412-7B58-B449-922B-DA0A4CC66E02.root',
'/store/data/Run2018D/ZeroBias/RAW/v1/000/324/021/00000/8A77F857-0022-FE4B-BD3E-1382A082868F.root',
'/store/data/Run2018D/ZeroBias/RAW/v1/000/324/021/00000/7665DF69-4838-4447-A55E-C7714303E359.root',
'/store/data/Run2018D/ZeroBias/RAW/v1/000/324/021/00000/619587B5-B7F1-414E-A9D1-BDE9DC1397E2.root',
'/store/data/Run2018D/ZeroBias/RAW/v1/000/324/021/00000/22B32972-F03B-E048-896B-C9FDC035F850.root',
'/store/data/Run2018D/ZeroBias/RAW/v1/000/324/021/00000/54D65CF5-D04B-6B46-A058-4CDBE28FAAD0.root',
'/store/data/Run2018D/ZeroBias/RAW/v1/000/324/021/00000/2A637F3F-212E-EB40-898F-299E5345CEE7.root',
'/store/data/Run2018D/ZeroBias/RAW/v1/000/324/021/00000/AF7CEED2-CAAE-B54B-9CA9-5F605CFCCB3E.root',
'/store/data/Run2018D/ZeroBias/RAW/v1/000/324/021/00000/50A2EC60-3F07-6844-8C52-4ED8114C0420.root'
    )),

)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '110X_dataRun2_v3', '')

# raw-to-digi conversion
process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")

#comment this out to use old mapping
process.totemDAQMappingESSourceXML_TimingDiamond = cms.ESSource("TotemDAQMappingESSourceXML",
          verbosity = cms.untracked.uint32(0),
          subSystem = cms.untracked.string("TimingDiamond"),
          configuration = cms.VPSet(
                cms.PSet(
                        validityRange=cms.EventRange("1:min -999999999:max"),
                        mappingFileNames=cms.vstring("CondFormats/CTPPSReadoutObjects/xml/mapping_timing_diamond_2021.xml"),
                        maskFileNames=cms.vstring()
)
)
)

# local RP reconstruction chain with standard settings
process.load("RecoCTPPS.Configuration.recoCTPPS_cff")

# rechits production
process.load('Geometry.VeryForwardGeometry.geometryRPFromDD_2021_cfi')
process.load('RecoCTPPS.TotemRPLocal.ctppsDiamondRecHits_cfi')

# local tracks fitter
process.load('RecoCTPPS.TotemRPLocal.ctppsDiamondLocalTracks_cfi')

# pixel
process.load('RecoCTPPS.PixelLocal.ctppsPixelLocalTracks_cfi')

# CTPPS DQM modules
process.load("DQM.CTPPS.ctppsDQM_cff")
process.ctppsDiamondDQMSource.excludeMultipleHits = cms.bool(True);

# Laurent's Trick to disable calib (so new geom actually works)
process.ctppsDiamondRecHits.applyCalibration = False

process.path = cms.Path(
    process.ctppsRawToDigi *
    process.recoCTPPS *
#    process.ctppsDiamondRawToDigi *
    process.ctppsDiamondRecHits *
    process.ctppsDiamondLocalTracks *
    process.ctppsPixelLocalTracks *
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
