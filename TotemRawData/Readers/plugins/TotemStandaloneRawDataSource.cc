/****************************************************************************
 *
 * This is a part of TOTEM offline software.
 * Authors:
 *   Jan Kašpar (jan.kaspar@gmail.com)
 *
 ****************************************************************************/

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/InputSource.h"

#include "DataFormats/Provenance/interface/EventID.h"
#include "DataFormats/Provenance/interface/RunAuxiliary.h"
#include "DataFormats/Provenance/interface/LuminosityBlockAuxiliary.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"
#include "DataFormats/Provenance/interface/ProcessHistoryRegistry.h"

#include "FWCore/Sources/interface/PuttableSourceBase.h"

#include "TotemRawData/Readers/interface/SRSFileReader.h"

#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"

#include <iostream>
#include <iomanip>
#include <deque>

//----------------------------------------------------------------------------------------------------

class TotemStandaloneRawDataSource : public edm::PuttableSourceBase
{
  public:
    struct FileInfo
	{
      std::string fileName;         ///< path to the file
      SRSFileReader *file;          ///< instance of reader class
      edm::RunNumber_t runNumber;   ///< associated run number
    };

    TotemStandaloneRawDataSource(const edm::ParameterSet &, const edm::InputSourceDescription&);
    virtual ~TotemStandaloneRawDataSource();
    virtual void beginJob();
    virtual void endJob();

    static void fillDescription(edm::ParameterSetDescription& desc);

  protected:
    unsigned int verbosity;

    std::vector<std::string> fileNames;                     ///< vector of raw data files names
    bool setRunNumberFromFileName;                          ///< whether run number shall be set from file names (see GetRunNumberFromFileName method)
    unsigned int printProgressFrequency;                    ///< frequency with which the progress (i.e. event number) is to be printed

    unsigned int fileIdx;                                   ///< current file index (within files), counted from 0

    std::vector<FileInfo> files;                            ///< to keep information about opened files

    /// \brief extracts run number from a file name
    edm::RunNumber_t GetRunNumberFromFileName(const std::string &str);

  private:
    /// list of the next state items
    std::deque<ItemType> items;
    
    /// tries to load a next raw event and updates the list of next states 'items'
    void LoadRawDataEvent();

    /// called by the framework to determine the next state (run, lumi, event, stop, ...)
    /// here it simply returns the popped state from the 'items' queue
    virtual ItemType getNextItemType();

    /// called by the framework for item type 'IsRun'
    virtual std::shared_ptr<edm::RunAuxiliary> readRunAuxiliary_();

    /// called by the framework for item type 'IsLumi'
    virtual std::shared_ptr<edm::LuminosityBlockAuxiliary> readLuminosityBlockAuxiliary_();

    /// called by the framework for item type 'IsEvent'
    /// the cached (pre-loaded) raw data (currentFEDCollection) are inserted into the event
    virtual void readEvent_(edm::EventPrincipal& eventPrincipal);

    /// pre-loaded raw event (counters and VFAT data)
    uint64_t currentTimestamp;
    std::unique_ptr<FEDRawDataCollection> currentFEDCollection;

    /// ID of the current (next) event
    edm::EventID eventID;

    /// UNIX timestamp of the previous event
    uint64_t previousTimestamp;
};

//----------------------------------------------------------------------------------------------------

using namespace std;
using namespace edm;

//#define DEBUG 1

//----------------------------------------------------------------------------------------------------


TotemStandaloneRawDataSource::TotemStandaloneRawDataSource(const edm::ParameterSet& pSet, const edm::InputSourceDescription& desc) :
  PuttableSourceBase(pSet, desc),
  verbosity(pSet.getUntrackedParameter<unsigned int>("verbosity", 0)),
  fileNames(pSet.getUntrackedParameter<vector<string> >("fileNames")),
  setRunNumberFromFileName(pSet.getParameter<bool>("setRunNumberFromFileName")),
  printProgressFrequency(pSet.getUntrackedParameter<unsigned int>("printProgressFrequency", 0)),
  currentTimestamp(0),
  eventID(0, 0, 0),
  previousTimestamp(0)
{
#ifdef DEBUG
  printf("TotemStandaloneRawDataSource::TotemStandaloneRawDataSource\n");
#endif

  produces<FEDRawDataCollection>();
}

