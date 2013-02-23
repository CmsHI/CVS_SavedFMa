#include <iostream>
#include <iomanip>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TString.h"
#include "TCut.h"
#include <TRandom3.h>
#include <time.h>
#include "CutAndBinCollection2011.h"
#include "HisMath.C"
#include "HIN-12-013.h"
using namespace std;

// --- Helper functions ----------------------------------------------------------
class JetReweight {
  public:
  float  inputPt,rewtPt,rewtCent,rewtAj;
};

int FindAnaCentBin(int cBin) {
  if (cBin<0||cBin>=40) {
    cout << endl << endl << "!!!!!! cBin=" << cBin << " is out of centrality table range !!!!!!" << endl << endl;
    return 3;
  }
  int anaCent;
  for (int ic=0; ic<4; ++ic) {
    if (cBin>=centBin1[ic]&&cBin<centBin1[ic+1]) {
      anaCent=ic+1;
      break;
    }
  }
  return anaCent;
}

class ReweightCorrection {
public:
  // setup
  TString name;
  int wtType; // 0: ana/ref (reweight type corr), 1: ref/ana (eff type corr.)
  int normMode;
  int useFit;

  // member data
  TH1D * ana[5];
  TH1D * ref[5];
  TH1D * ratio[5];

  ReweightCorrection(TString myname, int type=0, int mymode=0, int fit=0)
  : name(myname)
  , wtType(type), normMode(mymode), useFit(fit)
  {}

  // member functions
  void Init() {
    // Get ratios
    for ( int icent=1; icent<=4 ; icent++) {
      assert(ana[icent]);
      assert(ref[icent]);
      ratio[icent]  = (TH1D*)ana[icent]->Clone(Form("%s_ratio_icent%d",name.Data(),icent));
      ratio[icent]->Reset();
      normHist(ana[icent],normMode,true);
      normHist(ref[icent],normMode,true);
      if (wtType==0) ratio[icent]->Divide(ana[icent],ref[icent]);
      else if (wtType==1) ratio[icent]->Divide(ref[icent],ana[icent]);
    }
  }

  float GetWeight(float x, int icent) {
    assert(ratio[icent]);
    float wt = 1;

    if (useFit==1) {
      wt = ratio[icent]->GetFunction("fr")->Eval(x);
    } else  {
      wt = ratio[icent]->GetBinContent( ratio[icent]->FindBin(x) );
    }
    return wt;
  }
};

