#include "CondCore/CondDB/interface/ConnectionPool.h"
#include "CondFormats/Common/interface/TimeConversions.h"
#include "CondTools/RunInfo/interface/LHCInfoPerFillPopConSourceHandler.h"
#include "CondTools/RunInfo/interface/OMSAccess.h"
#include "CoralBase/Attribute.h"
#include "CoralBase/AttributeList.h"
#include "CoralBase/AttributeSpecification.h"
#include "CoralBase/TimeStamp.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "RelationalAccess/ICursor.h"
#include "RelationalAccess/IQuery.h"
#include "RelationalAccess/ISchema.h"
#include "RelationalAccess/ISessionProxy.h"
#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>




// void LHCInfoPerFillPopConSourceHandler::

// size_t LHCInfoPerFillPopConSourceHandler::getLumiData


// void LHCInfoPerFillPopConSourceHandler::getDipData(const cond::OMSService& oms,
//                                                    const boost::posix_time::ptime& beginFillTime,
//                                                    const boost::posix_time::ptime& endFillTime) 

// bool LHCInfoPerFillPopConSourceHandler::getCTTPSData(cond::persistency::Session& session,
//                                                      const boost::posix_time::ptime& beginFillTime,
//                                                      const boost::posix_time::ptime& endFillTime) 

// namespace theLHCInfoPerFillImpl {
//   static const std::map<std::string, int> vecMap = {
//       {"Beam1/beamPhaseMean", 1}, {"Beam2/beamPhaseMean", 2}, {"Beam1/cavPhaseMean", 3}, {"Beam2/cavPhaseMean", 4}};
//   void setElementData(cond::Time_t since,
//                       const std::string& dipVal,
//                       unsigned int elementNr,
//                       float value,
//                       LHCInfoPerFill& payload,
//                       std::set<cond::Time_t>& initList) {
//     if (initList.find(since) == initList.end()) {
//       payload.beam1VC().resize(LHCInfoPerFill::bunchSlots, 0.);
//       payload.beam2VC().resize(LHCInfoPerFill::bunchSlots, 0.);
//       payload.beam1RF().resize(LHCInfoPerFill::bunchSlots, 0.);
//       payload.beam2RF().resize(LHCInfoPerFill::bunchSlots, 0.);
//       initList.insert(since);
//     }
//     // set the current values to all of the payloads of the lumi section samples after the current since
//     if (elementNr < LHCInfoPerFill::bunchSlots) {
//       switch (vecMap.at(dipVal)) {
//         case 1:
//           payload.beam1VC()[elementNr] = value;
//           break;
//         case 2:
//           payload.beam2VC()[elementNr] = value;
//           break;
//         case 3:
//           payload.beam1RF()[elementNr] = value;
//           break;
//         case 4:
//           payload.beam2RF()[elementNr] = value;
//           break;
//         default:
//           break;
//       }
//     }
//   }
// }  // namespace theLHCInfoPerFillImpl

// bool LHCInfoPerFillPopConSourceHandler::getEcalData(cond::persistency::Session& session,
//                                                     const boost::posix_time::ptime& lowerTime,
//                                                     const boost::posix_time::ptime& upperTime,
//                                                     bool update) {
//   //run the sixth query against the CMS_DCS_ENV_PVSS_COND schema
//   //Initializing the CMS_DCS_ENV_PVSS_COND schema.
//   coral::ISchema& ECAL = session.nominalSchema();
//   //start the transaction against the fill logging schema
//   //execute query for ECAL Data
//   std::unique_ptr<coral::IQuery> ECALDataQuery(ECAL.newQuery());
//   //FROM clause
//   ECALDataQuery->addToTableList(std::string("BEAM_PHASE"));
//   //SELECT clause
//   ECALDataQuery->addToOutputList(std::string("CHANGE_DATE"));
//   ECALDataQuery->addToOutputList(std::string("DIP_value"));
//   ECALDataQuery->addToOutputList(std::string("element_nr"));
//   ECALDataQuery->addToOutputList(std::string("VALUE_NUMBER"));
//   //WHERE CLAUSE
//   coral::AttributeList ECALDataBindVariables;
//   ECALDataBindVariables.extend<coral::TimeStamp>(std::string("lowerTime"));
//   ECALDataBindVariables.extend<coral::TimeStamp>(std::string("upperTime"));
//   ECALDataBindVariables[std::string("lowerTime")].data<coral::TimeStamp>() = coral::TimeStamp(lowerTime);
//   ECALDataBindVariables[std::string("upperTime")].data<coral::TimeStamp>() = coral::TimeStamp(upperTime);
//   std::string conditionStr = std::string(
//       "(DIP_value LIKE '%beamPhaseMean%' OR DIP_value LIKE '%cavPhaseMean%') AND CHANGE_DATE >= :lowerTime AND "
//       "CHANGE_DATE < :upperTime");

