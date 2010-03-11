#include <TCut.h>
#include <TString.h>
#include <iostream>

class selectionCut
{  
    public:
    selectionCut(bool isMC, int sel, double jet=10, double jeta=2, double jdphi=2.14);
    ~selectionCut(){}
    
    TCut DJCut; 
    TCut TrkCut;
    TCut runCut;
    TCut evtSelection;
    TCut CutWOVtxCut;
    TCut vtxCut;
    TCut myCut;
    int nEtaBin;
    int nHitBin;
    int nVzBin;
    int VzRangeL;
    int VzRangeH;
    int selType;

    // run info
    int runNum;

    // dijet kinematics
    double jetEtMin;
    double jetEtaMax;
    double djDPhiMin;

    // variables
    TString NConeNP;
    TString AConeNP;
    TString NConeNPBg;
    TString AConeNPBg;
    
    // ana info
    TString AnaTag;
    TString SelTag;
};

selectionCut::selectionCut(bool isMC, int sel, double jet, double jeta, double jdphi) :
  selType(sel),
  jetEtMin(jet),
  jetEtaMax(jeta),
  djDPhiMin(jdphi)
{
   VzRangeL = -15; 
   VzRangeH = 15;
   vtxCut = Form("vz<%d&&vz>%d&&vtxdof>=5",VzRangeH,VzRangeL);

   // define selections here
   // base: vertex
   if (selType>=0)
     evtSelection = vtxCut;

   // jet kinematic cuts
   if (selType>=1) {
     evtSelection = evtSelection && Form("nljet>%.1f && aljet>%.1f && abs(nljeta)<%.1f && abs(aljeta)<%.1f",
	 jetEtMin,jetEtMin,jetEtaMax,jetEtaMax);
   }
   if (selType>=2) 
     evtSelection = evtSelection && Form("jdphi>%.2f",djDPhiMin);
   if (selType>=3) {
     evtSelection = evtSelection && Form("nljemf>0.01 && aljemf>0.01");
     TrkCut = ("ppt>0.3 && ppt<25 && trkHP==1 && trkNHits>=8");
     NConeNP = "Sum$("+TString(TrkCut)+"&&pndr<0.5)";
     AConeNP = "Sum$("+TString(TrkCut)+"&&padr<0.5)";
     NConeNPBg = "Sum$("+TString(TrkCut)+"&&pndrbg<0.5)";
     AConeNPBg = "Sum$("+TString(TrkCut)+"&&padrbg<0.5)";
   }
   // special cuts
   if (selType==4) {
     evtSelection = evtSelection && Form("nljemf<0.999 && aljemf<0.999");
   }
   if (selType==5) {
     evtSelection = evtSelection && (NConeNP+">0 && "+AConeNP+">0");
   }

   if (!isMC) {
     //evtSelection += Form("&&nLumi>=%d&&nLumi<=%d&&L1A[0]==1&&L1A[82]==1",nLumiL,nLumiH);
   }

   DJCut = evtSelection;
   cout <<TString(DJCut)<<endl;



   nEtaBin  = 10;
   nHitBin  = 14;
   nVzBin   = 20;
}
