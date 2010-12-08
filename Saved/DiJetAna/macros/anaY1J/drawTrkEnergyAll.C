#include <iostream>
#include "TH1.h"
#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TPad.h"
#include "TMath.h"
#include "TLatex.h"
using namespace std;


//----------------------------------------------------
void makeMultiPanelCanvas(TCanvas*& canv, const Int_t columns, 
			  const Int_t rows, const Float_t leftOffset=0.,
                          const Float_t bottomOffset=0., 
			  const Float_t leftMargin=0.2, 
			  const Float_t bottomMargin=0.2,
                          const Float_t edge=0.05);


TH1D* combine(TH1D* near, TH1D* away);
void drawTrkEnergy(TString infile="drawn_jfh_HCPR_J50U_Cent0to10_Aj24to100_SubEtaRefl.root",
		   bool drawLeg=false, bool drawYLab=false);
void drawText(const char *text, float xp, float yp);
//------------------------------------------------------

void drawTrkEnergyAll()
{
  TCanvas *c1 = new TCanvas("c1","",1200,630);

  makeMultiPanelCanvas(c1,3,2,0.0,0.0,0.2,0.18,0.02);

  c1->cd(1);
  drawTrkEnergy("drawn_jfh_HCPR_J50U_Cent30to100_Aj0to100_SubEtaRefl.root",false,true);
  drawText("30~100%",0.22,0.86);

  c1->cd(2);
  drawTrkEnergy("drawn_jfh_HCPR_J50U_Cent10to30_Aj0to100_SubEtaRefl.root",false,false);
  drawText("10~30%",0.04,0.86);

  c1->cd(3);
  drawTrkEnergy("drawn_jfh_HCPR_J50U_Cent0to10_Aj0to100_SubEtaRefl.root",true,false);
  drawText("0~10%",0.03,0.86);

  c1->cd(4);
  drawTrkEnergy("drawn_jfh_HCPR_J50U_Cent0to30_Aj0to24_SubEtaRefl.root",false,true);
  drawText("0~10%",0.22,0.90);
  drawText("A_{j}<0.24",0.22,0.80);

  c1->cd(5);
  drawTrkEnergy("drawn_jfh_HCPR_J50U_Cent0to30_Aj24to100_SubEtaRefl.root",false,false);
  drawText("0~10%",0.04,0.90);
  drawText("A_{j}>0.24",0.04,0.80);

  c1->cd(6);
  drawTrkEnergy("drawn_jfh_PyquenUQ80_Cent0to100_Aj0to100_SubNone.root",false,false);
  drawText("PYTHIA",0.03,0.90);

  c1->Print("TrackEnergyPtRBkgSub.gif");
  c1->Print("TrackEnergyPtRBkgSub.eps");
  c1->Print("TrackEnergyPtRBkgSub.C");
}