//----------------------------------------------------------------------------------------------------

TotemStandaloneRawDataSource::~TotemStandaloneRawDataSource()
{
}

//----------------------------------------------------------------------------------------------------

std::shared_ptr<RunAuxiliary> TotemStandaloneRawDataSource::readRunAuxiliary_()
{
#ifdef DEBUG
  printf(">> TotemStandaloneRawDataSource::readRunAuxiliary_\n");
#endif

  Timestamp ts_beg(currentTimestamp << 32);
  Timestamp ts_end(Timestamp::endOfTime().value() - 0);

  return std::shared_ptr < RunAuxiliary > (new RunAuxiliary(eventID.run(), ts_beg, ts_end));
}

//----------------------------------------------------------------------------------------------------

std::shared_ptr<LuminosityBlockAuxiliary> TotemStandaloneRawDataSource::readLuminosityBlockAuxiliary_()
{
#ifdef DEBUG
  printf(">> TotemStandaloneRawDataSource::readLuminosityBlockAuxiliary_\n");
#endif

  // we create luminosity blocks of 1s duration
  Timestamp ts_beg(currentTimestamp << 32);
  Timestamp ts_end(((currentTimestamp + 1) << 32) - 1);

  return std::shared_ptr <LuminosityBlockAuxiliary> (new LuminosityBlockAuxiliary(eventID.run(), eventID.luminosityBlock(), ts_beg, ts_end));
}

//----------------------------------------------------------------------------------------------------

void TotemStandaloneRawDataSource::readEvent_(EventPrincipal& eventPrincipal)
{
#ifdef DEBUG
  printf(">> TotemStandaloneRawDataSource::readEvent_ : %lu, %llu\n", currentTimestamp, eventID.event());
#endif
  
  // create Event structure and fill with auxiliary data
  Timestamp ts(currentTimestamp << 32);  // conversion from UNIX timestamp: see src/DataFormats/Provenance/interface/Timestamp.h
  bool isRealData = true;
  EventAuxiliary::ExperimentType expType(EventAuxiliary::Undefined);
  EventAuxiliary aux(eventID, processGUID(), ts, isRealData, expType);

  eventPrincipal.fillEventPrincipal(aux, processHistoryRegistry());

  Event e(eventPrincipal, moduleDescription(), nullptr);
  e.setProducer(this, nullptr);

  // put raw data into the event
  e.put(unique_ptr<FEDRawDataCollection>(currentFEDCollection.release()));
  e.commit_(std::vector<ProductResolverIndex>());

  if (printProgressFrequency > 0 && (eventID.event() % printProgressFrequency) == 0)
    cout << "\033[25Devent " << eventID.run() << ":" << eventID.event();
}

//----------------------------------------------------------------------------------------------------

void TotemStandaloneRawDataSource::LoadRawDataEvent()
{
#ifdef DEBUG
  printf(">> TotemStandaloneRawDataSource::LoadRawDataEvent\n");
#endif

  // prepare structure for the raw event
  currentFEDCollection = make_unique<FEDRawDataCollection>();

  // load next raw event
  bool newFile = false;
  while (true)
  {
    unsigned int result = files[fileIdx].file->GetNextEvent(currentTimestamp, *currentFEDCollection);

    if (result == 0)
      break;
    else {
      // try moving to next file
      fileIdx++;
      newFile = true;

      // stop if there are no more files
      if (fileIdx >= files.size())
      {
        items.push_back(IsStop);
        return;
      }
    }
  }

  bool beginning = (eventID.run() == 0);

  if (newFile || beginning)
  {
    items.push_back(IsRun);
    items.push_back(IsLumi);
    items.push_back(IsEvent);

    eventID = EventID(files[fileIdx].runNumber, 1, 1);

    previousTimestamp = currentTimestamp;

    return;
  } 

  // new luminosity block ??
  if (currentTimestamp != previousTimestamp)  // 1s resolution
  {
    items.push_back(IsLumi);
    eventID = EventID(eventID.run(), eventID.luminosityBlock() + 1, eventID.event()); // advance lumi number
  }

  previousTimestamp = currentTimestamp;

  eventID = EventID(eventID.run(), eventID.luminosityBlock(), eventID.event() + 1); // advance event number
  items.push_back(IsEvent);
}

