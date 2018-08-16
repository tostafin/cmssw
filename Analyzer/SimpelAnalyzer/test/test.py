import FWCore.ParameterSet.Config as cms
import string

process = cms.Process('RECODQM')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2000000) )
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

# process.source = cms.Source("NewEventStreamFileReader",
#     fileNames = cms.untracked.vstring(
#     *(
#     'file:///afs/cern.ch/user/n/nminafra/Work/public/SampicCMS/run319190_ls0130_streamPhysicsTOTEM3_StorageManager.dat',
#     )
#     )
# )
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    *(
        'file:///eos/user/n/nminafra/Sampic/319176/1C0EB6B8-E280-E811-9605-FA163E9626F3.root',
        'file:///eos/user/n/nminafra/Sampic/319176/1C5EA541-F280-E811-A270-FA163E94E947.root',
        'file:///eos/user/n/nminafra/Sampic/319176/200C1511-E380-E811-B511-FA163E7AED1B.root',
        'file:///eos/user/n/nminafra/Sampic/319176/208F4BFE-E680-E811-AC70-FA163EFF1846.root',
        'file:///eos/user/n/nminafra/Sampic/319176/248732EF-F480-E811-B56D-FA163E746706.root',
        'file:///eos/user/n/nminafra/Sampic/319176/3EFBA7E5-E680-E811-8DBC-FA163E342326.root',
        'file:///eos/user/n/nminafra/Sampic/319176/6A5A05D2-F880-E811-88BA-02163E015C05.root',
        'file:///eos/user/n/nminafra/Sampic/319176/7032DC19-F280-E811-8654-FA163E1A6759.root',
        'file:///eos/user/n/nminafra/Sampic/319176/A823C49E-F280-E811-AD8E-02163E012DA1.root',
        'file:///eos/user/n/nminafra/Sampic/319176/AA3933BF-E680-E811-A903-FA163E11B3E0.root',
        'file:///eos/user/n/nminafra/Sampic/319176/B65F6AC9-E680-E811-89E0-FA163EB67B9F.root',
        'file:///eos/user/n/nminafra/Sampic/319176/D41C73B1-F280-E811-8682-FA163E048308.root',
        'file:///eos/user/n/nminafra/Sampic/319176/E25531B6-F280-E811-8AA5-FA163E490F23.root',
        'file:///eos/user/n/nminafra/Sampic/319176/F25044C7-E680-E811-BCA5-FA163E8A2D90.root',
        'file:///eos/user/n/nminafra/Sampic/319176/F6BADB7B-EB80-E811-87D4-FA163E333059.root',
    # 'root:///afs/cern.ch/user/n/nminafra/Work/public/SampicCMS/319190_testAOD.root',
    # '/store/data/Run2018B/TOTEM40/RECO/PromptReco-v2/000/319/190/00000/00A9F18A-B481-E811-9ECD-02163E014BBF.root ',
    )
    )
)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '101X_dataRun2_HLT_v7', '')

# raw-to-digi conversion
process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")

# local RP reconstruction chain with standard settings
process.load("RecoCTPPS.Configuration.recoCTPPS_cff")

# CTPPS DQM modules
process.load("DQM.CTPPS.ctppsDQM_cff")

# rechits production
#process.load('RecoCTPPS.TotemRPLocal.totemTimingRecHits_cfi')
process.totemTimingRecHits.baselinePoints = cms.int32(6);
process.totemTimingRecHits.saturationLimit = cms.double(0.85);
process.totemTimingRecHits.cfdFraction = cms.double(0.3);
process.totemTimingRecHits.hysteresis = cms.double(10e-3);
process.totemTimingRecHits.smoothingPoints = cms.int32(6);
process.totemTimingRecHits.lowPassFrequency = cms.double(0.6);

process.totemTimingLocalTracks.maxPlaneActiveChannels = cms.int32(2);
process.totemTimingLocalTracks.tolerance = cms.double(0.);
process.totemTimingLocalTracks.resolution = cms.double(0.01);
process.totemTimingLocalTracks.threshold = cms.double(1.5);
#process.totemTimingLocalTracks.thresholdFromMaximum = cms.double(0.5);
process.totemTimingLocalTracks.sigma = cms.double(0.);


process.simpleAnalyzer = cms.EDAnalyzer('SimpleAnalyzer',
 tagDigi = cms.InputTag("totemTimingRawToDigi", "TotemTiming"),
 tagRecHit = cms.InputTag("totemTimingRecHits"),
 tagRPHit = cms.InputTag("totemRPRecHits"),
 tagLocalTrack = cms.InputTag("totemTimingLocalTracks"),
 tagStripTrack = cms.InputTag("totemRPLocalTrackFitter")
)

process.TFileService = cms.Service("TFileService",
     fileName = cms.string('output.root')
)

process.path = cms.Path(
    process.recoCTPPS *
    process.simpleAnalyzer
)

process.schedule = cms.Schedule(
process.path
)
