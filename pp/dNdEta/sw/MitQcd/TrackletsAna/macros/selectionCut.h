#include <TCut.h>
#include <TString.h>
#include <iostream>

class selectionCut
{  
    public:
    selectionCut(bool isMC, int sel, int nLumiL=0, int nLumiH=10000);
    ~selectionCut(){}
    
    TCut Cut;  
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
};

selectionCut::selectionCut(bool isMC, int sel, int nLumiL, int nLumiH) :
  selType(sel)
{
   VzRangeL = -20;   
   VzRangeH = 20;
   vtxCut = Form("vz[1]<%d&&vz[1]>%d",VzRangeH,VzRangeL);

   // define selections here
   // basic: loosest, just veto on halo
   if (selType==0)
     evtSelection      = ("L1T[36]!=1&&L1T[37]!=1&&L1T[38]!=1&&L1T[39]!=1");
   // halo veto + bit 34
   else if (selType==1)
     evtSelection      = ("L1T[34]>0&&L1T[36]!=1&&L1T[37]!=1&&L1T[38]!=1&&L1T[39]!=1");
   // halo veto + bit 40
   else if (selType==2)
     evtSelection      = ("L1T[40]>0&&L1T[36]!=1&&L1T[37]!=1&&L1T[38]!=1&&L1T[39]!=1");
   // halo veto + bit 41
   else if (selType==3)
     evtSelection      = ("L1T[41]>0&&L1T[36]!=1&&L1T[37]!=1&&L1T[38]!=1&&L1T[39]!=1");
   // halo veto + Hf tower count
   else if (selType==4)
     evtSelection      = ("nHFp>1&&nHFn>1&&L1T[36]!=1&&L1T[37]!=1&&L1T[38]!=1&&L1T[39]!=1");

   if (!isMC) evtSelection += Form("&&nLumi>=%d&&nLumi<=%d&&L1A[0]==1&&L1A[82]==1",nLumiL,nLumiH);

   CutWOVtxCut = TCut(evtSelection);
   if (selType==0)
     myCut = TCut(evtSelection);
   else
     myCut = vtxCut + "&&" + evtSelection;

   //cout <<myCut<<endl;
   
   Cut = TCut(myCut);

   nEtaBin  = 12;
   nHitBin  = 14;
   nVzBin   = 20;
}
