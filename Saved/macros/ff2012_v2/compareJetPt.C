#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TString.h"
#include "TCut.h"
#include <TRandom3.h>
#include <time.h>
#include "commonUtility.h"
#include "../forestv3/CutAndBinCollection2011.h"
using namespace std;

void compareJetPt() {
  TH1::SetDefaultSumw2();
  
  TFile * inf = new TFile("../forestv3/output_data_pp_Nov07_report_ak3PF_0_-1_-1000_saveTrk1_jmin10_tmin1_genj0_sm1.root");
  TTree * t = (TTree*)inf->Get("tgj");
  TFile * infref = new TFile("../hiPhotonAna2011/jskim_forest2_2760GeV_ppJet_full_ak3PF_Oct16-v8-8VtxBin-24PlnBin-40CentBin_jetPt_100_-1_jetEtaCut_2.00_noPbin_smearCentBin1_ak3PF_useGenJet0.root");
  TTree * tref = (TTree*)infref->Get("tdj");
  
  TFile * outf = new TFile("compare.root","recreate");
  TH1D * pt[4];
  TH1D * ptref[4];
  for (int i=0; i<3; ++i) {
    pt[i] = new TH1D(Form("hjetPt_%d",i),";p_{T} (GeV/c);#",50,0,500);
    ptref[i] = new TH1D(Form("hjetPtRef_%d",i),";p_{T} (GeV/c);#",50,0,500);
  }
  t->Project(pt[0]->GetName(),"inclJetPt","inclJetPt>100");
  tref->Project(ptref[0]->GetName(),"lJetPt","lJetPt>100");
  tref->Project(ptref[1]->GetName(),"slJetPt","lJetPt>100&&slJetPt>30");
  ptref[2]->Add(ptref[0],ptref[1]);
  
  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  c2->SetLogy();
  handsomeTH1(ptref[2],kBlack,1,kFullCircle);
  ptref[2]->Draw("E");
  handsomeTH1(ptref[0],kRed,1,kOpenSquare);
  ptref[0]->Draw("sameE");
  handsomeTH1(ptref[1],kBlue,1,kOpenSquare);
  ptref[1]->Draw("sameE");
//   handsomeTH1(pt[0],kBlack,1,kOpenCircle);
//   pt[0]->Draw("hist same");

  TLegend* l1 = new TLegend(0.47,0.72,0.94,0.93,NULL,"brNDC");
  easyLeg(l1,"");
  l1->AddEntry(ptref[2],"pp inclusive","p");
  l1->AddEntry(ptref[0],"pp leading","p");
  l1->AddEntry(ptref[1],"pp subleading","p");
  l1->Draw();

  
  c2->SaveAs("fig/jet_smear_debug.gif");
}
