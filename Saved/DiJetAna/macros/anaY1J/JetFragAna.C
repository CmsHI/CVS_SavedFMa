#include <iostream>
#include "TF1.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TCut.h>
#include "JetFragAna.h"
using namespace std;

JetFragAna::JetFragAna(TTree *tree,TString tag,Int_t doMC) :
  cut(tag,doMC),
  doEvtSel_(true),
  doEtaCorr_(false),
  doJetOnly_(true),
  anaGenpType_(0),
  anaJets_(2),
  refJets_(2)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      cout << "bad tree" << endl;
      return;
   }
   Init(tree);

   // event cut for centrality reweighting:
   evtCut = "nljet>120&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>2./3*TMath::Pi()";

   // ntuples
   ntjt = new TNtuple("ntjt","jet-trk nt","nljet:nljetacorr:aljet:aljetacorr:"
                                          "metx:metx0:metx1:metx2:metx3:metx4:metx5:"
                                          "mety:mety0:mety1:mety2:mety3:mety4:mety5:"
                                          "maskEvt:cent:jdphi:weight");
   ntjt->SetAlias("et1","nljet");
   ntjt->SetAlias("et2","aljet");
   ntjt->SetAlias("Aj","(nljet-aljet)/(nljet+aljet)");

   // new bining, use alias to keep flexibility
   ntjt->SetAlias("metxMerged0","metx0+metx1");
   ntjt->SetAlias("metxMerged1","metx2");
   ntjt->SetAlias("metxMerged2","metx3+metx4");

   ntjt->SetAlias("metyMerged0","mety0+mety1");
   ntjt->SetAlias("metyMerged1","mety2");
   ntjt->SetAlias("metyMerged2","mety3+mety4");

   // Histograms
   const Int_t numDRBins = 20;
   Double_t dRBins[numDRBins+1];
   for (int i=0;i<numDRBins+1;i++)   { dRBins[i] = 2./((double)numDRBins)*i; }
   const Int_t numPtBins = 6;
   Double_t ptBins[numPtBins+1]={0.5,1.0,1.5,4,8,20,180}; // v10,11,12
   const Int_t numDPhiBins = 20;
   Double_t dPhiBins[numDPhiBins+1];
   for (int i=0;i<numDPhiBins+1;i++)   { dPhiBins[i] = PI/2./((double)numDPhiBins)*i; }

   // -- 2D hists --
   hPtPNDR = new TH2D("hPtPNDR","",numPtBins,ptBins,numDRBins,dRBins);
   hPtPNDR->Sumw2();
   hPtPADR = new TH2D("hPtPADR","",numPtBins,ptBins,numDRBins,dRBins);
   hPtPADR->Sumw2();
   hPtPNDRBg = new TH2D("hPtPNDRBg","",numPtBins,ptBins,numDRBins,dRBins);
   hPtPNDRBg->Sumw2();
   hPtPADRBg = new TH2D("hPtPADRBg","",numPtBins,ptBins,numDRBins,dRBins);
   hPtPADRBg->Sumw2();

   // -- 1D hists --
   //TH1::SetDefaultSumw2();
   // evt
   // centrality weight
   hCentralityWeight_ = new TH1D("hCentralityWeight","",40,0,100);

   // jet
   hJDPhi = new TH1D("hJDPhi","",50,0,PI);
   hJDPhi->Sumw2();
   hJEtNr = new TH1D("hJEtNr","",cut.numJEtBins,cut.hisJEtMin,cut.hisJEtMax);
   hJEtNr->Sumw2();
   hJEtAw = new TH1D("hJEtAw","",cut.numJEtBins,cut.hisJEtMin,cut.hisJEtMax);
   hJEtAw->Sumw2();
   hAj = new TH1D("hAj","",20,0,1);
   hAj->Sumw2();
   hJEtaNr = new TH1D("hJEtaNr","",50,-3,3);
   hJEtaNr->Sumw2();
   hJEtaAw = new TH1D("hJEtaAw","",50,-3,3);
   hJEtaAw->Sumw2();
   hJDEta = new TH1D("hJDEta","",50,-6,6);
   hJDEta->Sumw2();
   hRefJEtNr = new TH1D("hRefJEtNr","",cut.numJEtBins,cut.hisJEtMin,cut.hisJEtMax);
   hRefJEtNr->Sumw2();
   hRefJEtAw = new TH1D("hRefJEtAw","",cut.numJEtBins,cut.hisJEtMin,cut.hisJEtMax);
   hRefJEtAw->Sumw2();
   // cone
   hNrCPt = new TH1D("hNrCPt","",cut.numJEtBins*2,cut.hisJEtMin,cut.hisJEtMax);
   hNrCPt->Sumw2();
   hNrCPtBg = new TH1D("hNrCPtBg","",cut.numJEtBins*2,cut.hisJEtMin,cut.hisJEtMax);
   hNrCPtBg->Sumw2();
   hNrCPtBgSub = new TH1D("hNrCPtBgSub","",cut.numJEtBins*2,-1*cut.hisJEtMax,cut.hisJEtMax);
   hNrCPtBgSub->Sumw2();
   hAwCPt = new TH1D("hAwCPt","",cut.numJEtBins*2,cut.hisJEtMin,cut.hisJEtMax);
   hAwCPt->Sumw2();
   hAwCPtBg = new TH1D("hAwCPtBg","",cut.numJEtBins*2,cut.hisJEtMin,cut.hisJEtMax);
   hAwCPtBg->Sumw2();
   hAwCPtBgSub = new TH1D("hAwCPtBgSub","",cut.numJEtBins*2,-1*cut.hisJEtMax,cut.hisJEtMax);
   hAwCPtBgSub->Sumw2();

   // trk
   const Int_t numPPtBins=18;
   Float_t pptBins[numPPtBins+1] = {0.0,0.2,1,2,3,4,6,8,10,14,18,22,26,30,40,50,60,70,80};
   hNrCPPt = new TH1D("hNrCPPt","",numPPtBins,pptBins);
   hNrCPPt->Sumw2();
   hNrCPPtBg = new TH1D("hNrCPPtBg","",numPPtBins,pptBins);
   hNrCPPtBg->Sumw2();
   hNrCPPtBgSub = new TH1D("hNrCPPtBgSub","",numPPtBins,pptBins);
   hNrCPPtBgSub->Sumw2();
   hAwCPPt = new TH1D("hAwCPPt","",numPPtBins,pptBins);
   hAwCPPt->Sumw2();
   hAwCPPtBg = new TH1D("hAwCPPtBg","",numPPtBins,pptBins);
   hAwCPPtBg->Sumw2();
   hAwCPPtBgSub = new TH1D("hAwCPPtBgSub","",numPPtBins,pptBins);
   hAwCPPtBgSub->Sumw2();

   hPNDR = new TH1D("hPNDR","",numDRBins,dRBins);
   hPNDR->Sumw2();
   hPADR = new TH1D("hPADR","",numDRBins,dRBins);
   hPADR->Sumw2();
   hPNDRBg = new TH1D("hPNDRBg","",numDRBins,dRBins);
   hPNDRBg->Sumw2();
   hPADRBg = new TH1D("hPADRBg","",numDRBins,dRBins);
   hPADRBg->Sumw2();
}

