#include "CutAndBinCollection2011.h"


void compareLastYearSingleVar(

			      TString varLable = "p_T{} (GeV)",
			      TString var = "photonEt",
			      TCut theCut = "cBin>=11",
			      int nBins = 15,
			      double lowBin = 60,
                     double highBin= 400
			      );

void compareLastYear3Panels(TCanvas* c1,
			    TString varLable ="p_{t}  (GeV)",
			    TString var =  "photonEt", 
			    TCut theCut = "",
			    int nBins = 15,
			    double lowBin =60,
			    double highBin=400);
void compareLastYear() 
{
   
   
   TCanvas* c1 = new TCanvas("c1","",1000,400);
   compareLastYear3Panels(c1, "p_{T} (GeV)", "photonEt", "photonEt>50 && evt.offlSel",15, 60, 400);
   //   drawText("p_{T} of isolated photon candidates",0.5,0.86);
   c1->cd(2);
   TLegend* leg1 = new TLegend(0.2998319,0.5004691,0.9992263,0.7001097,NULL,"brNDC");
   easyLeg(leg1,"");
   TH1D* htemp1 = new TH1D("htemp1","",1,0,1);  
   handsomeTH1(htemp1,1);
   TH1D* htemp2 = new TH1D("htemp2","",1,0,1); 
   handsomeTH1(htemp2,2);
   htemp1->SetMarkerStyle(kOpenCircle);
   leg1->AddEntry(htemp2,"2011 PbPb L=84 #mub^{-1}","pl");
   leg1->AddEntry(htemp1,"2010 PbPb L=6.8 #mub^{-1}","pl");
   leg1->Draw();
   c1->SaveAs("corrPt_MCvsDATA_allCent.eps");
   c1->SaveAs("corrPt_MCvsDATA_allCent.gif");
   
   
   TCanvas* c2 = new TCanvas("c2","",1000,400);
   compareLastYear3Panels(c2, "#eta", "photonEta", "photonEt>50 && evt.offlSel", 10, -1.5, 1.5);
   c2->cd(2);
   leg1->Draw();
   c2->SaveAs("eta_MCvsDATA_allCent.eps");
   c2->SaveAs("eta_MCvsDATA_allCent.gif");
   TCanvas* c3 = new TCanvas("c3","",1000,400);
   compareLastYear3Panels(c3, "#phi", "photonPhi", "photonEt>50 && evt.offlSel", 10, -3.141592, 3.141592);
   c3->cd(2);
   leg1->Draw();
   c3->SaveAs("phi_MCvsDATA_allCent.eps");
   c3->SaveAs("phi_MCvsDATA_allCent.gif");
}

void compareLastYear3Panels(TCanvas* c1,TString varLable, TString var, TCut theCut, int nBins, double lowBin, double highBin) 
{
   
   makeMultiPanelCanvas(c1,3,1,0.0,0.0,0.2,0.15,0.02);
   c1->cd(1);
   //   draw3Panel(c1, 
   compareLastYearSingleVar(varLable, var, theCut &&  "cBin>= 12 && cBin<=39", nBins, lowBin, highBin);
   drawText("30% - 100%",0.6,0.80);
   c1->cd(2);
   compareLastYearSingleVar(varLable, var, theCut &&  "cBin>= 4 && cBin<=11", nBins, lowBin, highBin);
   drawText("10% - 30%",0.5,0.80);
   c1->cd(3);
   compareLastYearSingleVar(varLable, var, theCut &&  "cBin>= 0 && cBin<=3", nBins, lowBin, highBin);
   drawText("0% - 10%",0.5,0.80);
   

}

void compareLastYearSingleVar(
			      TString varLable,
			      TString var,
			      TCut theCut,
			      int nBins,
			      double lowBin,
			      double highBin
		     )  
{
    TString fname1 = "franksTree/output-data-Photon-v3_v10.root";
   TFile *f1  =new TFile(fname1.Data());
   TTree *photon1 = (TTree*)f1->Get("tgj");
   
   TString fname2 = "franksTree/output-data-HI2010-Photon40_prod02_v10.root";
   TFile *f2  =new TFile(fname2.Data());
   TTree *photon2 = (TTree*)f2->Get("tgj");

   TH1D* h1 = new TH1D("h1",Form(";%s;Event Fraction",varLable.Data()),nBins,lowBin,highBin);
   TH1D* h2 = new TH1D("h2",Form(";%s;Event Fraction",varLable.Data()),nBins,lowBin,highBin);
   photon1->Draw(Form("%s>>h1",var.Data()),theCut);
   photon2->Draw(Form("%s>>h2",var.Data()),theCut);
   h1->Sumw2();
   h2->Sumw2();
   scaleInt(h1);
   scaleInt(h2);
   handsomeTH1(h1,2);
   handsomeTH1(h2,1);
   h2->SetMarkerStyle(kOpenCircle);
   h1->SetAxisRange(1.e-4,2,"Y");
   h1->DrawCopy();
   h2->DrawCopy("same");
   gPad->SetLogy();
   
   f1->Close();
   
}