//   ECALDataQuery->setCondition(conditionStr, ECALDataBindVariables);
//   //ORDER BY clause
//   ECALDataQuery->addToOrderList(std::string("CHANGE_DATE"));
//   ECALDataQuery->addToOrderList(std::string("DIP_value"));
//   ECALDataQuery->addToOrderList(std::string("element_nr"));
//   //define query output
//   coral::AttributeList ECALDataOutput;
//   ECALDataOutput.extend<coral::TimeStamp>(std::string("CHANGE_DATE"));
//   ECALDataOutput.extend<std::string>(std::string("DIP_value"));
//   ECALDataOutput.extend<unsigned int>(std::string("element_nr"));
//   ECALDataOutput.extend<float>(std::string("VALUE_NUMBER"));
//   //ECALDataQuery->limitReturnedRows( 14256 ); //3564 entries per vector.
//   ECALDataQuery->defineOutput(ECALDataOutput);
//   //execute the query
//   coral::ICursor& ECALDataCursor = ECALDataQuery->execute();
//   cond::Time_t changeTime = 0;
//   cond::Time_t firstTime = 0;
//   std::string dipVal = "";
//   unsigned int elementNr = 0;
//   float value = 0.;
//   std::set<cond::Time_t> initializedVectors;
//   theLHCInfoPerFillImpl::LumiSectionFilter filter(m_tmpBuffer);
//   bool ret = false;
//   if (m_prevPayload.get()) {
//     for (auto& lumiSlot : m_tmpBuffer) {
//       lumiSlot.second->setBeam1VC(m_prevPayload->beam1VC());
//       lumiSlot.second->setBeam2VC(m_prevPayload->beam2VC());
//       lumiSlot.second->setBeam1RF(m_prevPayload->beam1RF());
//       lumiSlot.second->setBeam2RF(m_prevPayload->beam2RF());
//     }
//   }
//   std::map<cond::Time_t, cond::Time_t> iovMap;
//   if (m_tmpBuffer.empty()) {
//     return ret;
//   }
//   cond::Time_t lowerLumi = m_tmpBuffer.front().first;
//   while (ECALDataCursor.next()) {
//     if (m_debug) {
//       std::ostringstream ECAL;
//       ECALDataCursor.currentRow().toOutputStream(ECAL);
//     }
//     coral::Attribute const& changeDateAttribute = ECALDataCursor.currentRow()[std::string("CHANGE_DATE")];
//     if (!changeDateAttribute.isNull()) {
//       ret = true;
//       boost::posix_time::ptime chTime = changeDateAttribute.data<coral::TimeStamp>().time();
//       // move the first IOV found to the start of the fill interval selected
//       if (changeTime == 0) {
//         firstTime = cond::time::from_boost(chTime);
//       }
//       changeTime = cond::time::from_boost(chTime);
//       cond::Time_t iovTime = changeTime;
//       if (!update and changeTime == firstTime)
//         iovTime = lowerLumi;
//       coral::Attribute const& dipValAttribute = ECALDataCursor.currentRow()[std::string("DIP_value")];
//       coral::Attribute const& valueNumberAttribute = ECALDataCursor.currentRow()[std::string("VALUE_NUMBER")];
//       coral::Attribute const& elementNrAttribute = ECALDataCursor.currentRow()[std::string("element_nr")];
//       if (!dipValAttribute.isNull() and !valueNumberAttribute.isNull()) {
//         dipVal = dipValAttribute.data<std::string>();
//         elementNr = elementNrAttribute.data<unsigned int>();
//         value = valueNumberAttribute.data<float>();
//         if (std::isnan(value))
//           value = 0.;
//         if (filter.process(iovTime)) {
//           iovMap.insert(std::make_pair(changeTime, filter.current()->first));
//           for (auto it = filter.current(); it != m_tmpBuffer.end(); it++) {
//             LHCInfoPerFill& payload = *(it->second);
//             theLHCInfoPerFillImpl::setElementData(it->first, dipVal, elementNr, value, payload, initializedVectors);
//           }
//         }
//         //}
//       }
//     }
//   }
//   if (m_debug) {
//     for (auto& im : iovMap) {
//       edm::LogInfo(m_name) << "Found iov=" << im.first << " (" << cond::time::to_boost(im.first) << " ) moved to "
//                            << im.second << " ( " << cond::time::to_boost(im.second) << " )";
//     }
//   }
//   return ret;
// }

