#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TString.h"
#include "TF1.h"
#include "TLegend.h"
#include "TLine.h"
#include <iostream>
#include <vector>
#include "../selectionCut.h"
using namespace std;

//TString wanted0("SD");
//TString wanted1("NSD");
TString wanted0("DF");
TString wanted1("ND");

// === helpers ===
Double_t histChi2(TH1 * h1, TH1 *h2)
{
  Double_t sum=0;
  for (Int_t i=1; i<=h1->GetNbinsX(); ++i) {
    Double_t binDiff = h2->GetBinContent(i)-h1->GetBinContent(i);
    Double_t binE1 = h1->GetBinError(i);
    Double_t binE2 = h2->GetBinError(i);
    Double_t binChi2 = binDiff*binDiff/(binE1*binE1+binE2*binE2);
    sum+=binChi2;
  }
  sum=sum/h1->GetNbinsX();
  return sum;
}
Double_t hist2Chi2(TH2 * h1, TH2 *h2)
{
  Double_t sum=0;
  for (Int_t i=1; i<=h1->GetNbinsX(); ++i) {
    for (Int_t j=1; j<=h1->GetNbinsY(); ++j) {
      Double_t binDiff = h2->GetBinContent(i,j)-h1->GetBinContent(i,j);
      Double_t binChi2 = binDiff*binDiff;
      sum+=binChi2;
    }
  }
  return sum;
}

Double_t histDiffrChi2(
    const vector<TString> & hists,
    const vector<TString> & etype,
    Double_t testWantedFrac   = 0.11,
    Int_t draw            = 0,
    Double_t ymax         = 0.025)
{
  if (draw) {
    for (Int_t i=0; i<hists.size(); ++i) {
      if (i<2 || (i==4||i==5)) cout << "use: " << hists[i] << endl;
    }
  }
  TH1D * hData = (TH1D*)(gDirectory->FindObject(hists[0])->Clone("hData"));
  TH1D * hMC = (TH1D*)(gDirectory->FindObject(hists[1])->Clone("hMC"));

  /*
  TH1D * h1 = (TH1D*)(gDirectory->FindObject(hists[2])->Clone("h1"));
  TH1D * h2 = (TH1D*)(gDirectory->FindObject(hists[3])->Clone("h2"));
  */
  TH1D * h1 = (TH1D*)(gDirectory->FindObject(hists[4])->Clone("h1"));
  TH1D * h2 = (TH1D*)(gDirectory->FindObject(hists[5])->Clone("h2"));

  // calc rel frac
  Double_t testNSDFrac = 1-testWantedFrac;
  Double_t MCSDFrac = (Double_t)h1->GetEntries()/(Double_t)hMC->GetEntries();
  Double_t MCNSDFrac = (Double_t)h2->GetEntries()/(Double_t)hMC->GetEntries();
  Double_t SDRelFrac = testWantedFrac/MCSDFrac;
  Double_t NSDRelFrac = testNSDFrac/MCNSDFrac;
  //printf("histDiffrChi2 - trial MC SD Frac(rel): %f(%f), trial NSD Frac(rel): %f(%f)\n",testWantedFrac,SDRelFrac,testNSDFrac,NSDRelFrac);
  // scale
  h1->Scale(1./hMC->GetEntries()/h1->GetBinWidth(1));
  h2->Scale(1./hMC->GetEntries()/h1->GetBinWidth(1));
  hMC->Scale(1./hMC->GetEntries()/h1->GetBinWidth(1));
  hData->Scale(1./hData->GetEntries()/h1->GetBinWidth(1));

  // combine different processes in MC with given weights
  TH1D * h3 = (TH1D*)h2->Clone("h3");
  h3->SetLineColor(kRed);
  h3->SetLineStyle(1);
  h3->SetMarkerColor(kRed);
  h3->SetMarkerStyle(kOpenSquare);

  h3->Scale(NSDRelFrac);
  h3->Add(h1,SDRelFrac);

  // Result
  Double_t result = histChi2(hData,h3);

  // if draw
  if (draw) {
    cout << "Draw: trial " << wanted0 << "frac: " << testWantedFrac << "  Raw hist chi2: " << result << endl;
    //hMC->Draw("h");
    hMC->SetMarkerStyle(0);
    hMC->SetLineWidth(1);
    hMC->SetLineStyle(7);
    hData->SetMaximum(ymax);
    hData->Draw("E");
    h3->Draw("hist same");
    h1->SetLineColor(kBlue);
    h1->SetLineStyle(7);
    h1->SetMarkerStyle(kOpenStar);
    h1->SetMarkerColor(kBlue);
    h1->Draw("E same");
    h2->SetLineStyle(3);
    h2->SetMarkerStyle(kOpenSquare);
    h2->SetMarkerColor(kRed-1);
    h1->SetLineColor(kRed-1);
    h2->Draw("E same");
    //  - add legend -
    TLegend *leg2 = new TLegend(0.651,0.776,0.953,0.928,NULL,"brNDC");
    leg2->SetFillColor(0);
    leg2->SetBorderSize(0);
    leg2->AddEntry(hData,"Data","p");
    leg2->AddEntry(h1,Form("MC - Best Fit %s",wanted0.Data()),"p");
    leg2->AddEntry(h2,Form("MC - Best Fit %s",wanted1.Data()),"p");
    leg2->AddEntry(h3,"MC - Best Fit All","l");
    leg2->Draw();
  }
  else {
    //cout << "SDRelFrac: " << SDRelFrac << "  Raw hist chi2: " << result << endl;
    hData->Delete();
    hMC->Delete();
    h1->Delete();
    h2->Delete();
    h3->Delete();
  }

  // done
  return result;
}

