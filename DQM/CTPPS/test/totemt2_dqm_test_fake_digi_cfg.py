import FWCore.ParameterSet.Config as cms
import string

process = cms.Process('RECODQM')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(20000) )

process.source = cms.Source("EmptySource")

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
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# load DQM framework
process.load("DQM.Integration.config.environment_cfi")
process.dqmEnv.subSystemFolder = "CTPPS"
process.dqmEnv.eventInfoFolder = "EventInfo"
process.dqmSaver.path = ""
process.dqmSaver.tag = "CTPPS"

# process.source = cms.Source('PoolSource',
#     fileNames = cms.untracked.vstring(
#         '/store/data/Run2018D/ZeroBias/RAW/v1/000/324/747/00000/97A72F4B-786F-5A48-B97E-C596DD73BD77.root',
#     ),
# )

################
# 2130706432 arm=1 plane=0 channel=0
# 2130722816 arm=1 plane=0 channel=1
# 2130739200 arm=1 plane=0 channel=2
# 2130755584 arm=1 plane=0 channel=3
# 2130771968 arm=1 plane=1 channel=0
# 2130788352 arm=1 plane=1 channel=1
# 2130804736 arm=1 plane=1 channel=2
# 2130821120 arm=1 plane=1 channel=3
# 2130837504 arm=1 plane=2 channel=0
# 2130853888 arm=1 plane=2 channel=1
# 2130870272 arm=1 plane=2 channel=2
# 2130886656 arm=1 plane=2 channel=3
# 2130903040 arm=1 plane=3 channel=0
# 2130919424 arm=1 plane=3 channel=1
# 2130935808 arm=1 plane=3 channel=2
# 2130952192 arm=1 plane=3 channel=3
# 2130968576 arm=1 plane=4 channel=0
# 2130984960 arm=1 plane=4 channel=1
# 2131001344 arm=1 plane=4 channel=2
# 2131017728 arm=1 plane=4 channel=3
# 2131034112 arm=1 plane=5 channel=0
# 2131050496 arm=1 plane=5 channel=1
# 2131066880 arm=1 plane=5 channel=2
# 2131083264 arm=1 plane=5 channel=3
# 2131099648 arm=1 plane=6 channel=0
# 2131116032 arm=1 plane=6 channel=1
# 2131132416 arm=1 plane=6 channel=2
# 2131148800 arm=1 plane=6 channel=3
# 2131165184 arm=1 plane=7 channel=0
# 2131181568 arm=1 plane=7 channel=1
# 2131197952 arm=1 plane=7 channel=2
# 2131214336 arm=1 plane=7 channel=3
ids = [
    2130706432,
    2130722816,
    2130739200,
    2130755584,
    2130771968,
    2130788352,
    2130804736,
    2130821120,
    2130837504,
    2130853888,
    2130870272,
    2130886656,
    2130903040,
    2130919424,
    2130935808,
    2130952192,
    2130968576,
    2130984960,
    2131001344,
    2131017728,
    2131034112,
    2131050496,
    2131066880,
    2131083264,
    2131099648,
    2131116032,
    2131132416,
    2131148800,
    2131165184,
    2131181568,
    2131197952,
    2131214336
]
#digi converter
################
process.totemT2FakeRawToDigi = cms.EDProducer('TotemT2DigiProducer',
	#input path of the testbeam data
	t2FilesVec=cms.vstring("/eos/home-a/acwikla/data/nT2_Nino.root"),
	################
	#channel mapping
	################
	idsMapping = cms.VPSet(
		cms.PSet(detId = cms.vuint32(*ids), treeChId = cms.uint32(4))
	)
)

#from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_hlt_relval', '')

# process.load('CalibPPS.ESProducers.totemT2DAQMapping_cff')
# process.load('EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff')
process.load('Geometry.ForwardCommonData.totemT22021V2XML_cfi')
process.load('Geometry.ForwardGeometry.totemGeometryESModule_cfi')
process.load('RecoPPS.Local.totemT2RecHits_cfi')
process.load('DQM.CTPPS.totemT2DQMSource_cfi')

process.path = cms.Path(
    # process.totemT2Digis *
    process.totemT2FakeRawToDigi *
    process.totemT2RecHits *
    process.totemT2DQMSource
)

process.end_path = cms.EndPath(
    process.dqmEnv +
    process.dqmSaver
)

process.schedule = cms.Schedule(
    process.path,
    process.end_path
)
