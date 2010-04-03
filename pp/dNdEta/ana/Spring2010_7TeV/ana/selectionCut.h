#include <TCut.h>
#include <TString.h>
#include <iostream>

class selectionCut
{  
    public:
    selectionCut(bool isMC, int sel, int runnum=-1, int nLumiL=0, int nLumiH=10000);
    ~selectionCut(){}
    
    TCut trigger;
    TCut noHalo;
    TCut noMonster;
    TCut hfCoinc;

    TCut Cut; 
    TCut runCut;
    TString evtSelection;
    TCut CutWOVtxCut;
    TString vtxCut;
    TString myCut;
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
   vtxCut = Form("vz[1]<%d&&vz[1]>%d",VzRangeH,VzRangeL);
   runCut = Form("nRun==%d",runNum);

   // define selections here
   trigger = "L1A[124]";
   noHalo = "(!L1T[36] && !L1T[37] && !L1T[38] && !L1T[39])";
   noMonster = "(vtxqual>(0.0045*npxhits) || npxhits<150 || vtxqual>2)";
   hfCoinc = "nHFp>0&&nHFn>0";

   // basic: loosest, just veto on halo
   if (selType==0 || selType==5)
     evtSelection      = noHalo;
   // halo veto + bit 34
   else if (selType==1) {
     evtSelection      = noHalo && "L1T[34]";
     Tag	       = "Bit34";
   }
   // halo veto + bit 40
   else if (selType==2) {
     evtSelection      = noHalo && "L1T[40]";
     Tag	       = "Bit40";
   }
   // halo veto + bit 41
   else if (selType==3) {
     evtSelection      = noHalo && "L1T[41]";
     Tag	       = "Bit40";
   }
   // halo veto + Hf tower count
   else if (selType==4) {
     evtSelection      = noHalo && "L1T[34]" && hfCoinc;
     Tag            = "HF Coincidence";
   }
   // Diffractive only
   else if (selType==10) {
     evtSelection      = noHalo && "L1T[34]" && !hfCoinc;
     Tag            = "HF Anti-Coincidence";
   }

   if (!isMC) {
     //evtSelection += Form("&&nLumi>=%d&&nLumi<=%d&&L1A[0]==1&&L1A[82]==1",nLumiL,nLumiH);
     evtSelection = trigger && "L1T[0]" && noMonster && TCut(evtSelection);
     //evtSelection = evtSelection && runCut;
   }

   CutWOVtxCut = TCut(evtSelection);
   if (selType==0)
     myCut = TCut(evtSelection);
   else
     myCut = evtSelection + "&&" + vtxCut;

   //cout <<myCut<<endl;
   
   Cut = TCut(myCut);

   nEtaBin  = 12;
   nHitBin  = 14;
   nVzBin   = 20;
}
