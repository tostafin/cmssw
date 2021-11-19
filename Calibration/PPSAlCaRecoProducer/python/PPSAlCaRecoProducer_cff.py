from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import *
from RecoPPS.Configuration.recoCTPPS_cff import *

alcaPPSreco = cms.Sequence( ctppsRawToDigi + recoCTPPS )

from RecoPPS.Configuration.RecoCTPPS_EventContent_cff import RecoCTPPSAOD

alcarecoPPSEventContent = cms.PSet(
    outputCommands = RecoCTPPSAOD.outputCommands
)
alcarecoPPSEventContent.outputCommands.extend([
        "keep GlobalObjectMapRecord_hltGtStage2ObjectMap_*_*", 
        "keep edmTriggerResults_*_*_*", 
        "keep triggerTriggerEvent_*_*_*" 
])