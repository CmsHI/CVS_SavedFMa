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
using namespace std;

// --- Helper functions ----------------------------------------------------------
class JetReweight {
  public:
  float  inputPt,rewtPt,rewtCent,rewtAj;
};

float getWeight(float pt, TH1D * ratio) {
  if (!ratio) {
    printf("\n\n\n bad ratio histogram!! \n\n\n");
    exit(1);
  }
  float wt = ratio->GetBinContent( ratio->FindBin(pt) );
  return wt;
}

void drawWeight(TH1D ** pp, TH1D ** pbpb, TH1D ** ratio);

// --- Main Function ----------------------------------------------------------
void addJetWeight(
  string infntpp="jskim_pp-full_ak3PF_Nov14_jetPt_50_jetEtaCut_2.00_noPbin_sm1_ak3PF_gj0.root",
  int rewtBin=1,
  TString inhisname="fig/Nov20/hisSmear1_Rewt0.root",
  int wtType=0 // 0=pbpb/pp, 1=pbpbskim90/pbpb
  ) {
  if (wtType==1) rewtBin=1;
  ////////////////////////////////////////////
  // Get Histograms
  ////////////////////////////////////////////
  TH1D * pp[5] ;
  TH1D * pbpb[5] ;
  TH1D * ratio[5] ;
  // Load histograms
  for ( int icent=1; icent<=4 ; icent++) {
    pbpb[icent]   = (TH1D*)load(inhisname,Form("hjetPt_hi_inclusiveJet_icent%d",icent));
    pp[icent]     = (TH1D*)load(inhisname,Form("hjetPt_pp_inclusiveJet_icent%d",icent));
    if (wtType==1 && icent!=1) pbpb[1]->Add(pbpb[icent]); // for wtType==1 this is a jet skim correction, merge centraity bins together
  }
  // Get ratios
  for ( int icent=1; icent<=4 ; icent++) {
    ratio[icent]  = (TH1D*)pbpb[icent]->Clone(Form("hjetPt_ratio_inclusiveJet_icent%d",icent));
    normHist(pbpb[icent],1,true);
    normHist(pp[icent],1,true);
    if (wtType==0) ratio[icent]->Divide(pbpb[icent],pp[icent]);
    else if (wtType==1) ratio[icent]->Divide(pp[icent],pbpb[icent]);
  }
  
  ////////////////////////////////////////////
  // Check Histograms
  ////////////////////////////////////////////
  drawWeight(pp,pbpb,ratio);

  ////////////////////////////////////////////
  // Setup input and output trees
  ////////////////////////////////////////////
  TFile* inf1 = new TFile(infntpp.data());
  TTree * tjin = (TTree*)inf1->Get("tdj");

  // Set Input Tree
  EvtSel evt;
  IndividualJet dj;
  TBranch        *b_evt;
  TBranch        *b_dj;
  tjin->SetBranchAddress("evt", &evt, &b_evt);
  tjin->SetBranchAddress("indiJet", &dj, &b_dj);

  // Set Output
  TString outputFile=infntpp.c_str();
  outputFile.ReplaceAll(".root","_addedReweight.root");
  TFile* newfile = new TFile(outputFile,"recreate");
  cout << "Output file :" << outputFile << endl;
    
  JetReweight jetRewt;
  TTree* tjnew = (TTree*)tjin->CloneTree(0);
  tjnew->Branch("jetRewt",&jetRewt.inputPt,"inputPt/F:rewtPt:rewtCent:rewtAj");

  TTree* yongsunTrackin = (TTree*)inf1->Get("yongsunTrack");
  TTree* newyongsunTrack= (TTree*)yongsunTrackin->CloneTree(0);

  ////////////////////////////////////////////
  // Loop tree and add Weights
  ////////////////////////////////////////////
  Long64_t nentries = tjin->GetEntries();
  for (Long64_t jentry=0;  jentry<nentries;jentry++) {
    if (jentry% 1000 == 0) cout <<jentry<<" / "<<nentries<<" "<<setprecision(4)<<(double)jentry/nentries*100<<endl;
    tjin->GetEntry(jentry);
    yongsunTrackin->GetEntry(jentry);
    jetRewt.inputPt = dj.jetPt;
    if (rewtBin>0) jetRewt.rewtPt = getWeight(jetRewt.inputPt,ratio[rewtBin]);
    else jetRewt.rewtPt = 1.;
    tjnew->Fill();
    newyongsunTrack->Fill();
  }

  ////////////////////////////////////////////
  // All done, wrap-up and save
  ////////////////////////////////////////////
  tjnew->AutoSave();
  newyongsunTrack->AutoSave();
  newfile->Close();
}

// Helper functions ----------------------------------------------------------
void drawWeight(TH1D ** pp, TH1D ** pbpb, TH1D ** ratio) {
  TCanvas* c1 =new TCanvas("c1","",1000,600);
  makeMultiPanelCanvas(c1,4,2,0.0,0.0,0.25,0.2,0.02);

  TLegend* l2  =  new  TLegend(0.03183659,0.5514706,0.9316217,0.8526261,NULL,"brNDC");
  for ( int icent=1; icent<=4 ; icent++) {
    c1->cd(5-icent);
    
    handsomeTH1(pp[icent],1,1,kOpenCircle);
    handsomeTH1(pbpb[icent],1,1,kFullCircle);
    pp[icent]->SetAxisRange(100,290,"X");
    pp[icent]->SetAxisRange(2e-5,0.2,"Y");
    pp[icent]->SetNdivisions(505);
    fixedFontHist(pp[icent],2,2.5);
    pp[icent]->SetYTitle("u.n.");
    pp[icent]->Draw();
    pbpb[icent]->Draw("same");
    gPad->SetLogy();
    if ( icent == 4) {
      drawText("CMS Preliminary",0.3,0.89,1,20);
      TLegend* l1 = new TLegend(0.52,0.51,0.95,0.93,NULL,"brNDC");
      easyLeg(l1);
      l1->AddEntry(pp[icent],"|#eta_{jet}| < 2","");
      l1->AddEntry(pp[icent],"pp","p");
      l1->AddEntry(pbpb[icent],"PbPb","p");
//       l1->AddEntry(pp[icent],"pp after smearing","p");
//       l1->AddEntry(pbpb[icent],"pp before smearing","p");
      l1->Draw();
    }
    int lowCent = centBin1[icent-1];
    int highCent = centBin1[icent]-1;
    if ( icent ==4 ) 
      drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),  0.3,0.09,1);
    else
      drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),  0.15,0.09,1);

  }
  for ( int icent=1; icent<=4 ; icent++) {
    c1->cd(9-icent);
    handsomeTH1(ratio[icent],1);
    ratio[icent]->SetMarkerStyle(20);
    ratio[icent]->SetAxisRange(100,290,"X");
    ratio[icent]->SetAxisRange(0,2.1,"Y");
    ratio[icent]->SetNdivisions(505);
    fixedFontHist(ratio[icent],1.8,2.5);
    ratio[icent]->SetYTitle("Ratio");
    ratio[icent]->Draw();
  }
}