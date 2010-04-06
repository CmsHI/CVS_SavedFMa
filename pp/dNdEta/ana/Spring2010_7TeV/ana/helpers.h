#include "TH1D.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLegend.h"
#include <iostream>
#include <vector>

// Declare some useful global variables
//Int_t anaMode; // 0 for D vs ND, 1 for SD vs NSD, 2 for SD, DD, ND
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
    // better interpretation of chi2: chi2/NDOF
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
    //const vector<TString> & hists,
    const vector<TH1D*> & hists,
    Int_t mode            = 0,
    Double_t testWantedFrac1   = 0.11,
    Double_t testWantedFrac2  = -1,
    Int_t draw            = 0,
    Double_t ymax         = 0.025)
{
  /*
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
      if (i<2 || (i==index1||i==index2)) cout << "use: " << hists[i]->GetName() << endl;
      if (mode==2 && i==index3) cout << "use: " << hists[i]->GetName() << endl;
    }
  }


  return -1;
  */
  /*
  TH1D * hData = (TH1D*)(gDirectory->FindObject(hists[0])->Clone("hData"));
  TH1D * hMC = (TH1D*)(gDirectory->FindObject(hists[1])->Clone("hMC"));
  TH1D * h1 = (TH1D*)(gDirectory->FindObject(hists[index1])->Clone("h1"));
  TH1D * h2 = (TH1D*)(gDirectory->FindObject(hists[index2])->Clone("h2"));
  TH1D * h3 = (TH1D*)(gDirectory->FindObject(hists[index3])->Clone("h3"));
  */
  cout << "data: " << hists[0]->GetName() << endl;
  cout << "h1: " << hists[1]->GetName() << endl;
  cout << "h2: " << hists[2]->GetName() << endl;
  TH1D * hData = (TH1D*)hists[0]->Clone("hData");
  //TH1D * hMC   = (TH1D*)hists[1]->Clone("hMC");
  TH1D * h1    = (TH1D*)hists[1]->Clone("h1");
  TH1D * h2    = (TH1D*)hists[2]->Clone("h2");
  //TH1D * h3    = (TH1D*)hists[4]->Clone("h3");

  // === calc rel frac ===
  ///////////////////////////////////////////////////////////
  //
  // === correction factor for limited range ===
  // Full range:
  // p0(x) = a*p1(x) + b*p2(x)
  // where p(x) are probabilities functions in the full range
  // and a,b are fractions in the full range
  //
  // Limited range:
  // p0'(x) = a'*p1'(x) + b'*p2'(x), p'(x) = 1/A p(x)
  // where p'(x) are probabilities functions in the limited range
  // and a',b' are fractions in the limited range
  // A is the area is the limited range
  //
  // Then:
  // a' = 1./A0 * a*A1
  // b' = 1./A0 * b*A2
  // where A1 = p1.Integra(x1,x2), A2 = p2.Integral(x1,x2)
  // and A0 = a*A1 + b*A2 is the overall normalization
  // obtained from (a*p1+ b*p2).Integral(x1,x2)
  //
  // In terms of a', b'
  // a = a'*A0/A1
  // b = b'*A0/A2
  // where A0 = 1/(a'/A1 + b'/A2) is the overall normalization
  // obtained from (a'*p1'+ b'*p2').Integral(x1,x2)
  //
  ///////////////////////////////////////////////////////////

  // Inputs to test:
  if (mode<2) {
    testWantedFrac2 = 1 - testWantedFrac1;
  }

  // - pdf areas -
  Double_t A1 = h1->Integral()/h1->GetEntries();
  Double_t A2 = h2->Integral()/h2->GetEntries();
  Double_t A0 = testWantedFrac1*A1 + testWantedFrac2*A2;

  // - correct test frac -
  Double_t testFrac1InRange = testWantedFrac1*A1/A0;
  Double_t testFrac2InRange = testWantedFrac2*A2/A0;

  // scale all pdf's in range to unity
  h1->Scale(1./h1->Integral()/h1->GetBinWidth(1));
  h2->Scale(1./h2->Integral()/h2->GetBinWidth(1));
  //hMC->Scale(1./hMC->Integral()/hMC->GetBinWidth(1));
  hData->Scale(1./hData->Integral()/hData->GetBinWidth(1));

  // combine
  TH1D * hFit = (TH1D*)h1->Clone("hFit");
  h1->Scale(testFrac1InRange);
  h2->Scale(testFrac2InRange);
  hFit->Add(h1,h2);
  //if (mode==2) hFit->Add(h3);

  // Result
  Double_t result = histChi2(hData,hFit);

  // if draw
  if (draw) {
    if (mode<2) {
      cout << "Draw: trial " << wanted0 << "frac: " << testWantedFrac1
	<< " In range: " << testFrac1InRange 
	<< " " << wanted1 << "frac: " << testWantedFrac2
	<< " In range: " << testFrac2InRange
	<< "  Raw hist chi2: " << result << endl;
      cout << "hData area: " << hData->Integral()*hData->GetBinWidth(1) << ", Entries: " << hData->GetEntries() << endl;
      //cout << "hMC area: " << hMC->Integral()*hMC->GetBinWidth(1) << ", Entries: " << hMC->GetEntries() << endl;
      cout << "h1 area: " << h1->Integral()*h1->GetBinWidth(1) << ", Entries: " << h1->GetEntries() << endl;
      cout << "h2 area: " << h2->Integral()*h2->GetBinWidth(1) << ", Entries: " << h2->GetEntries() << endl;
      cout << "hFit area: " << hFit->Integral()*hFit->GetBinWidth(1) << ", Entries: " << hFit->GetEntries() << endl;
    }
    else if (mode==2)
      cout << "Draw: trial " << wanted0 << ", " << wanted1 << "frac: " << testWantedFrac1 << ", " << testWantedFrac2<< "  Raw hist chi2: " << result << endl;
    //hMC->Draw("h");
    //hMC->SetMarkerStyle(0);
    //hMC->SetLineWidth(1);
    //hMC->SetLineStyle(7);
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
    /*
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
    */

    //  - add legend -
    TLegend *leg2 = new TLegend(0.589,0.776,0.891,0.928,NULL,"brNDC");
    leg2->SetFillColor(0);
    leg2->SetBorderSize(0);
    leg2->SetTextSize(0.03);
    TString drawMode="Best Fit";
    if (draw==2)
      drawMode="Default";
    leg2->AddEntry(hData,Form("%s",gDataSource.Data()),"p");
    leg2->AddEntry(h1,Form("%s - %s %s",gMCSource.Data(),drawMode.Data(),wanted0.Data()),"p");
    leg2->AddEntry(h2,Form("%s - %s %s",gMCSource.Data(),drawMode.Data(),wanted1.Data()),"p");
    //if (mode==2) leg2->AddEntry(h3,Form("%s - %s %s",gMCSource.Data(),drawMode.Data(),wanted2.Data()),"p");
    leg2->AddEntry(hFit,Form("%s - %s All",gMCSource.Data(),drawMode.Data()),"l");
    leg2->Draw();
  }
  else {
    //cout << "SDRelFrac: " << SDRelFrac << "  Raw hist chi2: " << result << endl;
    hData->Delete();
    //hMC->Delete();
    h1->Delete();
    h2->Delete();
    //h3->Delete();
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
