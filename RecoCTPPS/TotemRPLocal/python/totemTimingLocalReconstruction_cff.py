import FWCore.ParameterSet.Config as cms

# reco hit production
from RecoCTPPS.TotemRPLocal.totemTimingRecHits_cfi import totemTimingRecHits
totemTimingRecHits.saturationLimit = cms.double(0.85)
totemTimingRecHits.cfdFraction = cms.double(0.3)
totemTimingRecHits.smoothingPoints = cms.int32(20)
totemTimingRecHits.lowPassFrequency = cms.double(0.7)
totemTimingRecHits.hysteresis = cms.double(5e-3)
totemTimingRecHits.calibrationFile = cms.string("/afs/cern.ch/user/f/fdej/sampic_cal/correct.cal.json")

totemTimingLocalReconstruction = cms.Sequence(
    totemTimingRecHits
)