void fillHist(const char* var,const char* hname,
    TTree * treeData, TTree * treeMC, TCut dataSel, TCut mcSel,
    const vector<TString> & etype, const vector<TCut> & etypeCut, vector<TString> & outHists)
{
  TCanvas * c = new TCanvas(Form("c%s",hname),Form("c%s",hname),500,500);

  TString hData = TString(hname)+"_data";
  printf("%s>>%s,%s\n",var,hData.Data(),TString(dataSel).Data());
  treeData->Draw(Form("%s>>%s",var,hData.Data()),dataSel);
  outHists.push_back(hData);

  for (Int_t i=0; i<etype.size(); ++i) {
    TString hMC = TString(hname)+"_pythia_"+etype[i];
    TCut mcCut = mcSel&&etypeCut[i];
    printf("%s>>%s,%s\n",var,hMC.Data(),TString(mcCut).Data());
    treeMC->Draw(Form("%s>>%s",var,hMC.Data()),mcCut,"same");
    outHists.push_back(hMC);
  }
}
Double_t calcFrac(TTree * treeMC, TCut mcSel,
    const vector<TString> & etype, const vector<TCut> & etypeCut,
    TString want="none")
{
  Double_t ans=-1;
  Double_t den, num, frac, selDen, selNum, selFrac;

  den = treeMC->GetEntries(etypeCut[0]);
  selDen = treeMC->GetEntries(mcSel&&etypeCut[0]);
  cout << "-- Base Cut: " << TString(mcSel&&etypeCut[0]) << endl;
  for (Int_t i=0; i<etype.size(); ++i) {
    num = treeMC->GetEntries(etypeCut[i]); 
    frac = num/den;
    TCut mcCut = mcSel&&etypeCut[i];
    selNum = treeMC->GetEntries(mcCut);
    selFrac = selNum/selDen;
    TString t = etype[i];
    printf("MC input %s frac: %f, after selection MC %s frac: %f.\n",t.Data(),frac,t.Data(),selFrac);
    cout << "- " << mcCut << endl;
    printf("- numbers: before sel: %f/%f, after sel: %f/%f\n",num,den,selNum,selDen);
    if (t==want)
      ans=selFrac;
  }
  cout << "answer: " << ans << endl;
  return ans;
}

