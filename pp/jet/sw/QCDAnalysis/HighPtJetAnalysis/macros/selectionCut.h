#include <TCut.h>
#include <TString.h>
#include <iostream>

class selectionCut
{  
    public:
    selectionCut(bool isMC, int sel, int runnum=-1, int nLumiL=0, int nLumiH=10000);
    ~selectionCut(){}
    
    TCut Cut; 
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
    
    // ana info
    TString AnaTag;
    TString Tag;
};

selectionCut::selectionCut(bool isMC, int sel, int runnum, int nLumiL, int nLumiH) :
  selType(sel),
  runNum(runnum)
{
   VzRangeL = -15; 
   VzRangeH = 15;
   vtxCut = Form("eventVar.PVz<%d&&eventVar.PVz>%d&&eventVar.PVndof>5",VzRangeH,VzRangeL);

   // define selections here
   // base: vertex
   if (selType>=0)
     evtSelection = vtxCut;
   // basic: leading jet pt
   if (selType%10==1)
     evtSelection = evtSelection && TCut("jet1Var.corPt>15 && abs(jet1Var.etaD)<2.6");
   // with emf cut
   if (selType%10==2)
     evtSelection = evtSelection && TCut("jet1Var.corPt>15 && abs(jet1Var.etaD)<2.6 && jet1Var.emf>0.01");
   // with n90 cut
   if (selType%10==3)
     evtSelection = evtSelection && TCut("jet1Var.corPt>15 && abs(jet1Var.etaD)<2.6 && jet1Var.emf>0.01 && jet1Var.n90hits>2");
   // with hpd, rbx cut
   if (selType%10==4)
     evtSelection = evtSelection && TCut("jet1Var.corPt>15 && abs(jet1Var.etaD)<2.6 && jet1Var.emf>0.01 && jet1Var.n90hits>2 && jet1Var.fHPD<0.98 && jet1Var.fRBX<0.98");
   // all jet id
   if (selType%10==5)
     evtSelection = evtSelection && TCut("jet1Var.corPt>15 && abs(jet1Var.etaD)<2.6 && jet1Var.emf>0.01 && jet1Var.n90hits>2 && jet1Var.fHPD<0.98 && jet1Var.fRBX<0.98 && jet1Var.sigmaEta>0.01 && jet1Var.sigmaPhi>0.01");

   // with dijet requirement
   TCut dijet("abs(jet1Var.phi-jet2Var.phi)<4 && abs(jet1Var.phi-jet2Var.phi)>2");
   if (selType==11)
     evtSelection = evtSelection && dijet && "jet2Var.corPt>10 && abs(jet2Var.etaD)<2.6";
   if (selType==12)
     evtSelection = evtSelection && dijet && "jet2Var.corPt>10 && abs(jet2Var.etaD)<2.6 && jet2Var.emf>0.01";
   if (selType==13)
     evtSelection = evtSelection && dijet && "jet2Var.corPt>10 && abs(jet2Var.etaD)<2.6 && jet2Var.emf>0.01 && jet2Var.n90hits>2";

   if (!isMC) {
     //evtSelection += Form("&&nLumi>=%d&&nLumi<=%d&&L1A[0]==1&&L1A[82]==1",nLumiL,nLumiH);
   }

   Cut = evtSelection;
   cout <<TString(Cut)<<endl;

   nEtaBin  = 10;
   nHitBin  = 14;
   nVzBin   = 20;
}
