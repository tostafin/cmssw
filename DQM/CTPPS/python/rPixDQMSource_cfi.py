import FWCore.ParameterSet.Config as cms

rPixDQMSource = cms.EDAnalyzer("RPixDQMSource",
#    tagRPixDigi = cms.InputTag("RPixDetDigitizer", ""),
    tagRPixDigi = cms.InputTag("ctppsPixelDigis", ""),
    tagRPixCluster = cms.InputTag("clusterProd", ""),  
    RPStatusWord = cms.untracked.uint32(0x8000),
    verbosity = cms.untracked.uint32(1)
)
