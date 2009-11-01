#include <iostream>
#include "TFile.h"
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

void plotFF(char * infname1 = "/d01/frankma/scratch/HI/jet/pat/patanaCmssw331/hydjetNoCollElossFix03/tree_djana_j2_1_t1_RECO_runs1to40_sw43.root",
    // -pythia-
    Double_t NJetEtMin = 50,
    Double_t NJetEtMax = 200,
    Double_t AJetEtMin = 25,
    Double_t JDPhiMin = 2.6,
    char * plotdir = "plots",
    const Int_t NXIBIN = 10,
    const Double_t XIMAX = 9.,
    const Double_t XIYMAX = 10,
    Bool_t check = kTRUE
    )
{
  // === sanity checks ===
  bool NoNorm = false;
  cout << NJetEtMin << " " << NJetEtMax << " " << AJetEtMin << endl;

  //=== Get input dijet tree ===
  TFile * infile = findFile(infname1);
  TTree * djTree = findTree(infile,"dijetTree");

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
    // additional cuts
    vgen[ig]->cut_.AndJetCut("lajmul",1,"equ");
    // output
    vgen[ig]->SetVerbosity(2);
    cout << *(vgen[ig]) << endl;

    // -norm-
    printf("now jet cut: \n  %s\n",vgen[ig]->GetCut().GetDiJetCut().Data());
    vgen[ig]->CalcNumDiJets();
    printf("# of dijets after cut: %d, normalization: %f\n",vgen[ig]->GetNumDiJets(),vgen[ig]->GetDiJetsNorm());
    printf("===================\n\n");
  }
  // ===

  if (check) {
    printf("\n============================Do Checks======================\n");
    printf("%s\n",infname1);
    //---- Check dijet properties ----
    //--- inv mass ---
    drawTree(djTree, "mass","",drsgFF,"hMassP","inv mass of dijet",100,0,1800,true,kRed,1,3);
    // --from jet leading--
    drawTree(djTree, "nljet","",drsgFF,"hCheckNearJet",";Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX,true,kBlack,1,1,1,8);
    drawTree(djTree, "aljet","",drdbFF,"hCheckAwayJet",";Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX,true,kBlack,7,1,1,8);
    //-- *apply cuts* --
    drawTree(djTree, "nljet",treeana.GetCut().GetDiJetCut().Data(),drdbFF,"hCheckCutNearJet",";Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX,true,kBlack,1,3,1,8);
    drawTree(djTree, "aljet",treeana.GetCut().GetDiJetCut().Data(),drdbFF,"hCheckCutAwayJet",";Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX,true,kBlack,1,3,1,4);

    //=== Particles ===
    //--- inclusive angular correlations to leading jets---
    drawTree(djTree, "pndphi","",drsgFF,"hnPdPhiJet","Jet: dphi to leading partons",100,-0.5*PI,1.5*PI,false,kRed,1,3);
    drawTree(djTree, "padphi","",drdbFF,"haPdPhiJet","Jet: dphi to leading partons",100,-0.5*PI,1.5*PI,false,kRed,1,3);
    drawTree(djTree, "ppt","",drsgFF,"hPPt","Pt of final state particles",100,0,50,true,kRed,1,3);
  }

  float nJetNorm = treeana.GetDiJetsNorm();
  if (NoNorm) nJetNorm = 1.;
  printf("nDiJets: %d,  jnorm: %f\n",treeana.GetNumDiJets(),nJetNorm);

  //=== Finally: plot fragmentation properties ===
  printf("\n===================== Plot FF =======================\n");
  int log=0;
  // -near-
  drawTree(djTree, "log(1/zn)",treeana.GetCut().GetNJetPartlCut().Data(),drsgFFE,"hXiNearJet",";(near) #xi=ln(E_{t}^{Jet}/E_{t}^{Particle});#frac{1}{N_{jet}} #frac{dN}{d#xi}",NXIBIN,0,XIMAX,log,kBlack,1,3,1,8,nJetNorm,XIYMAX);
  // -away-
  drawTree(djTree, "log(1/za)",treeana.GetCut().GetAJetPartlCut().Data(),drsgFFE,"hXiAwayJet",";(away) #xi=ln(E_{t}^{Jet}/E_{t}^{Particle});#frac{1}{N_{jet}} #frac{dN}{d#xi}",NXIBIN,0,XIMAX,log,kBlack,1,3,1,8,nJetNorm,XIYMAX);

  //=== Save and exit ===
  printAllCanvases(plotdir);
  outfile->Write();
  outfile->Close();
}
