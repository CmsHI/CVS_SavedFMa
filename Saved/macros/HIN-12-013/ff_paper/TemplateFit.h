#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>

float CalcChi2 (TH1D * h1, TH1D * h2, int begBin=1, int endBin=-1) {
  if (begBin>endBin) { begBin=1; endBin=h1->GetNbinsX(); }
  float total=0;
  for (int i=begBin; i<=endBin; ++i) {
    float diff2 = pow(h1->GetBinContent(i) - h2->GetBinContent(i), 2);
    float err2 = pow(h1->GetBinError(i),2) + pow(h2->GetBinError(i),2);
    total += diff2/err2;
  }
  total /= (endBin-begBin+1);
  return total;
}

class TemplateFit {
public:
  // setup
  float stepSize;
  int nSteps;

  // data members
  TH1D * hTemplate[3];
  TH1D * hData;
  TH1D * hFit[3];
  TH1D * hChi2;
  float minChi2, frac1;
  TF1 * fQuad;
  int begBin, endBin;

  // Constructor
  TemplateFit(float dx=0.1)
  : stepSize(dx)
  , minChi2(999999999) {
    nSteps = 1./stepSize;
    fQuad = new TF1("fQuad","[0]+[1]*(x-[2])^2");
  }

  // Member Methods
  void Init(TH1D * myData, TH1D * h1, TH1D * h2) {
    hData = myData;
    hTemplate[1] = h1;
    hTemplate[2] = h2;
    hFit[0] = (TH1D*)hData->Clone("hFit");
    hFit[0]->Reset();
    hChi2 = new TH1D("hChi2",";x;#chi2/dof;",nSteps,0,1);
  }

  void Fit(float xmin=0, float xmax=-1) {
    // Get xbin range to fit
    if (xmin > xmax) {
      begBin = 1;
      endBin = hData->GetNbinsX();
    } else {
      begBin = hData->FindBin(xmin);
      endBin = hData->FindBin(xmax);
    }
    cout << "begBin: " << begBin << " endBin: " << endBin << endl;

    // Now fill chi2
    for (int i=0; i<nSteps; ++i) {
      float a = i*stepSize;
      hFit[0]->Add(hTemplate[1],hTemplate[2],a,1-a);
      float ichi2 = CalcChi2(hData,hFit[0],begBin,endBin);
      hChi2->SetBinContent(i,ichi2);
      hChi2->SetBinError(i,0.1);
      if (ichi2<minChi2) {
        minChi2 = ichi2;
        frac1 = a;
      }
    }
  }

  void GetMinChi2(float forceFrac=-1) {
    cout << "hChi2 min bin: " << frac1 << endl;
    fQuad->SetParameters(0.5, 10, frac1);
    float xmin = frac1-0.1;
    float xmax = frac1+0.1;
    if (xmin<0) xmin=0;
    if (xmax>1) xmax=1;
    hChi2->Fit("fQuad","0","",xmin,xmax);

    float fitmin = fQuad->GetParameter(2);
    if (fabs(frac1-fitmin)<0.1) frac1=fitmin;
    if (forceFrac>=0) frac1 = forceFrac;
    hFit[1] = (TH1D*)hTemplate[1]->Clone(Form("%s_fit",hTemplate[1]->GetName()));
    hFit[2] = (TH1D*)hTemplate[2]->Clone(Form("%s_fit",hTemplate[2]->GetName()));
    hFit[1]->Scale(frac1);
    hFit[2]->Scale(1-frac1);
    hFit[0]->Add(hFit[1],hFit[2]);
  }

  void Inspect(TString title, float xmin, float xmax, TString ytitle="PbPb/pp", float ymin=0, float ymax=-1, TCanvas * cInspIn=0, TString out="") {
    if (ymin>ymax) { ymin=0; ymax=hData->GetMaximum()*1.5; }
    // Make Frame
    TH1D * hFrame = (TH1D*)hData->Clone("hFrame");
    hFrame->Reset();
    hFrame->SetYTitle(ytitle);
    hFrame->SetAxisRange(xmin,xmax,"X");
    hFrame->SetAxisRange(ymin,ymax,"Y");

    TH1D * hFrameRat = (TH1D*)hData->Clone("hFrameRat");
    hFrameRat->Reset();
    hFrameRat->SetYTitle("Data/Fit");

    handsomeTH1(hFrame,1,1);
    handsomeTH1(hFrameRat,1,1);
    hFrameRat->GetYaxis()->SetNdivisions(504);

    fixedFontHist(hFrame,1.,1.,25);
    fixedFontHist(hFrameRat,1.,0.5,25);
  
    TCanvas * cInsp0 = cInspIn;
    if (!cInsp0) cInsp0 = new TCanvas("cInsp0","cInsp0",0,0,500,500);
    handsomeTH1(hData,kBlack,0.9);
    handsomeTH1(hFit[0],kRed);
    hFit[0]->SetLineWidth(2);
    handsomeTH1(hFit[1],kGreen+2);
    handsomeTH1(hFit[2],kBlue);
    // hFit[0]->SetLineColor(kRed);
    // hFit[1]->SetLineColor(kGreen+2);
    // hFit[2]->SetLineColor(kBlue);
    if (!cInsp0) hFrame->Draw();
    hFit[1]->Draw("hist same");
    hFit[2]->Draw("hist same");
    hFit[0]->Draw("hist same");
    hData->Draw("E same");

    TLegend * leg0 = new TLegend(0.21,0.72,0.63,0.95,NULL,"brNDC");
    easyLeg(leg0,"",23);
    leg0->AddEntry(hData,title,"p");
    leg0->AddEntry(hFit[0],Form("Fitted Sum (%.1f < #xi < %.1f)",hData->GetBinLowEdge(begBin),hData->GetBinLowEdge(endBin+1)),"l");
    leg0->AddEntry(hFit[1],Form("Gluon Jet, %.0f %%", frac1*100),"l");
    leg0->AddEntry(hFit[2],Form("Quark Jet, %.0f %%", (1-frac1)*100),"l");
    leg0->Draw();

    if (out!="") {
      cInsp0->Print(out+"_ff.gif");
      cInsp0->Print(out+"_ff.pdf");
    }

    TCanvas * cInsp2 = new TCanvas("cInsp2","cInsp2",0,520,500,250);
    cInsp2->SetBottomMargin(0.35);
    TH1D * hRat = (TH1D*)hData->Clone(Form("%s_templfit_ratio",hData->GetName()));
    hRat->Divide(hFit[0]);
    hFrameRat->SetAxisRange(xmin,xmax,"X");
    hFrameRat->SetAxisRange(0.5,1.5,"Y");
    hFrameRat->Draw("E");
    hRat->Draw("E same");

    TLine * l1 = new TLine(xmin,1,xmax,1);
    l1->SetLineStyle(7);
    l1->Draw("same");

    if (out!="") {
      cInsp2->Print(out+"_ratio.gif");
      cInsp2->Print(out+"_ratio.pdf");
    }

    TCanvas * cInsp1 = new TCanvas("cInsp1","cInsp1",600,0,500,500);
    hChi2->Draw();
    fQuad->Draw("same");
    TLine * l0 = new TLine(frac1,0,frac1,hChi2->GetMaximum());
    l0->SetLineColor(kRed);
    l0->Draw();
  }
};
