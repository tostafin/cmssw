import FWCore.ParameterSet.Config as cms

UFSDAnlzr = cms.EDAnalyzer('UFSDAnlzr',
     tagDigi = cms.InputTag("totemTimingRawToDigi","TotemTiming"),

     verbosity = cms.untracked.uint32(1),
)
