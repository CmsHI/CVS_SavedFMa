#include <TH2.h>
#include <TF1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <TMath.h>
#include "CutAndBinCollection2011.h"
#include <time.h>
#include <TRandom3.h>

using namespace std;

#define PI 3.141592653589
static const int MAXTRK = 30000;
const int kPP=1;
const int kHI=2;
const int kLead=1;
const int kSubl=2;



void getRatio( std::string ppInput_="", std::string pbpbInput_="",TH1D* ratioCentBin_[3][5]=0, TH1D* ratioAjBin_[3][5]=0, TH1D* ratioCentBiner_[3][5]=0, TH1D* ratioAjBiner_[3][5]=0);


void addDijetWieght( 
		    std::string ppInput_="jskim_forest2_2760GeV_ppJet_full_July14_akPu3PF_July-v6-8VtxBin-24PlnBin-40CentBin-EtaReflectionAdded_jetPt_100_-1_jetEtaCut_2.00_yesPbin_smearCentBin4_useGenJet0.root",
		    std::string pbpbInput_="jskim_HiForest-promptskim-hihighpt-hltjet80-pt90-v3_part2_akPu3PF_July-v6-8VtxBin-24PlnBin-40CentBin-EtaReflectionAdded_jetPt_100_-1_jetEtaCut_2.00_yesPbin_smearCentBin0_useGenJet0.root")

