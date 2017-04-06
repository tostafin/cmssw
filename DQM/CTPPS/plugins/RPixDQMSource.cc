/****************************************************************************
*
* This is a part of CTPPSDQM software.
* Authors:
*   Jan Kaspar (jan.kaspar@gmail.com)
*   F.Ferro INFN Genova
*   Vladimir Popov (vladimir.popov@cern.ch)
*
****************************************************************************/

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/Common/interface/DetSetVector.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"
//#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/CTPPSDigi/interface/CTPPSPixelDigi.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelCluster.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelIndices.h"
//#include "DataFormats/CTPPSReco/interface/RPixCluster.h"
//#include "DataFormats/CTPPSDigi/interface/CTPPSTrackerDigi.h"

#include <string>

//-----------------------------------------------------------------------------
 
class RPixDQMSource: public DQMEDAnalyzer
{
 public:
   RPixDQMSource(const edm::ParameterSet& ps);
   virtual ~RPixDQMSource();
  
 protected:
   void dqmBeginRun(edm::Run const &, edm::EventSetup const &) override;
   void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;
   void analyze(edm::Event const& e, edm::EventSetup const& eSetup);
   void beginLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& eSetup);
   void endLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& eSetup);
   void endRun(edm::Run const& run, edm::EventSetup const& eSetup);

 private:
   unsigned int verbosity;

//  edm::EDGetTokenT<edm::DetSetVector<RPixDigi>> RPixDigiInputTagToken_;
  edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelDigi>> RPixDigiToken;
  edm::EDGetTokenT< edm::DetSetVector<CTPPSPixelCluster> > tokenCluster;
//  edm::EDGetTokenT< edm::DetSetVector<RPixCluster> > tokenCluster;

 #define NRPotsMAX 6
 #define NplaneMAX 6
 #define NROCsMAX 6
 const int hitMultMAX = 300;
 const int ClusMultMAX = 90;
 const int CluSizeMAX = 25;

 CTPPSPixelIndices *ctppsInd;
 int pixRowMAX = 160;  // defaultDetSizeInX, CMS Y-axis
 int pixColMAX = 156;  // defaultDetSizeInY, CMS X-axis
 int ROCSizeInX = pixRowMAX/2;  // ROC row size in pixels = 80
 int ROCSizeInY = pixColMAX/3;  // ROC col size in pixels = 52
 long int nEvents = 0;

 const uint32_t row_shift=0, row_mask=0x7FF;
 const uint32_t column_shift = 11, column_mask = 0x7FF;
 const uint32_t adc_shift = 22, adc_mask = 0x3FF;

  MonitorElement *hBX, *hBXshort;

  MonitorElement *h2RPotActive[2];

  MonitorElement *hp2HitsOcc[2][3];
  MonitorElement *h2HitsMultipl[2][3];
  MonitorElement *h2PlaneActive[2][3];
  MonitorElement *h2ClusMultipl[2][3];
  MonitorElement *h2CluSize[2][3];

  int	          RPindexValid[2*3*NRPotsMAX];
  MonitorElement *hRPotActivPlanes[2*3*NRPotsMAX];
  MonitorElement *hRPotActivBX[2*3*NRPotsMAX];
  MonitorElement *hRPotActivBXroc[2*3*NRPotsMAX];
  MonitorElement *hRPotActivROCs[2*3*NRPotsMAX];
  MonitorElement *hRPotActivROCsMax[2*3*NRPotsMAX];
  MonitorElement   *hHitsMult[2*3*NRPotsMAX][NplaneMAX];
  MonitorElement    *h2xyHits[2*3*NRPotsMAX][NplaneMAX];
  MonitorElement    *hp2xyADC[2*3*NRPotsMAX][NplaneMAX];
  MonitorElement *h2xyROCHits[2*3*NRPotsMAX*NplaneMAX][NROCsMAX];
  int		  HitsMultROC[2*3*NRPotsMAX*NplaneMAX][NROCsMAX];
  int           HitsMultPlane[2*3*NRPotsMAX][NplaneMAX];


  unsigned int rpStatusWord = 0x8000; // 220 fr_hr (stn2rp3)
  int RPstatus[3][NRPotsMAX]; // symmetric in both arms
  int StationStatus[3]; // symmetric in both arms
  const int IndexNotValid = 0;

  int getRPindex(int arm, int station, int rp) {
	if(arm<0 || station<0 || rp<0) return(IndexNotValid);
	if(arm>1 || station>2 || rp>=NRPotsMAX) return(IndexNotValid);
	int rc = (arm*3+station)*NRPotsMAX + rp;
	return(rc);
  }

  int getPlaneIndex(int arm, int station, int rp, int plane) {
    if(plane<0 || plane>=NplaneMAX) return(IndexNotValid);
    int rc = getRPindex(arm, station, rp);
    if(rc == IndexNotValid) return(IndexNotValid);
    return(rc*NplaneMAX + plane);
  }

  int prIndex(int rp, int plane) {return(rp*NplaneMAX + plane);}
  int getDet(int id) { return (id>>28)&0xF; }
  int getPixPlane(int id) { return ((id>>16)&0x7); }
  int getSubdet(int id) { return ((id>>25)&0x7); }
  int getHitRow(uint32_t pkdata) {return (pkdata >> row_shift) & row_mask;}
 int getHitColumn(uint32_t pkdata) const  
	{return (pkdata >> column_shift) & column_mask;}
 unsigned short getHitADC(uint32_t pkdata) const 
	{return (pkdata >> adc_shift) & adc_mask;}

 int multHits, multClus, cluSizeMaxData; // tuning

};

