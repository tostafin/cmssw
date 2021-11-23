import FWCore.ParameterSet.Config as cms

alcarecoPPSEventContent_prompt = cms.PSet(
    outputCommands = cms.untracked.vstring(

    # timing diamonds
    'keep CTPPSDiamondRecHitedmDetSetVector_ctppsDiamondRecHits_*_*',
    'keep CTPPSDiamondLocalTrackedmDetSetVector_ctppsDiamondLocalTracks_*_*',
    
    #diamond sampic
    'keep TotemTimingLocalTrackedmDetSetVector_diamondSampicLocalTracks_*_*',

    # tracking pixels
    'keep CTPPSPixelClusteredmDetSetVector_ctppsPixelClusters_*_*',
    'keep CTPPSPixelRecHitedmDetSetVector_ctppsPixelRecHits_*_*',
    'keep CTPPSPixelLocalTrackedmDetSetVector_ctppsPixelLocalTracks_*_*',

    # CTPPS common
    'keep CTPPSLocalTrackLites_ctppsLocalTrackLiteProducer_*_*',
    'keep recoForwardProtons_ctppsProtons_*_*',

    # HLT info
    'keep GlobalObjectMapRecord_hltGtStage2ObjectMap_*_*', 
    'keep edmTriggerResults_*_*_*', 
    'keep triggerTriggerEvent_*_*_*' 

  )
)

alcarecoPPSEventContent_express = cms.PSet(
    outputCommands = cms.untracked.vstring(

    # timing diamonds
    'keep CTPPSDiamondRecHitedmDetSetVector_ctppsDiamondRecHits_*_*',
    'keep CTPPSDiamondLocalTrackedmDetSetVector_ctppsDiamondLocalTracks_*_*',
    
    #diamond sampic
    'keep TotemTimingLocalTrackedmDetSetVector_diamondSampicLocalTracks_*_*',

    # tracking pixels
    'keep CTPPSPixelClusteredmDetSetVector_ctppsPixelClusters_*_*',
    'keep CTPPSPixelRecHitedmDetSetVector_ctppsPixelRecHits_*_*',
    'keep CTPPSPixelLocalTrackedmDetSetVector_ctppsPixelLocalTracks_*_*',

    # CTPPS common
    'keep CTPPSLocalTrackLites_ctppsLocalTrackLiteProducer_*_*',
    'keep recoForwardProtons_ctppsProtons_*_*',

  )
)