// === Main function ===
void matchFrac(int testMC = 0, int doSel = 1,
    const char * datafname="pixelTree_merge_BSC_Tuned_v1_Pythia_MinBias_D6T_900GeV_d20091210_SDRelFrac0.5.root",
    const char * mcfname="pixelTree_merge_BSC_Tuned_v1_Pythia_MinBias_D6T_900GeV_d20091210.root",
    const char * databgfname="pixelTree_123596v5-emptytarget_SDRelFrac1.0.root")
{
  // get trees
  TFile * dataFile = new TFile(datafname);
  TFile * mcFile = new TFile(mcfname);
  TFile * databgFile = new TFile(databgfname);
  TTree * treeData; dataFile->GetObject("PixelTree",treeData);
  TTree * treeMC;   mcFile->GetObject("PixelTree",treeMC);
  TTree * treeDataBg; databgFile->GetObject("PixelTree",treeDataBg);

  // Now define output
  TFile * fout = new TFile(TString("histAna/")+TString(datafname).ReplaceAll("pixelTree","histo"),"RECREATE");

  // trigger
  selectionCut mcSel(1,doSel);
  selectionCut dataSel(0,doSel);
  printf("\n===== Triggering =====\n");
  cout << "Data: " << TString(dataSel.Cut) << endl;
  cout << "MC: " << TString(mcSel.Cut) << endl;

  // configuation
  // sources
  vector<TString> source;
  source.push_back("data");
  source.push_back("pythia");
  // colors
  vector<Color_t> color;
  color.push_back(kRed);
  color.push_back(kBlue);
  color.push_back(kGreen-1);
  // observables
  vector<TString> obs;
  obs.push_back("EvtEta");
  obs.push_back("EaddEp");
  obs.push_back("EsubEp");
  // event types
  vector<TString> etype;
  vector<TCut> etypeCut;
  vector<TCut> etypePhojCut;
  etype.push_back("All"); etypeCut.push_back("1==1");
  etype.push_back("SD"); etypeCut.push_back("evtType==92 || evtType==93");
  etype.push_back("NSD"); etypeCut.push_back("evtType!=92 && evtType!=93");
  etype.push_back("DF"); etypeCut.push_back("evtType==92 || evtType==93 || evtType==94");
  etype.push_back("ND"); etypeCut.push_back("evtType!=92 && evtType!=93 && evtType!=94");
  etype.push_back("DD"); etypeCut.push_back("evtType==94");
  if (testMC==2) {
    etypePhojCut.push_back("evtType==1 || evtType==5 || evtType==6 || evtType==7 || evtType==4");
    etypePhojCut.push_back("evtType==5 || evtType==6");
    etypePhojCut.push_back("evtType==1 || evtType==7 || evtType==4");
    etypePhojCut.push_back("evtType==5 || evtType==6 || evtType==7 || evtType==4");
    etypePhojCut.push_back("evtType==1");
    etypePhojCut.push_back("evtType==7 || evtType==4");
  }
  // container for all declared histograms
  vector<TH1*> vh1;

  //
  // declare histograms
  printf("now declare hists\n");
  const Double_t EPzMax=200;
  const Int_t EPzNBINS=EPzMax/5;
  for (Int_t i=0; i<source.size(); ++i) {
    vh1.push_back(new TH1D(Form("hEvtEta_%s",source[i].Data()),";Event #eta;",100,-5,5));
    vh1.push_back(new TH1D(Form("hEaddEp_%s",source[i].Data()),";#Sigma E+Pz;",EPzNBINS,0,EPzMax));
    vh1.push_back(new TH1D(Form("hEsubEp_%s",source[i].Data()),";#Sigma E-Pz;",EPzNBINS,0,EPzMax));
    vh1.push_back(new TH2D(Form("hEPz_%s",source[i].Data()),";#Sigma E+Pz;E-Pz",EPzNBINS,0,EPzMax,EPzNBINS,0,EPzMax));
    if (source[i]=="pythia") {
      for (Int_t j=0; j<etype.size(); ++j) {
	vh1.push_back(new TH1D(Form("hEvtEta_%s_%s",source[i].Data(),etype[j].Data()),";Event #eta;",100,-5,5));
	vh1.push_back(new TH1D(Form("hEaddEp_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E+Pz;",EPzNBINS,0,EPzMax));
	vh1.push_back(new TH1D(Form("hEsubEp_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E-Pz;",EPzNBINS,0,EPzMax));
	vh1.push_back(new TH2D(Form("hEPz_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E+Pz;E-Pz",EPzNBINS,0,EPzMax,EPzNBINS,0,EPzMax));
      }
    }
  }
  // set histograms
  for (Int_t ih1=0; ih1<vh1.size(); ++ih1) {
    //cout << "hist: " << vh1[ih1]->GetName() << " title: " << vh1[ih1]->GetXaxis()->GetTitle() << endl;
    vh1[ih1]->Sumw2();
  }

  // Fill histos
  vector<TString> evtEtaHists;
  fillHist("evtEta","hEvtEta",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,evtEtaHists);
  vector<TString> EaddEpHists;
  fillHist("SumEaddEp","hEaddEp",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,EaddEpHists);
  vector<TString> EPzHists;
  fillHist("SumEsubEp:SumEaddEp","hEPz",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,EPzHists);
  // calc cuts
  printf("\n===== MC Input =====\n");
  calcFrac(treeMC,mcSel.Cut,etype,etypeCut);
  Double_t truthFrac=-1;
  if (testMC==0) {
    printf("\n===== Data Input =====\n");
    printf("%d passed cut\n",treeData->GetEntries(dataSel.Cut));
  }
  if (testMC==1) {
    printf("\n===== \"Data\" Input =====\n");
    truthFrac = calcFrac(treeData,mcSel.Cut,etype,etypeCut,wanted0);
  }
  else if (testMC==2) {
    printf("\n===== \"Data\" Input =====\n");
    truthFrac = calcFrac(treeData,mcSel.Cut,etype,etypePhojCut,wanted0);
  }


  // calc chi2
  printf("\n=========== Chi2 clac ================\n");
  Double_t maxSDFrac=0.5;
  Int_t N=50;
  TH1D * hChi2 = new TH1D("hChi2",Form(";%s Fraction;#chi^{2}",wanted0.Data()),N,0,maxSDFrac);
  Double_t step = maxSDFrac/(Float_t)N;
  for (Int_t i=1; i<=N; ++i) {
    Double_t trialFrac = i*step;
    Double_t chi2 = histDiffrChi2(
	EaddEpHists,
	etype,
	trialFrac);
    hChi2->SetBinContent(i,chi2);
  }

  TCanvas * cChi2 = new TCanvas("cChi2","cChi2",600,600);
  hChi2->SetMinimum(0);
  hChi2->Draw();
  // fit chi2
  Double_t chi2Min = hChi2->GetBinCenter(hChi2->GetMinimumBin());
  TF1 *myfun = new TF1("myfun","[1]*(x-[0])*(x-[0])+[2]");
  myfun->SetParameters(chi2Min,100,1);
  hChi2->Fit("myfun","emw M","",chi2Min-0.05,chi2Min+0.05);
  // get error
  Double_t a = myfun->GetParameter(0);
  Double_t b = myfun->GetParameter(1);
  Double_t c = myfun->GetParameter(2);
  Double_t equRoot = sqrt(1./b*(1));
  Double_t chiELow = a-equRoot;
  Double_t chiEHigh = a+equRoot;
  Double_t bestX = myfun->GetParameter(0);
  printf("\n\n   Best %s fit fraction: %f\n",wanted0.Data(),bestX);
  printf("       Error: (%f,%f)\n\n",bestX-chiELow,chiEHigh-bestX);
  TLine * lELow = new TLine(chiELow,hChi2->GetMinimum(),chiELow,hChi2->GetMaximum());
  lELow->Draw("same");
  TLine * lEHigh = new TLine(chiEHigh,hChi2->GetMinimum(),chiEHigh,hChi2->GetMaximum());
  lEHigh->Draw("same");
  // mc truth if using mc
  if (testMC>0) {
    TLine * l = new TLine(truthFrac,hChi2->GetMinimum(),truthFrac,hChi2->GetMaximum());
    l->SetLineColor(2);
    l->Draw("same");
    TLegend *leg2 = new TLegend(0.1879,0.839,0.39,0.942,NULL,"brNDC");
    leg2->SetFillColor(0);
    leg2->SetBorderSize(0);
    leg2->AddEntry(l,"MC truth:","l");
    leg2->AddEntry("",Form("%.4f",truthFrac),"");
    leg2->Draw();
  }
  cChi2->Print(Form("plots/%s_cChi2_Sel%d.gif",datafname,doSel));

  // draw distributions
  TCanvas * cEaddPz = new TCanvas("cEaddPz","cEaddPz",600,600);
  histDiffrChi2(
      EaddEpHists,
      etype,
      myfun->GetParameter(0),
      1,
      0.035);
  cEaddPz->Print(Form("plots/%s_cEaddPz_Sel%d.gif",datafname,doSel));
  TCanvas * cEvtEta = new TCanvas("cEvtEta","cEvtEta",600,600);
  histDiffrChi2(
      evtEtaHists,
      etype,
      myfun->GetParameter(0),
      1,
      1);
  cEvtEta->Print(Form("plots/%s_cEvtEta_Sel%d.gif",datafname,doSel));

  // save
  fout->Write();
  //fout->Close();
}
