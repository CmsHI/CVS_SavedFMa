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
  refJets_(2),
  mixOffset_(0),
  jetTreeMode_(0),
  particleTreeMode_(0)
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

   // ===================================================
   // ntuples
   // ===================================================
   ntjt = new TNtuple("ntjt","jet-trk nt","nljet:nljetacorr:nljphi:aljet:aljetacorr:aljphi:"
                                          "metx:metx0:metx1:metx2:metx3:metx4:metx5:"
                                          "mety:mety0:mety1:mety2:mety3:mety4:mety5:"
                                          "metConex:metConex0:metConex1:metConex2:metConex3:metConex4:metConex5:"
                                          "metOutOfConex:metOutOfConex0:metOutOfConex1:metOutOfConex2:metOutOfConex3:metOutOfConex4:metOutOfConex5:"
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

   // trees
   tcone = new TTree("tcone","jet cone tree");
   tcone->Branch("cpt",&jc_.cpt);
   tcone->Branch("cptbg",&jc_.cptbg);
   tcone->Branch("cptpara",&jc_.cptpara);
   tcone->Branch("cptparabg",&jc_.cptparabg);

   // ===================================================
   // Histograms
   // ===================================================
   TString labelShort[2] = { "N","A" };
   TString label[2] = { "Nr","Aw" };
   const Int_t numDRBins = 20;
   Double_t dRBins[numDRBins+1];
   for (int i=0;i<numDRBins+1;i++)   { dRBins[i] = 1.6/((double)numDRBins)*i; }
   const Int_t numPtBins = 6;
   Double_t ptBins[numPtBins+1]={0.5,1.0,2,4,8,20,180};
   // trk distribution
   const Int_t numPPtBins=18;
   Float_t pptBins[numPPtBins+1] = {0.0,0.2,1,2,3,4,6,8,10,14,18,22,26,30,40,50,60,70,80};

   // -- 2D hists --
   for (Int_t j=0; j<2; ++j) {
     hPtPDR[j] = new TH2D("hPtP"+labelShort[j]+"DR","",numPtBins,ptBins,numDRBins,dRBins);
     hPtPDR[j]->Sumw2();
     hPtPDRBg[j] = new TH2D("hPtP"+labelShort[j]+"DRBg","",numPtBins,ptBins,numDRBins,dRBins);
     hPtPDRBg[j]->Sumw2();
   }

   // -- 1D hists --
   //TH1::SetDefaultSumw2();
   // evt
   // centrality weight
   hCentralityWeight_ = new TH1D("hCentralityWeight","",40,0,100);

   // jet
   hJDPhi = new TH1D("hJDPhi","",50,0,PI);
   hJDPhi->Sumw2();
   hJDEta = new TH1D("hJDEta","",50,-6,6);
   hJDEta->Sumw2();
   hAj = new TH1D("hAj","",20,0,1);
   hAj->Sumw2();
   for (Int_t j=0; j<2; ++j) {
     hJEt[j] = new TH1D("hJEt"+label[j],"",cut.numJEtBins,cut.hisJEtMin,cut.hisJEtMax);
     hJEt[j]->Sumw2();
     hJEta[j] = new TH1D("hJEta"+label[j],"",cut.numJEtaBins,cut.hisJEtaMin,cut.hisJEtaMax);
     hJEta[j]->Sumw2();
     hRefJEt[j] = new TH1D("hRefJEt"+label[j],"",cut.numJEtBins,cut.hisJEtMin,cut.hisJEtMax);
     hRefJEt[j]->Sumw2();
   }

   for (Int_t j=0; j<2; ++j) {
     // cone
     hCPt[j] = new TH1D("h"+label[j]+"CPt","",cut.numJEtBins,cut.hisJEtMin,cut.hisJEtMax);
     hCPt[j]->Sumw2();
     hCPtBg[j] = new TH1D("h"+label[j]+"CPtBg","",cut.numJEtBins,cut.hisJEtMin,cut.hisJEtMax);
     hCPtBg[j]->Sumw2();
     hCPtBgSub[j] = new TH1D("h"+label[j]+"CPtBgSub","",cut.numJEtBins*2,-1*cut.hisJEtMax,cut.hisJEtMax);
     hCPtBgSub[j]->Sumw2();
     // trk
     hPJDPhi[j] = new TH1D("h"+label[j]+"PJDPhi","",40,-TMath::Pi(),TMath::Pi());
     hPJDPhi[j]->Sumw2();
     hCPPt[j] = new TH1D("h"+label[j]+"CPPt","",numPPtBins,pptBins);
     hCPPt[j]->Sumw2();
     hCPPtBg[j] = new TH1D("h"+label[j]+"CPPtBg","",numPPtBins,pptBins);
     hCPPtBg[j]->Sumw2();
     hCPPtBgSub[j] = new TH1D("h"+label[j]+"CPPtBgSub","",numPPtBins,pptBins);
     hCPPtBgSub[j]->Sumw2();
   }

   // Random Number
   r3 = new TRandom3(21);
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
   if (cut.Name.Contains("PF")) {
     fChain->SetBranchAddress("pfid", pfid, &b_pfid);
   }
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
  if(run== 151211 && evt == 555881 && lumi ==  103 ) return true;
  if(run== 151238 && evt == 581194 && lumi ==  105 ) return true;
  if(run== 151238 && evt == 1299690 && lumi ==  242) return true;
  if(run== 151352 && evt == 55195 && lumi ==  10   ) return true;
  if(run== 151878 && evt == 594808 && lumi ==  135 ) return true;
  if(run== 152047 && evt == 666877 && lumi ==  114 ) return true;
  if(run== 152112 && evt == 2207843 && lumi ==  426) return true;
  if(run== 152112 && evt == 3151220 && lumi ==  608) return true;
  if(run== 152349 && evt == 939393 && lumi ==  220 ) return true;
  if(run== 152350 && evt == 595632 && lumi ==  111 ) return true;
  if(run== 152350 && evt == 2482917 && lumi ==  472) return true;
  if(run== 152350 && evt == 2686548 && lumi ==  512) return true;
  if(run== 152474 && evt == 2085185 && lumi ==  403) return true;
  if(run== 152477 && evt == 1890056 && lumi ==  392) return true;
  if(run== 152485 && evt == 55917 && lumi ==  12   ) return true;
  if(run== 152561 && evt == 3406888 && lumi ==  606) return true;
  if(run== 152561 && evt == 3758331 && lumi ==  670) return true;
  if(run== 152561 && evt == 4478132 && lumi ==  803) return true;
  if(run== 152561 && evt == 4797830 && lumi ==  863) return true;
  if(run== 152561 && evt == 5176016 && lumi ==  936) return true;
  if(run== 152592 && evt == 22234 && lumi ==  4    ) return true;
  if(run== 152592 && evt == 402212 && lumi ==  66  ) return true;
  if(run== 152594 && evt == 587793 && lumi ==  110 ) return true;
  if(run== 152601 && evt == 1417393 && lumi ==  267) return true;
  if(run== 152602 && evt == 686565 && lumi ==  111 ) return true;
  if(run== 152602 && evt == 3940942 && lumi ==  684) return true;
  if(run== 152624 && evt == 995626 && lumi ==  170 ) return true;
  if(run== 152624 && evt == 1063452 && lumi ==  182) return true;
  if(run== 152624 && evt == 1250655 && lumi ==  215) return true;
  if(run== 152624 && evt == 1846646 && lumi ==  322) return true;
  if(run== 152625 && evt == 1634959 && lumi ==  282) return true;
  if(run== 152625 && evt == 3162245 && lumi ==  563) return true;
  if(run== 152641 && evt == 173478 && lumi ==  31  ) return true;
  if(run== 152642 && evt == 359181 && lumi ==  57  ) return true;
  if(run== 152642 && evt == 1764595 && lumi ==  286) return true;
  if(run== 152642 && evt == 2686223 && lumi ==  446) return true;
  if(run== 152721 && evt == 1304320 && lumi ==  249) return true;
  if(run== 152721 && evt == 1983505 && lumi ==  358) return true;
  if(run== 152722 && evt == 2963949 && lumi ==  485) return true;
  if(run== 152741 && evt == 558533 && lumi ==  91  ) return true;
  if(run== 152751 && evt == 3432123 && lumi ==  582) return true;
  if(run== 152791 && evt == 246343 && lumi ==  39  ) return true;
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
    p_.clear();
    if (!doJetOnly_) {
      for (Int_t i=0; i<evtnp; ++i) {
	p_.push_back(math::PtEtaPhiMLorentzVector(ppt[i],peta[i],pphi[i],0.13957));
      }
    }
  } // finished with entry
  return result;
}

