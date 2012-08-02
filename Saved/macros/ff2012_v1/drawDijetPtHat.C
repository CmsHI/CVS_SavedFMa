#include "CutAndBinCollection2011.h"
#include <TRandom3.h>
#include <time.h>

void drawDijetPtHat() 
  
{
  TH1::SetDefaultSumw2();
 
  vector<int> ptHat_;
  vector<float> cs_;
  //  ptHat_.push_back(30);   cs_.push_back(1.079e-02);
  //  ptHat_.push_back(50);   cs_.push_back(1.021e-03);
  ptHat_.push_back(80);   cs_.push_back(9.913e-05);
  ptHat_.push_back(120);  cs_.push_back(1.128e-05);
  ptHat_.push_back(170);  cs_.push_back(1.470e-06);
  ptHat_.push_back(200);  cs_.push_back(5.310e-07);
  ptHat_.push_back(250);  cs_.push_back(1.192e-07);
  ptHat_.push_back(300);  cs_.push_back(3.176e-08);

  multiTreeUtil* tdj= new multiTreeUtil();
  /*  tdj->addFile("jskim_Dijet80_merged_nMix10_July-v1.root","tdj","pthat<120",1);
  tdj->addFile("jskim_Dijet120_merged_nMix10_July-v1.root","tdj","pthat<170",1);
  tdj->addFile("jskim_Dijet170_merged_nMix10_July-v1.root","tdj","pthat<200",1);
  tdj->addFile("jskim_Dijet200_merged_nMix10_July-v1.root","tdj","pthat<250",1);
  tdj->addFile("jskim_Dijet250_merged_nMix10_July-v1.root","tdj","pthat<300",1);
  tdj->addFile("jskim_Dijet300_merged_nMix10_July-v1.root","tdj","",1);
  
  */
    tdj->addFile("jskim_Dijet80_merged_nMix10_July-v1.root","tdj","pthat<120",(9.913e-05 )/78549);
    tdj->addFile("jskim_Dijet120_merged_nMix10_July-v1.root","tdj","pthat<170",( 1.128e-05)/34838);
    tdj->addFile("jskim_Dijet170_merged_nMix10_July-v1.root","tdj","pthat<200",( 1.470e-06)/6108);
    tdj->addFile("jskim_Dijet200_merged_nMix10_July-v1.root","tdj","pthat<250",(5.310e-07 )/35838);
    tdj->addFile("jskim_Dijet250_merged_nMix10_July-v1.root","tdj","pthat<300",( 1.192e-07)/29132);
    tdj->addFile("jskim_Dijet300_merged_nMix10_July-v1.root","tdj","",(3.176e-08)/43015);
  
  
  TH1D* htemp[10];
  int color[10];
  color[0]=1;
  color[1]=2;
  color[2]=4;
  color[3]=5;
  color[4]=6;
  color[5]=9;
  color[6]=1;
 
  TCanvas* c1= new TCanvas("c1","",1000,800);
  c1->Divide(1,2);
    c1->cd(1);
  TH1D* hptljet = new TH1D("hptljet",";leading jet pT (GeV);",48,120,600);
  tdj->Draw2(hptljet,"lJetPt","","");
  TLegend* l1 = new TLegend(0.6723432,0.670013,0.9206198,0.9506693,NULL,"brNDC");
  easyLeg(l1,"");

  for (int i=0;i<=5;i++) {
    htemp[i] = new TH1D(Form("htemp_%d",i),"",10,0,10);
    handsomeTH1(htemp[i], color[i]);
    l1->AddEntry(htemp[i],Form("pt-hat : %dGeV",ptHat_[i]),"l");
  }
  l1->Draw();
  drawText("Leading jet", 0.5,0.9);
  
  c1->cd(2);
  TH1D* hptsljet = new TH1D("hptsljet",";Subleading jet pT (GeV);",55,50,600);
  tdj->Draw2(hptsljet,"slJetPt","","");
  drawText("Subleading jet", 0.5,0.9);



}

