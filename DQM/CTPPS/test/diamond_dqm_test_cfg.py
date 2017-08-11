import FWCore.ParameterSet.Config as cms
import string

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RECODQM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')


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

# load DQM framework
process.load("DQM.Integration.config.environment_cfi")
process.dqmEnv.subSystemFolder = "CTPPS"
process.dqmEnv.eventInfoFolder = "EventInfo"
process.dqmSaver.path = ""
process.dqmSaver.tag = "CTPPS"

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_hlt_relval', '')


# raw data source
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    #'file:/afs/cern.ch/user/j/jkaspar/public/run273062_ls0001-2_stream.root'
    #'/store/t0streamer/Data/Physics/000/294/737/run294737_ls0011_streamPhysics_StorageManager.dat',
    '/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/025708B7-C678-E711-991A-02163E01A6B4.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/02BFB5EB-BD78-E711-B661-02163E011D5F.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/08E51B9B-C278-E711-9B54-02163E0136DA.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/0E5A2004-CB78-E711-976C-02163E01351B.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/121C3DEC-DC78-E711-860A-02163E0144E9.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/12C7B282-D078-E711-87EC-02163E019CCC.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/148F3968-BD78-E711-89A2-02163E0135EF.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/189F441B-DC78-E711-A447-02163E01A23D.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/1AEDB205-CA78-E711-927A-02163E0129C1.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/26BAB9B9-D678-E711-927D-02163E01A4DA.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/28C27C4E-D978-E711-A2A7-02163E019C98.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/2C463D09-D678-E711-BE88-02163E011836.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/2E19746B-BD78-E711-918A-02163E01479A.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/2E1DE623-CB78-E711-A72F-02163E01A1D9.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/324F89A0-C578-E711-B6A8-02163E01A46D.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/324FAEEA-BF78-E711-8772-02163E014126.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/3674B71C-DE78-E711-B9AF-02163E01A5F5.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/38031131-BC78-E711-99BD-02163E01A762.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/38B9F613-D978-E711-B9B1-02163E01A6BE.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/4450CA3A-D278-E711-8F30-02163E011BAB.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/44A50E44-BC78-E711-AABC-02163E019DD2.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/48A7657D-BC78-E711-BDCF-02163E019E72.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/4C0BFCAA-CC78-E711-B3B5-02163E0133FC.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/4E1F0741-C578-E711-A041-02163E01A30D.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/4E6D887B-C078-E711-98D9-02163E01386E.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/504B9F4D-D478-E711-808E-02163E0119D6.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/547FFADE-CD78-E711-9C9A-02163E011DA8.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/56A6BE4C-C878-E711-836B-02163E011F13.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/58F729E7-C478-E711-90EE-02163E01A2B7.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/5E0CC545-D378-E711-AADD-02163E011DF9.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/5E4110DC-CB78-E711-8BED-02163E01259B.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/62DD1B91-C078-E711-8F02-02163E0137A9.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/6642899D-BA78-E711-B229-02163E01A1C9.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/684A6349-C678-E711-B813-02163E013940.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/687C56DA-D378-E711-BC21-02163E01A6B4.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/6E1FA177-CA78-E711-830F-02163E01A5AD.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/723C519F-DB78-E711-89BE-02163E0137A9.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/7858EA7D-C478-E711-9FAA-02163E0119D7.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/7A514F4B-C978-E711-A78E-02163E01A4DA.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/80C36FD3-D578-E711-AA8B-02163E019DC2.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/8622100B-DA78-E711-B6CA-02163E01388F.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/86544BDB-BA78-E711-BF86-02163E01A44F.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/886DC4F8-D178-E711-9718-02163E0135EF.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/8C12021F-4C79-E711-AC2B-02163E011D84.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/8CC19F3E-DB78-E711-A5F0-02163E011836.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/8E48EB2F-C078-E711-A837-02163E01A671.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/8E7FE588-C778-E711-A818-02163E01284F.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/8EE58627-D178-E711-8EF2-02163E01A3F5.root',
'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/401/00000/90D88EAA-CE78-E711-812C-02163E013581.root',
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
