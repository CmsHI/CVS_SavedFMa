#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TString.h"
#include "TF1.h"
#include "TLegend.h"
#include "TLine.h"
#include "TSystem.h"
#include <iostream>
#include <vector>
#include "../selectionCut.h"
using namespace std;

// Declare some useful global variables
Int_t anaMode; // 0 for D vs ND, 1 for SD vs NSD, 2 for SD, DD, ND
TString gDataSource;
TString gMCSource;
TString wanted0;
TString wanted1;
TString wanted2;
TString dataHistLabel="data";
TString mcHistLabel="mc";

// === helpers ===
Double_t histChi2(TH1 * h1, TH1 *h2)
{
  Double_t sum=0;
  for (Int_t i=1; i<=h1->GetNbinsX(); ++i) {
    Double_t binDiff = h2->GetBinContent(i)-h1->GetBinContent(i);
    Double_t binE1 = h1->GetBinError(i);
    if (binE1==0) binE1=1;
    Double_t binE2 = h2->GetBinError(i);
    if (binE2==0) binE2=1;
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
    Int_t mode            = 0,
    Double_t testWantedFrac0   = 0.11,
    Double_t testWantedFrac1  = -1,
    Int_t draw            = 0,
    Double_t ymax         = 0.025)
{
  Int_t index1=-1,index2=-1,index3=6;
  //2=SD, 3=NSD, 4=D, 5=ND, 6=DD
  if ( mode==0) {
    index1=4; index2=5;
  } else if (mode==1) {
    index1=2; index2=3;
  } else if (mode==2) {
    index1=2; index2=6; index3=5;
  }

  if (draw) {
    for (Int_t i=0; i<hists.size(); ++i) {
      if (i<2 || (i==index1||i==index2)) cout << "use: " << hists[i] << endl;
      if (mode==2 && i==index3) cout << "use: " << hists[i] << endl;
    }
  }
  TH1D * hData = (TH1D*)(gDirectory->FindObject(hists[0])->Clone("hData"));
  TH1D * hMC = (TH1D*)(gDirectory->FindObject(hists[1])->Clone("hMC"));

  TH1D * h1 = (TH1D*)(gDirectory->FindObject(hists[index1])->Clone("h1"));
  TH1D * h2 = (TH1D*)(gDirectory->FindObject(hists[index2])->Clone("h2"));
  TH1D * h3 = (TH1D*)(gDirectory->FindObject(hists[index3])->Clone("h3"));

  // calc rel frac
  // correction factor for limited range
  Double_t trueFrac0=h1->GetEntries()/hMC->GetEntries();
  Double_t trueFrac0InRange=h1->Integral()/hMC->Integral();
  Double_t testFrac0InRange = testWantedFrac0*(trueFrac0InRange/trueFrac0);

  // scale all pdf's in range to unity
  h1->Scale(1./h1->Integral()/h1->GetBinWidth(1));
  h2->Scale(1./h2->Integral()/h2->GetBinWidth(1));
  hMC->Scale(1./hMC->Integral()/hMC->GetBinWidth(1));
  hData->Scale(1./hData->Integral()/hData->GetBinWidth(1));

  // combine
  TH1D * hFit = (TH1D*)hMC->Clone("hFit");
  h1->Scale(testFrac0InRange);
  h2->Scale(1-testFrac0InRange);
  hFit->Add(h1,h2);
  if (mode==2) hFit->Add(h3);

  // Result
  Double_t result = histChi2(hData,hFit);

  // if draw
  if (draw) {
    if (mode<2) {
      cout << "MC Truth frac0: " << trueFrac0 << " In range: " << trueFrac0InRange << endl;
      cout << "Draw: trial " << wanted0 << "frac: " << testWantedFrac0
	<< " In range: " << testFrac0InRange << "  Raw hist chi2: " << result << endl;
      cout << "hData area: " << hData->Integral()*hData->GetBinWidth(1) << ", Entries: " << hData->GetEntries() << endl;
      cout << "hMC area: " << hMC->Integral()*hMC->GetBinWidth(1) << ", Entries: " << hMC->GetEntries() << endl;
      cout << "h1 area: " << h1->Integral()*h1->GetBinWidth(1) << ", Entries: " << h1->GetEntries() << endl;
      cout << "h2 area: " << h2->Integral()*h2->GetBinWidth(1) << ", Entries: " << h2->GetEntries() << endl;
      cout << "hFit area: " << hFit->Integral()*hFit->GetBinWidth(1) << ", Entries: " << hFit->GetEntries() << endl;
    }
    else if (mode==2)
      cout << "Draw: trial " << wanted0 << ", " << wanted1 << "frac: " << testWantedFrac0 << ", " << testWantedFrac1<< "  Raw hist chi2: " << result << endl;
    //hMC->Draw("h");
    hMC->SetMarkerStyle(0);
    hMC->SetLineWidth(1);
    hMC->SetLineStyle(7);
    // Fit
    hFit->SetLineColor(kRed);
    hFit->SetLineStyle(1);
    hFit->SetMarkerStyle(0);
    hFit->SetMinimum(0.0001);
    hFit->SetMaximum(ymax);
    hFit->Draw("hist E");
    // data
    hData->Draw("Esame");
    // h1
    h1->SetLineColor(kBlue);
    h1->SetMarkerStyle(kOpenStar);
    h1->SetMarkerColor(kBlue);
    h1->Draw("E same");
    if (mode<2) {
      // h2
      h2->SetMarkerStyle(kOpenSquare);
      h2->SetMarkerColor(kRed-1);
      h2->SetLineColor(kRed-1);
      h2->Draw("E same");
    }
    if (mode==2) {
      // h2
      h2->SetMarkerStyle(kOpenCircle);
      h2->SetMarkerColor(kGreen-1);
      h2->SetLineColor(kGreen-1);
      h2->Draw("E same");
      // h3
      h3->SetMarkerStyle(kOpenSquare);
      h3->SetMarkerColor(kRed-1);
      h3->SetLineColor(kRed-1);
      h3->Draw("E same");
    }

    //  - add legend -
    TLegend *leg2 = new TLegend(0.651,0.776,0.953,0.928,NULL,"brNDC");
    leg2->SetFillColor(0);
    leg2->SetBorderSize(0);
    TString drawMode="Best Fit";
    if (draw==2)
      drawMode="Default";
    leg2->AddEntry(hData,Form("%s",gDataSource.Data()),"p");
    leg2->AddEntry(h1,Form("%s - %s %s",gMCSource.Data(),drawMode.Data(),wanted0.Data()),"p");
    leg2->AddEntry(h2,Form("%s - %s %s",gMCSource.Data(),drawMode.Data(),wanted1.Data()),"p");
    if (mode==2) leg2->AddEntry(h3,Form("%s - %s %s",gMCSource.Data(),drawMode.Data(),wanted2.Data()),"p");
    leg2->AddEntry(hFit,Form("%s - %s All",gMCSource.Data(),drawMode.Data()),"l");
    leg2->Draw();
  }
  else {
    //cout << "SDRelFrac: " << SDRelFrac << "  Raw hist chi2: " << result << endl;
    hData->Delete();
    hMC->Delete();
    h1->Delete();
    h2->Delete();
    h3->Delete();
    hFit->Delete();
  }

  // done
  return result;
}

void fillHist(const char* var,const char* hname,
    TTree * treeData, TTree * treeMC, TCut dataSel, TCut mcSel,
    const vector<TString> & etype, const vector<TCut> & etypeCut, vector<TString> & outHists)
{
  TCanvas * c = new TCanvas(Form("c%s",hname),Form("c%s",hname),500,500);

  TString hData = TString(hname)+"_"+dataHistLabel;
  //printf("%s>>%s,%s\n",var,hData.Data(),TString(dataSel).Data());
  treeData->Draw(Form("%s>>%s",var,hData.Data()),dataSel);
  outHists.push_back(hData);

  for (Int_t i=0; i<etype.size(); ++i) {
    TString hMC = TString(hname)+"_"+mcHistLabel+"_"+etype[i];
    TCut mcCut = mcSel&&etypeCut[i];
    //printf("%s>>%s,%s\n",var,hMC.Data(),TString(mcCut).Data());
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
    //cout << "- " << mcCut << endl;
    printf("- numbers: before sel: %f/%f, after sel: %f/%f\n",num,den,selNum,selDen);
    if (t==want)
      ans=selFrac;
  }
  cout << "answer: " << ans << endl;
  return ans;
}

// === Main function ===
void matchFrac(TString AnaVersion="V0",
    TString DataSource = "data", TString MCSource = "pythia",
    TString AnaObs = "EaddEp", //EvtEta, EsubEp
    int doSel = 1, int mode=0,
    float EPzMin=0, float EPzMax=200, float EPzBinSize=5)
{
  // top level info
  gDataSource=DataSource;
  gMCSource=MCSource;
  TString AnaTag = Form("ana%s_%s_Mode%d_Min%.0f_Max%.0f_Delta%.0f_Sel%d_%s_use_%s",
      AnaVersion.Data(),AnaObs.Data(),mode,
      EPzMin,EPzMax,EPzBinSize,
      doSel,
      DataSource.Data(),MCSource.Data());
  cout << "====== Ana: " << AnaTag << endl;
  // mkdir dir for output
  TString outdir=Form("plots/%s/%s_Sel%d",AnaVersion.Data(),AnaObs.Data(),doSel);
  gSystem->mkdir(Form("%s",outdir.Data()),kTRUE);

  // set anaMode
  anaMode=mode;
  if (mode==0) {
    wanted0 ="DF";
    wanted1 ="ND";
  }
  if (mode==1) {
    wanted0 ="SD";
    wanted1 ="NSD";
  }
  if (mode==2) {
    wanted0 = "SD";
    wanted1 = "DD";
    wanted2 = "ND";
  }

  // ==== get trees ====
  TString * datafname;
  TString * mcfname;
  TString * databgfname;
  // data
  if (DataSource=="data")
    datafname = new TString("../input/pixelTree_124022a3a4-vtxcomp_MB.root");
  if (DataSource=="pythia")
    datafname = new TString("pixelTree_Pythia_MinBias_D6T_900GeV_d20091229_Vertex1229.root");
  if (DataSource=="phojet") {
    datafname= new TString("pixelTree_Phojet_MinBias_900GeV_d20100108.root");
  }
  // mc
  if (MCSource=="pythia")
    mcfname= new TString("pixelTree_Pythia_MinBias_D6T_900GeV_d20091229_Vertex1229.root");
  if (MCSource=="phojet") {
    mcfname= new TString("pixelTree_Phojet_MinBias_900GeV_d20100108.root");
  }
  databgfname= new TString("pixelTree_123596v5-emptytarget_SDRelFrac1.0.root");
  cout << "Data: " << datafname->Data() << endl;
  cout << "MC:   " << mcfname->Data() << endl;

  TFile * dataFile = new TFile(*datafname);
  TFile * mcFile = new TFile(*mcfname);
  //TFile * databgFile = new TFile(*databgfname);
  TTree * treeData; dataFile->GetObject("PixelTree",treeData);
  TTree * treeMC;   mcFile->GetObject("PixelTree",treeMC);
  //TTree * treeDataBg; databgFile->GetObject("PixelTree",treeDataBg);

  // Now define output
  TFile * fout = new TFile(Form("%s/%s.root",outdir.Data(),AnaTag.Data()),"RECREATE");

  // ===== trigger =====
  bool isMC=true;
  selectionCut mcSel(isMC,doSel,124023,41,96);
  if (DataSource=="data") isMC=false;
  selectionCut dataSel(isMC,doSel,124023,41,96);
  dataSel.AnaTag = AnaTag;
  printf("\n===== Triggering =====\n");
  cout << "Data: " << TString(dataSel.Cut) << endl;
  cout << "MC: " << TString(mcSel.Cut) << endl;

  // configuation
  // sources
  vector<TString> source;
  source.push_back(dataHistLabel);
  source.push_back(mcHistLabel);
  // colors
  vector<Color_t> color;
  color.push_back(kRed);
  color.push_back(kBlue);
  color.push_back(kGreen-1);
  // event types
  vector<TString> etype;
  vector<TCut> etypeCut;
  vector<TCut> etypePhojCut;
  //  -pythia-
  etype.push_back("All"); etypeCut.push_back("1==1");
  etype.push_back("SD"); etypeCut.push_back("evtType==92 || evtType==93");
  etype.push_back("NSD"); etypeCut.push_back("evtType!=92 && evtType!=93");
  etype.push_back("DF"); etypeCut.push_back("evtType==92 || evtType==93 || evtType==94");
  etype.push_back("ND"); etypeCut.push_back("evtType!=92 && evtType!=93 && evtType!=94");
  etype.push_back("DD"); etypeCut.push_back("evtType==94");
  //  -phojet-
  etypePhojCut.push_back("evtType==1 || evtType==5 || evtType==6 || evtType==7 || evtType==4");
  etypePhojCut.push_back("evtType==5 || evtType==6");
  etypePhojCut.push_back("evtType==1 || evtType==7 || evtType==4");
  etypePhojCut.push_back("evtType==5 || evtType==6 || evtType==7 || evtType==4");
  etypePhojCut.push_back("evtType==1");
  etypePhojCut.push_back("evtType==7 || evtType==4");
  // container for all declared histograms
  vector<TH1*> vh1;

  //
  // declare histograms
  printf("now declare hists\n");
  Double_t EPzYMax=0.035/(EPzMax/200), Chi2YMax=60;
  if (doSel==4) {
    EPzYMax=0.01/(EPzMax/200);
    Chi2YMax=20;
  }
  if (doSel==10) {
    EPzYMax=0.12/(EPzMax/200);
  }
  const Int_t EPzNBINS=EPzMax/EPzBinSize;
  for (Int_t i=0; i<source.size(); ++i) {
    vh1.push_back(new TH1D(Form("hEvtEta_%s",source[i].Data()),";Event #eta;",100,-5,5));
    vh1.push_back(new TH1D(Form("hEaddEp_%s",source[i].Data()),";#Sigma E+Pz;",EPzNBINS,EPzMin,EPzMax));
    vh1.push_back(new TH1D(Form("hEsubEp_%s",source[i].Data()),";#Sigma E-Pz;",EPzNBINS,EPzMin,EPzMax));
    vh1.push_back(new TH2D(Form("hEPz_%s",source[i].Data()),";#Sigma E+Pz;E-Pz",EPzNBINS,EPzMin,EPzMax,EPzNBINS,EPzMin,EPzMax));
    if (source[i]==mcHistLabel) {
      for (Int_t j=0; j<etype.size(); ++j) {
	vh1.push_back(new TH1D(Form("hEvtEta_%s_%s",source[i].Data(),etype[j].Data()),";Event #eta;",100,-5,5));
	vh1.push_back(new TH1D(Form("hEaddEp_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E+Pz;",EPzNBINS,EPzMin,EPzMax));
	vh1.push_back(new TH1D(Form("hEsubEp_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E-Pz;",EPzNBINS,EPzMin,EPzMax));
	vh1.push_back(new TH2D(Form("hEPz_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E+Pz;E-Pz",EPzNBINS,EPzMin,EPzMax,EPzNBINS,EPzMin,EPzMax));
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
  vector<TString> EaddEpHists;
  vector<TString> EsubEpHists;
  vector<TString> EPzHists;
  if (MCSource=="pythia") {
    fillHist("evtEta","hEvtEta",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,evtEtaHists);
    fillHist("SumEaddEp","hEaddEp",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,EaddEpHists);
    fillHist("SumEsubEp","hEsubEp",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,EsubEpHists);
    fillHist("SumEsubEp:SumEaddEp","hEPz",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,EPzHists);
  }
  if (MCSource=="phojet") {
    fillHist("evtEta","hEvtEta",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypePhojCut,evtEtaHists);
    fillHist("SumEaddEp","hEaddEp",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypePhojCut,EaddEpHists);
    fillHist("SumEsubEp","hEsubEp",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypePhojCut,EsubEpHists);
    fillHist("SumEsubEp:SumEaddEp","hEPz",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypePhojCut,EPzHists);
  }
  // calc cuts
  // for mc
  printf("\n===== MC Input =====\n");
  Double_t mcTruthFrac=-1;
  if (MCSource=="pythia") {
    mcTruthFrac = calcFrac(treeMC,mcSel.Cut,etype,etypeCut,wanted0);
  }
  if (MCSource=="phojet") {
    mcTruthFrac = calcFrac(treeMC,mcSel.Cut,etype,etypePhojCut,wanted0);
  }
  // for data or "data"
  Double_t truthFrac=-1;
  if (DataSource=="data") {
    printf("\n===== Data Input =====\n");
    printf("%d passed cut\n",treeData->GetEntries(dataSel.Cut));
  }
  if (DataSource=="pythia") {
    printf("\n===== \"Data\" Input =====\n");
    truthFrac = calcFrac(treeData,mcSel.Cut,etype,etypeCut,wanted0);
  }
  if (DataSource=="phojet") {
    printf("\n===== \"Data\" Input =====\n");
    truthFrac = calcFrac(treeData,mcSel.Cut,etype,etypePhojCut,wanted0);
  }


  // === calc chi2 ===
  printf("\n=========== Chi2 clac ================\n");
  Int_t N=100;
  Double_t maxTestFrac=0.5;
  TCanvas * cChi2 = new TCanvas("cChi2","cChi2",600,600);
  TH1D * hChi2 = new TH1D("hChi2",Form(";%s Fraction;#chi^{2}",wanted0.Data()),N,0,maxTestFrac);
  hChi2->SetMinimum(0);
  if (Chi2YMax>0) hChi2->SetMaximum(Chi2YMax);
  TH2D * h2Chi2 = new TH2D("h2Chi2",Form(";%s Fraction;%s Fraction",wanted0.Data(),wanted1.Data()),N,0,maxTestFrac,N,0,maxTestFrac);
  h2Chi2->SetMinimum(0);

  Double_t bestX=0, bestY=0, bestZ=0;
  Double_t step = maxTestFrac/(Float_t)N;
  vector<TString> fitObsHists;
  // determine what var to fit
  if (AnaObs=="EvtEta") fitObsHists = evtEtaHists;
  if (AnaObs=="EaddEp") fitObsHists = EaddEpHists;
  if (AnaObs=="EsubEp") fitObsHists = EsubEpHists;
  // make chi2
  if (anaMode==0 || anaMode==1) {
    for (Int_t i=1; i<=N; ++i) {
      Double_t trialFrac = i*step;
      Double_t chi2 = histDiffrChi2(
	  fitObsHists,
	  anaMode,
	  trialFrac);
      hChi2->SetBinContent(i,chi2);
    }
    hChi2->Draw();
    // === fit chi2 ===
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
    printf("       Error: (%f,%f)\n",bestX-chiELow,chiEHigh-bestX);
    printf("       Analysis: %s, %s: %f(%f,%f)\n\n",AnaTag.Data(),wanted0.Data(),bestX,bestX-chiELow,chiEHigh-bestX);
    if (AnaObs=="EvtEta")
      printf("       Twiki - %s, %s: | %s | %s | %.1f+-%.1f\% |\n\n",AnaTag.Data(),wanted0.Data(),AnaObs.Data(),MCSource.Data(),bestX*100,(chiEHigh-bestX)*100);
    else
      printf("       Twiki - %s, %s: | %s (%.0fto%.0fGeV) | %s | %.1f+-%.1f\% |\n\n",AnaTag.Data(),wanted0.Data(),AnaObs.Data(),EPzMin,EPzMax,MCSource.Data(),bestX*100,(chiEHigh-bestX)*100);
    TLine * lELow = new TLine(chiELow,hChi2->GetMinimum(),chiELow,hChi2->GetMaximum());
    //lELow->Draw("same");
    TLine * lEHigh = new TLine(chiEHigh,hChi2->GetMinimum(),chiEHigh,hChi2->GetMaximum());
    //lEHigh->Draw("same");
    // mc truth if using mc as "data"
    if (DataSource=="pythia"||DataSource=="phojet") {
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
    cChi2->Print(Form("%s/%s_cChi2.gif",outdir.Data(),AnaTag.Data()));

    // draw distributions
    // -- default --
    TCanvas * cEaddPzDefault = new TCanvas("cEaddPzDefault","cEaddPzDefault",600,600);
    histDiffrChi2(
	EaddEpHists,
	anaMode,
	//0,
	mcTruthFrac,
	-1,
	2,
	EPzYMax);
    cEaddPzDefault->Print(Form("%s/%s_cEaddPzDefault.gif",outdir.Data(),AnaTag.Data()));

    // -- fitted --
    TCanvas * cEaddPz = new TCanvas("cEaddPz","cEaddPz",600,600);
    histDiffrChi2(
	EaddEpHists,
	anaMode,
	bestX,
	-1,
	1,
	EPzYMax);
    cEaddPz->Print(Form("%s/%s_cEaddPz.gif",outdir.Data(),AnaTag.Data()));
    TCanvas * cEvtEta = new TCanvas("cEvtEta","cEvtEta",600,600);
    histDiffrChi2(
	evtEtaHists,
	anaMode,
	bestX,
	-1,
	1,
	1);
    cEvtEta->Print(Form("%s/%s_cEvtEta.gif",outdir.Data(),AnaTag.Data()));
  } // end of anaMode<2


  if (anaMode==2) {
    for (Int_t i=1; i<=N; ++i) {
      Double_t trialFrac = i*step;
      for (Int_t j=1; j<=N;++j) {
	Double_t trialFrac2 = j*step;
	Double_t chi2 = histDiffrChi2(
	    evtEtaHists,
	    anaMode,
	    trialFrac,
	    trialFrac2);
	h2Chi2->SetBinContent(i,j,chi2);
      }
    }
    h2Chi2->Draw("Cont1");
    Int_t bestXBin,bestYBin,bestZBin;
    h2Chi2->GetMinimumBin(bestXBin,bestYBin,bestZBin);
    bestX=h2Chi2->GetXaxis()->GetBinCenter(bestXBin);
    bestY=h2Chi2->GetYaxis()->GetBinCenter(bestYBin);
    printf("\n\n   Best %s,%s fit fraction: %f,%f\n",wanted0.Data(),wanted1.Data(),bestX,bestY);
    cChi2->Print(Form("%s/%s_c2DChi2.gif",outdir.Data(),AnaTag.Data()));
    TCanvas * cEvtEta = new TCanvas("c2DEvtEta","c2DEvtEta",600,600);
    histDiffrChi2(
	evtEtaHists,
	anaMode,
	0.137,
	0.087,
	1,
	1);
  }


  // save
  fout->Write();
  //fout->Close();
}
