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
      // if use fit
      TF1 * fr = new TF1("fr","[0]+[1]*log(x-85)",0,500);
      fr->SetParameters(0.88,0.03);
      if (useFit) ratio[icent]->Fit("fr");
    }
  }

  float GetWeight(float x, int icent, float rmin=-999, float rmax=999) {
    assert(ratio[icent]);
    float wt = 1;

    if (useFit==1) {
      if (x>100) wt = ratio[icent]->GetFunction("fr")->Eval(x);
    } else  {
      wt = ratio[icent]->GetBinContent( ratio[icent]->FindBin(x) );
    }

    if (wt<rmin) wt=rmin;
    if (wt>rmax) wt=rmax;
    return wt;
  }
};

// --- Main Function ----------------------------------------------------------
void addMultiWeight(
  TString infnt="../ntout/jskim_hydj80_akPu3PF_Mar04v3job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root",
  bool isPP=false,
  int rewtBin=1,
  int doMC=1
  ) {
  ////////////////////////////////////////////
  // Setup Inputs
  ////////////////////////////////////////////
  ReweightCorrection wcJetRatio("wcJetRatio",0,1); // pp reweighting
  if (!isPP) wcJetRatio.wtType = 1; // calo90 efficiency correction
  ReweightCorrection wcJetRelBias("wcJetRelBias",0);
  ReweightCorrection wcJetBias("wcJetBias",1);
  ReweightCorrection wcBkgBias("wcBkgBias",1);

  TString dataset="hi";
  int ppsm=0;
  if (isPP) {
    dataset="pp";
    if (infnt.Contains("_sm2bin")) ppsm=2;
  }

  // Load histograms
  TH1D * bkg[5];
  TString infjetwt="His/Mar04/jet_spectrum_cmp4_sm1_rewt0_evsel1_Mar04v3job0.root";
  if (isPP) infjetwt=Form("His/Mar04/jet_spectrum_cmp1_sm%d_rewt0_evsel0_Mar04v3job0.root",ppsm);
  TString infhisanahi="His/Mar04/inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_jdr0.3_Mar04job0_data_mc80to170_hi_pp_corrjbias0_v2.root";
  TString infhisanapp="His/Mar04/inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_jdr0.3_Mar04v2job0_data_mc80to170_hi_pp_corrjbias0_v2.root";
  TString infhisanahi_refjetgt100="His/Mar04/inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_jdr0.3_Mar04job0_mc80to170_refjetgt100_hi_pp_corrjbias0_v2.root";
  TString infhisanapp_refjetgt100="His/Mar04/inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_jdr0.3_Mar04v2job0_mc80to170_refjetgt100_hi_pp_corrjbias0_v2.root";
  for ( int icent=1; icent<=4 ; icent++) {
    // data jet reweighting
    wcJetRatio.ana[icent]   = (TH1D*)load(infjetwt, Form("hjetPt_hi_inclusiveJet_icent%d",icent));
    wcJetRatio.ref[icent]   = (TH1D*)load(infjetwt, Form("hjetPt_pp_inclusiveJet_icent%d",icent));

    // mc jet reco bias relative
    wcJetRelBias.ana[icent] = (TH1D*)load(infhisanahi, Form("hpt_jet_sigTrk_himc_icent%d_irj999_fragMode1_closure101_rewt0_ppsm0_wtmode0_pt1to300",icent));
    wcJetRelBias.ref[icent] = (TH1D*)load(infhisanapp, Form("hpt_jet_sigTrk_ppmc_icent%d_irj999_fragMode1_closure101_rewt0_ppsm%d_wtmode0_pt1to300",icent,ppsm));
    // factorized correction for pp to match pbpb
    bkg[icent]              = (TH1D*)load(infhisanahi, Form("hpt_jet_mbTrk_himc_icent%d_irj999_fragMode1_closure101_rewt0_ppsm0_wtmode0_pt1to300",icent));
    wcJetRelBias.ana[icent]->Add(bkg[icent]);
    wcJetRelBias.ref[icent]->Add(bkg[icent]);

    // mc jet reco bias
    TString temphisana=infhisanahi, temphisref=infhisanahi_refjetgt100;
    if (isPP) {temphisana=infhisanapp; temphisref=infhisanapp_refjetgt100;};
    wcJetBias.ana[icent] = (TH1D*)load(temphisana, Form("hpt_jet_sigTrk_%smc_icent%d_irj999_fragMode1_closure101_rewt0_ppsm0_wtmode0_pt1to300",dataset.Data(),icent));
    wcJetBias.ref[icent] = (TH1D*)load(temphisref, Form("hpt_jet_sigTrk_%smc_icent%d_irj999_fragMode1_closure101_rewt0_ppsm%d_wtmode0_pt1to300",dataset.Data(),icent,ppsm));
    if (!isPP) {
      wcJetBias.ana[icent]->Add(bkg[icent]);
      wcJetBias.ref[icent]->Add(bkg[icent]);
    }

    // jet background bias
    wcBkgBias.ana[icent] = (TH1D*)load(infhisanahi, Form("hpt_jet_mbTrk_himc_icent%d_irj999_fragMode1_closure102_rewt0_ppsm0_wtmode0_pt1to300",icent));
    wcBkgBias.ref[icent]  = (TH1D*)load(infhisanahi, Form("hpt_jet_mbTrk_himc_icent%d_irj999_fragMode1_closure101_rewt0_ppsm0_wtmode0_pt1to300",icent));
  }
  
  ////////////////////////////////////////////
  // Make Corrections
  ////////////////////////////////////////////
  wcJetRatio.Init();
  wcJetRelBias.Init();
  wcJetBias.Init();
  wcBkgBias.Init();


  TCanvas* c1 = new TCanvas("c1","",1000,600);
  c1->Divide(4,2);
  Plot4x4 p1(wcJetRatio.ana,wcJetRatio.ref,wcJetRatio.ratio,wcJetRatio.normMode,wcJetRatio.name);
  p1.Draw(c1,100,299.9);

  TCanvas* c2 = new TCanvas("c2","",1000,600);
  c2->Divide(4,2);
  Plot4x4 p2(wcJetRelBias.ana,wcJetRelBias.ref,wcJetRelBias.ratio,wcJetRelBias.normMode,wcJetRelBias.name);
  p2.Draw(c2,1,103);

  TCanvas* c3 = new TCanvas("c3","",1000,600);
  c3->Divide(4,2);
  Plot4x4 p3(wcJetBias.ana,wcJetBias.ref,wcJetBias.ratio,wcJetBias.normMode,wcJetBias.name);
  p3.Draw(c3,1,103);

  TCanvas* c4 = new TCanvas("c4","",1000,600);
  c4->Divide(4,2);
  Plot4x4 p4(wcBkgBias.ana,wcBkgBias.ref,wcBkgBias.ratio,wcBkgBias.normMode,wcBkgBias.name);
  p4.Draw(c4,1,103);

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
  int nTrk=0;
  float trkPt[MAXTRK];
  int trkJetMatch[MAXTRK];
  TTree* yongsunTrackin = (TTree*)inf1->Get("yongsunTrack");
  assert(yongsunTrackin);
  yongsunTrackin->SetBranchAddress("ntrk",&nTrk);
  yongsunTrackin->SetBranchAddress("pt",trkPt);
  yongsunTrackin->SetBranchAddress("jetMatch",trkJetMatch);

  const int maxGenPar = 25000;
  int nGenPar=0;
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
  // outputFile.ReplaceAll(".root","_addedFitRewt.root");
  if (isPP && infnt.Contains("_sm0bin0")) outputFile.ReplaceAll("_sm0bin0",Form("_sm0bin%d",rewtBin));
  TFile* newfile = new TFile(outputFile,"recreate");
  cout << "Output file :" << outputFile << endl;
    
  JetReweight jetRewt;
  TTree* tjnew = (TTree*)tjin->CloneTree(0);
  tjnew->Branch("jetRewt",&jetRewt.inputPt,"inputPt/F:rewtPt:rewtCent:rewtAj");

  TTree* newyongsunTrack= (TTree*)yongsunTrackin->CloneTree(0);
  float trkJetRelBiasWt[MAXTRK], trkJetBiasWt[MAXTRK];
  float trkBkgBiasWt[MAXTRK];
  newyongsunTrack->Branch("jetRelBiasWt",trkJetRelBiasWt,"jetRelBiasWt[ntrk]/F");
  newyongsunTrack->Branch("jetBiasWt",trkJetBiasWt,"jetBiasWt[ntrk]/F");
  newyongsunTrack->Branch("bkgBiasWt",trkBkgBiasWt,"bkgBiasWt[ntrk]/F");

  TTree * newGenPar;
  float genParJetRelBiasWt[maxGenPar], genParJetBiasWt[maxGenPar];
  float genParBkgBiasWt[maxGenPar];
  if ( doMC && genParIn) {
    newGenPar = (TTree*)genParIn->CloneTree(0);
    newGenPar->Branch("jetRelBiasWt",genParJetRelBiasWt,"jetRelBiasWt[nGenPar]/F");
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
    jetRewt.rewtPt = 1.;
    if (rewtBin>0) jetRewt.rewtPt = wcJetRatio.GetWeight(jetRewt.inputPt,rewtBin);

    ////////////////////////////////
    // Get weights to track branches
    ////////////////////////////////
    for (int it=0; it<nTrk; ++it) {
      trkJetRelBiasWt[it]=1;
      trkJetBiasWt[it]=1;
      trkBkgBiasWt[it]=1;
      if (trkJetMatch[it]==1) {
        if (isPP) trkJetRelBiasWt[it]=wcJetRelBias.GetWeight(trkPt[it],rewtBin);
        trkJetBiasWt[it] = wcJetBias.GetWeight(trkPt[it],rewtBin);
      } else if (trkJetMatch[it]==-1) {
        if (!isPP&&rewtBin<=2) trkBkgBiasWt[it]=wcBkgBias.GetWeight(trkPt[it],rewtBin,0.8,1.2);
      }
    }

    ////////////////////////////////
    // Get weights to gen particle branches
    ////////////////////////////////
    for (int ig=0; ig<nGenPar; ++ig) {
      genParJetRelBiasWt[ig]=1;
      genParJetBiasWt[ig]=1;
      genParBkgBiasWt[ig]=1;
      if (genParJetMatch[ig]==1) {
        if (isPP) genParJetRelBiasWt[ig]=wcJetRelBias.GetWeight(genParPt[ig],rewtBin);
        genParJetBiasWt[ig] = wcJetBias.GetWeight(genParPt[ig],rewtBin);
      } else if (genParJetMatch[ig]==-1) {
        if (!isPP&&rewtBin<=2) genParBkgBiasWt[ig]=wcBkgBias.GetWeight(genParPt[ig],rewtBin,0.8,1.2);
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