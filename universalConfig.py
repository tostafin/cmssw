import FWCore.ParameterSet.Config as cms

process = cms.Process("DUMP")

import Geometry.VeryForwardGeometry.geometryRPFromDD_2021_cfi as geometry_rp
process.XMLIdealGeometryESSource = geometry_rp.XMLIdealGeometryESSource_CTPPS

import sys
if sys.argv[2] is None:
	print "No output file given"
	sys.exit()	

outputFName = sys.argv[2]+'.root'

#Very important: CMSE is not supported by cmsShow
process.XMLIdealGeometryESSource.rootNodeName = cms.string('cms:OCMS')

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger = cms.Service("MessageLogger",
		debugModules = cms.untracked.vstring('*'),
		destinations = cms.untracked.vstring('cout'),
		cout = cms.untracked.PSet(
		 threshold = cms.untracked.string('DEBUG'),
		 noLineBreaks = cms.untracked.bool(True)
		)
)

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
		input = cms.untracked.int32(1)
		)

process.add_(cms.ESProducer("TGeoMgrFromDdd",
			verbose = cms.untracked.bool(False),
			level   = cms.untracked.int32(25)
			))

process.dump = cms.EDAnalyzer("DumpSimGeometry",
	outputFileName = cms.untracked.string(outputFName)	
)
process.p = cms.Path(process.dump)

