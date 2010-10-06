#include <iostream>
#include "TTree.h"
#include "TH1D.h"
#include "TString.h"
#include <cassert>
#include "TCut.h"
#include "selectionCut.h"

class AnaFrag
{
  public:
    AnaFrag(TString src, TString t,TTree *tree,TCut djCut,TCut djTrkCut, TString var, TCut dRSig, TCut dRBkg,int nx=10,double min=0,double max=6, selectionCut * anaCut=0);

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

AnaFrag::AnaFrag(TString src, TString t,TTree *tree,TCut djCut,TCut trkCut, TString var, TCut dRSig, TCut dRBkg, int nx,double min,double max,selectionCut * anaSel) :
  tag(src),
  xtitle(t),
  numDJ(-1),
  nbin(nx),
  xmin(min),
  xmax(max)
{
  trDj = tree;
  tag+=t;
  if (!trDj) cout << tag << ": tree not found" << endl;
  assert(trDj);

  // Normalization
  numDJ = trDj->Draw("jdphi>>hDJDPhi",djCut,"goff");
  std::cout << "--- " << tag << ": ---" << std::endl;
  std::cout << " # of sel dijets: " << numDJ << std::endl;

  // DJ selection control plots
  if (tag=="XiE1Aw") {
    if (anaSel) {
      // jet
      TH1D * hJEtNr = new TH1D("hJEtNr",djCut,anaSel->numJEtBins,anaSel->hisJEtMin,anaSel->hisJEtMax);
      TH1D * hJEtAw = new TH1D("hJEtAw",djCut,anaSel->numJEtBins,anaSel->hisJEtMin,anaSel->hisJEtMax);
      TH1D * hRefJEtNr = new TH1D("hRefJEtNr",djCut,anaSel->numJEtBins,anaSel->hisJEtMin,anaSel->hisJEtMax);
      TH1D * hRefJEtAw = new TH1D("hRefJEtAw",djCut,anaSel->numJEtBins,anaSel->hisJEtMin,anaSel->hisJEtMax);
      // cone
      TH1D * hC5NPNr = new TH1D("hC5NPNr",djCut,anaSel->numC5NPBins,anaSel->hisC5NPMin,anaSel->hisC5NPMax);
      TH1D * hC5NPBgNr = new TH1D("hC5NPBgNr",djCut,anaSel->numC5NPBins,anaSel->hisC5NPMin,anaSel->hisC5NPMax);
      TH1D * hC5NPSubNr = new TH1D("hC5NPSubNr",djCut,anaSel->numC5NPSubBins,anaSel->hisC5NPSubMin,anaSel->hisC5NPSubMax);
      TH1D * hC5NPAw = new TH1D("hC5NPAw",djCut,anaSel->numC5NPBins,anaSel->hisC5NPMin,anaSel->hisC5NPMax);
      TH1D * hC5NPBgAw = new TH1D("hC5NPBgAw",djCut,anaSel->numC5NPBins,anaSel->hisC5NPMin,anaSel->hisC5NPMax);
      TH1D * hC5NPSubAw = new TH1D("hC5NPSubAw",djCut,anaSel->numC5NPSubBins,anaSel->hisC5NPSubMin,anaSel->hisC5NPSubMax);

      trDj->Draw("nljet>>hJEtNr",djCut,"goff");
      trDj->Draw("aljet>>hJEtAw",djCut,"goff");
      trDj->Draw("nlrjet>>hRefJEtNr",djCut,"goff");
      trDj->Draw("alrjet>>hRefJEtAw",djCut,"goff");

      trDj->Draw("nljC5NP>>hC5NPNr",djCut,"goff");
      trDj->Draw("nljC5NPBg>>hC5NPBgNr",djCut,"goff");
      trDj->Draw("nljC5NP-nljC5NPBg>>hC5NPSubNr",djCut,"goff");

      trDj->Draw("aljC5NP>>hC5NPAw",djCut,"goff");
      trDj->Draw("aljC5NPBg>>hC5NPBgAw",djCut,"goff");
      trDj->Draw("aljC5NP-aljC5NPBg>>hC5NPSubAw",djCut,"goff");
    }
    else cout << "no anaSel defined" << endl;
  }

  // Plot Histograms
  if (TString(dRBkg).Length()) {
    hRaw = new TH1D(Form("hRaw_%s",tag.Data()),(";"+xtitle+";"),nbin,xmin,xmax);
    hBkg = new TH1D(Form("hBkg_%s",tag.Data()),(";"+xtitle+";"),nbin,xmin,xmax);
    hSig = new TH1D(Form("hSig_%s",tag.Data()),(";"+xtitle+";"),nbin,xmin,xmax);

    TCut SigTrkCut = djCut && trkCut && dRSig;
    TCut BkgTrkCut = djCut && trkCut && dRBkg;

    std::cout << "Sig Trk Cut: " << TString(SigTrkCut) << std::endl;
    std::cout << "Bkg Trk Cut: " << TString(BkgTrkCut) << std::endl;
    trDj->Draw(Form("%s>>hRaw_%s",var.Data(),tag.Data()),SigTrkCut,"goff");
    trDj->Draw(Form("%s>>hBkg_%s",var.Data(),tag.Data()),BkgTrkCut,"goff");
  } else {
    hRaw = new TH1D(Form("h%s",tag.Data()),(";"+xtitle+";"),nbin,xmin,xmax);
    TCut Cut = djCut;
    if (TString(trkCut).Length())
      Cut = djCut && trkCut;
    std::cout << "Cut: " << TString(Cut) << std::endl;
    trDj->Draw(Form("%s>>h%s",var.Data(),tag.Data()),Cut,"goff");
  }

  // Calc
  hRaw->Scale(1./numDJ,"width");
  if (TString(dRBkg).Length()) {
    hBkg->Scale(1./numDJ,"width");
    hSig->Add(hRaw,hBkg,1,-1);
  }
}