// --- Main Function ----------------------------------------------------------
void addMultiWeight(
  // TString infnt="../ntout/jskim_dj80_ak3PF_Feb14v2_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm2bin1_ak3PF_gj0.root",
  // bool isPP=true,
  // int rewtBin=1,
  // int doMC=1
  TString infnt="../ntout/jskim_hydj80_akPu3PF_Feb14v2_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root",
  bool isPP=false,
  int rewtBin=1,
  int doMC=1
  // TString infnt="../ntout/jskim_pp-full_ak3PF_Feb14v2_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root",
  // bool isPP=true,
  // int rewtBin=1,
  // int doMC=0
  ) {
  ////////////////////////////////////////////
  // Setup Inputs
  ////////////////////////////////////////////
  ReweightCorrection wcJetRatio("wcJetRatio",0,1);
  ReweightCorrection wcJetBias("wcJetBias",1);
  ReweightCorrection wcBkgBias("wcBkgBias",1);

  TString dataset="hi";
  int ppsm=0;
  if (isPP) {
    dataset="pp";
    if (infnt.Contains("_sm2bin")) ppsm=2;
  }

  // Load histograms
  for ( int icent=1; icent<=4 ; icent++) {
    wcJetRatio.ana[icent]   = (TH1D*)load("fig/Jan17/jet_spectrum_cmp1_sm2_rewt0_evsel0_pbpbJan17_ppJan17.root",
      Form("hjetPt_hi_inclusiveJet_icent%d",icent));
    wcJetRatio.ref[icent]    = (TH1D*)load("fig/Jan17/jet_spectrum_cmp1_sm2_rewt0_evsel0_pbpbJan17_ppJan17.root",
      Form("hjetPt_pp_inclusiveJet_icent%d",icent));

    wcJetBias.ana[icent] = (TH1D*)load("His/Feb19/inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Feb14v2_data_mc80to170_hi_pp.root",
      Form("hpt_jet_rawTrk_%smc_icent%d_irj999_fragMode1_closure100_rewt0_ppsm%d_wtmode0_pt1to300",dataset.Data(),icent,ppsm));
    wcJetBias.ref[icent]  = (TH1D*)load("His/Feb19/inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Feb14v2_mc80to170_refjetsel_hi_pp.root",
      Form("hpt_jet_rawTrk_%smc_icent%d_irj999_fragMode1_closure100_rewt0_ppsm%d_wtmode0_pt1to300",dataset.Data(),icent,ppsm));

    wcBkgBias.ana[icent] = (TH1D*)load("His/Feb19/inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Feb14v2_data_mc80to170_hi_pp.root",
      Form("hpt_jet_mbTrk_%smc_icent%d_irj999_fragMode1_closure102_rewt0_ppsm%d_wtmode0_pt1to300",dataset.Data(),icent,ppsm));
    wcBkgBias.ref[icent]  = (TH1D*)load("His/Feb19/inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Feb14v2_data_mc80to170_hi_pp.root",
      Form("hpt_jet_mbTrk_%smc_icent%d_irj999_fragMode1_closure101_rewt0_ppsm%d_wtmode0_pt1to300",dataset.Data(),icent,ppsm));
  }
  
  ////////////////////////////////////////////
  // Make Corrections
  ////////////////////////////////////////////
  wcJetRatio.Init();
  wcJetBias.Init();
  wcBkgBias.Init();

  TCanvas* c1 = new TCanvas("c1","",1000,600);
  c1->Divide(4,2);
  Plot4x4 p1(wcJetRatio.ana,wcJetRatio.ref,wcJetRatio.normMode,wcJetRatio.name);
  p1.Draw(c1,100,299.9);

  TCanvas* c2 = new TCanvas("c2","",1000,600);
  c2->Divide(4,2);
  Plot4x4 p2(wcJetBias.ana,wcJetBias.ref,wcJetBias.normMode,wcJetBias.name);
  p2.Draw(c2,1,103);

  TCanvas* c3 = new TCanvas("c3","",1000,600);
  c3->Divide(4,2);
  Plot4x4 p3(wcBkgBias.ana,wcBkgBias.ref,wcBkgBias.normMode,wcBkgBias.name);
  p3.Draw(c3,1,103);

  ////////////////////////////////////////////
  // Setup input and output trees
  ////////////////////////////////////////////
  TFile* inf1 = new TFile(infnt);
  TTree * tjin = (TTree*)inf1->Get("tdj");

  // Set Input Jet trees ---------------
  EvtSel evt;
  IndividualJet dj;
  TBranch        *b_evt;
  TBranch        *b_dj;
  tjin->SetBranchAddress("evt", &evt, &b_evt);
  tjin->SetBranchAddress("indiJet", &dj, &b_dj);

  // Set Input Track trees ---------------
  int nTrk;
  float trkPt[MAXTRK];
  int trkJetMatch[MAXTRK];
  TTree* yongsunTrackin = (TTree*)inf1->Get("yongsunTrack");
  assert(yongsunTrackin);
  yongsunTrackin->SetBranchAddress("ntrk",&nTrk);
  yongsunTrackin->SetBranchAddress("pt",trkPt);
  yongsunTrackin->SetBranchAddress("jetMatch",trkJetMatch);

  const int maxGenPar = 25000;
  int nGenPar;
  float genParPt[maxGenPar];
  int   genParJetMatch[maxGenPar];
  TTree * genParIn = (TTree*)inf1->Get("genPar");
  if ( doMC && genParIn) {
    genParIn->SetBranchAddress("nGenPar",&nGenPar);
    genParIn->SetBranchAddress("pt",genParPt);
    genParIn->SetBranchAddress("jetMatch",genParJetMatch);
  }

  // Set Output Jet trees ---------------
  TString outputFile=infnt;
  outputFile.ReplaceAll(".root","_addedReweight.root");
  TFile* newfile = new TFile(outputFile,"recreate");
  cout << "Output file :" << outputFile << endl;
    
  JetReweight jetRewt;
  TTree* tjnew = (TTree*)tjin->CloneTree(0);
  tjnew->Branch("jetRewt",&jetRewt.inputPt,"inputPt/F:rewtPt:rewtCent:rewtAj");

  TTree* newyongsunTrack= (TTree*)yongsunTrackin->CloneTree(0);
  float trkJetBiasWt[MAXTRK];
  float trkBkgBiasWt[MAXTRK];
  newyongsunTrack->Branch("jetBiasWt",trkJetBiasWt,"jetBiasWt[ntrk]/F");
  newyongsunTrack->Branch("bkgBiasWt",trkBkgBiasWt,"bkgBiasWt[ntrk]/F");

  TTree * newGenPar;
  float genParJetBiasWt[maxGenPar];
  float genParBkgBiasWt[maxGenPar];
  if ( doMC && genParIn) {
    newGenPar = (TTree*)genParIn->CloneTree(0);
    newGenPar->Branch("jetBiasWt",genParJetBiasWt,"jetBiasWt[nGenPar]/F");
    newGenPar->Branch("bkgBiasWt",genParBkgBiasWt,"bkgBiasWt[nGenPar]/F");
  }

  ////////////////////////////////////////////
  // Loop tree and add Weights
  ////////////////////////////////////////////
  Long64_t nentries = tjin->GetEntries();
  for (Long64_t jentry=0;  jentry<nentries;jentry++) {
    // Load entries
    tjin->GetEntry(jentry);
    yongsunTrackin->GetEntry(jentry);
    if ( doMC && genParIn) genParIn->GetEntry(jentry);

    // Get analysis Centrality bin
    if (!isPP) rewtBin = FindAnaCentBin(evt.cBin);
    if (jentry% 1000 == 0) {
      cout <<jentry<<" / "<<nentries<<" "<<setprecision(4)<<(double)jentry/nentries*100
      << " cBin: " << evt.cBin << " rewtBin: " << rewtBin << " nTrk: " << nTrk << " nGenPar: " << nGenPar << endl;
    }

    ////////////////////////////////
    // Get weights to jet branches
    ////////////////////////////////
    jetRewt.inputPt = dj.jetPt;
    if (rewtBin>0) jetRewt.rewtPt = wcJetRatio.GetWeight(jetRewt.inputPt,rewtBin);
    else jetRewt.rewtPt = 1.;

    ////////////////////////////////
    // Get weights to track branches
    ////////////////////////////////
    for (int it=0; it<nTrk; ++it) {
      trkJetBiasWt[it]=1;
      trkBkgBiasWt[it]=1;
      if (trkJetMatch[it]==1) {
        trkJetBiasWt[it]=wcJetBias.GetWeight(trkPt[it],rewtBin);
      } else if (trkJetMatch[it]==-1) {
        if (!isPP) trkBkgBiasWt[it]=wcBkgBias.GetWeight(trkPt[it],rewtBin);
      }
    }

    ////////////////////////////////
    // Get weights to gen particle branches
    ////////////////////////////////
    for (int ig=0; ig<nGenPar; ++ig) {
      genParJetBiasWt[ig]=1;
      if (genParJetMatch[ig]==1) {
        genParJetBiasWt[ig]=wcJetBias.GetWeight(genParPt[ig],rewtBin);
      } else if (genParJetMatch[ig]==-1) {
        if (!isPP&&rewtBin<3) genParBkgBiasWt[ig]=wcBkgBias.GetWeight(genParPt[ig],rewtBin);
      }
    }

    // Fill tree
    tjnew->Fill();
    newyongsunTrack->Fill();
    if ( doMC && newGenPar) newGenPar->Fill();
  }

  ////////////////////////////////////////////
  // All done, wrap-up and save
  ////////////////////////////////////////////
  tjnew->AutoSave();
  newyongsunTrack->AutoSave();
  if ( doMC && newGenPar) newGenPar->AutoSave();
  // other needed histograms
  TH1D * hEvtCentNoSkim = (TH1D*)inf1->Get("hEvtCentNoSkim");
  newfile->cd();
  hEvtCentNoSkim->Write();
  newfile->Close();
}