JetFragAna::~JetFragAna()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Long64_t JetFragAna::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void JetFragAna::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   hlt = 0;
   ljcnp = 0;
   ljcnpbg = 0;
   ljcpt = 0;
   ljcpt2 = 0;
   ljcptr = 0;
   ljcptbg = 0;
   ljcpt2bg = 0;
   ljcptrbg = 0;
   lppt = 0;
   lpjdr = 0;
   lgppt = 0;
   lgpch = 0;
   lgppid = 0;

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("evt", &evt, &b_evt);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("hlt", &hlt, &b_hlt);
   fChain->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
   fChain->SetBranchAddress("vtxntrks", &vtxntrks, &b_vtxntrks);
   fChain->SetBranchAddress("vtxndof", &vtxndof, &b_vtxndof);
   fChain->SetBranchAddress("vtxchi2", &vtxchi2, &b_vtxchi2);
   fChain->SetBranchAddress("vz", &vz, &b_vz);
   fChain->SetBranchAddress("njets", &njets, &b_njets);
   fChain->SetBranchAddress("ntrks", &ntrks, &b_ntrks);
   fChain->SetBranchAddress("b", &b, &b_b);
   fChain->SetBranchAddress("npart", &npart, &b_npart);
   fChain->SetBranchAddress("ncoll", &ncoll, &b_ncoll);
   fChain->SetBranchAddress("cent", &cent, &b_cent);
   fChain->SetBranchAddress("djmass", &djmass, &b_djmass);
   fChain->SetBranchAddress("rdjmass", &rdjmass, &b_rdjmass);
   fChain->SetBranchAddress("cmeta", &cmeta, &b_cmeta);
   fChain->SetBranchAddress("nlrjid", &nlrjid, &b_nlrjid);
   fChain->SetBranchAddress("nlrjstat", &nlrjstat, &b_nlrjstat);
   fChain->SetBranchAddress("nlrjet", &nlrjet, &b_nlrjet);
   fChain->SetBranchAddress("nlrjeta", &nlrjeta, &b_nlrjeta);
   fChain->SetBranchAddress("nlrjphi", &nlrjphi, &b_nlrjphi);
   fChain->SetBranchAddress("alrjid", &alrjid, &b_alrjid);
   fChain->SetBranchAddress("alrjstat", &alrjstat, &b_alrjstat);
   fChain->SetBranchAddress("alrjet", &alrjet, &b_alrjet);
   fChain->SetBranchAddress("alrjeta", &alrjeta, &b_alrjeta);
   fChain->SetBranchAddress("alrjphi", &alrjphi, &b_alrjphi);
   fChain->SetBranchAddress("rjdphi", &rjdphi, &b_rjdphi);
   fChain->SetBranchAddress("nljet", &nljet, &b_nljet);
   fChain->SetBranchAddress("nljrawet", &nljrawet, &b_nljrawet);
   fChain->SetBranchAddress("nljeta", &nljeta, &b_nljeta);
   fChain->SetBranchAddress("nljphi", &nljphi, &b_nljphi);
   fChain->SetBranchAddress("nljarea", &nljarea, &b_nljarea);
   fChain->SetBranchAddress("aljet", &aljet, &b_aljet);
   fChain->SetBranchAddress("aljrawet", &aljrawet, &b_aljrawet);
   fChain->SetBranchAddress("aljeta", &aljeta, &b_aljeta);
   fChain->SetBranchAddress("aljphi", &aljphi, &b_aljphi);
   fChain->SetBranchAddress("aljarea", &aljarea, &b_aljarea);
   fChain->SetBranchAddress("nljemf", &nljemf, &b_nljemf);
   fChain->SetBranchAddress("nljN90hits", &nljN90hits, &b_nljN90hits);
   fChain->SetBranchAddress("nljfhpd", &nljfhpd, &b_nljfhpd);
   fChain->SetBranchAddress("aljemf", &aljemf, &b_aljemf);
   fChain->SetBranchAddress("aljN90hits", &aljN90hits, &b_aljN90hits);
   fChain->SetBranchAddress("aljfhpd", &aljfhpd, &b_aljfhpd);
   fChain->SetBranchAddress("jdphi", &jdphi, &b_jdphi);
   fChain->SetBranchAddress("numJec", &numJec, &b_numJec);
   fChain->SetBranchAddress("njec", njec, &b_njec);
   fChain->SetBranchAddress("ajec", ajec, &b_ajec);
   fChain->SetBranchAddress("evtnp", &evtnp, &b_evtnp);
   fChain->SetBranchAddress("psube", psube, &b_psube);
   fChain->SetBranchAddress("ppt", ppt, &b_ppt);
   fChain->SetBranchAddress("peta", peta, &b_peta);
   fChain->SetBranchAddress("pphi", pphi, &b_pphi);
   fChain->SetBranchAddress("pch", pch, &b_pch);
   fChain->SetBranchAddress("ppid", ppid, &b_ppid);
   fChain->SetBranchAddress("pndphi", pndphi, &b_pndphi);
   fChain->SetBranchAddress("pndr", pndr, &b_pndr);
   fChain->SetBranchAddress("pndrbg", pndrbg, &b_pndrbg);
   fChain->SetBranchAddress("padphi", padphi, &b_padphi);
   fChain->SetBranchAddress("padr", padr, &b_padr);
   fChain->SetBranchAddress("padrbg", padrbg, &b_padrbg);
   fChain->SetBranchAddress("lp", &lp_, &b__);
   fChain->SetBranchAddress("lp.fCoordinates.fRho", lp_fCoordinates_fRho, &b_lp_fCoordinates_fRho);
   fChain->SetBranchAddress("lp.fCoordinates.fEta", lp_fCoordinates_fEta, &b_lp_fCoordinates_fEta);
   fChain->SetBranchAddress("lp.fCoordinates.fPhi", lp_fCoordinates_fPhi, &b_lp_fCoordinates_fPhi);
   fChain->SetBranchAddress("trkNHits", trkNHits, &b_trkNHits);
   fChain->SetBranchAddress("trkdz", trkdz, &b_trkdz);
   fChain->SetBranchAddress("trkdze", trkdze, &b_trkdze);
   fChain->SetBranchAddress("trkd0", trkd0, &b_trkd0);
   fChain->SetBranchAddress("trkd0e", trkd0e, &b_trkd0e);
   fChain->SetBranchAddress("ljcnp", &ljcnp, &b_ljcnp);
   fChain->SetBranchAddress("ljcnpbg", &ljcnpbg, &b_ljcnpbg);
   fChain->SetBranchAddress("ljcpt", &ljcpt, &b_ljcpt);
   fChain->SetBranchAddress("ljcpt2", &ljcpt2, &b_ljcpt2);
   fChain->SetBranchAddress("ljcptr", &ljcptr, &b_ljcptr);
   fChain->SetBranchAddress("ljcptbg", &ljcptbg, &b_ljcptbg);
   fChain->SetBranchAddress("ljcpt2bg", &ljcpt2bg, &b_ljcpt2bg);
   fChain->SetBranchAddress("ljcptrbg", &ljcptrbg, &b_ljcptrbg);
   fChain->SetBranchAddress("lppt", &lppt, &b_lppt);
   fChain->SetBranchAddress("lpjdr", &lpjdr, &b_lpjdr);
   fChain->SetBranchAddress("lgppt", &lgppt, &b_lgppt);
   fChain->SetBranchAddress("lgpch", &lgpch, &b_lgpch);
   fChain->SetBranchAddress("lgppid", &lgppid, &b_lgppid);
   Notify();
}

