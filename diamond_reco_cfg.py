import FWCore.ParameterSet.Config as cms
process = cms.Process('CTPPS')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

process.source = cms.Source('PoolSource',
    fileNames = cms.untracked.vstring(
	'/store/data/Run2018D/DoubleMuon/RAW/v1/000/324/420/00000/E47F96E1-B5F6-4942-8D93-CACDFB09C659.root'
    ),
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
#process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

# raw-to-digi conversion
process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")

#process.load("CalibPPS.ESProducers.totemDAQMappingESSourceXML_cfi")
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
process.load("EventFilter.CTPPSRawToDigi.totemTriggerRawToDigi_cfi")
process.totemTriggerRawToDigi.rawDataTag = cms.InputTag("rawDataCollector")

process.load("EventFilter.CTPPSRawToDigi.ctppsDiamondRawToDigi_cfi")
process.ctppsDiamondRawToDigi.rawDataTag = cms.InputTag("rawDataCollector")
#from EventFilter.CTPPSRawToDigi.ctppsDiamondRawToDigi_cfi import ctppsDiamondRawToDigi
#ctppsDiamondRawToDigi.rawDataTag = cms.InputTag("rawDataCollector")

# local RP reconstruction chain with standard settings
process.load("RecoCTPPS.Configuration.recoCTPPS_cff")

# rechits production
process.load('Geometry.VeryForwardGeometry.geometryRPFromDD_2021_cfi')

#???
#process.load('RecoCTPPS.TotemRPLocal.ctppsDiamondRecHits_cfi')

# local tracks fitter
#???
#process.load('RecoCTPPS.TotemRPLocal.ctppsDiamondLocalTracks_cfi')

process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("file:AOD.root"),
    outputCommands = cms.untracked.vstring(
        'drop *',
        'keep *_ctpps*_*_*',
    ),
)

process.ctppsDiamondRecHits.applyCalibration = False

# execution configuration
process.p = cms.Path(
  process.totemTriggerRawToDigi *

    process.ctppsDiamondRawToDigi * 
    process.ctppsDiamondLocalReconstruction
)

process.outpath = cms.EndPath(process.output)

