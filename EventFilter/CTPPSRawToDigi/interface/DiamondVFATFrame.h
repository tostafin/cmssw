/**********************************************************
*
* Seyed Mohsen Etesami (setesami@cern.ch)    
*
**********************************************************/

#ifndef EventFilter_CTPPSRawToDigi_DiamondVFATFrame
#define EventFilter_CTPPSRawToDigi_DiamondVFATFrame

#include <vector>
#include <cstddef>
#include <stdint.h>

#include "EventFilter/CTPPSRawToDigi/interface/VFATFrame.h" 


/** 
 * This class intended to handle the timing infromation of diamond VFAT frame
**/
class DiamondVFATFrame : public VFATFrame
{
  private:
    enum { UNDEFINED, FULL, COMPACT } mode_ = UNDEFINED;
    enum {VFAT_11_HEADER=0xA000, VFAT_10_HEADER=0xC000, VFAT_9_HEADER=0xE000};
    
    void checkmode() {
      if ( ( (data[11]&0xf000) == VFAT_11_HEADER ) && ( (data[10]&0xf000) == VFAT_10_HEADER) && ( (data[9]&0xf000) == VFAT_9_HEADER) )
        mode_ = FULL;
      else mode_ = COMPACT;
    }
    
  public:
    DiamondVFATFrame(const word* inputData = NULL) : mode_(UNDEFINED) {
      checkmode();
    }
    
    DiamondVFATFrame(const DiamondVFATFrame& copy)
    {
      setData(copy.data);
      checkmode();
    }
    virtual ~DiamondVFATFrame() {}

    /// get timing infromation
    uint32_t getLeadingEdgeTime() const
    {
      uint32_t time = 0;
      if (mode_ == FULL) time = ((data[5]&0x1f)<<16)+data[6];
      else time = ((data[2]&0x1f)<<16)+data[3];
      time = (time & 0xFFE7FFFF) << 2 | (time & 0x00180000) >> 19;   //HPTDC inperpolation bits are MSB but should be LSB.... ask HPTDC designers...
      return time;
    }

    uint32_t getTrailingEdgeTime() const
    {
      uint32_t time = 0;
      if (mode_ == FULL) time = ((data[7]&0x1f)<<16)+data[8];
      else time = ((data[4]&0x1f)<<16)+data[5];
      time = (time & 0xFFE7FFFF) << 2 | (time & 0x00180000) >> 19;   //HPTDC inperpolation bits are MSB but should be LSB.... ask HPTDC designers...
      return time;
    }
    
    uint32_t getThresholdVoltage() const
    {
      uint32_t thV = 0;
      if (mode_ == FULL) thV = ((data[3]&0x7ff)<<16)+data[4];
      return thV;
    }

    VFATFrame::word getMultihit() const
    {
      uint32_t mhTmp = 0;
      if (mode_ == FULL) mhTmp = data[2] & 0x01;
      else mhTmp = data[1] & 0x01;
      return mhTmp;
    }

    VFATFrame::word getHptdcErrorFlag() const
    {
      uint32_t errTmp = 0;
      if (mode_ == FULL) errTmp = data[1] & 0xFFFF;
      else errTmp = data[0] & 0xFFFF;
      return errTmp;
    }

};                                                                     

#endif
