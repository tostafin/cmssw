import FWCore.ParameterSet.Config as cms
process = cms.Process("GeometryInfo")

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
    statistics = cms.untracked.vstring(),
    destinations = cms.untracked.vstring('cout'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    )
)

# geometry
process.load("Geometry.VeryForwardGeometry.geometryRPFromDD_2017_cfi")

# load alignment correction
#process.load("Geometry.VeryForwardGeometryBuilder.ctppsIncludeAlignmentsFromXML_cfi")
#process.ctppsIncludeAlignmentsFromXML.RealFiles = cms.vstring("Geometry/VeryForwardGeometryBuilder/test/sample_alignment_corrections.xml")

# load the alignment xml file
#process.load("CondFormats.CTPPSReadoutObjects.CTPPSRPAlignmentCorrectionsDataESSourceXML_cfi")
#process.ctppsRPAlignmentCorrectionsDataESSourceXML.XMLFile = cms.string("CondFormats/CTPPSReadoutObjects/xml/sample_alignment_corrections.xml")

##Database output service
process.load("CondCore.CondDB.CondDB_cfi")
## input database (in this case local sqlite file)
process.CondDB.connect = 'sqlite_file:CTPPSRPAlignment.db'

process.PoolDBESSource = cms.ESSource("PoolDBESSource",
    process.CondDB,
    DumpStat=cms.untracked.bool(True),
    toGet = cms.VPSet(
      cms.PSet(
        record = cms.string('CTPPSRPAlignmentCorrectionsDataRcd'),
        tag = cms.string("CTPPSRPAlignment_v1")
      )
    )
)

# no events to process
process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.ctppsGeometryInfo = cms.EDAnalyzer("CTPPSGeometryInfo",
    geometryType = cms.untracked.string("misaligned"),
    printRPInfo = cms.untracked.bool(True),
    printSensorInfo = cms.untracked.bool(True)
)

process.p = cms.Path(
    process.ctppsGeometryInfo
)
