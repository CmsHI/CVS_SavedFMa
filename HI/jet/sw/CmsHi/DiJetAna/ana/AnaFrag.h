#include "TTree.h"
#include "TH1D.h"
#include "TString.h"

class AnaFrag
{
  public:
    AnaFrag(TString src, TString t,TTree *tree,TString djCut,TString djTrkCut, TString var, TString dRSig, TString dRBkg);

    TString tag;
    TTree * trDj;
    TH1D * hXiRaw;
    TH1D * hXiBkg;
    TH1D * hXiSig;

    int numJets;
    int nbin;
    double ximin;
    double ximax;
    double ymin;
    double ymax;

    TString xtitle;
    TString ytitle;
};

AnaFrag::AnaFrag(TString src, TString t,TTree *tree,TString djCut,TString djTrkCut, TString var, TString dRSig, TString dRBkg) :
  tag(t),
  nbin(20),
  ximin(0),
  ximax(7),
  ymin(0.001),
  ymax(200),
  xtitle("#xi=ln(E_{T}^{Jet}/p_{T}^{trk})"),
  ytitle("#frac{1}{N_{jet}} #frac{dN}{d#xi}")
{
  trDj = tree;
  tag=tag+"_"+src;
  hXiRaw = new TH1D(Form("hXiRaw_%s",tag.Data()),(";"+xtitle+";"+ytitle),nbin,ximin,ximax);
  hXiBkg = new TH1D(Form("hXiBkg_%s",tag.Data()),(";"+xtitle+";"+ytitle),nbin,ximin,ximax);
  hXiSig = new TH1D(Form("hXiSig_%s",tag.Data()),(";"+xtitle+";"+ytitle),nbin,ximin,ximax);

  hXiBkg->SetLineColor(kBlue);
  hXiBkg->SetMarkerColor(kBlue);
  // 0 for data, 1 for mc
  if (src.Contains("data")) {
    hXiBkg->SetMarkerStyle(kOpenCircle);
  }
  if (src.Contains("mc")) {
    hXiRaw->SetLineColor(kBlack);
    hXiRaw->SetMarkerColor(kBlack);
    hXiSig->SetLineColor(kRed);
    hXiSig->SetMarkerColor(kRed);
    hXiBkg->SetLineColor(kBlue);

    hXiRaw->SetMarkerStyle(kFullCircle);
    hXiBkg->SetMarkerStyle(0);
    hXiSig->SetMarkerStyle(kOpenCircle);

    hXiBkg->SetLineStyle(2);
  }

  TString SigTrkCut = djTrkCut + Form("&&%s",dRSig.Data());
  TString BkgTrkCut = djTrkCut + Form("&&%s",dRBkg.Data());

  numJets = trDj->Draw("nljet",djCut);
  cout << tag << ": number of jets for FF: " << numJets << endl;

  cout << "Sig Trk Cut: " << SigTrkCut << endl;
  cout << "Bkg Trk Cut: " << BkgTrkCut << endl;
  trDj->Draw(Form("%s>>hXiRaw_%s",var.Data(),tag.Data()),SigTrkCut);
  trDj->Draw(Form("%s>>hXiBkg_%s",var.Data(),tag.Data()),BkgTrkCut);
  hXiRaw->Scale(1./numJets,"width");
  hXiBkg->Scale(1./numJets,"width");
  hXiSig->Add(hXiRaw,hXiBkg,1,-1);

  hXiRaw->SetMinimum(ymin);
  hXiRaw->SetMaximum(ymax);
  hXiBkg->SetMinimum(ymin);
  hXiBkg->SetMinimum(ymin);
  hXiSig->SetMaximum(ymax);
  hXiSig->SetMaximum(ymax);
}
