import FWCore.ParameterSet.Config as cms

# reco hit production
from RecoPPS.Local.totemT2RecHits_cfi import *

# not in database?
from Geometry.ForwardCommonData.totemT22021V2XML_cfi import *
from Geometry.ForwardGeometry.totemGeometryESModule_cfi import *

totemT2LocalReconstructionTask = cms.Task(
    totemT2RecHits 
)
totemT2LocalReconstruction = cms.Sequence(totemT2LocalReconstructionTask)