//----------------------------------------------------------------------------------------------------

#ifdef DEBUG
InputSource::ItemType prevItem = InputSource::ItemType::IsInvalid;
#endif

InputSource::ItemType TotemStandaloneRawDataSource::getNextItemType()
{
  if (items.empty())
    LoadRawDataEvent();

  ItemType item = items.front();
  items.pop_front();

#ifdef DEBUG
  if (prevItem != item)
    printf(">> TotemStandaloneRawDataSource::getNextItemType > %u --> %u (run=%u, lumi block=%u, event=%llu)\n", prevItem, item,
      eventID.run(), eventID.luminosityBlock(), eventID.event());

  prevItem = item;
#endif

  return item;
}

//----------------------------------------------------------------------------------------------------

void TotemStandaloneRawDataSource::beginJob()
{
#ifdef DEBUG
  printf(">> TotemStandaloneRawDataSource::beginJob\n");
#endif

  PuttableSourceBase::beginJob();

  // try to open all files
  for (unsigned int i = 0; i < fileNames.size(); i++)
  {
	if (verbosity)
    {
        printf(">> TotemStandaloneRawDataSource::beginJob > Opening file `%s'.\n", fileNames[i].c_str());
	}

    // run number
    FileInfo fi;
    fi.fileName = fileNames[i];
    if (setRunNumberFromFileName)
      fi.runNumber = GetRunNumberFromFileName(fi.fileName);
    else
      fi.runNumber = i + 1;

    // open file
    fi.file = new SRSFileReader();
    if (fi.file->Open(fi.fileName) != 0)
    {
      delete fi.file;
      fi.file = NULL;
      throw cms::Exception("TotemStandaloneRawDataSource") << "Cannot open file " << fileNames[i] << std::endl;
    }

    files.push_back(fi);
  }

  if (!files.size())
    throw cms::Exception("TotemStandaloneRawDataSource") << "No files to read." << std::endl;

  // print file info
  if (verbosity > 0)
  {
    printf(">> TotemStandaloneRawDataSource::beginJob > files opened:\n");
    for (unsigned int i = 0; i < files.size(); i++)
      printf("%2u : %s\n", i, files[i].fileName.c_str());
  }

  // initialize the state of reader
  fileIdx = 0;

  items.push_back(IsFile);  // needed for the logic in InputSource::nextItemType 
}

//----------------------------------------------------------------------------------------------------

void TotemStandaloneRawDataSource::endJob()
{
#ifdef DEBUG
  printf(">> TotemStandaloneRawDataSource::endJob\n");
#endif
}
  
//----------------------------------------------------------------------------------------------------

void TotemStandaloneRawDataSource::fillDescription(edm::ParameterSetDescription& desc)
{
  PuttableSourceBase::fillDescription(desc);
}

//----------------------------------------------------------------------------------------------------

edm::RunNumber_t TotemStandaloneRawDataSource::GetRunNumberFromFileName(const string &path)
{
  // get file part of path
  size_t pos = path.find_last_of('/');
  string s = (pos == string::npos) ? path : path.substr(pos + 1);

  // build array of numerical bits in the name
  string digits = "0123456789";
  vector<unsigned int> nBits;
  pos = 0;
  while (pos != string::npos)
  {
    size_t p1 = s.find_first_of(digits, pos);
    if (p1 == string::npos)
      break;

    size_t p2 = s.find_first_not_of(digits, p1);

    string valStr = (p2 == string::npos) ? s.substr(p1) : s.substr(p1, p2 - p1);
    nBits.push_back(atoi(valStr.c_str()));

    pos = p2;
  }

  if (nBits.size() == 4)
  {
    const unsigned int run = nBits[0];
    const unsigned int eventBuilderHost = nBits[1];
    const unsigned int eventBuilderProcess = nBits[2];
    const unsigned int file = nBits[3];
    const unsigned int evbID = (eventBuilderHost-11) * 2 + eventBuilderProcess-1;
    return run*100000 + evbID*10000 + file;
  }

  throw cms::Exception("TotemStandaloneRawDataSource::GetRunNumberFromFileName")
    << "Run and file number cannot be extracted from file name `" << path << "'." << endl;
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_INPUT_SOURCE(TotemStandaloneRawDataSource);
