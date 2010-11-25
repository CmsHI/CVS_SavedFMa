#include <iostream>
#include "TTree.h"
#include "TH1D.h"
#include "TH2.h"
#include "TProfile.h"
#include "TString.h"
#include <cassert>
#include "TCut.h"
#include "selectionCut.h"
#include "TCanvas.h"
#include "Saved/Utilities/macros/cplot/CPlot.h"           // helper class for plots

class AnaFragBase
{
  public:
    TString xvar;
    TString yvar;
    TString xtag;
    TString ytag;
    TString xtitle;
    TString ytitle;

    int numDJ;
    int nbin;
    double xmin;
    double xmax;

    AnaFragBase(TString xtg, TString ytg,TTree *tree, int nx=10, double min=0, double max=6);
    void PlotCorrelations(selectionCut & anaSel,TString var1, TString var2, int ny, double ymin, double ymax);

    TString tag;
    TTree * trDj;
    TH1D * hRaw;
    TH1D * hBkg;
    TH1D * hSig;
    TH2F * hCorrelNr;
    TH2F * hCorrelAw;
    TProfile * hCorrelProfNr;
    TProfile * hCorrelProfAw;
};

AnaFragBase::AnaFragBase(TString xtg, TString ytg,TTree *tree, int nx, double min, double max) :
  xtag(xtg),
  ytag(ytg),
  numDJ(-1),
  nbin(nx),
  xmin(min),
  xmax(max)
{
  tag = xtg+ytg;
  trDj = tree;
  if (!trDj) cout << tag << ": tree not found" << endl;
  assert(trDj);
}

void AnaFragBase::PlotCorrelations(selectionCut & anaSel,TString var1, TString var2, int nc, double cmin, double cmax)
{
  TString hisNameNr("h"+tag+"Nr");
  TString hisNameAw(anaSel.Nr2Aw(hisNameNr));
  hCorrelNr = new TH2F(hisNameNr,";"+xtitle+";",nc,cmin,cmax,nbin,xmin,xmax);
  hCorrelAw = new TH2F(hisNameAw,";"+xtitle+";",nc,cmin,cmax,nbin,xmin,xmax);
  TString drawStrNr(var1+":"+var2);
  TString drawStrAw(anaSel.Nr2Aw(drawStrNr));
  trDj->Draw(drawStrNr+">>"+hisNameNr,anaSel.FinLJCut(),"goff");
  trDj->Draw(drawStrAw+">>"+hisNameAw,anaSel.FinAJCut(),"goff");
  cout << tag << endl;
  cout << "  Nr draw: " << drawStrNr << " to " << hisNameNr << endl;
  cout << "      Sel: " << anaSel.FinLJCut() << endl;
  cout << "  Aw draw: " << drawStrAw << " to " << hisNameAw << endl;
  cout << "      Sel: " << anaSel.FinAJCut() << endl;

  TCanvas * cCorrel2D = new TCanvas("c"+tag,"c"+tag,800,800);
  cCorrel2D->Divide(2,2);

  CPlot cpCorrel2DNr(tag+"Nr"+anaSel.Tag2,tag,"(Near) "+xtitle,ytitle);
  cpCorrel2DNr.AddHist2D(hCorrelNr,"colz");
  cCorrel2D->cd(1); cpCorrel2DNr.Draw((TPad*)cCorrel2D->GetPad(1),false);

  CPlot cpCorrel2DAw(tag+"Aw"+anaSel.Tag2,tag,"(Away) "+xtitle,ytitle);
  cpCorrel2DAw.AddHist2D(hCorrelAw,"colz");
  cCorrel2D->cd(3); cpCorrel2DAw.Draw((TPad*)cCorrel2D->GetPad(3),false);

  hCorrelProfNr = hCorrelNr->ProfileX(tag+"NrProfX");
  hCorrelProfAw = hCorrelAw->ProfileX(tag+"AwProfX");
  CPlot cpCorrel2DProf(tag+"Prof"+anaSel.Tag2,tag,xtitle,ytitle);
  cpCorrel2DProf.SetYRange(xmin,xmax);
  cpCorrel2DProf.AddProfile(hCorrelProfNr,"Lead Jet","E",kRed,kOpenCircle);
  cpCorrel2DProf.AddProfile(hCorrelProfAw,"Away Jet","E",kBlue,kOpenSquare);
  cCorrel2D->cd(2); cpCorrel2DProf.Draw((TPad*)cCorrel2D->GetPad(2),false);
  cCorrel2D->Print(CPlot::sOutDir+"/"+tag+anaSel.Tag2+".gif");
}

class AnaFrag : public AnaFragBase
{
  public:
    AnaFrag(TString xtg, TString ytg,TTree *tree,int nx=10,double min=0,double max=6, selectionCut * anaCut=0)
      : AnaFragBase(xtg,ytg,tree,nx,min,max) {}
    AnaFrag(TString xtg, TString ytg,TTree *tree,TCut djCut,TCut djTrkCut, TString var, TCut dRSig, TCut dRBkg,int nx=10,double min=0,double max=6, selectionCut * anaCut=0);
};

AnaFrag::AnaFrag(TString xtg, TString ytg,TTree *tree,TCut djCut,TCut trkCut, TString var, TCut dRSig, TCut dRBkg, int nx,double min,double max,selectionCut * anaSel) :
  AnaFragBase(xtg,ytg,tree,nx,min,max)
{
  // Normalization
  numDJ = trDj->Draw("jdphi>>hDJDPhi",djCut,"goff");
  std::cout << "--- " << tag << ", Draw: " << var << " --- " << std::endl;
  std::cout << " # of sel jet events: " << numDJ << std::endl;

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

      trDj->Draw("ljcnp[0]>>hC5NPNr",djCut,"goff");
      trDj->Draw("ljcnpbg[0]>>hC5NPBgNr",djCut,"goff");
      trDj->Draw("ljcnp[0]-ljcnpbg[0]>>hC5NPSubNr",djCut,"goff");

      trDj->Draw("ljcnp[1]>>hC5NPAw",djCut,"goff");
      trDj->Draw("ljcnpbg[1]>>hC5NPBgAw",djCut,"goff");
      trDj->Draw("ljcnp[1]-ljcnpbg[1]>>hC5NPSubAw",djCut,"goff");
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