Bool_t JetFragAna::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void JetFragAna::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

Bool_t JetFragAna::GetEvtMask()
{
  // Bad hcal events from Marguerite
  if(run== 151211 && evt == 555881 && lumi ==  103 ) return true;;
  if(run== 151238 && evt == 581194 && lumi ==  105 ) return true;;
  if(run== 151238 && evt == 1299690 && lumi ==  242) return true;;
  if(run== 151352 && evt == 55195 && lumi ==  10   ) return true;;
  if(run== 151878 && evt == 594808 && lumi ==  135 ) return true;;
  if(run== 152047 && evt == 666877 && lumi ==  114 ) return true;;
  if(run== 152112 && evt == 2207843 && lumi ==  426) return true;;
  if(run== 152112 && evt == 3151220 && lumi ==  608) return true;;
  if(run== 152349 && evt == 939393 && lumi ==  220 ) return true;;
  if(run== 152350 && evt == 595632 && lumi ==  111 ) return true;;
  if(run== 152350 && evt == 2482917 && lumi ==  472) return true;;
  if(run== 152350 && evt == 2686548 && lumi ==  512) return true;;
  if(run== 152474 && evt == 2085185 && lumi ==  403) return true;;
  if(run== 152477 && evt == 1890056 && lumi ==  392) return true;;
  if(run== 152485 && evt == 55917 && lumi ==  12   ) return true;;
  if(run== 152561 && evt == 3406888 && lumi ==  606) return true;;
  if(run== 152561 && evt == 3758331 && lumi ==  670) return true;;
  if(run== 152561 && evt == 4478132 && lumi ==  803) return true;;
  if(run== 152561 && evt == 4797830 && lumi ==  863) return true;;
  if(run== 152561 && evt == 5176016 && lumi ==  936) return true;;
  if(run== 152592 && evt == 22234 && lumi ==  4    ) return true;;
  if(run== 152592 && evt == 402212 && lumi ==  66  ) return true;;
  if(run== 152594 && evt == 587793 && lumi ==  110 ) return true;;
  if(run== 152601 && evt == 1417393 && lumi ==  267) return true;;
  if(run== 152602 && evt == 686565 && lumi ==  111 ) return true;;
  if(run== 152602 && evt == 3940942 && lumi ==  684) return true;;
  if(run== 152624 && evt == 995626 && lumi ==  170 ) return true;;
  if(run== 152624 && evt == 1063452 && lumi ==  182) return true;;
  if(run== 152624 && evt == 1250655 && lumi ==  215) return true;;
  if(run== 152624 && evt == 1846646 && lumi ==  322) return true;;
  if(run== 152625 && evt == 1634959 && lumi ==  282) return true;;
  if(run== 152625 && evt == 3162245 && lumi ==  563) return true;;
  if(run== 152641 && evt == 173478 && lumi ==  31  ) return true;;
  if(run== 152642 && evt == 359181 && lumi ==  57  ) return true;;
  if(run== 152642 && evt == 1764595 && lumi ==  286) return true;;
  if(run== 152642 && evt == 2686223 && lumi ==  446) return true;;
  if(run== 152721 && evt == 1304320 && lumi ==  249) return true;;
  if(run== 152721 && evt == 1983505 && lumi ==  358) return true;;
  if(run== 152722 && evt == 2963949 && lumi ==  485) return true;;
  if(run== 152741 && evt == 558533 && lumi ==  91  ) return true;;
  if(run== 152751 && evt == 3432123 && lumi ==  582) return true;;
  if(run== 152791 && evt == 246343 && lumi ==  39  ) return true;;
  return false;
}