//----------------------------------------------------------------------------------

using namespace std;
using namespace edm;

//-------------------------------------------------------------------------------

RPixDQMSource::RPixDQMSource(const edm::ParameterSet& ps) :
  verbosity(ps.getUntrackedParameter<unsigned int>("verbosity", 0)),
  rpStatusWord(ps.getUntrackedParameter<unsigned int>("RPStatusWord",0x8000))
{
 RPixDigiToken = consumes<DetSetVector<CTPPSPixelDigi> >(ps.getParameter<edm::InputTag>("tagRPixDigi"));
 tokenCluster= 
consumes<DetSetVector<CTPPSPixelCluster>>(ps.getParameter<edm::InputTag>("tagRPixCluster"));
//consumes<DetSetVector<RPixCluster>>(ps.getParameter<edm::InputTag>("tagRPixCluster"));

}

//----------------------------------------------------------------------------------

RPixDQMSource::~RPixDQMSource()
{
}

//--------------------------------------------------------------------------

void RPixDQMSource::dqmBeginRun(edm::Run const &run, edm::EventSetup const &)
{
 nEvents = 0;

 unsigned int rpSts = rpStatusWord<<1;
 for(int stn=0; stn<3; stn++) {
   int stns = 0;
   for(int rp=0; rp<NRPotsMAX; rp++) {
     rpSts = (rpSts >> 1); RPstatus[stn][rp] = rpSts&1;
     if(RPstatus[stn][rp] > 0) stns = 1;
   }
   StationStatus[stn]=stns;
 }
 
 for(int ind=0; ind<2*3*NRPotsMAX; ind++) RPindexValid[ind] = 0;

 multHits = multClus = cluSizeMaxData = -1;
}

//-------------------------------------------------------------------------------------

