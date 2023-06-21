# Introduction

These changes, built on a CMSSW_13_0_5 release are meant to create a db payload of the record IdealGeometryRecord that includes also the internal detector alignment (this is to prevent a bug of the PCL alignment code of PPS that overwrite internal alignments while calculating global ones).

# The code

The new code is based on Francesco Brivio's commit:
https://github.com/francescobrivio/cmssw/commit/d6aa6c16f4d9cd4fc43f500b14e80e8231cc8aae

The changes are the following:

       * modified:   Geometry/VeryForwardGeometryBuilder/plugins/BuildFile.xml
       * modified:   Geometry/VeryForwardGeometryBuilder/plugins/CTPPSGeometryESModule.cc
       * modified:   Geometry/VeryForwardGeometryBuilder/test/print_geometry_info_geomFromDB_cfg.py

       * new: Geometry/VeryForwardGeometryBuilder/test/print_geometry_info_geomFromDB_IntAlignDB.py

The db file is saved as Geometry/VeryForwardGeometryBuilder/test/Geometry_internally_aligned.db and produced by Geometry/VeryForwardGeometryBuilder/test/print_geometry_info_geomFromDB_IntAlignDB.py.

The db file contents can be shown using
conddb --db Geometry_internally_aligned.db listTags
and are:

 * Name: PPSGeometry_IntAligned            
 * ObjectType: PDetGeomDesc   


