#include <iostream>
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TROOT.h"
#include "/home/frankma/UserCode/SavedFMa/analysis/root/macros/savedfrankTools.C"
#include "DiJets.h"
#include <vector>
using namespace jetana;
using namespace std;

void plotFF(//char * infname1 = "/d01/frankma/scratch/HI/jet/pat/patanaCmssw331/hydjetNoCollElossFix03/tree_djana_j2_1_t1_RECO_runs1to40_sw43.root",
    //char * infname1 = "/d01/frankma/scratch/data/ana/hydjet_noColl_dijet_2009_10_1/merged/back/tree_djana_j2_1_t1_RECO_all40k_sw44.root",
    char * infname1 = "/d01/frankma/scratch/data/ana/hydjet_noColl_dijet_2009_10_1/tree_djana_j2_1_t1_RECO_*to*_sw44.root",
    // -pythia-
    Double_t NJetEtMin = 65,
    Double_t NJetEtMax = 200,
    Double_t AJetEtMin = 30,
    Double_t JDPhiMin = 3.14-0.5,
    char * plotdir = "plots1",
    const Int_t NXIBIN = 30,
    const Double_t XIMAX = 6,
    const Double_t XIYMAX = 10,
    Bool_t check = kTRUE
    )
{
  // === sanity checks ===
  bool NoNorm = false;
  cout << NJetEtMin << " " << NJetEtMax << " " << AJetEtMin << endl;

  //=== Get input dijet tree ===
  //TFile * infile = findFile(infname1);
  //TTree * djTree = findTree(infile,"dijetTree");
  TChain * djTree = new TChain("dijetTree");
  djTree->Add(infname1);

  //---output---
  TFile * outfile = new TFile(Form("%s/FFHistos.root",plotdir),"RECREATE");

  // === setup cuts ===
  printf("\n=========================Cuts setup======================\n");
  DiJets treeana("dijet","vjet1",djTree);
  treeana.SetNearJetEtMin(NJetEtMin);
  treeana.SetNearJetEtMax(NJetEtMax);
  treeana.SetAwayJetEtMin(AJetEtMin);

  // == common cuts, actions==
  vector<DiJets*> vgen;
  vgen.push_back(&treeana);
  for (int ig=0; ig<vgen.size(); ++ig) {
    // make cut
    vgen[ig]->SetDPhiMin(JDPhiMin);
    vgen[ig]->SetJetPartlDRMax(0.5);
    vgen[ig]->SetPartlPtMin(2.);
    vgen[ig]->CreateCuts();
    // dijet cuts
    vgen[ig]->cut_.AndJetCut("lajmul",1,"equ");
    vgen[ig]->cut_.AndJetParticlesCut("lajmul",1,"equ");
    // output
    vgen[ig]->SetVerbosity(2);
    //cout << *(vgen[ig]) << endl;

    // -norm-
    printf("now jet cut: \n  %s\n",vgen[ig]->GetCut().GetDiJetCut().Data());
    vgen[ig]->CalcNumDiJets();
    printf("# of dijets after cut: %d, normalization: %f\n",vgen[ig]->GetNumDiJets(),vgen[ig]->GetDiJetsNorm());
    printf("===================\n\n");
  }
  // ===

  // hist styles
  const Color_t nrColor = kBlack;
  const Color_t awColor = kBlue;
  const Color_t nrStyle = 1;
  const Color_t awStyle = 1;

  if (check) {
    printf("\n============================Do Checks======================\n");
    printf("%s\n",infname1);
    //---- Check dijet properties ----
    //--- inv mass ---
    //drawTree(djTree, "mass","",drsgFF,"hUncutMass","Dijet Inv Mass [GeV];",100,0,1800,true,kBlack,1,3);
    // --from jet leading--

    //=== Particles ===
    //--- inclusive angular correlations to leading jets---
    //drawTree(djTree, "pndphi","",drsgFF,"hnPdPhiJet","Jet: dphi to near jet",100,-0.5*PI,1.5*PI,false,kBlack,1,3);
    //drawTree(djTree, "padphi","",drdbFF,"haPdPhiJet","Jet: dphi to away jet",100,-0.5*PI,1.5*PI,false,kBlack,1,3);
  }
  
  // ===== ana plots =====
  // Hi event
  drawTree(djTree, "b","",drsgFF,"hUncutB",";b [fm];",HBBINS,HBMIN,HBMAX,0,kBlack,1,1,1,8);
  drawTree(djTree, "b",treeana.GetCut().GetDiJetCut().Data(),drdbFF,"hB",";b [fm];",HBBINS,HBMIN,HBMAX,0,kBlack,1,3,1,8);

  //=== Particles ===
  drawTree(djTree, "ppt","pndr<0.5",drsgFF,"hNPPt",";Pt of near/away particles [GeV];",100,0,10,true,nrColor,nrStyle,3);
  drawTree(djTree, "ppt","padr<0.5",drdbFF,"hAPPt",";Pt of away particles [GeV];",100,0,10,true,awColor,awStyle,3);

  // === jet selection ===
  drawTree(djTree, "nljet","",drsgFF,"hUncutNJetEt",";Jet Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX,true,nrColor,nrStyle,1,1,8);
  drawTree(djTree, "aljet","",drdbFF,"hUncutAJetEt",";Jet Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX,true,awColor,awStyle,1,1,8);
  //-- *apply cuts* --
  drawTree(djTree, "nljet",treeana.GetCut().GetDiJetCut().Data(),drdbFF,"hNJetEt",";Jet Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX,true,nrColor,nrStyle,3,1,8);
  drawTree(djTree, "aljet",treeana.GetCut().GetDiJetCut().Data(),drdbFF,"hAJetEt",";Jet Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX,true,awColor,awStyle,3,1,4);

  // Dijets
  drawTree(djTree, "jdphi","",drsgFF,"hUncutJDPhi",";Dijet d#phi;",40,3.14/2,3.14,0,nrColor,nrStyle,1,1,8);
  drawTree(djTree, "jdphi","",drsgFF,"hJDPhi",";Dijet d#phi;",40,3.14/2,3.14,0,nrColor,nrStyle,1,1,8);

  // ratio
  drawTree2(djTree, "aljet:nljet","","colz","hUncutNJetEtvsAJetEt",";near Jet E_{t} [GeV];away Jet E_{t} [GeV]",HJETETBINS,HJETETMIN,HJETETMAX,HJETETBINS,HJETETMIN,HJETETMAX,1);
  drawTree2(djTree, "aljet:nljet",treeana.GetCut().GetDiJetCut().Data(),"colz","hNJetEtvsAJetEt",";near Jet E_{t} [GeV];away Jet E_{t} [GeV]",HJETETBINS,HJETETMIN,HJETETMAX,HJETETBINS,HJETETMIN,HJETETMAX,1);
  // rato vs b
  drawTree2(djTree, "aljet/nljet:b","","colz","hUncutANRatVsB",";b [fm];E_{t}^{away jet}/E_{t}^{near jet}",HBBINS,HBMIN,HBMAX,HJETETBINS,0,1,1);
  drawTree2(djTree, "aljet/nljet:b",treeana.GetCut().GetDiJetCut().Data(),"colz","hANRatVsB",";b [fm];E_{t}^{away jet}/E_{t}^{near jet}",HBBINS,HBMIN,HBMAX,HJETETBINS,0,1,1);

  //=== Finally: plot fragmentation properties ===
  float nJetNorm = treeana.GetDiJetsNorm();
  if (NoNorm) nJetNorm = 1.;
  printf("nDiJets: %d,  jnorm: %f\n",treeana.GetNumDiJets(),nJetNorm);

  printf("\n===================== Plot FF =======================\n");
  int log=0;
  printf("  - with jet cut: \n  %s\n", vgen[0]->GetCut().GetDiJetCut().Data());
  printf("  - and jet particle cut:\n  %s\n  %s\n",vgen[0]->GetCut().GetNJetPartlCut().Data(),
      vgen[0]->GetCut().GetAJetPartlCut().Data());
  // -near-
  drawTree(djTree, "log(1/zn)",treeana.GetCut().GetNJetPartlCut().Data(),drsgFF,"hXiNearJet",";#xi=ln(E_{t}^{Jet}/E_{t}^{Particle});#frac{1}{N_{jet}} #frac{dN}{d#xi}",NXIBIN,0,XIMAX,log,nrColor,nrStyle,3,2,kFullDotLarge,nJetNorm,XIYMAX);
  // -away-
  drawTree(djTree, "log(1/za)",treeana.GetCut().GetAJetPartlCut().Data(),drdbFF,"hXiAwayJet",";#xi=ln(E_{t}^{Jet}/E_{t}^{Particle});#frac{1}{N_{jet}} #frac{dN}{d#xi}",NXIBIN,0,XIMAX,log,awColor,awStyle,3,2,kCircle,nJetNorm,XIYMAX);

  //=== Save and exit ===
  printAllCanvases(plotdir);
  outfile->Write();
  outfile->Close();
}