{
  TH1::SetDefaultSumw2();
  
  //  double jetPtBin[30] = { 40,45,50,55,60,65,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300};
  double jetPtBin[22] = { 40,45,50,55,60,65,70,80,90,100,110,120,140,160,180,200,220,240,260,300,400,500};
  TH1D* ratioCentBin[3][5];
  TH1D* ratioAjBin[3][5];
  TH1D* ratioCentBiner[3][5];  // er stands for eta refelction eta > 0.3
  TH1D* ratioAjBiner[3][5];

  for ( int icent=1 ; icent<=4 ; icent++) {
    ratioCentBin[kLead][icent] = new TH1D(Form("ratio_pp_leadingJet_icent%d",icent),";jet p_{T} (GeV);Entries",21, jetPtBin);
    ratioCentBin[kSubl][icent] = (TH1D*)ratioCentBin[kLead][icent]->Clone(Form("ratio_pp_subleadingJet_icent%d",icent));
    ratioAjBin[kLead][icent] = (TH1D*)ratioCentBin[kLead][icent]->Clone(Form("ratio_pp_leadingJet_irj%d",icent));
    ratioAjBin[kSubl][icent] = (TH1D*)ratioCentBin[kLead][icent]->Clone(Form("ratio_pp_subleadingJet_irj%d",icent));

    ratioCentBiner[kLead][icent] = new TH1D(Form("ratio_pp_leadingJet_icent%d_er",icent),";jet p_{T} (GeV);Entries",21, jetPtBin);
    ratioCentBiner[kSubl][icent] = (TH1D*)ratioCentBin[kLead][icent]->Clone(Form("ratio_pp_subleadingJet_icent%d_er",icent));
    ratioAjBiner[kLead][icent] = (TH1D*)ratioCentBin[kLead][icent]->Clone(Form("ratio_pp_leadingJet_irj%d_er",icent));
    ratioAjBiner[kSubl][icent] = (TH1D*)ratioCentBin[kLead][icent]->Clone(Form("ratio_pp_subleadingJet_irj%d_er",icent));
  }
  getRatio(ppInput_, pbpbInput_, ratioCentBin, ratioAjBin, ratioCentBiner, ratioAjBiner);

  TFile* inf = new TFile(ppInput_.data());
  
  std::string remword=".root";
  size_t pos = ppInput_.find(remword);
  std::string forOutput = ppInput_;
  if(pos!= std::string::npos)
    forOutput.swap(forOutput.erase(pos,remword.length()));
  std::string endfix = "_jetWeightAdded.root";
  std::string outputFile = forOutput + endfix;
  
  TFile* newfile_data = new TFile(outputFile.data(),"recreate");
  cout << "Output file :" << outputFile << endl;
  
  
  TTree* tdjin = (TTree*)inf->Get("tdj");
  TTree* newtdj= (TTree*)tdjin->CloneTree(0);

  TTree* yongsunHltTreein = (TTree*)inf->Get("yongsunHltTree");
  TTree* newyongsunHltTree= (TTree*)yongsunHltTreein->CloneTree(0);
  newyongsunHltTree->SetName("yongsunHltTreeCopied");

  TTree* yongsunSkimTreein = (TTree*)inf->Get("yongsunSkimTree");
  TTree* newyongsunSkimTree= (TTree*)yongsunSkimTreein->CloneTree(0);
  
  TTree* mTrkin = (TTree*)inf->Get("mTrk");
  TTree* newmTrk= (TTree*)mTrkin->CloneTree(0);

  TTree* yongsunTrackin = (TTree*)inf->Get("yongsunTrack");
  TTree* newyongsunTrack= (TTree*)yongsunTrackin->CloneTree(0);
  
  TTree* yongsunHiEvtin = (TTree*)inf->Get("yongsunHiEvt");
  TTree* newyongsunHiEvt= (TTree*)yongsunHiEvtin->CloneTree(0);
  

    
  // now open new root file                                                                                             

  EvtSel evt;
  DiJet  dj;
  TBranch        *b_evt;
  TBranch        *b_dj;
  tdjin->SetBranchAddress("evt", &evt, &b_evt);
  tdjin->SetBranchAddress("dijet", &dj, &b_dj);
  
  Float_t        ljetWeightAj1, ljetWeightAj2, ljetWeightAj3, ljetWeightAj4;
  Float_t        ljetWeightCent1, ljetWeightCent2, ljetWeightCent3, ljetWeightCent4;
  Float_t        sljetWeightAj1, sljetWeightAj2, sljetWeightAj3, sljetWeightAj4;
  Float_t        sljetWeightCent1, sljetWeightCent2, sljetWeightCent3, sljetWeightCent4;
  
  // er = eta reflection
  Float_t        ljetWeightAj1er, ljetWeightAj2er, ljetWeightAj3er, ljetWeightAj4er;
  Float_t        ljetWeightCent1er, ljetWeightCent2er, ljetWeightCent3er, ljetWeightCent4er;
  Float_t        sljetWeightAj1er, sljetWeightAj2er, sljetWeightAj3er, sljetWeightAj4er;
  Float_t        sljetWeightCent1er, sljetWeightCent2er, sljetWeightCent3er, sljetWeightCent4er;

  newtdj->Branch("ljetWeightAj1",&ljetWeightAj1,"ljetWeightAj1/F");
  newtdj->Branch("ljetWeightAj2",&ljetWeightAj2,"ljetWeightAj2/F");
  newtdj->Branch("ljetWeightAj3",&ljetWeightAj3,"ljetWeightAj3/F");
  newtdj->Branch("ljetWeightAj4",&ljetWeightAj4,"ljetWeightAj4/F");
  newtdj->Branch("ljetWeightCent1",&ljetWeightCent1,"ljetWeightCent1/F");
  newtdj->Branch("ljetWeightCent2",&ljetWeightCent2,"ljetWeightCent2/F");
  newtdj->Branch("ljetWeightCent3",&ljetWeightCent3,"ljetWeightCent3/F");
  newtdj->Branch("ljetWeightCent4",&ljetWeightCent4,"ljetWeightCent4/F");
  newtdj->Branch("sljetWeightAj1",&sljetWeightAj1,"sljetWeightAj1/F");
  newtdj->Branch("sljetWeightAj2",&sljetWeightAj2,"sljetWeightAj2/F");
  newtdj->Branch("sljetWeightAj3",&sljetWeightAj3,"sljetWeightAj3/F");
  newtdj->Branch("sljetWeightAj4",&sljetWeightAj4,"sljetWeightAj4/F");
  newtdj->Branch("sljetWeightCent1",&sljetWeightCent1,"sljetWeightCent1/F");
  newtdj->Branch("sljetWeightCent2",&sljetWeightCent2,"sljetWeightCent2/F");
  newtdj->Branch("sljetWeightCent3",&sljetWeightCent3,"sljetWeightCent3/F");
  newtdj->Branch("sljetWeightCent4",&sljetWeightCent4,"sljetWeightCent4/F");
  
  newtdj->Branch("ljetWeightAj1er",&ljetWeightAj1er,"ljetWeightAj1er/F");
  newtdj->Branch("ljetWeightAj2er",&ljetWeightAj2er,"ljetWeightAj2er/F");
  newtdj->Branch("ljetWeightAj3er",&ljetWeightAj3er,"ljetWeightAj3er/F");
  newtdj->Branch("ljetWeightAj4er",&ljetWeightAj4er,"ljetWeightAj4er/F");
  newtdj->Branch("ljetWeightCent1er",&ljetWeightCent1er,"ljetWeightCent1er/F");
  newtdj->Branch("ljetWeightCent2er",&ljetWeightCent2er,"ljetWeightCent2er/F");
  newtdj->Branch("ljetWeightCent3er",&ljetWeightCent3er,"ljetWeightCent3er/F");
  newtdj->Branch("ljetWeightCent4er",&ljetWeightCent4er,"ljetWeightCent4er/F");
  newtdj->Branch("sljetWeightAj1er",&sljetWeightAj1er,"sljetWeightAj1er/F");
  newtdj->Branch("sljetWeightAj2er",&sljetWeightAj2er,"sljetWeightAj2er/F");
  newtdj->Branch("sljetWeightAj3er",&sljetWeightAj3er,"sljetWeightAj3er/F");
  newtdj->Branch("sljetWeightAj4er",&sljetWeightAj4er,"sljetWeightAj4er/F");
  newtdj->Branch("sljetWeightCent1er",&sljetWeightCent1er,"sljetWeightCent1er/F");
  newtdj->Branch("sljetWeightCent2er",&sljetWeightCent2er,"sljetWeightCent2er/F");
  newtdj->Branch("sljetWeightCent3er",&sljetWeightCent3er,"sljetWeightCent3er/F");
  newtdj->Branch("sljetWeightCent4er",&sljetWeightCent4er,"sljetWeightCent4er/F");


  Long64_t nentries = tdjin->GetEntries();
  for (Long64_t jentry=0;  jentry<nentries;jentry++) {
    if (jentry% 500 == 0) cout <<jentry<<" / "<<nentries<<" "<<setprecision(2)<<(double)jentry/nentries*100<<endl;
    tdjin->GetEntry(jentry);
    yongsunHltTreein->GetEntry(jentry);
    yongsunSkimTreein->GetEntry(jentry);
    mTrkin->GetEntry(jentry);
    yongsunTrackin->GetEntry(jentry);
    yongsunHiEvtin->GetEntry(jentry);

    ljetWeightAj1 = ratioAjBin[kLead][1]->GetBinContent( ratioAjBin[kLead][1]->FindBin(dj.lJetPt) );
    ljetWeightAj2 = ratioAjBin[kLead][2]->GetBinContent( ratioAjBin[kLead][2]->FindBin(dj.lJetPt) );
    ljetWeightAj3 = ratioAjBin[kLead][3]->GetBinContent( ratioAjBin[kLead][3]->FindBin(dj.lJetPt) );
    ljetWeightAj4 = ratioAjBin[kLead][4]->GetBinContent( ratioAjBin[kLead][4]->FindBin(dj.lJetPt) );
    sljetWeightAj1 = ratioAjBin[kSubl][1]->GetBinContent( ratioAjBin[kSubl][1]->FindBin(dj.slJetPt) );
    sljetWeightAj2 = ratioAjBin[kSubl][2]->GetBinContent( ratioAjBin[kSubl][2]->FindBin(dj.slJetPt) );
    sljetWeightAj3 = ratioAjBin[kSubl][3]->GetBinContent( ratioAjBin[kSubl][3]->FindBin(dj.slJetPt) );
    sljetWeightAj4 = ratioAjBin[kSubl][4]->GetBinContent( ratioAjBin[kSubl][4]->FindBin(dj.slJetPt) );
    
    ljetWeightCent1 = ratioCentBin[kLead][1]->GetBinContent( ratioCentBin[kLead][1]->FindBin(dj.lJetPt) );
    ljetWeightCent2 = ratioCentBin[kLead][2]->GetBinContent( ratioCentBin[kLead][2]->FindBin(dj.lJetPt) );
    ljetWeightCent3 = ratioCentBin[kLead][3]->GetBinContent( ratioCentBin[kLead][3]->FindBin(dj.lJetPt) );
    ljetWeightCent4 = ratioCentBin[kLead][4]->GetBinContent( ratioCentBin[kLead][4]->FindBin(dj.lJetPt) );
    sljetWeightCent1 = ratioCentBin[kSubl][1]->GetBinContent( ratioCentBin[kSubl][1]->FindBin(dj.slJetPt) );
    sljetWeightCent2 = ratioCentBin[kSubl][2]->GetBinContent( ratioCentBin[kSubl][2]->FindBin(dj.slJetPt) );
    sljetWeightCent3 = ratioCentBin[kSubl][3]->GetBinContent( ratioCentBin[kSubl][3]->FindBin(dj.slJetPt) );
    sljetWeightCent4 = ratioCentBin[kSubl][4]->GetBinContent( ratioCentBin[kSubl][4]->FindBin(dj.slJetPt) );

    ljetWeightAj1er = ratioAjBiner[kLead][1]->GetBinContent( ratioAjBiner[kLead][1]->FindBin(dj.lJetPt) );
    ljetWeightAj2er = ratioAjBiner[kLead][2]->GetBinContent( ratioAjBiner[kLead][2]->FindBin(dj.lJetPt) );
    ljetWeightAj3er = ratioAjBiner[kLead][3]->GetBinContent( ratioAjBiner[kLead][3]->FindBin(dj.lJetPt) );
    ljetWeightAj4er = ratioAjBiner[kLead][4]->GetBinContent( ratioAjBiner[kLead][4]->FindBin(dj.lJetPt) );
    sljetWeightAj1er = ratioAjBiner[kSubl][1]->GetBinContent( ratioAjBiner[kSubl][1]->FindBin(dj.slJetPt) );
    sljetWeightAj2er = ratioAjBiner[kSubl][2]->GetBinContent( ratioAjBiner[kSubl][2]->FindBin(dj.slJetPt) );
    sljetWeightAj3er = ratioAjBiner[kSubl][3]->GetBinContent( ratioAjBiner[kSubl][3]->FindBin(dj.slJetPt) );
    sljetWeightAj4er = ratioAjBiner[kSubl][4]->GetBinContent( ratioAjBiner[kSubl][4]->FindBin(dj.slJetPt) );
    
    ljetWeightCent1er = ratioCentBiner[kLead][1]->GetBinContent( ratioCentBiner[kLead][1]->FindBin(dj.lJetPt) );
    ljetWeightCent2er = ratioCentBiner[kLead][2]->GetBinContent( ratioCentBiner[kLead][2]->FindBin(dj.lJetPt) );
    ljetWeightCent3er = ratioCentBiner[kLead][3]->GetBinContent( ratioCentBiner[kLead][3]->FindBin(dj.lJetPt) );
    ljetWeightCent4er = ratioCentBiner[kLead][4]->GetBinContent( ratioCentBiner[kLead][4]->FindBin(dj.lJetPt) );
    sljetWeightCent1er = ratioCentBiner[kSubl][1]->GetBinContent( ratioCentBiner[kSubl][1]->FindBin(dj.slJetPt) );
    sljetWeightCent2er = ratioCentBiner[kSubl][2]->GetBinContent( ratioCentBiner[kSubl][2]->FindBin(dj.slJetPt) );
    sljetWeightCent3er = ratioCentBiner[kSubl][3]->GetBinContent( ratioCentBiner[kSubl][3]->FindBin(dj.slJetPt) );
    sljetWeightCent4er = ratioCentBiner[kSubl][4]->GetBinContent( ratioCentBiner[kSubl][4]->FindBin(dj.slJetPt) );
    
    
    
    newtdj->Fill();
    newyongsunHltTree->Fill();
    newyongsunSkimTree->Fill();
    newyongsunHiEvt->Fill();
    newyongsunTrack->Fill();
    newmTrk->Fill();

  }
  
 
  newtdj->AutoSave();
  newyongsunHltTree->AutoSave();
  newyongsunSkimTree->AutoSave();
  newyongsunHiEvt->AutoSave();
  newyongsunTrack->AutoSave();
  newmTrk->AutoSave();
  
  newfile_data->Close();
}



