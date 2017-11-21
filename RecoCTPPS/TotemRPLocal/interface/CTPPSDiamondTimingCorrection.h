/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *   Nicola Minafra (nicola.minafra@cern.ch)
 *
 ****************************************************************************/

#ifndef RecoCTPPS_TotemRPLocal_CTPPSDiamondTimingCorrection
#define RecoCTPPS_TotemRPLocal_CTPPSDiamondTimingCorrection

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"

#include "DataFormats/CTPPSReco/interface/CTPPSDiamondRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondLocalTrack.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"

#include "RecoCTPPS/TotemRPLocal/interface/CTPPSDiamondTimingGetParameters.h"

#include <vector>
#include <unordered_map>
#include "TF1.h"

/**
 * \brief Class performing smart reconstruction for CTPPS Diamond Detectors.
 * \date Sep 2017
**/
class CTPPSDiamondTimingCorrection
{
  public:
    CTPPSDiamondTimingCorrection( const edm::ParameterSet& );
    ~CTPPSDiamondTimingCorrection();

    /// Writes in the RecHit the Time of arrival corrected using the Time Over Threshold
    float correctTiming( const CTPPSDiamondDetId& detId, const float& t, const float& tot );
    
    /// Returns the coarse time shift (in bits) needed to align the channels
    int getCoarseAlignment( const CTPPSDiamondDetId& detId );
    
    /// Reads time precision from database
    float getPrecision( const CTPPSDiamondDetId& detId, const bool withTot=true);

  private:
    const float startFromT_;
    const float stopAtT_;

    /// Function for Time Over Threshold correction
    TF1 tot_f_;
    
    CTPPSDiamondTimingGetParameters params_;
};

#endif