Int_t JetFragAna::Cut(Long64_t entry)
{
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.

  // preselection
  if (cut.BaseCutType=="S0")
    if (vz<=cut.VzMin || vz>= cut.VzMax)
      return -1;

  if (cut.BaseCutType=="S1")
    if (vz<=cut.VzMin || vz>= cut.VzMax ||
       cent<cut.CentMin || cent>=cut.CentMax)
      return -1;

  if (cut.BaseCutType=="S2")
    if (vz<=cut.VzMin || vz>= cut.VzMax ||
	cent<cut.CentMin || cent>=cut.CentMax ||
	!hlt->at(2))
      return -1;

  Float_t Aj = (anaJets_[0].pt()-anaJets_[1].pt())/(anaJets_[0].pt()+anaJets_[1].pt());
  if (Aj<cut.AjMin||Aj>=cut.AjMax) return -1;

  Int_t result=-1;
  if (cut.DJCutType=="Ana") {
    if (anaJets_[0].pt()>=cut.NrJEtMin && anaJets_[0].pt()<cut.NrJEtMax && fabs(anaJets_[0].eta())<cut.NrJEtaMax &&
	anaJets_[1].pt()>=cut.AwJEtMin && anaJets_[1].pt()<cut.AwJEtMax && fabs(anaJets_[1].eta())<cut.AwJEtaMax &&
	jdphi>cut.DjDPhiMin
       )
      result=1;
  }

  return result;
}

