#ifndef CondTools_RunInfo_LHCInfoPerLSPopConSourceHandler_h
#define CondTools_RunInfo_LHCInfoPerLSPopConSourceHandler_h

#include <string>

#include "CondCore/CondDB/interface/Types.h"
#include "CondCore/PopCon/interface/PopConSourceHandler.h"
#include "CondFormats/RunInfo/interface/LHCInfoPerLS.h"
#include "CondTools/RunInfo/interface/OMSAccess.h"
#include "FWCore/ParameterSet/interface/ParameterSetfwd.h"

namespace cond {
  class OMSService;
}

namespace edm {
  class ConfigurationDescriptions;
}

class LHCInfoPerLSPopConSourceHandler : public popcon::PopConSourceHandler<LHCInfoPerLS> {
public:
  LHCInfoPerLSPopConSourceHandler(const edm::ParameterSet& pset);
  ~LHCInfoPerLSPopConSourceHandler() override;
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  void getNewObjects() override;
  std::string id() const override;

  static constexpr unsigned int kLumisectionsQueryLimit = 4000;

private:
  void addEmptyPayload(cond::Time_t iov);

  bool makeFillPayload(std::unique_ptr<LHCInfoPerLS>& targetPayload, const cond::OMSServiceResult& queryResult);

  void addPayloadToBuffer(cond::OMSServiceResultRef& row);
  size_t bufferAllLS(const cond::OMSServiceResult& queryResult);
  size_t bufferFirstStableBeamLS(const cond::OMSServiceResult& queryResult);

  size_t getLumiData(const cond::OMSService& service,
                     unsigned short fillId,
                     const boost::posix_time::ptime& beginFillTime,
                     const boost::posix_time::ptime& endFillTime);
  bool getCTTPSData(cond::persistency::Session& session,
                    const boost::posix_time::ptime& beginFillTime,
                    const boost::posix_time::ptime& endFillTime);

private:
  bool m_debug;
  // starting date for sampling
  boost::posix_time::ptime m_startTime;
  boost::posix_time::ptime m_endTime;
  // sampling interval in seconds
  unsigned int m_samplingInterval;
  bool m_endFillMode = true;
  std::string m_name;
  //for reading from relational database source
  std::string m_connectionString, m_ecalConnectionString;
  std::string m_dipSchema, m_authpath;
  std::string m_omsBaseUrl;
  std::unique_ptr<LHCInfoPerLS> m_fillPayload;
  std::shared_ptr<LHCInfoPerLS> m_prevPayload;
  cond::Time_t m_startFillTime;
  cond::Time_t m_endFillTime;
  cond::Time_t m_prevEndFillTime;
  cond::Time_t m_prevStartFillTime;
  std::vector<std::pair<cond::Time_t, std::shared_ptr<LHCInfoPerLS> > > m_tmpBuffer;
  bool m_lastPayloadEmpty = false;
};

#endif
