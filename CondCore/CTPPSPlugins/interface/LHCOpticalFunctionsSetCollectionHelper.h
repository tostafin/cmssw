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
#include <vector>
#include <utility> 

// ROOT includes
#include "TCanvas.h"
#include "TLatex.h"
#include "TMultiGraph.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TMatrixT.h"

class CTPPSOptics {
public:
  enum Angle { A130 = 130, A140 = 140, A150 = 150, A160 = 160};

  enum RP { RP3 = 3, RP23 = 23, RP103 = 103, RP123 = 123 };

  enum Function { evx, eLx, e14, exd, evpx, eLpx, e24, expd, e32, evy, eLy, eyd, e42, evpy, eLpy, eypd, nFunctions };
  
  static std::string getFunctionName(int idx){
    switch(idx){
        case 0:
            return "evx";
        case 1:
            return "eLx";
        case 2:
            return "e14";
        case 3: 
            return "exd";
        case 4:
            return "evpx";
        case 5:
            return "eLpx";
        case 6:
            return "e24";
        case 7:
            return "expd";
        case 8:
            return "e32";
        case 9:
            return "evy";
        case 10:
            return "eLy";
        case 11:
            return "eyd";
        case 12:
            return "e42";
        case 13:
            return "evpy";
        case 14:
            return "eLpy";
        case 15:
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

   static uint decodeRP(uint r) {
      CTPPSDetId* config = new CTPPSDetId(r);
      int potId = config->arm() * 100 + config->station() * 10 + config->rp();
      delete config;
      return potId;
  }

};

/************************************************
    History plots
*************************************************/
template <class PayloadType, CTPPSOptics::Angle angle, CTPPSOptics::RP rp, CTPPSOptics::Function fun>
class OpticalFunction_History : public cond::payloadInspector::HistoryPlot<PayloadType, float> {
public:
  OpticalFunction_History()
      : cond::payloadInspector::HistoryPlot<PayloadType, float>( "Angle: " + std::to_string(angle) + ", " +
            CTPPSOptics::getStringFromRPEnum(rp) + ", " + CTPPSOptics::getFunctionName(fun) +
                " Xi = 0.1 vs. Runs",
            CTPPSOptics::getStringFromRPEnum(rp) + " " + CTPPSOptics::getFunctionName(fun)) {}

  float getFromPayload(PayloadType& payload) override {
    float Xi = 0.001; //hard coded Xi value
      for(auto& map_item: payload){
        if(map_item.first == angle){
          for(auto& pot: map_item.second){
            if(CTPPSOptics::decodeRP(pot.first) == rp){
                return pot.second.getFcnValues()[fun][Xi*1000];
            }
          }
        }
      }  
      return 0;  
  }
};

template <class PayloadType, cond::payloadInspector::IOVMultiplicity nIOVs, int ntags, CTPPSOptics::Angle angle, CTPPSOptics::Function fun>
class OpticalFunctionRPs_History : public cond::payloadInspector::PlotImage<PayloadType, nIOVs, ntags> {
public:
  OpticalFunctionRPs_History()
      : cond::payloadInspector::PlotImage<PayloadType, nIOVs, ntags> ("Angle: " + std::to_string(angle) + ", " +
            CTPPSOptics::getFunctionName(fun) +
                ", Xi = 0.001 vs. Runs") {}

  bool fill() override {

    //data collect
    auto theIOVs = cond::payloadInspector::PlotBase::getTag<0>().iovs;
    auto firstiov = theIOVs.front();

    std::vector<std::pair<long long, float>> rp3, rp23, rp103, rp123;

    for(auto IOV : theIOVs){
        auto m_payload = this->fetchPayload(std::get<1>(IOV));

        //     // PRINTING
        // std::ofstream file;
        // file.open("text.txt", std::ios_base::app);
        // file<<std::to_string(std::get<0>(IOV))<<"\n";
        // for(auto& map_item: *m_payload){
        //     file<<"angle: "<<map_item.first<<"\t\t";
        //     for(auto& pot: map_item.second){
        //         CTPPSDetId* config = new CTPPSDetId(pot.first);
        //         int potId = config->arm() * 100 + config->station() * 10 + config->rp();
        //         file<<potId<<"\t"<<"Xi[ ";
        //         for(auto item: pot.second.getXiValues()){
        //             file<<item<<", ";
        //         }
        //         file<<"] Fcn {";
        //         for(auto item: pot.second.getFcnValues()){
        //             file<<"\n[ ";
        //             for(auto el: item){
        //                 file<<el<<", ";
        //             }
        //             file<<"] ";
        //         }
        //         delete config;
        //         file<<"\n\n";
        //     }
        //     file<<"\n\n\n";
        // }

        // file.close();

        float Xi = 0.001; //hard coded Xi value
        for(auto& map_item: *m_payload){
            if(map_item.first == angle){
                for(auto& pot: map_item.second){
                    switch (CTPPSOptics::decodeRP(pot.first))
                    {
                    case 3:
                        rp3.push_back(std::make_pair(static_cast<long long>(std::get<0>(IOV)), pot.second.getFcnValues()[fun][Xi*1000]));
                        break;
                    case 23:
                        rp23.push_back(std::make_pair(static_cast<long long>(std::get<0>(IOV)), pot.second.getFcnValues()[fun][Xi*1000]));
                        break;
                    case 103:
                        rp103.push_back(std::make_pair(static_cast<long long>(std::get<0>(IOV)), pot.second.getFcnValues()[fun][Xi*1000]));                        
                        break;
                    case 123:
                        rp123.push_back(std::make_pair(static_cast<long long>(std::get<0>(IOV)), pot.second.getFcnValues()[fun][Xi*1000]));                        
                        break;
                    
                    default:
                        break;
                    }
                }
            }
        }
    } 

    //drawing part
    TCanvas canvas("LHCOpticalFunctions payload information", "LHCOpticalFunctions payload information", 2000, 1000);
    canvas.cd(1);
    canvas.SetGrid();

    TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle(("Angle: " + std::to_string(angle) + ", " + CTPPSOptics::getFunctionName(fun) +", Xi = 0.001 vs Runs;Run no;" + CTPPSOptics::getFunctionName(fun)).c_str()); 
    Double_t *rp_x, *rp_y;
    
    rp_x = new  Double_t [rp3.size()]; 
    rp_y = new Double_t [rp3.size()];   
    for(unsigned idx = 0; idx < rp3.size(); idx++){
        rp_x[idx] = rp3[idx].first;
        rp_y[idx] = rp3[idx].second;
    }
    TGraph *gr1 = new TGraph(rp3.size(), rp_x, rp_y);
      gr1->SetMarkerColor(2);
      gr1->SetLineColor(2);
      gr1->SetMarkerSize(1.5);
      gr1->SetMarkerStyle(21);
    gr1->SetTitle("RP3");
    delete[] rp_x;
    delete[] rp_y;

    rp_x = new  Double_t [rp23.size()]; 
    rp_y = new Double_t [rp23.size()];  
    for(unsigned idx = 0; idx < rp23.size(); idx++){
        rp_x[idx] = rp23[idx].first;
        rp_y[idx] = rp23[idx].second;
    }
    TGraph *gr2 = new TGraph(rp23.size(), rp_x, rp_y);
      gr2->SetMarkerColor(4);
      gr2->SetLineColor(4);
      gr2->SetMarkerSize(1.5);
      gr2->SetMarkerStyle(21);
    gr2->SetTitle("RP23");
    delete[] rp_x;
    delete[] rp_y;

    rp_x = new  Double_t [rp103.size()]; 
    rp_y = new Double_t [rp103.size()]; 
    for(unsigned idx = 0; idx < rp103.size(); idx++){
        rp_x[idx] = rp103[idx].first;
        rp_y[idx] = rp103[idx].second;
    }
    TGraph *gr3 = new TGraph(rp103.size(), rp_x, rp_y);
      gr3->SetMarkerColor(8);
      gr3->SetLineColor(8);
      gr3->SetMarkerSize(1.5);
      gr3->SetMarkerStyle(21);
    gr3->SetTitle("RP103");
    delete[] rp_x;
    delete[] rp_y;

    rp_x = new  Double_t [rp123.size()]; 
    rp_y = new Double_t [rp123.size()]; 
    for(unsigned idx = 0; idx < rp123.size(); idx++){
        rp_x[idx] = rp123[idx].first;
        rp_y[idx] = rp123[idx].second;
    }
    TGraph *gr4 = new TGraph(rp23.size(), rp_x, rp_y);
      gr4->SetMarkerColor(6);
      gr4->SetLineColor(6);
      gr4->SetMarkerSize(1.5);
      gr4->SetMarkerStyle(21);
    gr4->SetTitle("RP123");
    delete[] rp_x;
    delete[] rp_y;

    mg->Add(gr1);
    mg->Add(gr2);
    mg->Add(gr3);
    mg->Add(gr4);
    mg->Draw("ALP");
    
    TLegend *legend = new TLegend(0.1, 0.1, 0.3, 0.3);
    legend->SetNColumns(2);
    legend->AddEntry(gr1,"RP3 values","lp");
    legend->AddEntry(gr2,"RP23 values","lp");
    legend->AddEntry(gr3,"RP103 values","lp");
    legend->AddEntry(gr4,"RP123 values","lp");

    legend->Draw();
    
    //canvas.BuildLegend();


    std::string fileName(this->m_imageFileName);
    canvas.SaveAs(fileName.c_str());

    return true;

  }
};

/************************************************
    Image plots
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

    TCanvas canvas("LHCOpticalFunctions payload information", "LHCOpticalFunctions payload information", 1000, 600);
    canvas.cd(1);
    TLatex t;
    t.SetTextSize(0.018);

    if (m_payload != nullptr) {
        std::string line;
        std::vector<std::string> lines;
        std::stringstream ss;
        float Xi [] = {0.05, 0.1, 0.15};
        
        std::ofstream file;
        file.open("try.txt", std::ios::app);
        
        file<<"RP: "<<rp;    
        
        for(auto& map_item: *m_payload){
            ss<<"\nangle: "<<map_item.first<<" ";
            file<<std::fixed<<std::setprecision(2)<<"\nangle: "<<map_item.first<<" ";
            for(auto& pot: map_item.second){
                if(CTPPSOptics::decodeRP(pot.first) == rp){
                    for(auto item: Xi){
                        ss<<"\nXi: "<<item<<"\n";
                        file<<"\nXi: "<<std::fixed<<std::setprecision(2)<<item<<"\n";
                        int fun = CTPPSOptics::Function::evx;
                        while(fun != CTPPSOptics::Function::nFunctions){
                            for(int i=0; i<4; i++){
                                ss<<" | ";
                                file<< "|";
                                for(int j=0; j<4; j++){
                                    //if((i==0 && j==2) || (i==1 && j==2) || (i==3 && j==0) || (i==4 && j==0)) ss << " 0 "<<"\t\t";
                                    
                                    ss<<std::fixed<<std::setprecision(10)<<pot.second.getFcnValues()[fun][item]<<"\t\t";
                                    file<<std::fixed<<std::setprecision(10)<<pot.second.getFcnValues()[fun][item]<<"\t\t\t";
                                    fun++;
                                    
                                }
                                ss<<" | \n";
                                file<<"|\n";
                            }
                        }
                    }
                }
            }
        }    
        while (getline(ss, line)) {
             lines.push_back(line);
         }


         int index = 0;
         for (float y = 0.98; index < int(lines.size()); y -= 0.02) {
             if (index < int(lines.size()))
             t.DrawLatex(0.02, y, lines[index++].c_str());
         }
        t.Draw();

        std::string fileName(this->m_imageFileName);
        canvas.SaveAs(fileName.c_str());

        return true;
        } else {
        return false;
        }


  }
};

#endif