Int_t JetFragAna::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  Int_t result = fChain->GetEntry(entry);
  if (result<0) return result;
  // Got Entry
  else {
    anaJets_[0].SetCoordinates(nljet,nljeta,nljphi,0);
    anaJets_[1].SetCoordinates(aljet,aljeta,aljphi,0);
    if (doEtaCorr_) {
      for (Int_t i=0;i<2;++i) {
	if (anaJets_[i].pt()<0) continue;
	Float_t etain = anaJets_[i].eta();
	Float_t etaout = etain;
	if(etain<-1.5)
	  etaout = etain-jetaCorr_["ec1"]->Eval(etain);
	else if(etain<-0.8)
	  etaout = etain-jetaCorr_["ec2"]->Eval(etain);
	else if(etain<0.9)
	  etaout = etain-jetaCorr_["ec3"]->Eval(etain);
	else if(etain<1.5)
	  etaout = etain-jetaCorr_["ec4"]->Eval(etain);
	else if(etain<2.5)
	  etaout = etain-jetaCorr_["ec5"]->Eval(etain);
	anaJets_[i].SetEta(etaout);
	//cout << "entry: " << entry << " old eta: " << etain << "  new eta: " << etaout << " " << anaJets_[i] << endl;
      }
    }
    particles_.clear();
    if (!doJetOnly_) {
      for (Int_t i=0; i<evtnp; ++i) {
	particles_.push_back(math::PtEtaPhiMLorentzVector(ppt[i],peta[i],pphi[i],0.13957));
      }
    }
  } // finished with entry
  return result;
}

