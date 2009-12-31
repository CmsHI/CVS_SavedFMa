#include <TCut.h>
#include <TString.h>
#include <iostream>

class selectionCut
{  
    public:
    selectionCut(bool isMC, int nLumiL=0, int nLumiH=10000);
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
};

selectionCut::selectionCut(bool isMC,int nLumiL, int nLumiH)
{
   VzRangeL = -20;   
   VzRangeH = 20;
   vtxCut = Form("vz[1]<%d&&vz[1]>%d",VzRangeH,VzRangeL);
//   evtSelection      = ("L1T[40]>0&&L1T[36]!=1&&L1T[37]!=1&&L1T[38]!=1&&L1T[39]!=1");
   evtSelection      = ("L1T[34]>0&&L1T[36]!=1&&L1T[37]!=1&&L1T[38]!=1&&L1T[39]!=1");
//   evtSelection      = ("nHFp>1&&nHFn>1&&L1T[36]!=1&&L1T[37]!=1&&L1T[38]!=1&&L1T[39]!=1");
//   evtSelection      = ("L1T[36]!=1&&L1T[37]!=1&&L1T[38]!=1&&L1T[39]!=1");

   if (!isMC) evtSelection += Form("&&nLumi>=%d&&nLumi<=%d&&l1ABit[0]==1&&l1ABit[82]==1",nLumiL,nLumiH);

   CutWOVtxCut = TCut(evtSelection);
   myCut = vtxCut + "&&" + evtSelection;

   cout <<myCut<<endl;
   
   Cut = TCut(myCut);

   nEtaBin  = 12;
   nHitBin  = 14;
   nVzBin   = 20;
}
