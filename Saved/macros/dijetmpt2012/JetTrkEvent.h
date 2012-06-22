#ifndef JetTrkEvent_h
#define JetTrkEvent_h

static const int MAXTRK = 30000;
static const int NTRKQUAL = 2;
// static const int NTRKQUAL = 3;

// Convinient Output Classes
class EvtSel {
public:
   int run,evt,nOccur,cBin;
   int nG,nJ,nT;
   bool trig,offlSel,noiseFilt,anaEvtSel;
   float vz,weight,npart,ncoll,sampleWeight,evtPlane,pthat,samplePtHat;
   TString leaves;
   EvtSel(){
      leaves="run/I:evt:nOccur:cBin:nG:nJ:nT:trig/O:offlSel:noiseFilt:anaEvtSel:vz/F:weight:npart:ncoll:sampleWeight:evtPlane:pthat:samplePtHat";
   }
};

class DiJet{
public:
   DiJet() :
   pt1(-99),eta1(0),phi1(0),
   pt2(-99),eta2(0),phi2(0),
   deta(-99),dphi(-99), Aj(-99),
   nTrk(0),nJet(0) {
      leaves = "pt1/F:pt1raw:eta1:phi1:pt2:pt2raw:eta2:phi2:deta:dphi:Aj"
      ":ltrkPt:ltrkEta:ltrkPhi:ltrkJetDr:jltrkPt:jltrkEta:jltrkPhi:jltrkJetDr"
      ":jlpfPt:jlpfEta:jlpfPhi:jlpfJetDr:jlpfId:pfPhoPt"
      ":ref1pt:ref1eta:ref1phi:ref1partonpt:ref1partonflavor:ref2pt:ref2eta:ref2phi:ref2partonpt:ref2partonflavor"
      ":genjetpt1:genjeteta1:genjetphi1:genjetpt2:genjeteta2:genjetphi2";
   }
   float pt1,pt1raw,eta1,phi1,pt2,pt2raw,eta2,phi2,deta,dphi,Aj;
   float ltrkPt,ltrkEta,ltrkPhi,ltrkJetDr,jltrkPt,jltrkEta,jltrkPhi,jltrkJetDr;
   float jlpfPt,jlpfEta,jlpfPhi,jlpfJetDr,jlpfId,pfPhoPt;
   float ref1pt,ref1eta,ref1phi,ref1partonpt,ref1partonflavor,ref2pt,ref2eta,ref2phi,ref2partonpt,ref2partonflavor;
   float genjetpt1,genjeteta1,genjetphi1,genjetpt2,genjeteta2,genjetphi2;
   int nTrk;
   float trkPt[MAXTRK];
   float trkEta[MAXTRK];
   float trkPhi[MAXTRK];   
   float trkJetDr[MAXTRK];
   float trkWt[MAXTRK];
   float vtrkWt[MAXTRK][NTRKQUAL];
   float vtrkEff[MAXTRK][NTRKQUAL];
   float vtrkFak[MAXTRK][NTRKQUAL];
   float trkEff[MAXTRK];
   float trkFak[MAXTRK];
   float trkNHit[MAXTRK];
   float trkChi2Norm[MAXTRK];
   bool trkHP[MAXTRK];
   int trkAlgo[MAXTRK];
   bool vtrkQual[MAXTRK][NTRKQUAL];
   bool trkSel[MAXTRK];
   bool trkIsFake[MAXTRK];
   int nJet;
   float inclJetPt[MAXTRK];
   float inclJetEta[MAXTRK];
   float inclJetPhi[MAXTRK];   
   float inclJetRefPt[MAXTRK];
   float inclJetRefPartonPt[MAXTRK];
   float inclJetRefResp[MAXTRK];
   int nGenJet;
   float inclGenJetPt[MAXTRK];
   float inclGenJetEta[MAXTRK];
   float inclGenJetPhi[MAXTRK];   
   float inclGenJetResp[MAXTRK];
   int nPf;
   float pfPt[MAXTRK];
   float pfEta[MAXTRK];
   float pfPhi[MAXTRK];
   int pfId[MAXTRK];
   int nGenp;
   float genpPt[MAXTRK];
   float genpEta[MAXTRK];
   float genpPhi[MAXTRK];
   int genpCh[MAXTRK];
   int genpSube[MAXTRK];
   bool genpSel[MAXTRK];
   bool genpHasRec[MAXTRK];
   TString leaves;
   void clear() {
      pt1=-99; pt1raw=-99; eta1=-99; phi1=-99; pt2=-99; pt2raw=-99; eta2=-99; phi2=-99; deta=-99; dphi=-99; Aj=-99;
      ltrkPt=-99; ltrkEta=-99; ltrkPhi=-99; ltrkJetDr=-99;
      jltrkPt=-99; jltrkEta=-99; jltrkPhi=-99; jltrkJetDr=-99;
      jlpfPt=-99; jlpfEta=-99; jlpfPhi=-99; jlpfJetDr=-99; jlpfId=-99; pfPhoPt=0;
      ref1pt=-99; ref1eta=-99; ref1phi=-99; ref1partonpt=-99; ref1partonflavor=-99; ref2pt=-99; ref2eta=-99; ref2phi=-99; ref2partonpt=-99; ref2partonflavor;
      genjetpt1=-99; genjeteta1=-99; genjetphi1=-99; genjetpt2=-99; genjeteta2=-99; genjetphi2;
      nTrk=0; nJet=0; nGenJet=0; nPf=0; nGenp=0;
   }
   void clearParticles() {
      nTrk=0; nPf=0; nGenp=0;
   }
   void clearTracks() {
      nTrk=0; nPf=0;
   }
};