double JetFragAna::getEffFakeCorrection(double pt,double eta, double cent)
{
   
   int bin = 0;

   // Get the corresponding centrality bin
   if (cent<10) {
      bin = 0;
   } else if (cent<20) {
      bin = 1;
   } else if (cent<30) {
      bin = 2;
   } else if (cent<50) {
      bin = 3;   
   } else if (cent<100) {
      bin = 4;   
   }          
   
   int etaBin = trackingEtaBin_->FindBin(fabs(eta));
   int ptBin = trackingPtBin_->FindBin(pt);
   
   if (ptBin>21) ptBin= 21;
 
   double eff = trackingEffCorr_[bin]->GetBinContent(etaBin,ptBin);
   double effErr = trackingEffCorr_[bin]->GetBinError(etaBin,ptBin);
   double fake = trackingFakeCorr_[bin]->GetBinContent(etaBin,ptBin);
   if ((1-fake)/eff>10) {
      cout <<"Correction: Pt = "<<pt <<" eta = "<<eta<<" cent = "<<cent<<endl;
      cout <<"Correction: Fake = "<<fake <<" eff = "<<eff<<" cor = "<<(1-fake)/eff<<endl;
      cout <<"Err = "<<effErr/eff<<endl;
   }
   if (pt>20) eff*=1.20;
   return (1-fake)/eff;
//   return 1.3;
}

