/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *   Nicola Minafra (nicola.minafra@cern.ch)
 *
 ****************************************************************************/

#ifndef RecoCTPPS_TotemRPLocal_CTPPSDiamondTimingGetParameters
#define RecoCTPPS_TotemRPLocal_CTPPSDiamondTimingGetParameters

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"

#include <vector>
#include <unordered_map>

/**
 * \brief Class retriving parameters for CTPPS Diamond Detectors. TEMP IMPLEMENTATION!
 * \date Oct 2017
**/
class CTPPSDiamondTimingGetParameters
{
  public:
    CTPPSDiamondTimingGetParameters( const edm::ParameterSet& );
    ~CTPPSDiamondTimingGetParameters();

    float getT0( const CTPPSDiamondDetId& detId );
    
    float getPrecision( const CTPPSDiamondDetId& detId );
    
    /// Returns the coarse time shift (in bits) needed to align the channels
    int getCoarseAlignment( const CTPPSDiamondDetId& detId );
    
    std::vector<float> getTOTParameters( const CTPPSDiamondDetId& detId );
    
  private:
    std::unordered_map<unsigned int, float> t0_map_;
    std::unordered_map<unsigned int, int> t0_shiftmap_;

};

#endif