// void LHCInfoPerFillPopConSourceHandler::addEmptyPayload(cond::Time_t iov) {

// namespace theLHCInfoPerFillImpl {
//   bool comparePayloads(const LHCInfoPerFill& rhs, const LHCInfoPerFill& lhs) {
//     if (rhs.fillNumber() != lhs.fillNumber() || rhs.delivLumi() != lhs.delivLumi() || rhs.recLumi() != lhs.recLumi() ||
//         rhs.instLumi() != lhs.instLumi() || rhs.instLumiError() != lhs.instLumiError() ||
//         rhs.lhcState() != rhs.lhcState() || rhs.lhcComment() != rhs.lhcComment() ||
//         rhs.ctppsStatus() != rhs.ctppsStatus()) {
//       return false;
//     }
//     return true;
//   }

//   size_t transferPayloads(const std::vector<std::pair<cond::Time_t, std::shared_ptr<LHCInfoPerFill>>>& buffer,
//                           std::map<cond::Time_t, std::shared_ptr<LHCInfoPerFill>>& iovsToTransfer,
//                           std::shared_ptr<LHCInfoPerFill>& prevPayload) {
//     size_t niovs = 0;
//     std::stringstream condIovs;
//     std::stringstream formattedIovs;
//     for (auto& iov : buffer) {
//       bool add = false;
//       auto payload = iov.second;
//       cond::Time_t since = iov.first;
//       if (iovsToTransfer.empty()) {
//         add = true;
//       } else {
//         LHCInfoPerFill& lastAdded = *iovsToTransfer.rbegin()->second;
//         if (!comparePayloads(lastAdded, *payload)) {
//           add = true;
//         }
//       }
//       if (add) {
//         niovs++;
//         condIovs << since << " ";
//         formattedIovs << boost::posix_time::to_iso_extended_string(cond::time::to_boost(since)) << " ";
//         iovsToTransfer.insert(std::make_pair(since, payload));
//         prevPayload = iov.second;
//       }
//     }
//     edm::LogInfo("transferPayloads") << "TRANSFERED IOVS: " << condIovs.str();
//     edm::LogInfo("transferPayloads") << "FORMATTED TRANSFERED IOVS: " << formattedIovs.str();
//     return niovs;
//   }

// }  // namespace theLHCInfoPerFillImpl

// void LHCInfoPerFillPopConSourceHandler::getNewObjects() {
//   //reference to the last payload in the tag
//   Ref previousFill;

//   //if a new tag is created, transfer fake fill from 1 to the first fill for the first time
//   if (tagInfo().size == 0) {
//     edm::LogInfo(m_name) << "New tag " << tagInfo().name << "; from " << m_name << "::getNewObjects";
//   } else {
//     //check what is already inside the database
//     edm::LogInfo(m_name) << "got info for tag " << tagInfo().name << ": size " << tagInfo().size
//                          << ", last object valid since " << tagInfo().lastInterval.since << " ( "
//                          << boost::posix_time::to_iso_extended_string(
//                                 cond::time::to_boost(tagInfo().lastInterval.since))
//                          << " ); from " << m_name << "::getNewObjects";
//   }

//   cond::Time_t lastSince = tagInfo().lastInterval.since;
//   if (tagInfo().isEmpty()) {
//     // for a new or empty tag, an empty payload should be added on top with since=1
//     addEmptyPayload(1);
//     lastSince = 1;
//   } else {
//     edm::LogInfo(m_name) << "The last Iov in tag " << tagInfo().name << " valid since " << lastSince << "from "
//                          << m_name << "::getNewObjects";
//   }

//   boost::posix_time::ptime executionTime = boost::posix_time::second_clock::local_time();
//   cond::Time_t targetSince = 0;
//   cond::Time_t executionTimeIov = cond::time::from_boost(executionTime);
//   if (!m_startTime.is_not_a_date_time()) {
//     targetSince = cond::time::from_boost(m_startTime);
//   }
//   if (lastSince > targetSince)
//     targetSince = lastSince;

//   edm::LogInfo(m_name) << "Starting sampling at "
//                        << boost::posix_time::to_simple_string(cond::time::to_boost(targetSince));

