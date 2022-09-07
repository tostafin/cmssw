/****************************************************************************
 *
 * This is a part of PPS PI software.
 *
 ****************************************************************************/

#ifndef CONDCORE_CTPPSPLUGINS_LHCOPTICALFUNCTIONSSETCOLLECTIONHELPER_H
#define CONDCORE_CTPPSPLUGINS_LHCOPTICALFUNCTIONSSETCOLLECTIONHELPER_H

// User includes
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CondCore/Utilities/interface/PayloadInspectorModule.h"
#include "CondCore/Utilities/interface/PayloadInspector.h"
#include "CondCore/CondDB/interface/Time.h"
#include "CondFormats/PPSObjects/interface/LHCOpticalFunctionsSetCollection.h"
#include "CondFormats/PPSObjects/interface/LHCOpticalFunctionsSet.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"

// system includes
#include <memory>
#include <sstream>

// ROOT includes
#include "TCanvas.h"
#include "TLatex.h"

class CTPPSOptics {
public:
  enum RP { RP3 = 3, RP23 = 23, RP103 = 103, RP123 = 123 };
  
  static std::string getFunctionName(int idx){
    switch(idx){
        case 1:
            return "evx";
        case 2:
            return "eLx";
        case 3:
            return "e14";
        case 4: 
            return "exd";
        case 5:
            return "evpx";
        case 6:
            return "eLpx";
        case 7:
            return "e24";
        case 8:
            return "expd";
        case 9:
            return "e32";
        case 10:
            return "evy";
        case 11:
            return "eLy";
        case 12:
            return "eyd";
        case 13:
            return "e42";
        case 14:
            return "evpy";
        case 15:
            return "eLpy";
        case 16:
            return "eypd";

        default:
            return "no Func";    
    }
  }

  static std::string getStringFromRPEnum(const RP& rp) {
    switch (rp) {
      case 3:
        return "RP 3";
      case 23:
        return "RP 23";
      case 103:
        return "RP 103";
      case 123:
        return "RP 123";

      default:
        return "not here";
    }
  }

};

/************************************************
    Example plots
*************************************************/
template <class PayloadType, CTPPSOptics::RP rp>
class OpticalFunctionsInfo : public cond::payloadInspector::PlotImage<PayloadType, cond::payloadInspector::SINGLE_IOV> {
public:
  OpticalFunctionsInfo()
      : cond::payloadInspector::PlotImage<PayloadType, cond::payloadInspector::SINGLE_IOV>(
            "OpticalFunctionsInfo payload information") {}

  bool fill() override {
    auto tag = cond::payloadInspector::PlotBase::getTag<0>();
    auto tagname = tag.name;
    auto iov = tag.iovs.back();
    auto m_payload = this->fetchPayload(std::get<1>(iov));

    // if (m_payload != nullptr) {
    //     std::string line;
    //     std::vector<std::string> lines;
    //     std::stringstream ss;
    //     int counter = 0;

    //     for(auto& map_item: *m_payload){
    //         ss<<"angle: "<<map_item.first<<" ";
    //         for(auto& pot: map_item.second){
    //             CTPPSDetId* config = new CTPPSDetId(pot.first);
    //             int potId = config->arm() * 100 + config->station() * 10 + config->rp();
    //             if(potId == rp){
    //                 ss<<"RP: "<<potId<<" "<<"Xi [ ";
    //                 for(auto item: pot.second.getXiValues()){
    //                     if(counter==50){
    //                         ss<<item<<", ";
    //                         counter = 0;
    //                     }  
    //                     counter++;  
    //                 }
    //                 ss<<"]\n Fcn {";
    //                 int func = 1;
    //                 for(auto item: pot.second.getFcnValues()){
    //                     ss<<"\n"<< CTPPSOptics::getFunctionName(func)<<" [ ";
    //                     for(auto el: item){
    //                         if(counter==50){
    //                             ss<<el<<", ";
    //                             counter = 0;
    //                         }  
    //                         counter++;
    //                     }
    //                     ss<<"] ";
    //                     func++;
    //                 }
    //                 delete config;
    //                 ss<<"\n";
    //             }
    //             ss<<"\n";
    //             }
    //     }    
    //     while (getline(ss, line)) {
    //         lines.push_back(line);
    //     }

    //     TCanvas canvas("LHCOpticalFunctions payload information", "LHCOpticalFunctions payload information", 6000, 3000);
    //     canvas.cd(1);
    //     TLatex t;
    //     t.SetTextSize(0.018);

    //     int index = 0;
    //     for (float y = 0.98; index < int(lines.size()); y -= 0.02) {
    //         if (index < int(lines.size()))
    //         t.DrawLatex(0.02, y, lines[index++].c_str());
    //     }
    //     t.Draw();

    //     std::string fileName(this->m_imageFileName);
    //     canvas.SaveAs(fileName.c_str());

    //     return true;
    //     } else {
    //     return false;
    // }



    // PRINTING
    std::ofstream file;
    file.open("text.txt", std::ios_base::app);

    for(auto& map_item: *m_payload){
        file<<"angle: "<<map_item.first<<"\t\t";
        for(auto& pot: map_item.second){
            CTPPSDetId* config = new CTPPSDetId(pot.first);
            int potId = config->arm() * 100 + config->station() * 10 + config->rp();
            file<<potId<<"\t"<<"Xi[ ";
            for(auto item: pot.second.getXiValues()){
                file<<item<<", ";
            }
            file<<"] Fcn {";
            for(auto item: pot.second.getFcnValues()){
                file<<"\n[ ";
                for(auto el: item){
                    file<<el<<", ";
                }
                file<<"] ";
            }
            delete config;
            file<<"\n\n";
        }
        file<<"\n\n\n";
    }

    file.close();

    return true;
  }
};

#endif

