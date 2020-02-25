import FWCore.ParameterSet.Config as cms

# load common code
from direct_simu_reco_cff import *
process = cms.Process('CTPPSTestAcceptance', era)
process.load("direct_simu_reco_cff")
SetDefaults(process)

# minimal logger settings
process.MessageLogger = cms.Service("MessageLogger",
  statistics = cms.untracked.vstring(),
  destinations = cms.untracked.vstring('cerr'),
  cerr = cms.untracked.PSet(
    threshold = cms.untracked.string('WARNING')
  )
)

# event source
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring("file:///afs/cern.ch/user/r/rebassoo/public/ForJan/062366B6-DF2B-E911-BB28-C45444922958.root")
)

# number of events
process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(10000)
)

# update settings of beam-smearing module
process.beamDivergenceVtxGenerator.src = cms.InputTag("")
process.beamDivergenceVtxGenerator.srcGenParticle = cms.VInputTag(
  cms.InputTag("genPUProtons", "genPUProtons"),
  cms.InputTag("prunedGenParticles")
)

# do not apply vertex smearing again
process.ctppsBeamParametersESSource.vtxStddevX = 0
process.ctppsBeamParametersESSource.vtxStddevY = 0
process.ctppsBeamParametersESSource.vtxStddevZ = 0

# undo CMS vertex shift
process.ctppsBeamParametersESSource.vtxOffsetX45 = +0.2475 * 1E-1
process.ctppsBeamParametersESSource.vtxOffsetY45 = -0.6924 * 1E-1
process.ctppsBeamParametersESSource.vtxOffsetZ45 = -8.1100 * 1E-1

# LHCInfo plotter
process.ctppsLHCInfoPlotter = cms.EDAnalyzer("CTPPSLHCInfoPlotter",
  lhcInfoLabel = cms.string(""),
  outputFile = cms.string("output_LHCInfo.root")
)

# HepMC plotter
process.ctppsHepMCDistributionPlotter = cms.EDAnalyzer("CTPPSHepMCDistributionPlotter",
  tagHepMC = cms.InputTag("beamDivergenceVtxGenerator"),
  lhcInfoLabel = cms.string(""),
  outputFile = cms.string("output_HepMC.root")
)

# XY distribution plotter
process.ctppsTrackDistributionPlotter = cms.EDAnalyzer("CTPPSTrackDistributionPlotter",
  tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer"),
  outputFile = cms.string("output_track_xy.root")
)

process.dump = cms.EDAnalyzer("EventContentAnalyzer")

# processing path
process.p = cms.Path(
  process.beamDivergenceVtxGenerator
  * process.ctppsDirectProtonSimulation

  * process.reco_local

  * process.ctppsLHCInfoPlotter
  * process.ctppsHepMCDistributionPlotter
  * process.ctppsTrackDistributionPlotter
)
