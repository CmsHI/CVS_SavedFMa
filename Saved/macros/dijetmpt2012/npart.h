#ifndef npart_h
#define npart_h
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

//---------------------------------------------------------------------
// Convinient Output Classes
class EvtSel {
public:
   int run,evt,nOccur,cBin;
   int nG,nJ,nT;
   bool trig,offlSel,noiseFilt,anaEvtSel;
   float vz,weight,npart,ncoll,sampleWeight;
   TString leaves;
   EvtSel(){
      leaves="run/I:evt:nOccur:cBin:nG:nJ:nT:trig/O:offlSel:noiseFilt:anaEvtSel:vz/F:weight:npart:ncoll:sampleWeight";
   }
};

static const int MAXTRK = 10000;

class GammaJet{
public:
   GammaJet() :
   photonEt(-99),photonEta(0),photonPhi(0),
   jetEt(-99),jetEta(0),jetPhi(0),
   deta(-99),dphi(-99), Aj(-99),
   sigmaIetaIeta(-99),
   isEle(false),
   nTrk(0) {
      leaves = "photonEt/F:photonRawEt:photonEta:photonPhi:jetEt:jetRawEt:jetEta:jetPhi:deta:dphi:Agj:hovere:sigmaIetaIeta:sumIsol"
      ":phoMatJetEt:phoMatJetEta:phoMatJetPhi"
      ":ltrkPt:ltrkEta:ltrkPhi:ltrkJetDr:jltrkPt:jltrkEta:jltrkPhi:jltrkJetDr:jlpfPt:jlpfEta:jlpfPhi:jlpfJetDr:jlpfId"
      ":refPhoPt:refPhoEta:refPhoPhi:refPhoFlavor:refJetEt:refJetEta:refJetPhi:refPartonPt:refPartonFlavor"
      ":isEle/O";
   }
   float photonEt,photonRawEt,photonEta,photonPhi;
   float jetEt,jetRawEt,jetEta,jetPhi;
   float deta,dphi,Aj;
   float hovere,sigmaIetaIeta,sumIsol;
   float phoMatJetEt,phoMatJetEta,phoMatJetPhi;
   float ltrkPt,ltrkEta,ltrkPhi,ltrkJetDr;
   float jltrkPt,jltrkEta,jltrkPhi,jltrkJetDr;
   float jlpfPt,jlpfEta,jlpfPhi,jlpfJetDr,jlpfId;
   float refPhoPt,refPhoEta,refPhoPhi,refPhoFlavor,refJetEt,refJetEta,refJetPhi,refPartonPt,refPartonFlavor;
   bool isEle;
   int nTrk;
   float trkPt[MAXTRK];
   float trkEta[MAXTRK];
   float trkPhi[MAXTRK];   
   float trkJetDr[MAXTRK];
   TString leaves;
   void clear() {
      photonEt=-99; photonEta=-99; photonPhi=-99;
      jetEt=-99; jetRawEt=-99; jetEta=-99; jetPhi=-99;
      deta=-99; dphi=-99; Aj=-99;
      sigmaIetaIeta=-99;
      phoMatJetEt=-99; phoMatJetEta=-99; phoMatJetPhi=-99;
      ltrkPt=-99; ltrkEta=-99; ltrkPhi=-99; ltrkJetDr=-99;
      jltrkPt=-99; jltrkEta=-99; jltrkPhi=-99; jltrkJetDr=-99;
      jlpfPt=-99; jlpfEta=-99; jlpfPhi=-99; jlpfJetDr=-99; jlpfId=-99;
      refPhoPt=-99; refPhoFlavor=-99; refJetEt=-99; refJetEta=-99; refJetPhi=-99; refPartonPt=-99; refPartonFlavor=-99;
      isEle=false;
      nTrk=0;
   }
};


void GetNPartBins(TString infname, const int nBin, float * npart, const float * m, float threshold1, int dataType)
{
   float npartValue[40];
   npartValue[0] = 393.633;
   npartValue[1] = 368.819;
   npartValue[2] = 343.073;
   npartValue[3] = 317.625;
   npartValue[4] = 292.932;
   npartValue[5] = 271.917;
   npartValue[6] = 249.851;
   npartValue[7] = 230.72;
   npartValue[8] = 212.465;
   npartValue[9] = 194.752;
   npartValue[10] = 178.571;
   npartValue[11] = 163.23;
   npartValue[12] = 149.187;
   npartValue[13] = 136.011;
   npartValue[14] = 123.414;
   npartValue[15] = 111.7;
   npartValue[16] = 100.831;
   npartValue[17] = 90.7831;
   npartValue[18] = 80.9823;
   npartValue[19] = 72.6236;
   npartValue[20] = 64.1508;
   npartValue[21] = 56.6284;
   npartValue[22] = 49.9984;
   npartValue[23] = 43.3034;
   npartValue[24] = 37.8437;
   npartValue[25] = 32.6659;
   npartValue[26] = 27.83;
   npartValue[27] = 23.7892;
   npartValue[28] = 20.1745;
   npartValue[29] = 16.8453;
   npartValue[30] = 14.0322;
   npartValue[31] = 11.602;
   npartValue[32] = 9.52528;
   npartValue[33] = 7.6984;
   npartValue[34] = 6.446;
   npartValue[35] = 4.96683;
   npartValue[36] = 4.23649;
   npartValue[37] = 3.50147;
   npartValue[38] = 3.16107;
   npartValue[39] = 2.7877;
   
   TH1D *hStat = new TH1D("hStat","",nBin,m);
   TH1D *hNpartSum = new TH1D("hNpartSum","",nBin,m);

   TFile *inf = TFile::Open(infname);
   TTree *nt =(TTree*)inf->FindObjectAny("tgj");
   EvtSel evt;
   GammaJet gj;
   nt->SetBranchAddress("evt",&evt.run);
   nt->SetBranchAddress("jet",&gj.photonEt);
   for (int i=0;i<nt->GetEntries();i++)
   {
      nt->GetEntry(i);
      if (i%50000==0) {
         if (gj.photonEt>0) cout <<i<<" / "<< nt->GetEntries() << " run: " << evt.run << " evt: " << evt.evt << " bin: " << evt.cBin << " gamma pt: " << gj.photonEt <<endl;
      }
      
      if (dataType!=0) { // data or pp
         if (!evt.anaEvtSel) continue;
      }
      else { //mc
         if (!evt.offlSel) continue;
      }

      if (gj.photonEt>threshold1) {
         hNpartSum->Fill(evt.cBin,npartValue[evt.cBin]);
         hStat->Fill(evt.cBin); 
      }
   }
   
   hNpartSum->Divide(hStat);
   
   for (int i=0;i<nBin;i++)
   {
      cout <<hNpartSum->GetBinContent(i+1)<<endl;
      npart[i]=hNpartSum->GetBinContent(i+1);
   }
   
   //done
   inf->Close();
}
#endif