#include <iostream>
#include "TTree.h"
#include "TH1D.h"
#include "TString.h"
#include "TCut.h"

class AnaFrag
{
  public:
    AnaFrag(TString src, TString t,TTree *tree,TCut djCut,TCut djTrkCut, TString var, TCut dRSig, TCut dRBkg,int nx=20,double min=0,double max=7);

    TString tag;
    TTree * trDj;
    TH1D * hRaw;
    TH1D * hBkg;
    TH1D * hSig;

    TString xtitle;

    int numDJ;
    int nbin;
    double xmin;
    double xmax;
};

AnaFrag::AnaFrag(TString src, TString t,TTree *tree,TCut djCut,TCut trkCut, TString var, TCut dRSig, TCut dRBkg, int nx,double min,double max) :
  tag(src),
  xtitle(t),
  numDJ(-1),
  nbin(nx),
  xmin(min),
  xmax(max)
{
  trDj = tree;
  tag+=t;

  // Normalization
  numDJ = trDj->Draw("nljet",djCut,"goff");
  std::cout << tag << ": number of selected dijets: " << numDJ << std::endl;

  // Plot Histograms
  if (TString(dRBkg).Length()) {
    hRaw = new TH1D(Form("hRaw_%s",tag.Data()),(";"+xtitle+";"),nbin,xmin,xmax);
    hBkg = new TH1D(Form("hBkg_%s",tag.Data()),(";"+xtitle+";"),nbin,xmin,xmax);
    hSig = new TH1D(Form("hSig_%s",tag.Data()),(";"+xtitle+";"),nbin,xmin,xmax);

    TCut SigTrkCut = djCut && trkCut && dRSig;
    TCut BkgTrkCut = djCut && trkCut && dRBkg;

    std::cout << "Sig Trk Cut: " << TString(SigTrkCut) << std::endl;
    std::cout << "Bkg Trk Cut: " << TString(BkgTrkCut) << std::endl;
    trDj->Draw(Form("%s>>hRaw_%s",var.Data(),tag.Data()),SigTrkCut);
    trDj->Draw(Form("%s>>hBkg_%s",var.Data(),tag.Data()),BkgTrkCut);
  } else {
    hRaw = new TH1D(Form("h%s",tag.Data()),(";"+xtitle+";"),nbin,xmin,xmax);
    if (TString(trkCut).Length()) {
      trDj->Draw(Form("%s>>h%s",var.Data(),tag.Data()),djCut&&trkCut);
    } else {
      trDj->Draw(Form("%s>>h%s",var.Data(),tag.Data()),djCut);
    }
  }

  // Calc
  hRaw->Scale(1./numDJ,"width");
  if (TString(dRBkg).Length()) {
    hBkg->Scale(1./numDJ,"width");
    hSig->Add(hRaw,hBkg,1,-1);
  }
}