void drawTrkEnergy(TString infile,
		   bool drawLeg, bool drawYLab){

  TFile *f = new TFile(infile);
  gStyle->SetMarkerStyle(0);

  TH1D *n0 = (TH1D*) f->Get("hPNDRSubBg_1_1");
  TH1D *n1 = (TH1D*) f->Get("hPNDRSubBg_2_2");
  TH1D *n2 = (TH1D*) f->Get("hPNDRSubBg_3_3");
  TH1D *n4 = (TH1D*) f->Get("hPNDRSubBg_4_4");
  TH1D *n8 = (TH1D*) f->Get("hPNDRSubBg_5_7");
  TH1D *nall = (TH1D*) f->Get("hPNDRSubBg_1_7");

  TH1D *a0 = (TH1D*) f->Get("hPADRSubBg_1_1");
  TH1D *a1 = (TH1D*) f->Get("hPADRSubBg_2_2");
  TH1D *a2 = (TH1D*) f->Get("hPADRSubBg_3_3");
  TH1D *a4 = (TH1D*) f->Get("hPADRSubBg_4_4");
  TH1D *a8 = (TH1D*) f->Get("hPADRSubBg_5_7");
  TH1D *aall = (TH1D*) f->Get("hPADRSubBg_1_7");

  TH1::SetDefaultSumw2();

  n0->SetFillColor(kGray);
  TH1D *n01 = (TH1D*) n0->Clone("n01");
  n01->Add(n1); n01->SetFillColor(kBlue-3);
  TH1D *n12 = (TH1D*) n01->Clone("n12");
  n12->Add(n2); n12->SetFillColor(38);
  TH1D *n124 = (TH1D*) n12->Clone("n124");
  n124->Add(n4); n124->SetFillColor(kOrange-8);
  TH1D *n1248 = (TH1D*) n124->Clone("n1248");
  n1248->Add(n8); n1248->SetFillColor(kRed-6);
  nall->SetFillColor(kBlack);

  a0->SetFillColor(kGray);
  TH1D *a01 = (TH1D*) a0->Clone("a01");
  a01->Add(a1); a01->SetFillColor(kBlue-3);
  TH1D *a12 = (TH1D*) a01->Clone("a12");
  a12->Add(a2); a12->SetFillColor(38);
  TH1D *a124 = (TH1D*) a12->Clone("a124");
  a124->Add(a4); a124->SetFillColor(kOrange-8);
  TH1D *a1248 = (TH1D*) a124->Clone("a1248");
  a1248->Add(a8); a1248->SetFillColor(kRed-6);
  aall->SetFillColor(kBlack);

  /*
  TCanvas *c = new TCanvas("c","c",800,500);
  c->Divide(2,1,-1,-1);
  c->cd(1);
  n1248->SetMinimum(1); n1248->SetMaximum(3000);
  n1248->Draw("chist");
  n124->Draw("chistsame");
  n12->Draw("chistsame");
  n1->Draw("chistsame");
  gPad->SetLogy();
  c->cd(2);
  a1248->SetMinimum(1); a1248->SetMaximum(3000);
  a1248->Draw("chist");
  a124->Draw("chistsame");
  a12->Draw("chistsame");
  a1->Draw("chistsame");
  gPad->SetLogy();
  */

  //TCanvas *c2 = new TCanvas("c2","c2",600,500);
  TH1D* hcall = combine(nall,aall);
  TH1D* hc1248 = combine(n1248,a1248);
  TH1D* hc124 = combine(n124,a124);
  TH1D* hc12 = combine(n12,a12);
  TH1D* hc01 = combine(n01,a01);
  TH1D* hc0 = combine(n0,a0);

  hcall->GetXaxis()->SetNdivisions(000,true);
  if(!drawYLab) hcall->GetYaxis()->SetTitle("");

  hcall->Draw("hist"); hcall->Draw("esame");
  hc1248->Draw("histsame"); hc1248->Draw("esame"); //chist
  hc124->Draw("histsame"); hc124->Draw("esame");
  hc12->Draw("histsame"); hc12->Draw("esame");
  hc01->Draw("histsame"); hc01->Draw("esame");
  hc0->Draw("histsame"); hc0->Draw("esame");

  hc1248->GetXaxis()->SetAxisColor(0);
  hc1248->GetXaxis()->SetLabelColor(0);

  TGaxis *naxis = new TGaxis(-1.4,0.08,1.4,0.08,-1.4,1.4,510,"-");
  TGaxis *aaxis = new TGaxis(TMath::Pi()-1.4,.08,TMath::Pi()+1.4,0.08,-1.4,1.4,510,"-");
  naxis->SetLabelOffset(-0.05); naxis->SetLabelSize(0.03);
  naxis->SetTitle("#DeltaR^{track}_{leading jet}"); naxis->CenterTitle(); naxis->SetTitleOffset(-1.5);
  aaxis->SetLabelOffset(-0.05); aaxis->SetLabelSize(0.03);
  aaxis->SetTitle("#DeltaR^{track}_{sub-leading jet}"); aaxis->CenterTitle(); aaxis->SetTitleOffset(-1.5);
  naxis->Draw();
  aaxis->Draw();
  TGaxis *nuaxis = new TGaxis(-1.0*TMath::PiOver2(),3000.0,1.4,3000.0,-1.0*TMath::PiOver2(),1.4,510,"U-");
  TGaxis *auaxis = new TGaxis(TMath::Pi()-1.4,3000.0,3.0*TMath::PiOver2(),3000.0,-1.4,TMath::PiOver2(),510,"U-");
  //nuaxis->Draw();
  //auaxis->Draw();

  gPad->SetLogy();
  //gPad->SetRightMargin(0.05);
  //gPad->SetLeftMargin(0.18);
  //gPad->SetBottomMargin(0.18);

  if (drawLeg) {
    TLegend *leg = new TLegend(0.35,0.67,0.75,0.95);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetNColumns(2);
    leg->AddEntry(hc0,"0.5-1 GeV/c","f");
    leg->AddEntry(hc01,"1-2 GeV/c","f");
    leg->AddEntry(hc12,"2-4 GeV/c","f");
    leg->AddEntry(hc124,"4-8 GeV/c","f");
    leg->AddEntry(hc1248,"16+ GeV/c","f");
    //leg->AddEntry(hcall,"16+ GeV/c","f");
    leg->Draw();
  }


  double nearsum=0.0, awaysum=0.0;
  for(Int_t ibin=1; ibin<=8; ibin++) {
    /*
    double bc = hcall->GetBinContent(ibin);
    cout << bc << endl;
    if(ibin>12 && ibin<=18) nearsum+=bc;
    if(ibin>32 && ibin<=38) awaysum+=bc;
    */
    double bcNr = nall->GetBinContent(ibin);
    double bcAw = aall->GetBinContent(ibin);
    nearsum+=bcNr;
    awaysum+=bcAw;
  }


  cout << "integral of dET/dR = " << nearsum << "(near-side) \t"
       << awaysum << "(away-side)" << endl;

}