Int_t JetFragAna::GetJetEntry(TChain * t, AnaJet & jet, Long64_t entry)
{
  // Read contents of entry.
  if (!t) return 0;
  Int_t result = t->GetEntry(entry);
  if (result<0) return result;
  // Got Entry
  else {
    anaJets_[0].SetCoordinates(jet.nljet,jet.nljeta,jet.nljphi,0);
    anaJets_[1].SetCoordinates(jet.aljet,jet.aljeta,jet.aljphi,0);
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
    //for (Int_t i=0;i<2;++i) {
    //  anaJets_[i].SetPhi(r3->Rndm()*TMath::TwoPi()-TMath::Pi());
    //}
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
   Long64_t jetTreeNEntries[10] = {nentries,jetTree_[1]->GetEntries(),0,0,0};
   cout << "==============" << endl;
   cout << " Begin Loop" << endl;
   cout << "Tree: " << nentries << " jetTree: " << jetTreeNEntries[jetTreeMode_] << endl;
   cout << "==============" << endl;

   // =====================================================
   // Initialize Counters
   // =====================================================
   numDJ_=0,numDJReWeighted_=0;
   for (Int_t i=0; i<2; ++i) {
     numJ_[i]=0;
     numJReWeighted_[i]=0;
   }
   Int_t numTotEvt=0, numDJNoBkgLimit=0;
   Long64_t nbytes = 0, nb = 0;

   // Pt Bin
   TH1D * hPt = (TH1D*)hPtPDR[0]->ProjectionX();
   Int_t numPtBins = hPt->GetNbinsX();

   // Set Tree Pt bins
   jc_.resizePtBins(numPtBins);

   // =====================================================
   // Centrality ReWeighting
   // =====================================================
   TH1D *hCent = new TH1D("hCent","",40,0,100);
   fChain->Project("hCent","cent",""); // no evt sel, assume centrality distribution of bkg is independent of the signal
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
      GetJetEntry(jetTree_[jetTreeMode_],vj_[jetTreeMode_],jentry%jetTreeNEntries[jetTreeMode_]);
      ++numTotEvt;

      // =====================================================
      // Main Event Selection
      // =====================================================
      if (!doEvtSel_||(Cut(ientry)>=0&&!GetEvtMask())) {
	++numDJNoBkgLimit;
	// =====================================================
	// Jet Phase Space Limit (if Bkg Subtraction)
	// =====================================================
	if (cut.BkgSubType=="EtaRefl") {
	  if (fabs(anaJets_[0].eta())<cut.ConeSize||fabs(anaJets_[1].eta())<cut.ConeSize) continue;
	}
	if (cut.BkgSubType=="PhiRot") {
	  if (fabs(anaJets_[0].eta()-anaJets_[1].eta())<cut.ConeSize*2) continue;
	}
	// If we want to restrict eta for j0,j1 separately
	// Classify events into j0 events or j1 events
	bool jetEvt[2] = { true, true };
	if (cut.BkgSubType=="EtaReflSingle") {
	  for (Int_t j=0; j<2; ++j) {
	    if (fabs(anaJets_[j].eta())<cut.ConeSize) jetEvt[j]=false;
	  }
	}
	// systematic checks on eta regions
	if (cut.BkgSubType=="EtaReflSingleEtaPos") {
	  for (Int_t j=0; j<2; ++j) {
	    if (fabs(anaJets_[j].eta())<cut.ConeSize||anaJets_[j].eta()<0) jetEvt[j]=false;
	  }
	}
	if (cut.BkgSubType=="EtaReflSingleEtaNeg") {
	  for (Int_t j=0; j<2; ++j) {
	    if (fabs(anaJets_[j].eta())<cut.ConeSize||anaJets_[j].eta()>=0) jetEvt[j]=false;
	  }
	}
 
	// =====================================================
	// Set Centrality Weight
	// =====================================================
        int cBin = hCent->FindBin(cent);
        double weight=1;
        if (doCentralityReweighting_) {
           if (hCentralityData_->GetBinContent(cBin)==0 || hCent->GetBinContent(cBin)==0) {
             weight = 0; 
           } else {
             weight = hCentralityWeight_->GetBinContent(cBin);
           }
        }

	// =====================================================
	// Fill Event Level Histograms
	// =====================================================
	hJDPhi->Fill(jdphi,weight);
	hAj->Fill((anaJets_[0].pt()-anaJets_[1].pt())/(anaJets_[0].pt()+anaJets_[1].pt()),weight);
	hJDEta->Fill(anaJets_[1].eta()-anaJets_[0].eta(),weight);
	hCentReWeighted->Fill(cent,weight);
	++numDJ_;
	numDJReWeighted_+=weight;

	for (Int_t j=0; j<2; ++j) {
	  if (jetEvt[j]) {
	    hJEt[j]->Fill(anaJets_[j].pt(),weight);
	    hJEta[j]->Fill(anaJets_[j].eta(),weight);
	    ++numJ_[j];
	    numJReWeighted_[j]+=weight;
	  }
	}

	if (doJetOnly_) continue;

	// Initialize Counters
	Double_t conePtSum[2] = { 0,0 };
	Double_t conePtBgSum[2] = { 0,0 };
	Double_t metx=0,metx0=0,metx1=0,metx2=0,metx3=0,metx4=0,metx5=0;
	Double_t mety=0,mety0=0,mety1=0,mety2=0,mety3=0,mety4=0,mety5=0;
	Double_t metConex=0,metConex0=0,metConex1=0,metConex2=0,metConex3=0,metConex4=0,metConex5=0;
	Double_t metOutOfConex=0,metOutOfConex0=0,metOutOfConex1=0,metOutOfConex2=0,metOutOfConex3=0,metOutOfConex4=0,metOutOfConex5=0;
        jc_.clear();
	// =====================================================
	// Fill Particle Level Histograms
	// =====================================================
	//GetEntry((jentry+mixOffset_)%nentries);
	for (Int_t i=0; i<evtnp;++i) {
	  // ------------------------
	  // Trk Cut
	  // ------------------------
	  if (anaGenpType_==1 && pch[i]==0) continue;
	  if (anaGenpType_==10 && (psube[i]>0 || pch[i]==0)) continue;
	  if (cut.Name.Contains("PFChHad") && pfid[i]!=1) continue;
	  if (cut.Name.Contains("PFPhoton") && pfid[i]!=4) continue;
	  if (ppt[i]<cut.TrkPtMin||fabs(peta[i])>=2.4) continue;
	  // ------------------------
	  // Track Efficiency/Fake Correction
	  // ------------------------
          double trackWeight=1;
          if (doTrackingEffFakeCorr_) trackWeight = getEffFakeCorrection(ppt[i],peta[i],cent);
	  // Dead forward pixel xcheck
	  //if (peta[i]>2&&pphi[i]>-0.1&&pphi[i]<0.8) trackWeight=0;

	  // ------------------------
	  // calculate particle jet correlations
	  // ------------------------
	  Double_t pdphi[2]={ 9999,9999 };
	  Double_t pdr[2]={ 9999,9999 };
	  Double_t pdrbg[2]={ 9999,9999 };
	  for (Int_t j=0; j<2; ++j) {
	    // signal
	    pdphi[j] = reco::deltaPhi(p_[i].phi(),anaJets_[j].phi());
	    pdr[j] = reco::deltaR(p_[i].eta(),p_[i].phi(),anaJets_[j].eta(),anaJets_[j].phi());
	    // bcksub
	    // * If don't do event selection, make eta reflection the default bkg axis
	    if (cut.BkgSubType.Contains("EtaRefl"||!doEvtSel_)) {
	      pdrbg[j] = reco::deltaR(p_[i].eta(),p_[i].phi(),-1*anaJets_[j].eta(),anaJets_[j].phi());
	    }
	    // monitor histograms
	    hPJDPhi[j]->Fill(pdphi[j],trackWeight);
	  }

	  Float_t trkEnergy=p_[i].pt();
	  // ------------------------
	  // met calculation
	  // ------------------------
	  Float_t pptx=cos(pdphi[0])*trkEnergy*trackWeight;
	  metx+=pptx;
	  Float_t ppty=sin(pdphi[0])*trkEnergy*trackWeight;
	  mety+=ppty;
          if (fabs(pdr[0])<0.8||fabs(pdr[1])<0.8) metConex+=pptx; else metOutOfConex+=pptx;
	  //for (int i=0;i<hPt->GetNbinsX()+2;++i) cout << "Bin " << i << " ledge: " << hPt->GetBinLowEdge(i) << endl;
	  if (trkEnergy>=hPt->GetBinLowEdge(1)&&trkEnergy<hPt->GetBinLowEdge(2)) {
            metx0+=pptx;
            mety0+=ppty;
            if (fabs(pdr[0])<0.8||fabs(pdr[1])<0.8) metConex0+=pptx; else metOutOfConex0+=pptx;
          } else if (trkEnergy>=hPt->GetBinLowEdge(2)&&trkEnergy<hPt->GetBinLowEdge(3)) {
            metx1+=pptx;
            mety1+=ppty;
            if (fabs(pdr[0])<0.8||fabs(pdr[1])<0.8) metConex1+=pptx; else metOutOfConex1+=pptx;
          } else if (trkEnergy>=hPt->GetBinLowEdge(3)&&trkEnergy<hPt->GetBinLowEdge(4)) {
            metx2+=pptx;
            mety2+=ppty;
            if (fabs(pdr[0])<0.8||fabs(pdr[1])<0.8) metConex2+=pptx; else metOutOfConex2+=pptx;
          } else if (trkEnergy>=hPt->GetBinLowEdge(4)&&trkEnergy<hPt->GetBinLowEdge(5)) {
            metx3+=pptx;
            mety3+=ppty;
            if (fabs(pdr[0])<0.8||fabs(pdr[1])<0.8) metConex3+=pptx; else metOutOfConex3+=pptx;
          } else if (trkEnergy>=hPt->GetBinLowEdge(5)&&trkEnergy<hPt->GetBinLowEdge(6)) { 
            metx4+=pptx;
            mety4+=ppty;
            if (fabs(pdr[0])<0.8||fabs(pdr[1])<0.8) metConex4+=pptx; else metOutOfConex4+=pptx;
          } else if (trkEnergy>=hPt->GetBinLowEdge(6)&&trkEnergy<hPt->GetBinLowEdge(7)) { 
            metx5+=pptx;
            mety5+=ppty;
            if (fabs(pdr[0])<0.8||fabs(pdr[1])<0.8) metConex5+=pptx; else metOutOfConex5+=pptx;
          }

	  // =====================================================
	  // Calculate Cone Sums
	  // =====================================================
	  // cone sum
	  for (Int_t j=0; j<2; ++j) {
	    for (Int_t b=0; b<numPtBins; ++b) {
	      if (trkEnergy>=hPt->GetBinLowEdge(b+1)&&trkEnergy<hPt->GetBinLowEdge(b+2)) {
		// Signal Cone
		if (pdr[j]<cut.ConeSize) {
		  jc_.cpt[j][b]+=trkEnergy*trackWeight;
		  jc_.cptpara[j][b]+=cos(reco::deltaPhi(p_[i].phi(),anaJets_[0].phi()))*trkEnergy*trackWeight;
		}
		// Bkg Cone
		if (pdrbg[j]<cut.ConeSize) {
		  jc_.cptbg[j][b]+=trkEnergy*trackWeight;
		  jc_.cptparabg[j][b]+=cos(reco::deltaPhi(p_[i].phi(),anaJets_[0].phi()))*trkEnergy*trackWeight;
		}
		break;
	      }
	    }
	  }

	  // =====================================================
          // Take the reweighting into account for later histogram
          // **This should not be applied before the met calculation.**
	  // =====================================================
          trackWeight *= weight;

	  // =====================================================
	  // Fill Jet-Particle Histograms
	  // =====================================================
	  for (Int_t j=0; j<2; ++j) {
	    if (jetEvt[j]) {
	      // Signal Cone
	      if (pdr[j]<cut.ConeSize) {
		conePtSum[j]+=trkEnergy*trackWeight;
		hCPPt[j]->Fill(trkEnergy,trackWeight);
		hPtPDR[j]->Fill(trkEnergy,pdr[j],trkEnergy*trackWeight);
	      }
	      // Background Cone
	      if (pdrbg[j]<cut.ConeSize) {
		hCPPtBg[j]->Fill(trkEnergy,trackWeight);
		conePtBgSum[j]+=trkEnergy*trackWeight;
		hPtPDRBg[j]->Fill(trkEnergy,pdrbg[j],trkEnergy*trackWeight);
	      }
	    }
	  }
	} // end of particles loop
	//GetEntry(jentry);

	// =====================================================
	// Fill Cone Sums
	// =====================================================
	for (Int_t j=0; j<2; ++j) {
	  if (jetEvt[j]) {
	    hCPt[j]->Fill(conePtSum[j]);
	    hCPtBg[j]->Fill(conePtBgSum[j]);
	    hCPtBgSub[j]->Fill(conePtSum[j]-conePtBgSum[j]);
	  }
	}

	// =====================================================
	// Fill Ntuple
	// =====================================================
        Float_t var[100];
	var[0]=anaJets_[0].pt();
	var[1]=anaJets_[0].eta();
	var[2]=anaJets_[0].phi();
	var[3]=anaJets_[1].pt();
	var[4]=anaJets_[1].eta();
	var[5]=anaJets_[1].phi();
	var[6]=metx;
	var[7]=metx0;
	var[8]=metx1;
	var[9]=metx2;
	var[10]=metx3;
	var[11]=metx4;
	var[12]=metx5;
	var[13]=mety;
	var[14]=mety0;
	var[15]=mety1;
	var[16]=mety2;
	var[17]=mety3;
	var[18]=mety4;
	var[19]=mety5;
	var[20]=metConex;
	var[21]=metConex0;
	var[22]=metConex1;
	var[23]=metConex2;
	var[24]=metConex3;
	var[25]=metConex4;
	var[26]=metConex5;
	var[27]=metOutOfConex;
	var[28]=metOutOfConex0;
	var[29]=metOutOfConex1;
	var[30]=metOutOfConex2;
	var[31]=metOutOfConex3;
	var[32]=metOutOfConex4;
	var[33]=metOutOfConex5;
	var[34]=GetEvtMask();
	var[35]=cent;
	var[36]=jdphi;
	var[37]=weight;
	ntjt->Fill(var);    // fit ntuple

	tcone->Fill();
      } // End of Main Event Selection
      // if (Cut(ientry) < 0) continue;
   }

   // =====================================================
   // Summarize Event Loop
   // =====================================================
   cout << "Total Events: " << numTotEvt << endl;
   cout << "DiJets Selected w/o Bkg Limit: " << numDJNoBkgLimit << " (same as draw cut unless there is jet eta correction)" << endl;
   cout << "DiJets Selected: " << numDJ_ << " Reweighted: " << numDJReWeighted_ << endl;
   for (Int_t j=0; j<2; ++j) {
     cout << "num Jet" << j << " Selected: " << numJ_[j] << " Reweighted: " << numJReWeighted_[j] << endl;
   }

   // =====================================================
   // Normalize by Number of Selected Events
   // =====================================================
   hJDPhi->Scale(1./(numDJReWeighted_));
   hJDEta->Scale(1./(numDJReWeighted_));
   hAj->Scale(1./(numDJReWeighted_));
   hCentReWeighted->Scale(1./(numDJReWeighted_));

   for (Int_t j=0; j<2; ++j) {
     hJEt[j]->Scale(1./(numJReWeighted_[j]));
     hJEta[j]->Scale(1./(numJReWeighted_[j]));

     hCPPt[j]->Scale(1./numJReWeighted_[j]);
     hCPPtBg[j]->Scale(1./numJReWeighted_[j]);
     hCPPtBgSub[j]->Add(hCPPt[j],hCPPtBg[j],1,-1);

     hCPt[j]->Scale(1./(numJReWeighted_[j]));
     hCPtBg[j]->Scale(1./(numJReWeighted_[j]));
     hCPtBgSub[j]->Scale(1./(numJReWeighted_[j]));

     hPtPDR[j]->Scale(1./(numJReWeighted_[j]));
     hPtPDRBg[j]->Scale(1./(numJReWeighted_[j]));
   }
}

//
// Set 
//
void JetFragAna::SetJetTree(Int_t mode, TString tfile, TString tname)
{
  jetTreeMode_=mode;

  // GetTree
  jetTree_[mode] = new TChain(tname);
  jetTree_[mode]->Add(tfile);
  
   jetTree_[mode]->SetBranchAddress("nljet",&vj_[mode].nljet);
   jetTree_[mode]->SetBranchAddress("nljeta",&vj_[mode].nljeta);
   jetTree_[mode]->SetBranchAddress("nljphi",&vj_[mode].nljphi);
   jetTree_[mode]->SetBranchAddress("aljet",&vj_[mode].aljet);
   jetTree_[mode]->SetBranchAddress("aljeta",&vj_[mode].aljeta);
   jetTree_[mode]->SetBranchAddress("aljphi",&vj_[mode].aljphi);  
}
