#include <iostream>
#include "TTree.h"
#include "TH1D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TCut.h"
#include "TLegend.h"
#include "TSystem.h"
#include "CmsHi/DiJetAna/ana/aliases_dijet.C"
#include "CmsHi/DiJetAna/ana/selectionCut.h"
#include "CmsHi/DiJetAna/ana/AnaFrag.h"
#include "CmsHi/DiJetAna/ana/compareHist.h"
using namespace std;

void anaDiJetFF(int doMC=1,
    const char * inFile0Name="../process_aod/outputs/dijetAna_anaJet_Mc1_2k.root",
    TString title1="MC Reco (Hyd2.76TeV+dijet)",
    TString title2="MC Input (Hyd2.76TeV+dijet)")
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;

  TFile * inFile0 = new TFile(inFile0Name);
  inFile0->ls();
  TTree *mcj2t3, *mcj2t3peri, *mcj2t0, *mcj1t0;
  inFile0->GetObject("dijetAna_mc/djTree",mcj2t3);
  inFile0->GetObject("dijetAna_mc_periph/djTree",mcj2t3peri);
  inFile0->GetObject("dijetAna_mc_calojet_genp/djTree",mcj2t0);
  inFile0->GetObject("dijetAna_mc_genjet_genp/djTree",mcj1t0);

  // Define dijet selection
  selectionCut mcAna(doMC,1);
  gSystem->mkdir(Form("plots/%s",mcAna.AnaTag.Data()),kTRUE);
  cout << endl << "====== Ana: " << mcAna.AnaTag << " ======" << endl;
  cout << "DJ selection: " << TString(mcAna.DJ) << endl;
  cout << "dijetAna_mc/mcj2t3 # entries: " << mcj2t3->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj2t3->GetEntries(mcAna.DJ) << endl;
  cout << "dijetAna_mc/mcj2t0 # entries: " << mcj2t0->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj2t0->GetEntries(mcAna.DJ) << endl;
  cout << "dijetAna_mc/mcj1t0 # entries: " << mcj1t0->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj1t0->GetEntries(mcAna.DJ) << endl;

  // Setup root
  TH1::SetDefaultSumw2();

  // ============== pdf comparisons ===============
  // check dijet
  cout << " --- Check Jet Distributions --- " << endl;
  cout << " Evt Sel: " << mcAna.Evt << endl;
  TCanvas *ccomp2 = new TCanvas("ccomp2","",500,500);
  compareHist comp2(mcj2t3,mcj1t0,"jdphi","dPhi",mcAna.Evt.Data(),mcAna.Evt.Data(),0,3.14,30);
  comp2.Normalize(1);
  comp2.SetHistName1(title1);
  comp2.SetHistName2(title2);
  comp2.SetXTitle("leading dijet d #phi");
  comp2.SetYTitle("Arbitrary normalization");
  comp2.SetLegend(0.222,0.830,0.516,0.930);
  comp2.SetMaximum(6);
  comp2.Draw("E");
  ccomp2->Print(Form("plots/%s/%s_dPhi.gif",mcAna.AnaTag.Data(),mcAna.Tag.Data()) );

  TCanvas *ccomp3 = new TCanvas("ccomp3","",500,500);
  compareHist comp3(mcj2t3,mcj1t0,"2*(nljet-aljet)/(nljet+aljet)","Balance",mcAna.Evt.Data(),mcAna.Evt.Data(),0,1.2,30);
  comp3.Normalize(1);
  comp3.SetHistName1(title1);
  comp3.SetHistName2(title2);
  comp3.SetXTitle("(p_{T}^{j1}-p_{T}^{j2})/((p_{T}^{j1}+p_{T}^{j2})/2)");
  comp3.SetYTitle("Arbitrary normalization");
  comp3.SetLegend(0.45,0.80,0.75,0.9);
  comp3.SetMaximum(7);
  comp3.Draw("E");
  ccomp3->Print(Form("plots/%s/%s_Balance.gif",mcAna.AnaTag.Data(),mcAna.Tag.Data()));

  cout << " --- Check Trk-Jet Distributions --- " << endl;
  TString djTrkCut = TString(mcAna.DJ && mcAna.Trk);
  cout << "djTrkCut: " << djTrkCut << endl;

  // check inclusive tracks
  TCanvas *ccomp4 = new TCanvas("ccomp4","",500,500);
  ccomp4->SetLogy(1);
  compareHist comp4(mcj2t3,mcj1t0,"ppt","trkPt",djTrkCut.Data(),djTrkCut.Data(),0,25,80);
  comp4.Normalize(1);
  comp4.SetHistName1(title1);
  comp4.SetHistName2(title2);
  comp4.SetXTitle("track p_{T}");
  comp4.SetYTitle("Arbitrary normalization");
  comp4.SetLegend(0.45,0.80,0.75,0.9);
  comp4.Draw("E");
  ccomp4->Print(Form("plots/%s/%s_trkPt.gif",mcAna.AnaTag.Data(),mcAna.Tag.Data()));


  // check track jet correlations
  TString djTrkCut1 = TString(mcAna.DJ && "ppt>1 && ppt<nljet");
  TCanvas *ccomp7 = new TCanvas("ccomp7","",500,500);
  compareHist comp7(mcj2t3,mcj1t0,"pndphi","TrkDPhi",djTrkCut1.Data(),djTrkCut1.Data(),0,3.14,30);
  comp7.Normalize(1);
  comp7.SetHistName1(title1);
  comp7.SetHistName2(title2);
  comp7.SetXTitle("d #phi (j1,trk) (p_{T}^{trk}>1.GeV)");
  comp7.SetYTitle("Arbitrary normalization");
  comp7.SetLegend(0.41,0.27,0.71,0.37);
  comp7.SetMinimum(0);
  comp7.SetMaximum(0.4);
  comp7.Draw("E");
  ccomp7->Print(Form("plots/%s/%s_TrkDPhi.gif",mcAna.AnaTag.Data(),mcAna.Tag.Data()));

  // === dijet ana ===
  cout << endl << "==================== DiJet Ana ===================" << endl;
  cout << "Evt Cut: " << mcAna.Evt << endl;
  cout << "djTrkCut: " << djTrkCut << endl;
  TCanvas *ctemp = new TCanvas("ctemp","ctemp",500,500);
  // Reco
  AnaFrag mcRecoNr("mcReco","Near",mcj2t3,mcAna.Evt,djTrkCut,"log(1./zn)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcRecoAw("mcReco","Away",mcj2t3,mcAna.Evt,djTrkCut,"log(1./za)","padr<0.5","padrbg<0.5");

  // Reco jet + genp
  AnaFrag mcj2t0Nr("mcj2t0","Near",mcj2t0,mcAna.Evt,djTrkCut,"log(1./zn)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcj2t0Aw("mcj2t0","Away",mcj2t0,mcAna.Evt,djTrkCut,"log(1./za)","padr<0.5","padrbg<0.5");

  // Gen
  AnaFrag mcGenNr("mcGen","Near",mcj1t0,mcAna.Evt,djTrkCut,"log(1/zn)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcGenAw("mcGen","Away",mcj1t0,mcAna.Evt,djTrkCut,"log(1/za)","padr<0.5","padrbg<0.5");

  // Gen-Truth
  TString djTrkTruthCut = TString(mcAna.DJ && mcAna.Trk && "psube==0");
  AnaFrag mcGenTruthNr("mcGenTruth","Near",mcj1t0,mcAna.Evt,djTrkTruthCut,"log(1/zn)","pndr<0.5","pndrbg<0.5");
  mcGenTruthNr.hXiSig->SetLineColor(2);
  mcGenTruthNr.hXiSig->SetMarkerStyle(0);
  AnaFrag mcGenTruthAw("mcGenTruth","Away",mcj1t0,mcAna.Evt,djTrkTruthCut,"log(1/za)","padr<0.5","padrbg<0.5");

  // Final Plots
  // gen truth
  TCanvas *cFFGenTruth = new TCanvas("cFFGenTruth","cFFGenTruth",500,500);
  cFFGenTruth->SetLogy();
  mcGenTruthNr.hXiRaw->Draw("E");
  mcGenTruthNr.hXiBkg->Draw("hist Esame");
  mcGenTruthNr.hXiSig->Draw("E hist same");
  TLegend * leg1 = new TLegend(0.18,0.756,0.58,0.945,NULL,"brNDC");
  leg1->SetFillStyle(0);
  leg1->SetFillColor(0);
  leg1->SetTextSize(0.025);
  leg1->AddEntry("","Leading Jet","");
  leg1->AddEntry(mcGenTruthNr.hXiRaw,"Signal genjet+ptl Raw","p");
  leg1->AddEntry(mcGenTruthNr.hXiBkg,"Signal genjet+ptl Bkg","l");
  leg1->AddEntry(mcGenTruthNr.hXiSig,"Signal genjet+ptl Raw-Bkg","p");
  leg1->Draw();
  cFFGenTruth->Print(Form("plots/%s/%s_McGenTruthXi.gif",mcAna.AnaTag.Data(),mcAna.Tag.Data()));

  // gen
  TCanvas *cFFGen = new TCanvas("cFFGen","cFFGen",500,500);
  cFFGen->SetLogy();
  mcGenNr.hXiRaw->Draw("E");
  mcGenNr.hXiBkg->Draw("hist Esame");
  mcGenTruthNr.hXiSig->Draw("E hist same");
  mcGenNr.hXiSig->Draw("Esame");
  TLegend * leg2 = new TLegend(0.18,0.756,0.58,0.945,NULL,"brNDC");
  leg2->SetFillStyle(0);
  leg2->SetFillColor(0);
  leg2->SetTextSize(0.025);
  leg2->AddEntry("","Leading Jet","");
  leg2->AddEntry(mcGenNr.hXiRaw,"genjet+ptl Raw","p");
  leg2->AddEntry(mcGenNr.hXiBkg,"genjet+ptl Bkg","l");
  leg2->AddEntry(mcGenNr.hXiSig,"genjet+ptl Raw-Bkg","p");
  leg2->AddEntry(mcGenTruthNr.hXiSig,"Gen signal jet,ptl","l");
  leg2->Draw();
  cFFGen->Print(Form("plots/%s/%s_McGenXi.gif",mcAna.AnaTag.Data(),mcAna.Tag.Data()));

  // reco full
  TCanvas *cFFMcReco = new TCanvas("cFFMcReco","cFFMcReco",500,500);
  cFFMcReco->SetLogy();
  mcRecoNr.hXiRaw->Draw("E");
  mcRecoNr.hXiBkg->Draw("hist Esame");
  mcGenTruthNr.hXiSig->Draw("E hist same");
  mcRecoNr.hXiSig->Draw("Esame");
  TLegend * leg3 = new TLegend(0.18,0.756,0.58,0.945,NULL,"brNDC");
  leg3->SetFillStyle(0);
  leg3->SetFillColor(0);
  leg3->SetTextSize(0.025);
  leg3->AddEntry("","Leading Jet","");
  leg3->AddEntry(mcRecoNr.hXiRaw,"calojet+trk Raw","p");
  leg3->AddEntry(mcRecoNr.hXiBkg,"calojet+trk Bkg","l");
  leg3->AddEntry(mcRecoNr.hXiSig,"calojet+trk Raw-Bkg","p");
  leg3->AddEntry(mcGenTruthNr.hXiSig,"Signal genjet+ptl","l");
  leg3->Draw();
  cFFMcReco->Print(Form("plots/%s/%s_McRecoXi.gif",mcAna.AnaTag.Data(),mcAna.Tag.Data()));

  // reco jet + genp
  TCanvas *cFFMcj2t0 = new TCanvas("cFFMcj2t0","cFFMcj2t0",500,500);
  cFFMcj2t0->SetLogy();
  mcj2t0Nr.hXiRaw->Draw("E");
  mcj2t0Nr.hXiBkg->Draw("hist Esame");
  mcGenTruthNr.hXiSig->Draw("E hist same");
  mcj2t0Nr.hXiSig->Draw("Esame");
  TLegend * leg4 = new TLegend(0.18,0.756,0.58,0.945,NULL,"brNDC");
  leg4->SetFillStyle(0);
  leg4->SetFillColor(0);
  leg4->SetTextSize(0.025);
  leg4->AddEntry("","Leading Jet","");
  leg4->AddEntry(mcj2t0Nr.hXiRaw,"calojet+ptl Raw","p");
  leg4->AddEntry(mcj2t0Nr.hXiBkg,"calojet+ptl Bkg","l");
  leg4->AddEntry(mcj2t0Nr.hXiSig,"calojet+ptl Raw-Bkg","p");
  leg4->AddEntry(mcGenTruthNr.hXiSig,"Signal genjet+ptl","l");
  leg4->Draw();
  cFFMcj2t0->Print(Form("plots/%s/%s_Mcj2t0Xi.gif",mcAna.AnaTag.Data(),mcAna.Tag.Data()));

}