TH1D* combine(TH1D* near, TH1D* away) {

  TH1D* hcombine = new TH1D(Form("hcombine_%s_%s",near->GetName(),away->GetName()),"",40,-1.0*TMath::PiOver2(),3.0*TMath::PiOver2());
  Int_t delta=4;
  for(int bin=1+delta; bin<=10; bin++) {
    hcombine->SetBinContent(bin,near->GetBinContent(11-bin));
    hcombine->SetBinError(bin,near->GetBinError(11-bin));
  }
  for(int bin=11; bin<=20-delta; bin++) {
    hcombine->SetBinContent(bin,near->GetBinContent(bin-10));
    hcombine->SetBinError(bin,near->GetBinError(bin-10));
  }
  for(int bin=21+delta; bin<=30; bin++) {
    hcombine->SetBinContent(bin,away->GetBinContent(31-bin));
    hcombine->SetBinError(bin,away->GetBinError(31-bin));
  }
  for(int bin=31; bin<=40-delta; bin++) {
    hcombine->SetBinContent(bin,away->GetBinContent(bin-30));
    hcombine->SetBinError(bin,away->GetBinError(bin-30));
  }

  hcombine->SetFillColor(near->GetFillColor());
  hcombine->SetStats(0);
  hcombine->SetMinimum(0.1);
  hcombine->SetTitle(";;#frac{1}{N_{dijet}}  #frac{d#sump_{T}^{track} }{ dR } [GeV]"); // no 2piR in denominator
  //hcombine->SetTitle(";;#frac{1}{N_{dijet}}  #frac{d#sump_{T}^{track} }{  2#pi R dR }");
  hcombine->GetYaxis()->CenterTitle();
  hcombine->GetYaxis()->SetTitleOffset(1.7);

  return hcombine;
}

void drawText(const char *text, float xp, float yp){
   TLatex *tex = new TLatex(xp,yp,text);
   tex->SetTextFont(63);
   tex->SetTextSize(16);
   //tex->SetTextSize(0.05);                                                                   
   tex->SetTextColor(kBlack);
   tex->SetLineWidth(1);
   tex->SetNDC();
   tex->Draw();
}


void makeMultiPanelCanvas(TCanvas*& canv,
                          const Int_t columns,
                          const Int_t rows,
                          const Float_t leftOffset,
                          const Float_t bottomOffset,
                          const Float_t leftMargin,
                          const Float_t bottomMargin,
                          const Float_t edge) {
   if (canv==0) {
      Error("makeMultiPanelCanvas","Got null canvas.");
      return;
   }
   canv->Clear();
   
   TPad* pad[columns][rows];

   Float_t Xlow[columns];
   Float_t Xup[columns];
   Float_t Ylow[rows];
   Float_t Yup[rows];
   Float_t PadWidth = 
   (1.0-leftOffset)/((1.0/(1.0-leftMargin)) +
   (1.0/(1.0-edge))+(Float_t)columns-2.0);
   Float_t PadHeight =
   (1.0-bottomOffset)/((1.0/(1.0-bottomMargin)) +
   (1.0/(1.0-edge))+(Float_t)rows-2.0);
   Xlow[0] = leftOffset;
   Xup[0] = leftOffset + PadWidth/(1.0-leftMargin);
   Xup[columns-1] = 1;
   Xlow[columns-1] = 1.0-PadWidth/(1.0-edge);

   Yup[0] = 1;
   Ylow[0] = 1.0-PadHeight/(1.0-edge);
   Ylow[rows-1] = bottomOffset;
   Yup[rows-1] = bottomOffset + PadHeight/(1.0-bottomMargin);

   for(Int_t i=1;i<columns-1;i++) {
      Xlow[i] = Xup[0] + (i-1)*PadWidth;
      Xup[i] = Xup[0] + (i)*PadWidth;
   }
   Int_t ct = 0;
   for(Int_t i=rows-2;i>0;i--) {
      Ylow[i] = Yup[rows-1] + ct*PadHeight;
      Yup[i] = Yup[rows-1] + (ct+1)*PadHeight;
      ct++;
   }

   TString padName;
   for(Int_t i=0;i<columns;i++) {
      for(Int_t j=0;j<rows;j++) {
         canv->cd();
         padName = Form("p_%d_%d",i,j);
         pad[i][j] = new TPad(padName.Data(),padName.Data(),
            Xlow[i],Ylow[j],Xup[i],Yup[j]);
         if(i==0) pad[i][j]->SetLeftMargin(leftMargin);
         else pad[i][j]->SetLeftMargin(0);

         if(i==(columns-1)) pad[i][j]->SetRightMargin(edge);
         else pad[i][j]->SetRightMargin(0);

         if(j==0) pad[i][j]->SetTopMargin(edge);
         else pad[i][j]->SetTopMargin(0);

         if(j==(rows-1)) pad[i][j]->SetBottomMargin(bottomMargin);
         else pad[i][j]->SetBottomMargin(0);

         pad[i][j]->Draw();
         pad[i][j]->cd();
         pad[i][j]->SetNumber(columns*j+i+1);
      }
   }
}
