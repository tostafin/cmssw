/****************************************************************************
 *
 * This is a part of TOTEM offline software.
 * Authors:
 * Jan Kaspar
 * Helena Malbouisson
 * Clemencia Mora Herrera
 *
 ****************************************************************************/

#ifndef CondFormats_CTPPSReadoutObjects_CTPPSRPAlignmentCorrectionsMethods
#define CondFormats_CTPPSReadoutObjects_CTPPSRPAlignmentCorrectionsMethods

#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSRPAlignmentCorrectionData.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSRPAlignmentCorrectionsData.h"

#include <xercesc/dom/DOM.hpp>

//----------------------------------------------------------------------------------------------------

class CTPPSRPAlignmentCorrectionsMethods
{
 public:
  CTPPSRPAlignmentCorrectionsMethods() {}

  /// loads sequence of alignment corrections from XML file
  static CTPPSRPAlignmentCorrectionsData loadFromXML( const std::string& fileName );

 protected:
  /// load corrections data corresponding to one IOV
  static CTPPSRPAlignmentCorrectionsData getCorrectionsData( xercesc::DOMNode* );

  /// writes data of a correction in XML format
  static void writeXML( const CTPPSRPAlignmentCorrectionData& data, FILE* f, bool precise, bool wrErrors,
			bool wrSh_xy, bool wrSh_z, bool wrRot_xy, bool wrRot_z );

  /// writes a block of corrections into a file
  static void writeXMLBlock( const CTPPSRPAlignmentCorrectionsData&, FILE*, bool precise = false, bool wrErrors = true,
			     bool wrSh_xy=true, bool wrSh_z=false, bool wrRot_xy=false, bool wrRot_z=true );
};

#endif
