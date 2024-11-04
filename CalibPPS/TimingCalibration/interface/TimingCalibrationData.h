#ifndef CalibPPS_TimingCalibration_TimingCalibrationData_h
#define CalibPPS_TimingCalibration_TimingCalibrationData_h

// #include "CalibPPS/TimingCalibration/interface/DoublePeakCorrection.h"
#include "DoublePeakCorrection.h"
// #include "CalibPPS/TimingCalibration/interface/PlaneMap.h"
#include "PlaneMap.h"

#include "DQMServices/Core/interface/MonitorElement.h"

struct TimingCalibrationData {
  using MonitorMap = std::unordered_map<uint32_t, dqm::reco::MonitorElement*>;

  MonitorMap leadingTime;
  MonitorMap toT;
  MonitorMap leadingTimeVsToT;
  std::unordered_map<PlaneKey, dqm::reco::MonitorElement*, PlaneKeyHash> leadingTimeVsLs;

  DoublePeakCorrection doublePeakCorrection;
};

#endif
