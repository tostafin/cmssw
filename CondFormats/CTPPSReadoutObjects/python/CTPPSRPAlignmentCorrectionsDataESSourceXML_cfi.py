import FWCore.ParameterSet.Config as cms

ctppsRPAlignmentCorrectionsDataESSourceXML = cms.ESSource("CTPPSRPAlignmentCorrectionsDataESSourceXML",
    verbosity = cms.untracked.uint32(1),
    XMLFile = cms.string("")
)
