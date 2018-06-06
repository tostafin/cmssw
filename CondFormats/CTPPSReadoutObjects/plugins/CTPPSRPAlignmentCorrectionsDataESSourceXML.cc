/****************************************************************************
 *
 * This is a part of CMS-TOTEM PPS offline software.
 * Authors: 
 *  Jan Kaspar (jan.kaspar@gmail.com) 
 *  Helena Malbouisson 
 *  Clemencia Mora Herrera
 ****************************************************************************/

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/SourceFactory.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/EventSetupRecordIntervalFinder.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"


#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSRPAlignmentCorrectionsMethods.h"
#include "CondFormats/DataRecord/interface/CTPPSRPAlignmentCorrectionsDataRcd.h"

#include <vector>
#include <string>
#include <map>
#include <set>


/**
 * Loads alignment corrections to EventSetup.
 **/
class  CTPPSRPAlignmentCorrectionsDataESSourceXML : public edm::ESProducer , public edm::EventSetupRecordIntervalFinder
{
public:
  CTPPSRPAlignmentCorrectionsDataESSourceXML(const edm::ParameterSet &p);
  ~CTPPSRPAlignmentCorrectionsDataESSourceXML() override;

  std::unique_ptr<CTPPSRPAlignmentCorrectionsData> produceMeasured(const CTPPSRPAlignmentCorrectionsDataRcd &);
  
protected:
  unsigned int verbosity;
  CTPPSRPAlignmentCorrectionsData acMeasured;

  void setIntervalFor(const edm::eventsetup::EventSetupRecordKey&, const edm::IOVSyncValue&, edm::ValidityInterval&) override;

};

//----------------------------------------------------------------------------------------------------

using namespace std;
using namespace edm;

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

CTPPSRPAlignmentCorrectionsDataESSourceXML::CTPPSRPAlignmentCorrectionsDataESSourceXML(const edm::ParameterSet &pSet) :
  verbosity(pSet.getUntrackedParameter<unsigned int>("verbosity", 0))
{
  std::string measuredFiles;
  const auto &f= pSet.getParameter<std::string>("XMLFile");
  measuredFiles=edm::FileInPath(f).fullPath();
  acMeasured =  CTPPSRPAlignmentCorrectionsMethods::loadFromXML(measuredFiles);
  
  setWhatProduced(this, &CTPPSRPAlignmentCorrectionsDataESSourceXML::produceMeasured);

  findingRecord<CTPPSRPAlignmentCorrectionsDataRcd>();

}

//----------------------------------------------------------------------------------------------------


CTPPSRPAlignmentCorrectionsDataESSourceXML::~CTPPSRPAlignmentCorrectionsDataESSourceXML()
{
}

//----------------------------------------------------------------------------------------------------

std::unique_ptr<CTPPSRPAlignmentCorrectionsData> CTPPSRPAlignmentCorrectionsDataESSourceXML::produceMeasured(const CTPPSRPAlignmentCorrectionsDataRcd &iRecord)
{
  return std::make_unique<CTPPSRPAlignmentCorrectionsData>(acMeasured);
}


//----------------------------------------------------------------------------------------------------

void CTPPSRPAlignmentCorrectionsDataESSourceXML::setIntervalFor(const edm::eventsetup::EventSetupRecordKey &key,
						   const IOVSyncValue& iosv, ValidityInterval& valInt) 
{
  if (verbosity)
    {
      time_t unixTime = iosv.time().unixTime();
      char timeStr[50];
      strftime(timeStr, 50, "%F %T", localtime(&unixTime));

      LogVerbatim("CTPPSIncludeAlignmentsFromXML")
	<< ">> CTPPSIncludeAlignmentsFromXML::setIntervalFor(" << key.name() << ")";

      LogVerbatim("CTPPSIncludeAlignmentsFromXML")
	<< "    event=" << iosv.eventID() << ", UNIX timestamp=" << unixTime << " (" << timeStr << ")";
    }

  // // determine what sequence and corrections should be used
  CTPPSRPAlignmentCorrectionsData *p_corr = nullptr;
  
  if (strcmp(key.name(), "CTPPSRPAlignmentCorrectionsDataRcd") == 0)
    {
      p_corr = &acMeasured;
    }

   if (p_corr == nullptr)
     throw cms::Exception("CTPPSIncludeAlignmentsFromXML::setIntervalFor") << "Unknown record " << key.name();

   // // find the corresponding interval
   edm::EventID event_next_start(edm::EventID::maxRunNumber(), edm::EventID::maxLuminosityBlockNumber(), 1);

   valInt = ValidityInterval(iosv, iosv.endOfTime());
  
}

DEFINE_FWK_EVENTSETUP_SOURCE(CTPPSRPAlignmentCorrectionsDataESSourceXML);
