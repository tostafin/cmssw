/****************************************************************************
*
* This is a part of TOTEM offline software.
* Authors: 
*   Maciej Wróbel (wroblisko@gmail.com)
*
****************************************************************************/

#include "FWCore/Utilities/interface/typelookup.h"

#include "CondFormats/PPSObjects/interface/TotemDAQMapping.h"

using namespace std;

//----------------------------------------------------------------------------------------------------

std::ostream &operator<<(std::ostream &s, const TotemVFATInfo &vi) {
  s << vi.symbolicID << ", hw id=0x" << hex << vi.hwID << dec;

  return s;
}

//----------------------------------------------------------------------------------------------------

void TotemDAQMapping::insert(const TotemFramePosition &fp, const TotemVFATInfo &vi) {
  auto it = VFATMapping.find(fp);
  if (it != VFATMapping.end()) {
    cerr << "WARNING in DAQMapping::insert > Overwriting entry at " << fp << ". Previous: " << endl
         << "    " << VFATMapping[fp] << "," << endl
         << "  new: " << endl
         << "    " << vi << ". " << endl;
  }

  VFATMapping[fp] = vi;
}

//----------------------------------------------------------------------------------------------------
void TotemDAQMapping::insert(const TotemT2FramePosition &fp2, const TotemVFATInfo &vi) {
  TotemFramePosition fp1(fp2.getRawPosition());
  auto it = VFATMapping.find(fp1);
  if (it != VFATMapping.end()) {
    cerr << "WARNING in DAQMapping::insert > Overwriting T2 entry at " << fp2 << ". Previous: " << endl
         << "    " << VFATMapping[fp1] << "," << endl
         << "  new: " << endl
         << "    " << vi << ". " << endl;
  }

  VFATMapping[fp1] = vi;
}

//----------------------------------------------------------------------------------------------------

const TotemDAQMapping::TotemTimingPlaneChannelPair TotemDAQMapping::getTimingChannel(const uint8_t hwId) const {
  TotemTimingPlaneChannelPair pair;
  auto iterator = totemTimingChannelMap.find(hwId);
  if (iterator != totemTimingChannelMap.end())
    pair = iterator->second;
  return pair;
}

//----------------------------------------------------------------------------------------------------

TYPELOOKUP_DATA_REG(TotemDAQMapping);
