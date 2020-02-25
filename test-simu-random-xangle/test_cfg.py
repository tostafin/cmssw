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

# number of events
process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(10000)
)

# change LS often - to allow changes of ES data
process.source.numberEventsInLuminosityBlock = cms.untracked.uint32(10)

# override LHCInfo source
process.load("CalibPPS.ESProducers.ctppsLHCInfoRandomXangleESSource_cfi")
process.ctppsLHCInfoRandomXangleESSource.generateEveryNEvents = 1
process.ctppsLHCInfoRandomXangleESSource.xangleHistogramFile = "xangle_hist.root"
process.ctppsLHCInfoRandomXangleESSource.xangleHistogramObject = "h_xangle"
process.ctppsLHCInfoRandomXangleESSource.beamEnergy = 6500.
process.ctppsLHCInfoRandomXangleESSource.betaStar = 0.40

process.esPreferLHCInfo = cms.ESPrefer("CTPPSLHCInfoRandomXangleESSource", "ctppsLHCInfoRandomXangleESSource")

# override beam-parameter source
process.load("CalibPPS.ESProducers.ctppsBeamParametersFromLHCInfoESSource_cfi")

process.ctppsBeamParametersFromLHCInfoESSource.beamDivX45 = process.ctppsBeamParametersESSource.beamDivX45
process.ctppsBeamParametersFromLHCInfoESSource.beamDivX56 = process.ctppsBeamParametersESSource.beamDivX56
process.ctppsBeamParametersFromLHCInfoESSource.beamDivY45 = process.ctppsBeamParametersESSource.beamDivY45
process.ctppsBeamParametersFromLHCInfoESSource.beamDivY56 = process.ctppsBeamParametersESSource.beamDivY56

process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetX45 = process.ctppsBeamParametersESSource.vtxOffsetX45
process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetX56 = process.ctppsBeamParametersESSource.vtxOffsetX56
process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetY45 = process.ctppsBeamParametersESSource.vtxOffsetY45
process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetY56 = process.ctppsBeamParametersESSource.vtxOffsetY56
process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetZ45 = process.ctppsBeamParametersESSource.vtxOffsetZ45
process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetZ56 = process.ctppsBeamParametersESSource.vtxOffsetZ56

process.ctppsBeamParametersFromLHCInfoESSource.vtxStddevX = process.ctppsBeamParametersESSource.vtxStddevX
process.ctppsBeamParametersFromLHCInfoESSource.vtxStddevY = process.ctppsBeamParametersESSource.vtxStddevY
process.ctppsBeamParametersFromLHCInfoESSource.vtxStddevZ = process.ctppsBeamParametersESSource.vtxStddevZ

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
  process.generator
  * process.beamDivergenceVtxGenerator
  * process.ctppsDirectProtonSimulation

  * process.reco_local

  * process.ctppsLHCInfoPlotter
  * process.ctppsHepMCDistributionPlotter
  * process.ctppsTrackDistributionPlotter
)
