import FWCore.ParameterSet.Config as cms

from CalibPPS.ESProducers.totemDAQMappingESSourceXML_cfi import totemDAQMappingESSourceXML as _xml

totemDAQMappingESSourceXML = _xml.clone(
    subSystem = "TotemT2",
    multipleChannelsPerPayload = cms.untracked.bool(True),
    configuration = cms.VPSet(
        #initial dummy diamond map copy
        cms.PSet(
            validityRange = cms.EventRange("1:min - 364982:max"),
            mappingFileNames = cms.vstring("CondFormats/PPSObjects/xml/mapping_totem_nt2_2021.xml"),
            maskFileNames = cms.vstring()
        ),
        #old v2.1 T2 firmware test file
        cms.PSet(
            validityRange = cms.EventRange("364983:min - 368022:max"),
            mappingFileNames = cms.vstring("CondFormats/PPSObjects/xml/mapping_totem_nt2_2023.xml"),
            maskFileNames = cms.vstring()
        ),
        #final T2 mapping test files
        cms.PSet(
            validityRange = cms.EventRange("368023:min - 999999999:max"),
            mappingFileNames = cms.vstring("CondFormats/PPSObjects/xml/mapping_totem_nt2_2023_final.xml"),
            maskFileNames = cms.vstring()
        )
    ),
    sampicSubDetId = cms.uint32(6),
)