void RPixDQMSource::bookHistograms(DQMStore::IBooker &ibooker, edm::Run const &, edm::EventSetup const &)
{
  ctppsInd = new CTPPSPixelIndices();
  pixRowMAX = ctppsInd->getDefaultRowDetSize();
  pixColMAX = ctppsInd->getDefaultColDetSize();

  ROCSizeInX = pixRowMAX/2;  // ROC row size in pixels = 80
  ROCSizeInY = pixColMAX/3;
  if(verbosity) printf("bookHistograms: sizeX/Y=%d/%d ROCSizeInX/Y=%d/%d\n",
	pixColMAX,pixRowMAX,ROCSizeInX,ROCSizeInY);

  ibooker.cd();
  ibooker.setCurrentFolder("CTPPS/TrackingPixel");
  char s[50];
  hBX = ibooker.book1D("events per BX", "ctpps_pixel;Event.BX", 4002, -1.5, 4000. +0.5);
  hBXshort = ibooker.book1D("events per BX(short)", "ctpps_pixel;Event.BX", 102, -1.5, 100. + 0.5);

 for(int arm=0; arm<2; arm++) {
   CTPPSDetId ID(CTPPSDetId::sdTrackingPixel,arm,0);
   string sd, armTitle;
   CTPPSDetId(ID.getArmId()).armName(sd, CTPPSDetId::nPath);
   CTPPSDetId(ID.getArmId()).armName(armTitle, CTPPSDetId::nFull);

   ibooker.setCurrentFolder(sd);

   string  st = "RPots activity";
//  h2RPotActive[arm] = ibooker.book2DD(st,st+": "+armTitle, NRPotsMAX,0,NRPotsMAX,3,0,3);
  h2RPotActive[arm] = ibooker.book2DD(st,st+": "+armTitle, 2,2,4, 3,0,3);
  TH2D *h6 = h2RPotActive[arm]->getTH2D();
   h6->SetOption("colz");
   TAxis *ya = h6->GetYaxis();
   TAxis *xa = h6->GetXaxis();

 for(int stn=0; stn<3; stn++) {
   ID.setStation(stn);
   string stnd, stnTitle;
   CTPPSDetId(ID.getStationId()).stationName(stnTitle, CTPPSDetId::nShort);
   char *cstr = new char [stnTitle.length()+1];
   strcpy(cstr, stnTitle.c_str());
   ya->SetBinLabel(stn+1, cstr);
   if(StationStatus[stn]==0) continue;

   CTPPSDetId(ID.getStationId()).stationName(stnd, CTPPSDetId::nPath);
   CTPPSDetId(ID.getStationId()).stationName(stnTitle, CTPPSDetId::nFull);

   ibooker.setCurrentFolder(stnd);

   string st = "planes activity";
   string st2 = ": " + stnTitle;

  h2PlaneActive[arm][stn] = ibooker.book2DD(st,st+st2+";Plane #",
		NplaneMAX,0,NplaneMAX, 2, 2,4);
//		NplaneMAX,0,NplaneMAX, NRPotsMAX, 0,NRPotsMAX);
  TH2D *h = h2PlaneActive[arm][stn]->getTH2D();
   h->SetOption("colz");
   TAxis *yah = h->GetYaxis();
   
   st = "hit average multiplicity in planes";

   hp2HitsOcc[arm][stn]= ibooker.bookProfile2D(st,st+st2+";Plane #",
     NplaneMAX, 0, NplaneMAX, 2,2,4,0,20000);
//     NplaneMAX, 0, NplaneMAX, NRPotsMAX,0,NRPotsMAX,0,20000); //MAXSPAN^2
   TProfile2D *h2 = hp2HitsOcc[arm][stn]->getTProfile2D();
   h2->SetOption("textcolz");
   TAxis *yah2 = h2->GetYaxis();

   int xmin = NplaneMAX*2;
   int xmax = (int)NplaneMAX*4;
   int multbins = xmax - xmin;

   st = "hit multiplicity in planes";
   string st3 = ";PlaneIndex(=rpot*PlaneMAX + plane)";
   h2HitsMultipl[arm][stn]= ibooker.book2DD(st,st+st2+st3+";multiplicity",
	multbins,xmin,xmax,hitMultMAX,0,hitMultMAX);
   h2HitsMultipl[arm][stn]->getTH2D()->SetOption("colz");

   st = "cluster multiplicity in planes";
   h2ClusMultipl[arm][stn] = ibooker.book2DD(st,st+st2+st3+";multiplicity",
	multbins,xmin,xmax, ClusMultMAX,0,ClusMultMAX);
   h2ClusMultipl[arm][stn]->getTH2D()->SetOption("colz");

   st = "cluster size in planes";
  h2CluSize[arm][stn] = ibooker.book2D(st,st+st2+st3+";Cluster size",
	multbins,xmin,xmax, CluSizeMAX,0,CluSizeMAX);
   h2CluSize[arm][stn]->getTH2F()->SetOption("colz");

//--------- Hits ---
   int pixBinW = 4;
   for(int rp=0; rp<NRPotsMAX; rp++) {
    ID.setRP(rp);
    string rpd, rpTitle;
    CTPPSDetId(ID.getRPId()).rpName(rpTitle, CTPPSDetId::nShort);
    char *cstr2 = new char [rpTitle.length()+1];
    strcpy(cstr2, rpTitle.c_str());   
    if(rp==2 || rp==3) { xa->SetBinLabel(rp-1, cstr2); // h6
			yah->SetBinLabel(rp-1, cstr2); // h
		       yah2->SetBinLabel(rp-1, cstr2); } //h2

    if(RPstatus[stn][rp]==0) continue;
    int indexP = getRPindex(arm,stn,rp);
    RPindexValid[indexP] = 1;

    CTPPSDetId(ID.getRPId()).rpName(rpTitle, CTPPSDetId::nFull);
    CTPPSDetId(ID.getRPId()).rpName(rpd, CTPPSDetId::nPath);

    ibooker.setCurrentFolder(rpd);

    hRPotActivPlanes[indexP] = 
     ibooker.book1D("number of fired planes per event", rpTitle+";nPlanes",
	 NplaneMAX, -0.5, NplaneMAX+0.5);
    hRPotActivBX[indexP] = 
  ibooker.book1D("5 fired planes per BX", rpTitle+";Event.BX", 4002, -1.5, 4000.+0.5);
    hRPotActivBXroc[indexP] = 
     ibooker.book1D("4 fired ROCs per BX", rpTitle+";Event.BX", 4002, -1.5, 4000.+0.5);

    hRPotActivROCs[indexP] = ibooker.book2D("number of fired aligned_ROCs per event",
	rpTitle+";ROC ID;number of fired ROCs", NROCsMAX,-0.5,NROCsMAX-0.5, 7,-0.5,6.5);
    hRPotActivROCs[indexP]->getTH2F()->SetOption("colz");

    hRPotActivROCsMax[indexP]= ibooker.book2D("max number of fired aligned_ROCs per event",
	rpTitle+";ROC ID;number of fired ROCs", NROCsMAX,-0.5,NROCsMAX-0.5, 7,-0.5,6.5);
    hRPotActivROCsMax[indexP]->getTH2F()->SetOption("colz");

//    hRPotActivROCsMax[indexP] = 
//     ibooker.book1D("number of fired aligned_ROCs per event (max)", rpTitle+";nROCs", 
//	NROCsMAX+1,-0.5,NROCsMAX+0.5);

    int nbins = pixRowMAX/pixBinW;

    for(int p=0; p<NplaneMAX; p++) {
       sprintf(s,"plane_%d",p);
       string pd = rpd+"/"+string(s);
       ibooker.setCurrentFolder(pd);
       string st1 = ": "+rpTitle+"_"+string(s);
       st = "hits position";
       h2xyHits[indexP][p]=ibooker.book2DD(st,st1+";pix col;pix row", 
	nbins,0,pixRowMAX,nbins,0,pixRowMAX);
	h2xyHits[indexP][p]->getTH2D()->SetOption("colz");

	st = "adc average value";
//	hp2xyADC[indexP][p]=ibooker.bookProfile2D(st,st+st1,
	hp2xyADC[indexP][p]=ibooker.bookProfile2D(st,st1+";pix col;pix row",
	  nbins,0,pixRowMAX,nbins,0,pixRowMAX,100,0,1.e10,"");
	hp2xyADC[indexP][p]->getTProfile2D()->SetOption("colz");

     st = "hits multiplicity";
     hHitsMult[indexP][p]=ibooker.book1DD(st,st1+";number of hits;N / 1 hit",
	hitMultMAX,0,hitMultMAX);

     ibooker.setCurrentFolder(pd + "/ROCs");
     int index = getPlaneIndex(arm,stn,rp,p);

     for(int roc=0; roc<6; roc++) {
      sprintf(s,"hits in ROC_%d",roc);
      string st = string(s);

      h2xyROCHits[index][roc]=	ibooker.book2DD(st,st+st1+";pix row;pix col",
		ROCSizeInX,0,ROCSizeInX,ROCSizeInY,0,ROCSizeInY);
       h2xyROCHits[index][roc]->getTH2D()->SetOption("colz");
     }
    }// end of for(int p=0; p<NplaneMAX;..

   } // end for(int rp=0; rp<NRPotsMAX;...
  } // end of for(int stn=0; stn<2; stn++
  } //end of for(int arm=0; arm<2;...

 return;
}

