import FWCore.ParameterSet.Config as cms
from SimG4Core.Application.g4SimHits_cfi import *
g4SimHits.Generator.MinEtaCut = cms.double(-13.0)
g4SimHits.Generator.MaxEtaCut = cms.double( 13.0)
g4SimHits.Generator.HepMCProductLabel   = 'LHCTransport'
g4SimHits.SteppingAction.MaxTrackTime = cms.double(2000.0)
g4SimHits.StackingAction.MaxTrackTime = cms.double(2000.0)

from IOMC.RandomEngine.IOMC_cff import *
RandomNumberGeneratorService.LHCTransport.engineName   = cms.untracked.string('TRandom3')

from SimTransport.PPSProtonTransport.CommonParameters_cfi import commonParameters
from SimTransport.PPSProtonTransport.HectorTransport_cfi import *
from SimTransport.PPSProtonTransport.TotemTransport_cfi import *
from SimTransport.PPSProtonTransport.OpticalFunctionsTransport_cfi import *


LHCTransportPSet = cms.PSet()

# so far, it is not yet defined the optic for 2017 and 2018

from Configuration.Eras.Modifier_ctpps_2016_cff import ctpps_2016
ctpps_2016.toReplaceWith(LHCTransportPSet, totemTransportSetup_2016)

from Configuration.Eras.Modifier_ctpps_2017_cff import ctpps_2017
#ctpps_2017.toReplaceWith(LHCTransportPSet, totemTransportSetup_2016)
ctpps_2017.toReplaceWith(LHCTransportPSet, opticalfunctionsTransportSetup_2017_preTS2)

from Configuration.Eras.Modifier_ctpps_2018_cff import ctpps_2018
#ctpps_2018.toReplaceWith(LHCTransportPSet, totemTransportSetup_2016)
ctpps_2018.toReplaceWith(LHCTransportPSet, opticalfunctionsTransportSetup_2018)


LHCTransport = cms.EDProducer("PPSSimTrackProducer",commonParameters,LHCTransportPSet)