void getRatio( std::string ppInput_, std::string pbpbInput_,TH1D* ratioCentBin_[3][5], TH1D* ratioAjBin_[3][5],TH1D* ratioCentBiner_[3][5], TH1D* ratioAjBiner_[3][5]) {

  
  //  TCut dijetSelCut = Form("lJetPt>100 && slJetPt>40 && acos(cos(lJetPhi - slJetPhi))>%f",jetDPhiCut);  
  TCut dijetSelCut = "lJetPt>100";
    
  TH1D* ppSpec[3]; 
  ppSpec[kLead] = (TH1D*)ratioCentBin_[1][1]->Clone("hjetPt_pp_leadingJet");
  ppSpec[kSubl] = (TH1D*)ratioCentBin_[1][1]->Clone("hjetPt_pp_subleadingJet");
  TH1D* hiSpecCentBin[3][5];
  TH1D* hiSpecAjBin[3][5];
  TH1D* ratioCentBin[3][5];
  TH1D* ratioAjBin[3][5];

  TH1D* hiSpecCentBiner[3][5];
  TH1D* hiSpecAjBiner[3][5];
  TH1D* ratioCentBiner[3][5];
  TH1D* ratioAjBiner[3][5];

  TChain * tHI = new TChain("tdj");
  tHI->Add(pbpbInput_.data());
  for ( int icent = 1 ; icent<=4 ; icent++) {
    int lowCent = centBin1[icent-1];
    int highCent = centBin1[icent] - 1;
    TCut theCentCut = Form("cBin>=%d && cBin<=%d",lowCent,highCent);
    hiSpecCentBin[kLead][icent] = (TH1D*)ppSpec[kLead]->Clone(Form("hjetPt_hi_leadingJet_icent%d",icent));
    hiSpecCentBin[kSubl][icent] = (TH1D*)ppSpec[kLead]->Clone(Form("hjetPt_hi_subleadJet_icent%d",icent));
    
    hiSpecCentBiner[kLead][icent] = (TH1D*)ppSpec[kLead]->Clone(Form("hjetPt_hi_leadingJet_icent%d_er",icent));
    hiSpecCentBiner[kSubl][icent] = (TH1D*)ppSpec[kLead]->Clone(Form("hjetPt_hi_subleadJet_icent%d_er",icent));
    
    tHI->Draw(Form("lJetPt>>%s",hiSpecCentBin[kLead][icent]->GetName()), dijetSelCut && theCentCut);
    tHI->Draw(Form("slJetPt>>+%s",hiSpecCentBin[kLead][icent]->GetName()), dijetSelCut && theCentCut);
    tHI->Draw(Form("lJetPt>>%s",hiSpecCentBiner[kLead][icent]->GetName()), dijetSelCut && theCentCut &&"abs(lJetEta)>0.3");
    tHI->Draw(Form("slJetPt>>+%s",hiSpecCentBiner[kLead][icent]->GetName()), dijetSelCut && theCentCut &&"abs(slJetEta)>0.3");
   
    scaleInt(hiSpecCentBin[kLead][icent]);
    scaleInt(hiSpecCentBin[kSubl][icent]);

    scaleInt(hiSpecCentBiner[kLead][icent]);
    scaleInt(hiSpecCentBiner[kSubl][icent]);

    TH1ScaleByWidth(hiSpecCentBin[kLead][icent]);
    TH1ScaleByWidth(hiSpecCentBin[kSubl][icent]);
    handsomeTH1(hiSpecCentBin[kLead][icent],2);
    handsomeTH1(hiSpecCentBin[kSubl][icent],2);
    
    TH1ScaleByWidth(hiSpecCentBiner[kLead][icent]);
    TH1ScaleByWidth(hiSpecCentBiner[kSubl][icent]);
    handsomeTH1(hiSpecCentBiner[kLead][icent],2);
    handsomeTH1(hiSpecCentBiner[kSubl][icent],2);

  }  

  
  for ( int irj = 1 ; irj<=4 ; irj++) {
    TCut rjCut = Form("(slJetPt/lJetPt)>%f && (slJetPt/lJetPt) <=%f", (float)rjBin1[irj-1], (float)rjBin1[irj]);
    hiSpecAjBin[kLead][irj] = (TH1D*)ppSpec[kLead]->Clone(Form("hjetPt_hi_leadingJet_irj%d",irj));
    hiSpecAjBin[kSubl][irj] = (TH1D*)ppSpec[kLead]->Clone(Form("hjetPt_hi_subleadJet_irj%d",irj));
    hiSpecAjBiner[kLead][irj] = (TH1D*)ppSpec[kLead]->Clone(Form("hjetPt_hi_leadingJet_irj%d_er",irj));
    hiSpecAjBiner[kSubl][irj] = (TH1D*)ppSpec[kLead]->Clone(Form("hjetPt_hi_subleadJet_irj%d_er",irj));

    tHI->Draw(Form("lJetPt>>%s",hiSpecAjBin[kLead][irj]->GetName()), dijetSelCut && "cBin<12" && rjCut );
    tHI->Draw(Form("slJetPt>>+%s",hiSpecAjBin[kLead][irj]->GetName()), dijetSelCut && "cBin<12" && rjCut );
    tHI->Draw(Form("lJetPt>>%s",hiSpecAjBiner[kLead][irj]->GetName()), dijetSelCut && "cBin<12" && rjCut &&"abs(lJetEta)>0.3");
    tHI->Draw(Form("slJetPt>>+%s",hiSpecAjBiner[kLead][irj]->GetName()), dijetSelCut && "cBin<12" && rjCut  &&"abs(slJetEta)>0.3");
    
    scaleInt(hiSpecAjBin[kLead][irj]);
    scaleInt(hiSpecAjBin[kSubl][irj]);  
    scaleInt(hiSpecAjBiner[kLead][irj]);
    scaleInt(hiSpecAjBiner[kSubl][irj]);  

    TH1ScaleByWidth(hiSpecAjBin[kLead][irj]);
    TH1ScaleByWidth(hiSpecAjBin[kSubl][irj]);
    handsomeTH1(hiSpecAjBin[kLead][irj],2);
    handsomeTH1(hiSpecAjBin[kSubl][irj],2);
    TH1ScaleByWidth(hiSpecAjBiner[kLead][irj]);
    TH1ScaleByWidth(hiSpecAjBiner[kSubl][irj]);
    handsomeTH1(hiSpecAjBiner[kLead][irj],2);
    handsomeTH1(hiSpecAjBiner[kSubl][irj],2);

  }
  
  TChain * tPP = new TChain("tdj");
  tPP->Add(ppInput_.data());
  tPP->Draw(Form("lJetPt>>%s",ppSpec[kLead]->GetName()),dijetSelCut  );
  tPP->Draw(Form("slJetPt>>+%s",ppSpec[kLead]->GetName()),dijetSelCut );
  scaleInt(ppSpec[kLead]);
  scaleInt(ppSpec[kSubl]);
  TH1ScaleByWidth(ppSpec[kLead]);
  TH1ScaleByWidth(ppSpec[kSubl]);
  handsomeTH1(ppSpec[kLead],1);
  handsomeTH1(ppSpec[kSubl],1);
  
  ppSpec[kLead]->SetAxisRange(0.000001,.1,"Y");
  ppSpec[kSubl]->SetAxisRange(0.000001,.1,"Y");

  //// FIT!!
  double min(100), max(400);
  TF1* fPt1 = new TF1(Form("%s_fit",ppSpec[kLead]->GetName()),"[0]*x^([1])",min,max);
  TF1* fPt2 = new TF1(Form("%s_fit",ppSpec[kSubl]->GetName()),"[0]*x^([1])",min,max);
  fPt1->SetParameter(0,10);
  fPt1->SetParameter(1,-0.3);
  fPt2->SetParameter(0,0.1);
  fPt2->SetParameter(1,-0.3);
  //  ppSpec[kLead]->Fit(fPt1,"MRLL");
  //  ppSpec[kSubl]->Fit(fPt2,"MRLL");


  TH1D* ppSpecFit[3]; 
  ppSpecFit[kLead] = (TH1D*)ppSpec[kLead]->Clone(Form("%s_fit1",ppSpec[kLead]->GetName()));
  ppSpecFit[kSubl] = (TH1D*)ppSpec[kSubl]->Clone(Form("%s_fit1",ppSpec[kSubl]->GetName()));
  for ( int j=1;j<=ppSpecFit[kLead]->GetNbinsX();j++) {
    float theX = ppSpecFit[kLead]->GetBinCenter(j);
    if ( theX < min )  
      continue;
//    ppSpecFit[kLead]->SetBinContent( j, fPt1->Eval(theX) );
//    ppSpecFit[kSubl]->SetBinContent( j, fPt2->Eval(theX) );
  }
  handsomeTH1(ppSpecFit[kLead],2);
  handsomeTH1(ppSpecFit[kSubl],2);

  TCanvas* c0 = new TCanvas("c0","",800,400);
  c0->Divide(2,1);
  c0->cd(1);
  ppSpec[kLead]->Draw();
  ppSpecFit[kLead]->Draw("same hist");
  gPad->SetLogy();
  drawText("pp leading jet", 0.46,0.7);
  c0->cd(2);
  ppSpec[kSubl]->Draw();
  ppSpecFit[kSubl]->Draw("same hist");
  drawText("pp subleading jet", 0.46,0.7);
  drawText("fit p_{T} > 100GeV tail",0.46,0.6);
  gPad->SetLogy();
  c0->SaveAs("plotsOfDijetFF/fitting_ppSpectra_withPowerLaw.pdf");
  cout <<" fitted funciton is used!!! " << endl << endl << endl;
  //  ppSpec = ppSpecFit[kLead] ;
  
  TCanvas* c2 = new TCanvas("c1","",1200,350);
  c2->Divide(4,1);
  for ( int icent = 1 ; icent<=4 ; icent++) {
    int lowCent = centBin1[icent-1];
    int highCent = centBin1[icent]-1;

    c2->cd(5-icent);
    handsomeTH1(ppSpecFit[kLead],1);
    ppSpecFit[kLead]->SetAxisRange(101,299,"X");
    ppSpecFit[kLead]->SetNdivisions(505);
    ppSpecFit[kLead]->SetMarkerStyle(24);
    ppSpecFit[kLead]->SetYTitle("Normalized Entries");
    ppSpecFit[kLead]->Draw();
    handsomeTH1(hiSpecCentBin[kLead][icent],2);
    hiSpecCentBin[kLead][icent]->Draw("same");
    //  ppSpecFit[kLead]->Draw("hist same");
    gPad->SetLogy();
    drawText("jets |#eta|<2", 0.46,0.7);
    drawText("pp ", 0.3,0.3);
    drawText(Form("PbPb %.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),  0.3,0.22,2);
    
    // inclusive 
    /*
    c2->cd(9-icent);
    handsomeTH1(ppSpecFit[kSubl],1);
    ppSpecFit[kSubl]->Draw();
    handsomeTH1(hiSpecCentBin[kSubl][icent],2);
    hiSpecCentBin[kSubl][icent]->Draw("same");
    ppSpecFit[kSubl]->Draw("hist same");
    gPad->SetLogy();
    drawText("Subleading jet",  0.6,0.62);
    drawText("pp ", 0.3,0.3);
    drawText(Form("PbPb %.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),  0.3,0.22,2);
    */
  }
  c2->SaveAs("plotsOfDijetFF/spectraComparison_pp_pbpb_centBin.pdf");
  
  TCanvas* c3 = new TCanvas("c3","",1200,700);
  c3->Divide(4,2);
  for ( int irj = 1 ; irj<=4 ; irj++) {
    c3->cd(5-irj);
    handsomeTH1(ppSpecFit[kLead],1);
    ppSpecFit[kLead]->Draw();
    handsomeTH1(hiSpecAjBin[kLead][irj],2);
    hiSpecAjBin[kLead][irj]->Draw("same");
    ppSpecFit[kLead]->Draw("hist same");
    gPad->SetLogy();
    drawText("Leading jet", 0.46,0.7);
    drawText("pp ", 0.3,0.3);
    drawText(Form("PbPb %.2f < pt_{2}/pt_{1} < %.2f", float(rjBin1[irj-1]), float(rjBin1[irj])),  0.3,0.22,2);

    c3->cd(9-irj);
    handsomeTH1(ppSpecFit[kSubl],1);
    ppSpecFit[kSubl]->Draw();
    handsomeTH1(hiSpecAjBin[kSubl][irj],2);
    hiSpecAjBin[kSubl][irj]->Draw("same");
    ppSpecFit[kSubl]->Draw("hist same");
    gPad->SetLogy();
    drawText("Subleading jet", 0.46,0.7);
    drawText("pp ",  0.3,0.3);
    drawText(Form("PbPb %.2f< pt_{2}/pt_{1}<%.2f", float(rjBin1[irj-1]), float(rjBin1[irj])),  0.3,0.22,2);
  }
  c3->SaveAs("plotsOfDijetFF/spectraComparison_pp_pbpb_rjBin.pdf");

  TCanvas* c4 =  new TCanvas("c4","",1200,700);
  c4->Divide(4,2);
  for ( int icent = 1 ; icent<=4 ; icent++) {
    c4->cd(5-icent);
    ratioCentBin[kLead][icent] = (TH1D*)hiSpecCentBin[kLead][icent]->Clone(Form("ratio_leadJet_icent%d",icent));
    ratioCentBin[kLead][icent]->Divide(ppSpecFit[kLead]);
    ratioCentBiner[kLead][icent] = (TH1D*)hiSpecCentBiner[kLead][icent]->Clone(Form("ratio_leadJet_icent%d_er",icent));
    ratioCentBiner[kLead][icent]->Divide(ppSpecFit[kLead]);
    
    ratioCentBin[kLead][icent]->Draw();
    ratioCentBin[kLead][icent]->SetAxisRange(0,3);
    
    ratioCentBin_[kLead][icent]->Reset();
    ratioCentBiner_[kLead][icent]->Reset();
    for ( int i=1; i<=ratioCentBin[kLead][icent]->GetNbinsX(); i++) {
      ratioCentBin_[kLead][icent]->SetBinContent( i, ratioCentBin[kLead][icent]->GetBinContent(i));
      ratioCentBin_[kLead][icent]->SetBinError(   i, ratioCentBin[kLead][icent]->GetBinError(i));
      ratioCentBiner_[kLead][icent]->SetBinContent( i, ratioCentBiner[kLead][icent]->GetBinContent(i));
      ratioCentBiner_[kLead][icent]->SetBinError(   i, ratioCentBiner[kLead][icent]->GetBinError(i));
    }
    
    c4->cd(9-icent);
    ratioCentBin[kSubl][icent] = (TH1D*)hiSpecCentBin[kSubl][icent]->Clone(Form("ratio_subleadJet_icent%d",icent));
    ratioCentBin[kSubl][icent]->Divide(ppSpecFit[kSubl]);

    ratioCentBiner[kSubl][icent] = (TH1D*)hiSpecCentBiner[kSubl][icent]->Clone(Form("ratio_subleadJet_icent%d_er",icent));
    ratioCentBiner[kSubl][icent]->Divide(ppSpecFit[kSubl]);


    ratioCentBin[kSubl][icent]->Draw();
    ratioCentBin[kSubl][icent]->SetAxisRange(0,3);
    for ( int i=1; i<=ratioCentBin[kSubl][icent]->GetNbinsX(); i++) {
      ratioCentBin_[kSubl][icent]->SetBinContent( i, ratioCentBin[kSubl][icent]->GetBinContent(i));
      ratioCentBin_[kSubl][icent]->SetBinError(   i, ratioCentBin[kSubl][icent]->GetBinError(i));
      ratioCentBiner_[kSubl][icent]->SetBinContent( i, ratioCentBiner[kSubl][icent]->GetBinContent(i));
      ratioCentBiner_[kSubl][icent]->SetBinError(   i, ratioCentBiner[kSubl][icent]->GetBinError(i));
    }
  }
    
    
  c4->SaveAs("jetPtReweight_ratio_centBin.gif");

  TCanvas* c5 =  new TCanvas("c5","",1200,700);
  c5->Divide(4,2);
  for ( int irj = 1 ; irj<=4 ; irj++) {
    
    c5->cd(5-irj);
    ratioAjBin[kLead][irj] = (TH1D*)hiSpecAjBin[kLead][irj]->Clone(Form("ratio_leadJet_irj%d",irj));
    ratioAjBin[kLead][irj]->Divide(ppSpecFit[kLead]);

    ratioAjBiner[kLead][irj] = (TH1D*)hiSpecAjBiner[kLead][irj]->Clone(Form("ratio_leadJet_irj%d_er",irj));
    ratioAjBiner[kLead][irj]->Divide(ppSpecFit[kLead]);

    ratioAjBin[kLead][irj]->Draw();
    ratioAjBin[kLead][irj]->SetAxisRange(0,3);

    for ( int i=1; i<=ratioAjBin[kLead][irj]->GetNbinsX(); i++) {
      ratioAjBin_[kLead][irj]->SetBinContent( i, ratioAjBin[kLead][irj]->GetBinContent(i));
      ratioAjBin_[kLead][irj]->SetBinError(   i, ratioAjBin[kLead][irj]->GetBinError(i));
      ratioAjBiner_[kLead][irj]->SetBinContent( i, ratioAjBiner[kLead][irj]->GetBinContent(i));
      ratioAjBiner_[kLead][irj]->SetBinError(   i, ratioAjBiner[kLead][irj]->GetBinError(i));
    }
    c5->cd(9-irj);
    ratioAjBin[kSubl][irj] = (TH1D*)hiSpecAjBin[kSubl][irj]->Clone(Form("ratio_subleadJet_irj%d",irj));
    ratioAjBin[kSubl][irj]->Divide(ppSpecFit[kSubl]);
    
    ratioAjBiner[kSubl][irj] = (TH1D*)hiSpecAjBiner[kSubl][irj]->Clone(Form("ratio_subleadJet_irj%d_er",irj));
    ratioAjBiner[kSubl][irj]->Divide(ppSpecFit[kSubl]);
    
    ratioAjBin[kSubl][irj]->Draw();
    ratioAjBin[kSubl][irj]->SetAxisRange(0,3);
    for ( int i=1; i<=ratioAjBin[kSubl][irj]->GetNbinsX(); i++) {
      ratioAjBin_[kSubl][irj]->SetBinContent( i, ratioAjBin[kSubl][irj]->GetBinContent(i));
      ratioAjBin_[kSubl][irj]->SetBinError(   i, ratioAjBin[kSubl][irj]->GetBinError(i));

     ratioAjBiner_[kSubl][irj]->SetBinContent( i, ratioAjBiner[kSubl][irj]->GetBinContent(i));
     ratioAjBiner_[kSubl][irj]->SetBinError(   i, ratioAjBiner[kSubl][irj]->GetBinError(i));
      
    }
  }
  
  c5->SaveAs("jetPtReweight_ratio_rjBin.gif");
}




/*  
  TTree *yongsunTrack = (TTree*)inf->Get("yongsunTrack");
  TTree *tmixTrk      = (TTree*)inf->Get("mTrk2");
  TTree *tmixTrkMoM      = (TTree*)inf->Get("mTrk1");
  TTree *yongsunPhotonTree  = (TTree*)inf->Get("yongsunPhotonTree");
  TTree *yongsunSkimTree = (TTree*)inf->Get("yongsunSkimTree");
  TTree *yongsunHiEvt  = (TTree*)inf->Get("yongsunHiEvt");
  TTree *yongsunJetakPu3PF  = (TTree*)inf->Get("yongsunJetakPu3PF");
  TTree *tgj  = (TTree*)inf->Get("tgj");
  TTree *tmixJet  = (TTree*)inf->Get("tmixJet");
  
  TTree *newyongsunTrack = (TTree*)yongsunTrack->CloneTree(0);
  newyongsunTrack->SetMaxTreeSize(20000000000);

  TTree *newtmixTrk = (TTree*)tmixTrk->CloneTree(0);
  newtmixTrk->SetMaxTreeSize(20000000000);

  TTree *newtmixTrkMoM = (TTree*)tmixTrkMoM->CloneTree(0);
  newtmixTrkMoM->SetMaxTreeSize(20000000000);

  TTree *newyongsunPhotonTree = (TTree*)yongsunPhotonTree->CloneTree(0);
  newyongsunPhotonTree->SetMaxTreeSize(20000000000);

  TTree *newyongsunSkimTree = (TTree*)yongsunSkimTree->CloneTree(0);
  newyongsunSkimTree->SetMaxTreeSize(20000000000);

  TTree *newyongsunHiEvt = (TTree*)yongsunHiEvt->CloneTree(0);
  newyongsunHiEvt->SetMaxTreeSize(20000000000);

  TTree *newyongsunJetakPu3PF = (TTree*)yongsunJetakPu3PF->CloneTree(0);
  newyongsunJetakPu3PF->SetMaxTreeSize(20000000000);

  TTree *newtgj = (TTree*)tgj->CloneTree(0);
  newtgj->SetMaxTreeSize(20000000000);

  TTree *newtmixJet = (TTree*)tmixJet->CloneTree(0);
  newtmixJet->SetMaxTreeSize(20000000000);


  EvtSel evt;
  GammaJet gj;

  TBranch        *b_evt;
  TBranch        *b_gj;
  tgj->SetBranchAddress("evt", &evt, &b_evt);
  tgj->SetBranchAddress("lpho", &gj, &b_gj);
  
  //////////// usual track ///////////////
  Int_t           nTrk;
  TBranch        *b_nTrk;   //!                                                                                                                      
  Float_t         injetPt[80000];   //[nTrk]                                                                                         
  TBranch        *b_injetPt;   //!                                                         
  
  Float_t         injetWeight[80000];   //[nTrk]                                                                                                          
  yongsunTrack->SetBranchAddress("ntrk", &nTrk, &b_nTrk);
  yongsunTrack->SetBranchAddress("injetPt", injetPt, &b_injetPt);
  newyongsunTrack->Branch("injetWeight",injetWeight,"injetWeight[ntrk]");
  ///////////  mixed track //////////////
  Int_t           nTrkm;
  TBranch        *b_nTrkm;   //!                                                                                                   
  
  Float_t         injetPtm[80000];   //[nTrk]   
  TBranch        *b_injetPtm;   //!                                                                                             
  
  Float_t         injetWeightm[80000];   //[nTrk]                                                                          

  tmixTrk->SetBranchAddress("ntrk", &nTrkm, &b_nTrkm);
  tmixTrk->SetBranchAddress("injetPt", injetPtm, &b_injetPtm);
  newtmixTrk->Branch("injetWeight",injetWeightm,"injetWeight[ntrk]");
  
  ///////////// second mix track ////////////
  Int_t           nTrkMoM;
  TBranch        *b_nTrkMoM;  
  Float_t         xjetPtMoM[80000];   //[nTrk]                                                                           
  float           xmjetPtMoM[80000];
  TBranch        *b_xjetPtMoM;   //!                                                                   
  TBranch        *b_xmjetPtMoM;   //!                                                                                                                               
  Float_t         xjetWeightMoM[80000];   //[nTrk]                                                                                
  Float_t         xmjetWeightMoM[80000];
  tmixTrkMoM->SetBranchAddress("ntrk", &nTrkMoM, &b_nTrkMoM);
  tmixTrkMoM->SetBranchAddress("xjetPt",  xjetPtMoM,  &b_xjetPtMoM);
  tmixTrkMoM->SetBranchAddress("xmjetPt",  xmjetPtMoM,  &b_xmjetPtMoM);
  newtmixTrkMoM->Branch("xjetWeight",xjetWeightMoM,"xjetWeight[ntrk]");
  newtmixTrkMoM->Branch("xmjetWeight",xmjetWeightMoM,"xmjetWeight[ntrk]");
  
  ////////////////////////////////////////// 
  

  
  
  /// Get the weight table!! //
  int cutOpt = 9999;
  TFile *infTable = new TFile(Form("jetReweightingTable_cutOpt%d.root",cutOpt));
  TH3D* corrTable = (TH3D*)infTable->Get("hRatio3D");
  ///////////////////////////

  Long64_t nentries = tgj->GetEntries();
  for (Long64_t jentry=0;  jentry<nentries;jentry++) {
  //  for (Long64_t jentry=0;  jentry<1000 ; jentry++) {
    if (jentry% 500 == 0) cout <<jentry<<" / "<<nentries<<" "<<setprecision(2)<<(double)jentry/nentries*100<<endl;
    yongsunTrack->GetEntry(jentry);
    tmixTrk->GetEntry(jentry);
    tmixTrkMoM->GetEntry(jentry);
    yongsunPhotonTree->GetEntry(jentry);
    yongsunSkimTree->GetEntry(jentry);
    yongsunHiEvt->GetEntry(jentry);
    yongsunJetakPu3PF->GetEntry(jentry);
    tgj->GetEntry(jentry);
    tmixJet->GetEntry(jentry);
    
    // normal tracks
    for ( int it = 0 ; it<=nTrk ; it++) { 
      if ( injetPt[it] < 20 ) {  /// change into flexible cut 
	injetWeight[it] = 0 ;
      }
      else {
	injetWeight[it] = corrTable->GetBinContent(  corrTable->FindBin( evt.cBin, gj.photonEt, injetPt[it] )) ; 
      }
    }
    
    // mTrk2 
    for ( int it = 0 ; it<=nTrkm ; it++) {
      if ( injetPtm[it] < 20 ) {  /// change into flexible cut                                                                                                                    
	injetWeightm[it] = 0 ;
      }
      else {
        injetWeightm[it] = corrTable->GetBinContent(  corrTable->FindBin( evt.cBin, gj.photonEt, injetPtm[it] )) ;
      }
      
    }
    
    // mTrk1 
    for ( int it = 0 ; it<=nTrkMoM ; it++) {
      if ( xjetPtMoM[it] < 20 ) {  /// change into flexible cut                                                                                                                          
        xjetWeightMoM[it] = 0 ;
      }
      else {
        xjetWeightMoM[it] = corrTable->GetBinContent(  corrTable->FindBin( evt.cBin, gj.photonEt, xjetPtMoM[it] )) ;
      }
      
      
      if ( xmjetPtMoM[it] < 20 ) { 
	xmjetWeightMoM[it] = 0 ;
      }
      else {
	xmjetWeightMoM[it] = corrTable->GetBinContent(  corrTable->FindBin( evt.cBin, gj.photonEt, xmjetPtMoM[it] )) ;
      }

      
      
    }
    
    newyongsunTrack->Fill();
    newtmixTrk->Fill();
    newtmixTrkMoM->Fill();
    newyongsunPhotonTree->Fill();
    newyongsunSkimTree->Fill();
    newyongsunHiEvt->Fill();
    newyongsunJetakPu3PF->Fill();
    newtgj->Fill();
    newtmixJet->Fill();

    
  }
  
  newyongsunTrack->AutoSave();
  newtmixTrk->AutoSave();
  newtmixTrkMoM->AutoSave();
  newyongsunPhotonTree->AutoSave();
  newyongsunSkimTree->AutoSave();
  newyongsunHiEvt->AutoSave();
  newyongsunJetakPu3PF->AutoSave();
  newtgj->AutoSave();
  newtmixJet->AutoSave();

  //   newfile_data->Close();   // <<=== If there is close() function. writing stucks in the middle of looping.. I don't know why!!
  cout << " Done! "<< endl;
}



  */
