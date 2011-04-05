#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "Corrector3D.h"

void drawCorr3D(Int_t corrLevel=0,
    Float_t ptHatMin=110,
    Int_t etaPM=2 // +/- 2 for |eta|<1
    )
{
  TString mod="B2InConeJ2";
  TString modref="B2InConeJ1";
  //TString modref="B2",
  TString app="_jtv2_2_cv3";
  TString appref="_jtv2_2_cv3";
  Int_t isample=0;
  Int_t sampleMode = 0; //0 for choosing individual sample, 1 for merge samples
  TString tag(Form("TrkCorr%d_%s_%s_%.0f",corrLevel,mod.Data(),modref.Data(),ptHatMin));

  Corrector3D trkCorr("trkCorrHisAna_djuq",app,mod);
  //Corrector3D trkCorr("trkhist_mar292011_jet_v1_hydjetBass_dijet","_nq_jettrkonly_lead","hitrkEffAnalyzer");
  if (ptHatMin>0) {
    trkCorr.ptHatMin_.clear();
    trkCorr.ptHatMin_.push_back(ptHatMin);
  }
  trkCorr.sampleMode_ = sampleMode;
  trkCorr.Init();

  Corrector3D trkCorrRef("trkCorrHisAna_djuq",appref,modref);
  if (ptHatMin>0) {
    trkCorrRef.ptHatMin_.clear();
    trkCorrRef.ptHatMin_.push_back(ptHatMin);
  }
  trkCorrRef.sampleMode_ = sampleMode;
  trkCorrRef.Init();

  TCanvas * cEff = new TCanvas("cEff","cEff",1200,1000);
  cEff->Divide(5,5);
  Int_t begJetBin=3,dJetBin=2;
  for (Int_t ibin=0; ibin<5; ++ibin) {
    for (Int_t j=0; j<5; ++j) {
      Int_t ipad = 1+ibin*5+j;
      cout << "cbin: " << ibin << " jet: " << j << " ipad: " << ipad << endl;
      cEff->cd(ipad);
      Int_t jetBin1=begJetBin+j*dJetBin;
      Int_t jetBin2=begJetBin-1+(j+1)*dJetBin;
      TH1D * hCorrPtRef = (TH1D*) trkCorrRef.InspectCorr(corrLevel,isample,ibin,jetBin1,jetBin2,2,7-etaPM,7+etaPM);
      hCorrPtRef->SetAxisRange(0,120,"X");
      hCorrPtRef->SetMarkerStyle(0);
      hCorrPtRef->SetMarkerColor(kBlack);
      hCorrPtRef->SetLineColor(kBlack);
      hCorrPtRef->Draw("histE");
      TH1D * hCorrPt = (TH1D*) trkCorr.InspectCorr(corrLevel,isample,ibin,jetBin1,jetBin2,2,7-etaPM,7+etaPM);
      hCorrPt->SetMarkerStyle(0);
      hCorrPt->SetMarkerColor(kRed);
      hCorrPt->SetLineColor(kRed);
      hCorrPt->Draw("histE same ");
      Float_t JEt0=trkCorr.jetBin_->GetBinLowEdge(jetBin1),JEt1=trkCorr.jetBin_->GetBinLowEdge(jetBin2+1);
      if (j==0) {
	TLegend *leg = new TLegend(0.2,0.78,0.9,0.98);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->SetTextSize(0.1);
	leg->AddEntry(hCorrPt,Form("cbin %s",trkCorr.centBin_[ibin].Data()),"");
	leg->Draw();
      }
      if (ibin==0) {
	TLegend *leg2 = new TLegend(0.02,0.68,0.9,0.88);
	leg2->SetBorderSize(0);
	leg2->SetFillStyle(0);
	leg2->SetTextSize(0.1);
	leg2->AddEntry(hCorrPt,Form("%.0f<Jet p_{T}<%.0fGeV",JEt0,JEt1),"");
	leg2->Draw();
      }
    }
  }
  cEff->Print(Form("%s.gif",tag.Data()));
}
