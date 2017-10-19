import FWCore.ParameterSet.Config as cms

from Geometry.VeryForwardGeometry.geometryRP_cfi import totemGeomXMLFiles, ctppsDiamondGeomXMLFiles

XMLIdealGeometryESSource_CTPPS = cms.ESSource("XMLIdealGeometryESSource",
    #geomXMLFiles = totemGeomXMLFiles+ctppsDiamondGeomXMLFiles+["SimCTPPS/OpticsParameterisation/data/RP_Dist_Beam_Cent.xml"],
    geomXMLFiles = totemGeomXMLFiles+ctppsDiamondGeomXMLFiles+["SimCTPPS/OpticsParameterisation/data/RP_Dist_Beam_Cent_2016_15sigma.xml"],
    rootNodeName = cms.string('cms:CMSE'),
)

TotemRPGeometryESModule = cms.ESProducer("TotemRPGeometryESModule")

