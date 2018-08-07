/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *   Nicola Minafra
 *
 ****************************************************************************/

#ifndef RecoCTPPS_TotemRPLocal_TotemTimingConversions
#define RecoCTPPS_TotemRPLocal_TotemTimingConversions

#include <string>
#include <vector>
#include "TF1.h"

#include "DataFormats/CTPPSDigi/interface/TotemTimingDigi.h"
#include "RecoCTPPS/TotemRPLocal/interface/CalibrationData.h"

class TotemTimingConversions {
public:
  TotemTimingConversions();
  TotemTimingConversions(const std::string& calibrationFile);

  void openCalibrationFile(const std::string& calibrationFile="/dev/null");

  const float getTimeOfFirstSample(const TotemTimingDigi& digi) const;

  const float getTriggerTime(const TotemTimingDigi& digi) const;

  std::vector<float> getTimeSamples(const TotemTimingDigi& digi) const;

  std::vector<float> getVoltSamples(const TotemTimingDigi& digi);

private:

  static const float SAMPIC_SAMPLING_PERIOD_NS;
  static const float SAMPIC_ADC_V;
  static const int SAMPIC_MAX_NUMBER_OF_SAMPLES;
  static const int SAMPIC_DEFAULT_OFFSET;

  bool calibrationFileOk_;
  TF1 calibrationFunction_;
  std::string calibrationFile_;
  CalibrationData calibrationData_;
};

#endif