//   //retrieve the data from the relational database source
//   cond::persistency::ConnectionPool connection;
//   //configure the connection
//   if (m_debug) {
//     connection.setMessageVerbosity(coral::Debug);
//   } else {
//     connection.setMessageVerbosity(coral::Error);
//   }
//   connection.setAuthenticationPath(m_authpath);
//   connection.configure();
//   //create the sessions
//   cond::persistency::Session session = connection.createSession(m_connectionString, false);
//   cond::persistency::Session session2 = connection.createSession(m_ecalConnectionString, false);
//   // fetch last payload when available
//   if (!tagInfo().lastInterval.payloadId.empty()) {
//     cond::persistency::Session session3 = dbSession();
//     session3.transaction().start(true);
//     m_prevPayload = session3.fetchPayload<LHCInfoPerFill>(tagInfo().lastInterval.payloadId);
//     session3.transaction().commit();
//   }

//   // bool iovAdded = false;
//   while (true) {
//     if (targetSince >= executionTimeIov) {
//       edm::LogInfo(m_name) << "Sampling ended at the time "
//                            << boost::posix_time::to_simple_string(cond::time::to_boost(executionTimeIov));
//       break;
//     }
//     bool updateEcal = false;
//     boost::posix_time::ptime targetTime = cond::time::to_boost(targetSince);
//     boost::posix_time::ptime startSampleTime;
//     boost::posix_time::ptime endSampleTime;

//     cond::OMSService oms;
//     oms.connect(m_omsBaseUrl);
//     auto query = oms.query("fills");

//     edm::LogInfo(m_name) << "Searching new fill after " << boost::posix_time::to_simple_string(targetTime);
//     query->filterNotNull("start_stable_beam").filterNotNull("fill_number");
//     if (targetTime > cond::time::to_boost(m_prevPayload->createTime())) {
//       query->filterGE("start_time", targetTime);
//     } else {
//       query->filterGT("start_time", targetTime);
//     }

//     query->filterLT("start_time", m_endTime);
//     if (m_endFillMode)
//       query->filterNotNull("end_time");
//     bool foundFill = query->execute();
//     if (foundFill)
//       foundFill = theLHCInfoPerFillImpl::makeFillPayload(m_fillPayload, query->result());
//     if (!foundFill) {
//       edm::LogInfo(m_name) << "No fill found - END of job.";
//       // if (iovAdded)
//       //   addEmptyPayload(targetSince);
//       break;
//     }

//     startSampleTime = cond::time::to_boost(m_fillPayload->createTime());
//     cond::Time_t startFillTime = m_fillPayload->createTime();
//     cond::Time_t endFillTime = m_fillPayload->endTime();
//     unsigned short lhcFill = m_fillPayload->fillNumber();
//     bool ongoingFill = endFillTime == 0ULL;
//     if (ongoingFill) {
//       edm::LogInfo(m_name) << "Found ongoing fill " << lhcFill << " created at " << cond::time::to_boost(startFillTime);
//       endSampleTime = executionTime;
//       targetSince = executionTimeIov;
//     } else {
//       edm::LogInfo(m_name) << "Found fill " << lhcFill << " created at " << cond::time::to_boost(startFillTime)
//                            << " ending at " << cond::time::to_boost(endFillTime);
//       endSampleTime = cond::time::to_boost(endFillTime);
//       targetSince = endFillTime;
//     }
//     if (m_endFillMode || ongoingFill) {
//       getDipData(oms, startSampleTime, endSampleTime);
//       getLumiData(oms, lhcFill, startSampleTime, endSampleTime);
//       if (!m_tmpBuffer.empty()) {
//         boost::posix_time::ptime flumiStart = cond::time::to_boost(m_tmpBuffer.front().first);
//         boost::posix_time::ptime flumiStop = cond::time::to_boost(m_tmpBuffer.back().first);
//         edm::LogInfo(m_name) << "First lumi starts at " << flumiStart << " last lumi starts at " << flumiStop;
//         session.transaction().start(true);
//         getCTTPSData(session, startSampleTime, endSampleTime);
//         session.transaction().commit();
//         session2.transaction().start(true);
//         getEcalData(session2, startSampleTime, endSampleTime, updateEcal);
//         session2.transaction().commit();
//       }
//     }

//     size_t niovs = theLHCInfoPerFillImpl::transferPayloads(m_tmpBuffer, m_iovs, m_prevPayload);
//     edm::LogInfo(m_name) << "Added " << niovs << " iovs within the Fill time";
//     m_tmpBuffer.clear();
//     // iovAdded = true;
//     if (m_prevPayload->fillNumber() and !ongoingFill)
//       addEmptyPayload(endFillTime);
//   }
// }

// std::string LHCInfoPerFillPopConSourceHandler::id() const { return m_name; }