void JTSetBranchAddress(TTree * tgj, EvtSel & evt, DiJet & gj) {
   tgj->SetBranchAddress("evt",&evt.run);
   tgj->SetBranchAddress("jet",&gj.pt1);
   tgj->SetBranchAddress("nTrk",&gj.nTrk);
   tgj->SetBranchAddress("trkPt",gj.trkPt);
   tgj->SetBranchAddress("trkEta",gj.trkEta);
   tgj->SetBranchAddress("trkPhi",gj.trkPhi);

//    tgj->SetBranchAddress("trkJetDr",gj.trkJetDr,"trkJetDr[nTrk]/F");
   tgj->SetBranchAddress("trkWt",gj.trkWt);
   tgj->SetBranchAddress("vtrkWt",gj.vtrkWt);
   tgj->SetBranchAddress("vtrkEff",gj.vtrkEff);
   tgj->SetBranchAddress("vtrkFak",gj.vtrkFak);
   tgj->SetBranchAddress("trkEff",gj.trkEff);
   tgj->SetBranchAddress("trkFak",gj.trkFak);
   tgj->SetBranchAddress("trkNHit",gj.trkNHit);
   tgj->SetBranchAddress("trkChi2Norm",gj.trkChi2Norm);
   tgj->SetBranchAddress("trkHP",gj.trkHP);
   tgj->SetBranchAddress("trkAlgo",gj.trkAlgo);
   tgj->SetBranchAddress("vtrkQual",gj.vtrkQual);
   tgj->SetBranchAddress("trkIsFake",gj.trkIsFake);
   tgj->SetBranchAddress("nJet",&gj.nJet);
   tgj->SetBranchAddress("inclJetPt",gj.inclJetPt);
   tgj->SetBranchAddress("inclJetEta",gj.inclJetEta);
   tgj->SetBranchAddress("inclJetPhi",gj.inclJetPhi);
   tgj->SetBranchAddress("inclJetRefPt",gj.inclJetRefPt);
   tgj->SetBranchAddress("inclJetRefPartonPt",gj.inclJetRefPartonPt);
   tgj->SetBranchAddress("inclJetRefResp",gj.inclJetRefResp);
   tgj->SetBranchAddress("nGenJet",&gj.nGenJet);
   tgj->SetBranchAddress("inclGenJetPt",gj.inclGenJetPt);
   tgj->SetBranchAddress("inclGenJetEta",gj.inclGenJetEta);
   tgj->SetBranchAddress("inclGenJetPhi",gj.inclGenJetPhi);
   tgj->SetBranchAddress("inclGenJetResp",gj.inclGenJetResp);
   tgj->SetBranchAddress("nPf",&gj.nPf);
   tgj->SetBranchAddress("pfPt",gj.pfPt);
   tgj->SetBranchAddress("pfEta",gj.pfEta);
   tgj->SetBranchAddress("pfPhi",gj.pfPhi);
   tgj->SetBranchAddress("pfId",gj.pfId);
   tgj->SetBranchAddress("nGenp",&gj.nGenp);
   tgj->SetBranchAddress("genpPt",gj.genpPt);
   tgj->SetBranchAddress("genpEta",gj.genpEta);
   tgj->SetBranchAddress("genpPhi",gj.genpPhi);
//    tgj->SetBranchAddress("genpCh",gj.genpCh);
   tgj->SetBranchAddress("genpSube",gj.genpSube);
   tgj->SetBranchAddress("genpHasRec",gj.genpHasRec);
}
#endif