//-------------------------------------------------------------------------------
void RPixDQMSource::beginLuminosityBlock(edm::LuminosityBlock const& lumiSeg, 
                                            edm::EventSetup const& context) 
{
}
//---------------------------------------------------------------------------------

void RPixDQMSource::analyze(edm::Event const& event, edm::EventSetup const& eventSetup)
{
  ++nEvents;
 for(int ind=0; ind<2*3*NRPotsMAX; ind++) 
   for(int p=0; p<NplaneMAX; p++) HitsMultPlane[ind][p] = 0;

 for(int ind=0; ind<2*3*NRPotsMAX*NplaneMAX; ind++) 
   for(int rp=0; rp<NROCsMAX; rp++) HitsMultROC[ind][rp] = 0;

//  if(verbosity>2) printf("irun=%d\n",event.id().run());

//  Handle< DetSetVector<RPixDigi> > pixDigi;
  Handle< DetSetVector<CTPPSPixelDigi> > pixDigi;
  event.getByToken(RPixDigiToken, pixDigi);

//  Handle< DetSetVector<RPixCluster> > Clus;
  Handle< DetSetVector<CTPPSPixelCluster> > Clus;
  event.getByToken(tokenCluster, Clus);

  bool valid = true;
  valid &= pixDigi.isValid();
  valid &= Clus.isValid();
//  valid &= hits.isValid();

  hBX->Fill(event.bunchCrossing());
  hBXshort->Fill(event.bunchCrossing());

if(pixDigi.isValid())
  for(const auto &ds_digi : *pixDigi)
  {
   int idet = getDet(ds_digi.id);
   if(idet != DetId::VeryForward) {
     if(verbosity>1) printf("not CTPPS: ds_digi.id=0x%x\n",ds_digi.id);
     continue;
   }
//   int subdet = getSubdet(ds_digi.id);

   int plane = getPixPlane(ds_digi.id);
   int arm = CTPPSDetId(ds_digi.id).arm()&0x1;
   int station = CTPPSDetId(ds_digi.id).station();
   int rpot = CTPPSDetId(ds_digi.id).rp();

   h2RPotActive[arm]->Fill(rpot,station);

 if(StationStatus[station]) {

   hp2HitsOcc[arm][station]->Fill(plane,rpot,(int)ds_digi.data.size());
   h2HitsMultipl[arm][station]->Fill(prIndex(rpot,plane),ds_digi.data.size());
   h2PlaneActive[arm][station]->Fill(plane,rpot);

   if(RPstatus[station][rpot]) {
    int index = getRPindex(arm,station,rpot);
    HitsMultPlane[index][plane] += ds_digi.data.size();
    if(RPindexValid[index])
      hHitsMult[index][plane]->Fill(ds_digi.data.size());
    int rocHistIndex = getPlaneIndex(arm,station,rpot,plane);

    for(DetSet<CTPPSPixelDigi>::const_iterator dit = ds_digi.begin();
	 dit != ds_digi.end(); ++dit) {
//      int pkdata = dit->packedData();
      int row = getHitRow(dit->packedData());
      int col = getHitColumn(dit->packedData());
      int adc = getHitADC(dit->packedData());

      if(RPindexValid[index]) {
        h2xyHits[index][plane]->Fill(col,row);
        hp2xyADC[index][plane]->Fill(col,row,adc);
        int colROC, rowROC;
        int trocId;
        if(!ctppsInd->transformToROC(col,row, trocId, colROC, rowROC)) {
	 if(trocId>=0 && trocId<NROCsMAX) {
	   h2xyROCHits[rocHistIndex][trocId]->Fill(rowROC,colROC);
	   ++HitsMultROC[rocHistIndex][trocId];
	 }
        }
      } //end if(RPindexValid[index]) {
     }
   } // end  if(RPstatus[station][rpot]) {

   if(int(ds_digi.data.size()) > multHits) multHits = ds_digi.data.size();
  } // end  if(StationStatus[station]) {
 } // end for(const auto &ds_digi : *pixDigi)

 for(int arm=0; arm<2; arm++) for(int stn=0; stn<3; stn++) 
  for(int rp=0; rp<NRPotsMAX; rp++) {
    int index = getRPindex(arm,stn,rp);
    if(RPindexValid[index]==0) continue;
    int np = 0; 
    for(int p=0; p<NplaneMAX; p++) if(HitsMultPlane[index][p]>0) np++;
    hRPotActivPlanes[index]->Fill(np);
    if(np>5) hRPotActivBX[index]->Fill(event.bunchCrossing());

    int rocf[NRPotsMAX];
    for(int r=0; r<NROCsMAX; r++) rocf[r]=0;
    for(int p=0; p<NplaneMAX; p++) {
      int indp = getPlaneIndex(arm,stn,rp,p);
      for(int r=0; r<NROCsMAX; r++) if(HitsMultROC[indp][r] > 0) ++rocf[r];
    }
    int max = 0, rmax=-2;
    for(int r=0; r<NROCsMAX; r++) if(max < rocf[r]) 
    { max = rocf[r]; rmax = r; }
    for(int r=0; r<NROCsMAX; r++) hRPotActivROCs[index]->Fill(r,rocf[r]);
    hRPotActivROCsMax[index]->Fill(rmax,max);
    if(max > 4) hRPotActivBXroc[index]->Fill(event.bunchCrossing());
  }

 if(Clus.isValid())
  for(const auto &ds : *Clus)
  {
//   int idet = getDet(ds.id);
//   int subdet = getSubdet(ds.id);
   int station = CTPPSDetId(ds.id).station();

   int plane = getPixPlane(ds.id);
   int arm = CTPPSDetId(ds.id).arm()&0x1;
   int rpot = CTPPSDetId(ds.id).rp();

   if(StationStatus[station]) {
    h2ClusMultipl[arm][station]->Fill(prIndex(rpot,plane),ds.data.size());
    if(multClus < (int)ds.data.size()) multClus = ds.data.size();
    for (const auto &p : ds) {
//     int isize = p.size();
      h2CluSize[arm][station]->Fill(prIndex(rpot,plane),p.size());
      if(cluSizeMaxData < p.size()) cluSizeMaxData = p.size(); //tuning
    }
   } // end if(StationStatus[station]) 
  } // end 'for(const auto &ds : *Clus)'

  if((nEvents % 50)) return;
  if(verbosity) printf("RPixAnalyser::analyze event= %d\n",(int)nEvents);
}

//--------------------------------------------------------------
void RPixDQMSource::endLuminosityBlock(edm::LuminosityBlock const& lumiSeg, edm::EventSetup const& eSetup) 
{
}

//-----------------------------------------------------------------------------
void RPixDQMSource::endRun(edm::Run const& run, edm::EventSetup const& eSetup)
{
 if(!verbosity) return; 
  printf("end of Run %d: %d events\n",run.run(),(int)nEvents);
  printf("mult Hits/Clus: %d / %d\n",multHits,multClus);
  printf("cluSizeMaxData=%d\n",cluSizeMaxData);
  printf("---------- end of Run -------\n");
}


//---------------------------------------------------------------------------
DEFINE_FWK_MODULE(RPixDQMSource);