void JetFragAna::Loop()
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   numDJ_=0,numDJReWeighted_=0;
   Int_t numTotEvt=0, numDJNoBkgLimit=0;
   Long64_t nbytes = 0, nb = 0;

   // Pt Bin
   TH1D * hPt = (TH1D*)hPtPNDR->ProjectionX();

   // Centrality distribution
   TH1D *hCent = new TH1D("hCent","",40,0,100);
   fChain->Project("hCent","cent",evtCut);
   hCent->Scale(1./hCent->GetEntries());   
   // Centrality Weight
   hCentralityWeight_->Divide(hCentralityData_,hCent);
   // ReWeighted Centrality distribution
   TH1D *hCentReWeighted = new TH1D("hCentReWeighted","",40,0,100);

  //=======================================================================================================================
   // Main Loop 
  //=======================================================================================================================
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (jentry%500==0) cout << "jentry: " << jentry << " " << jentry/float(nentries) << endl;
      if (ientry < 0) break;
      nb = GetEntry(jentry);   nbytes += nb;
      ++numTotEvt;

      if (!doEvtSel_||(Cut(ientry)>=0&&!GetEvtMask())) {
	++numDJNoBkgLimit;
	if (cut.BkgSubType=="EtaRefl") {
	  if (fabs(nljeta)<cut.ConeSize||fabs(aljeta)<cut.ConeSize) continue;
	}
	if (cut.BkgSubType=="PhiRot") {
	  if (fabs(nljeta-aljeta)<cut.ConeSize*2) continue;
	}
 
        int cBin = hCent->FindBin(cent);
        double weight=1;
        if (doCentralityReweighting_) {
           if (hCentralityData_->GetBinContent(cBin)==0 || hCent->GetBinContent(cBin)==0) {
             weight = 0; 
           } else {
             weight = hCentralityWeight_->GetBinContent(cBin);
           }
        }

	// Event Level Histograms
	hJDPhi->Fill(jdphi,weight);
	hJEtNr->Fill(anaJets_[0].pt(),weight);
	hJEtAw->Fill(anaJets_[1].pt(),weight);
	hAj->Fill((anaJets_[0].pt()-anaJets_[1].pt())/(anaJets_[0].pt()+anaJets_[1].pt()),weight);
	hJEtaNr->Fill(anaJets_[0].eta(),weight);
	hJEtaAw->Fill(anaJets_[1].eta(),weight);
	hJDEta->Fill(anaJets_[1].eta()-anaJets_[0].eta(),weight);
	hCentReWeighted->Fill(cent,weight);

	++numDJ_;
	numDJReWeighted_+=weight;
	if (doJetOnly_) continue;

	// -- Loop over Particles --
	Double_t nrConePt=0,nrConePtBg=0;
	Double_t awConePt=0,awConePtBg=0;
	Double_t metx=0,metx0=0,metx1=0,metx2=0,metx3=0,metx4=0,metx5=0;
	Double_t mety=0,mety0=0,mety1=0,mety2=0,mety3=0,mety4=0,mety5=0;
	for (Int_t i=0; i<evtnp;++i) {
	  // Trk Cut
	  if (anaGenpType_==1 && pch[i]==0) continue;
	  if (ppt[i]<cut.TrkPtMin||fabs(peta[i])>=2.4) continue;
          double trackWeight=1;
          if (doTrackingEffFakeCorr_) trackWeight = getEffFakeCorrection(ppt[i],peta[i],cent);
	  //cout << "particle " << i << ": ch " << pch[i] << " pt: " << ppt[i] << " pndr: " << pndr[i] << endl;
	  // Trk histograms

	  // met calculation
	  Float_t pptx=cos(pndphi[i])*ppt[i]*trackWeight;
	  metx+=pptx;
	  Float_t ppty=sin(pndphi[i])*ppt[i]*trackWeight;
	  mety+=ppty;
	  //for (int i=0;i<hPt->GetNbinsX()+2;++i) cout << "Bin " << i << " ledge: " << hPt->GetBinLowEdge(i) << endl;
	  if (ppt[i]>=hPt->GetBinLowEdge(1)&&ppt[i]<hPt->GetBinLowEdge(2)) {
            metx0+=pptx;
            mety0+=ppty;
          } else if (ppt[i]>=hPt->GetBinLowEdge(2)&&ppt[i]<hPt->GetBinLowEdge(3)) {
            metx1+=pptx;
            mety1+=ppty;
          } else if (ppt[i]>=hPt->GetBinLowEdge(3)&&ppt[i]<hPt->GetBinLowEdge(4)) {
            metx2+=pptx;
            mety2+=ppty;
          } else if (ppt[i]>=hPt->GetBinLowEdge(4)&&ppt[i]<hPt->GetBinLowEdge(5)) {
            metx3+=pptx;
            mety3+=ppty;
          } else if (ppt[i]>=hPt->GetBinLowEdge(5)&&ppt[i]<hPt->GetBinLowEdge(6)) { 
            metx4+=pptx;
            mety4+=ppty;
          } else if (ppt[i]>=hPt->GetBinLowEdge(6)&&ppt[i]<hPt->GetBinLowEdge(7)) { 
            metx5+=pptx;
            mety5+=ppty;
          }

          // Take the reweighting into account for later histogram
          // This should not be applied before the met calculation.
          trackWeight *= weight;

	  // bcksub
	  Double_t PNdRBkg=999,PAdRBkg=999;
	  if (cut.BkgSubType=="EtaRefl") {
	    PNdRBkg = reco::deltaR(peta[i],pphi[i],-nljeta,nljphi);
	    PAdRBkg = reco::deltaR(peta[i],pphi[i],-aljeta,aljphi);
	  }
	  if (cut.BkgSubType=="PhiRot") {
	    PNdRBkg = reco::deltaR(peta[i],pphi[i],nljeta,nljphi+TMath::Pi());
	    PAdRBkg = reco::deltaR(peta[i],pphi[i],aljeta,aljphi+TMath::Pi());
	  }

	  // For the moment cone analysis start from 1GeV
	  if (ppt[i]<hPt->GetBinLowEdge(2)) continue;

	  // Signal Cone
	  if (pndr[i]<cut.ConeSize) {
	    nrConePt+=ppt[i]*trackWeight;
	    //cout << "Sum so far: " << nrConePt << endl;
	    hNrCPPt->Fill(ppt[i],trackWeight);
	    hPNDR->Fill(pndr[i],ppt[i]*trackWeight);
	    hPtPNDR->Fill(ppt[i],pndr[i],ppt[i]*trackWeight);
	  }
	  if (padr[i]<cut.ConeSize) {
	    hAwCPPt->Fill(ppt[i],trackWeight);
	    awConePt+=ppt[i]*trackWeight;
	    hPADR->Fill(padr[i],ppt[i]*trackWeight);
	    hPtPADR->Fill(ppt[i],padr[i],ppt[i]*trackWeight);
	  }

	  // Background Cone
	  if (PNdRBkg<cut.ConeSize) {
	    hNrCPPtBg->Fill(ppt[i],trackWeight);
	    nrConePtBg+=ppt[i]*trackWeight;
	    hPNDRBg->Fill(PNdRBkg,ppt[i]*trackWeight);
	    hPtPNDRBg->Fill(ppt[i],PNdRBkg,ppt[i]*trackWeight);
	  }
	  if (PAdRBkg<cut.ConeSize) {
	    hAwCPPtBg->Fill(ppt[i],trackWeight);
	    awConePtBg+=ppt[i]*trackWeight;
	    hPADRBg->Fill(PAdRBkg,ppt[i]*trackWeight);
	    hPtPADRBg->Fill(ppt[i],PAdRBkg,ppt[i]*trackWeight);
	  }
	} // end of particles loop
	hNrCPt->Fill(nrConePt);
	hAwCPt->Fill(awConePt);
	hNrCPtBg->Fill(nrConePtBg);
	hAwCPtBg->Fill(awConePtBg);
	hNrCPtBgSub->Fill(nrConePt-nrConePtBg);
	hAwCPtBgSub->Fill(awConePt-awConePtBg);

	// fill ntuple
        Float_t var[100];
        var[0]=anaJets_[0].pt();
        var[1]=anaJets_[0].eta();
        var[2]=anaJets_[1].pt();
        var[3]=anaJets_[1].eta();
        var[4]=metx;
        var[5]=metx0;
        var[6]=metx1;
        var[7]=metx2;
        var[8]=metx3;
        var[9]=metx4;
        var[10]=metx5;
        var[11]=mety;
        var[12]=mety0;
        var[13]=mety1;
        var[14]=mety2;
        var[15]=mety3;
        var[16]=mety4;
        var[17]=mety5;
        var[18]=GetEvtMask();
        var[19]=cent;
        var[20]=jdphi;
        var[21]=weight;
	ntjt->Fill(var);
      }
      // if (Cut(ientry) < 0) continue;
   }

   // After Event Loop
   cout << "Total Events: " << numTotEvt << endl;
   cout << "DiJets Selected w/o Bkg Limit: " << numDJNoBkgLimit << " (same as draw cut unless there is jet eta correction)" << endl;
   cout << "DiJets Selected: " << numDJ_ << endl;
   cout << "DiJets Selected Reweighted: " << numDJReWeighted_ << endl;

   hJDPhi->Scale(1./(numDJReWeighted_));
   hJEtNr->Scale(1./(numDJReWeighted_));
   hJEtAw->Scale(1./(numDJReWeighted_));
   hAj->Scale(1./(numDJReWeighted_));
   hJEtaNr->Scale(1./(numDJReWeighted_));
   hJEtaAw->Scale(1./(numDJReWeighted_));
   hJDEta->Scale(1./(numDJReWeighted_));
   hCentReWeighted->Scale(1./(numDJReWeighted_));

   hNrCPPt->Scale(1./numDJReWeighted_);
   hAwCPPt->Scale(1./numDJReWeighted_);
   hNrCPPtBg->Scale(1./numDJReWeighted_);
   hAwCPPtBg->Scale(1./numDJReWeighted_);
   hNrCPPtBgSub->Add(hNrCPPt,hNrCPPtBg,1,-1);
   hAwCPPtBgSub->Add(hAwCPPt,hAwCPPtBg,1,-1);

   hNrCPt->Scale(1./(numDJReWeighted_));
   hAwCPt->Scale(1./(numDJReWeighted_));
   hNrCPtBg->Scale(1./(numDJReWeighted_));
   hAwCPtBg->Scale(1./(numDJReWeighted_));
   hNrCPtBgSub->Scale(1./(numDJReWeighted_));
   hAwCPtBgSub->Scale(1./(numDJReWeighted_));

   hPNDR->Scale(1./(numDJReWeighted_));
   hPADR->Scale(1./(numDJReWeighted_));
   hPNDRBg->Scale(1./(numDJReWeighted_));
   hPADRBg->Scale(1./(numDJReWeighted_));

   hPtPNDR->Scale(1./(numDJReWeighted_));
   hPtPADR->Scale(1./(numDJReWeighted_));
   hPtPNDRBg->Scale(1./(numDJReWeighted_));
   hPtPADRBg->Scale(1./(numDJReWeighted_));
}
