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
    const char * inFile0Name="../process_aod/outputs/dijetAna_anaJet_Mc1_try26_10k.root",
    TString outdir = "plots/dijetAna_anaJet_Mc1_try26_10k",
    TString title1="MC Calojet (Hyd2.76TeV+dijet)",
    TString title2="MC Genjet (Hyd2.76TeV+dijet)")
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
  selectionCut mcMatAna(doMC,11);
  gSystem->mkdir(Form("%s/%s",outdir.Data(),mcAna.AnaTag.Data()),kTRUE);
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

  // Save output
  TFile * outf = new TFile(Form("%s/%s/ffana_hists.root",outdir.Data(),mcAna.AnaTag.Data()),"RECREATE");

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
  ccomp2->Print(Form("%s/%s/%s_dPhi.gif",outdir.Data(),mcAna.AnaTag.Data(),mcAna.Tag.Data()) );

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
  ccomp3->Print(Form("%s/%s/%s_Balance.gif",outdir.Data(),mcAna.AnaTag.Data(),mcAna.Tag.Data()));

  cout << " --- Check Trk-Jet Distributions --- " << endl;
  TString djTrkCut = TString(mcAna.DJ && mcAna.Trk);
  cout << "djTrkCut: " << djTrkCut << endl;

  // check inclusive tracks
  TCanvas *ccomp4 = new TCanvas("ccomp4","",500,500);
  ccomp4->SetLogy(1);
  compareHist comp4(mcj2t3,mcj1t0,"ppt","trkPt",djTrkCut.Data(),djTrkCut.Data(),0,25,80);
  comp4.Normalize(1);
  comp4.SetHistName1("Reco Trk");
  comp4.SetHistName2("Gen Partl");
  comp4.SetXTitle("track p_{T}");
  comp4.SetYTitle("Arbitrary normalization");
  comp4.SetLegend(0.45,0.80,0.75,0.9);
  comp4.Draw("E");
  ccomp4->Print(Form("%s/%s/%s_trkPt.gif",outdir.Data(),mcAna.AnaTag.Data(),mcAna.Tag.Data()));


  // check track jet correlations
  TString djTrkCut1 = TString(mcAna.DJ && "ppt>1 && ppt<nljet");
  TCanvas *ccomp7 = new TCanvas("ccomp7","",500,500);
  compareHist comp7(mcj2t3,mcj1t0,"pndphi","TrkDPhi",djTrkCut1.Data(),djTrkCut1.Data(),0,3.14,30);
  comp7.Normalize(1);
  comp7.SetHistName1("Calojet+RecoTrk");
  comp7.SetHistName2("Genjet+GenPartl");
  comp7.SetXTitle("d #phi (j1,trk) (p_{T}^{trk}>1.GeV)");
  comp7.SetYTitle("Arbitrary normalization");
  comp7.SetLegend(0.41,0.27,0.71,0.37);
  comp7.SetMinimum(0);
  comp7.SetMaximum(0.4);
  comp7.Draw("E");
  ccomp7->Print(Form("%s/%s/%s_TrkDPhi.gif",outdir.Data(),mcAna.AnaTag.Data(),mcAna.Tag.Data()));

  // === dijet ana ===
  cout << endl << "==================== DiJet Ana ===================" << endl;
  cout << "DJ Cut: " << mcAna.DJ << endl;
  cout << "mcAna.DJTrk: " << mcAna.DJTrk << endl;
  cout << "Matched DJ Cut: " << mcMatAna.DJ << endl;
  cout << "matched mcAna.DJTrk: " << mcMatAna.DJTrk << endl;
  TCanvas *ctemp = new TCanvas("ctemp","ctemp",500,500);
  // Reco
  AnaFrag mcRecoNr("mcReco","Near",mcj2t3,mcAna.DJ,mcAna.DJTrk,"log(1./zn)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcRecoAw("mcReco","Away",mcj2t3,mcAna.DJ,mcAna.DJTrk,"log(1./za)","padr<0.5","padrbg<0.5");
  AnaFrag mcRecoNrMat("mcRecoMat","Near",mcj2t3,mcMatAna.DJ,mcMatAna.DJTrk,"log(1./zn)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcRecoAwMat("mcRecoMat","Away",mcj2t3,mcMatAna.DJ,mcMatAna.DJTrk,"log(1./za)","padr<0.5","padrbg<0.5");

  // Reco jet + genp
  AnaFrag mcj2t0Nr("mcj2t0","Near",mcj2t0,mcAna.DJ,mcAna.DJTrk,"log(1./zn)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcj2t0Aw("mcj2t0","Away",mcj2t0,mcAna.DJ,mcAna.DJTrk,"log(1./za)","padr<0.5","padrbg<0.5");
  AnaFrag mcj2t0NrMat("mcj2t0Mat","Near",mcj2t0,mcMatAna.DJ,mcMatAna.DJTrk,"log(1./zn)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcj2t0AwMat("mcj2t0Mat","Away",mcj2t0,mcMatAna.DJ,mcMatAna.DJTrk,"log(1./za)","padr<0.5","padrbg<0.5");
  AnaFrag mcj2t0NrMatOrder("mcj2t0MatOrder","Near",mcj2t0,mcMatAna.DJ,mcMatAna.DJTrk,
      "log(1./zn)*(nlrjet>alrjet)+log(1./za)*(nlrjet<alrjet)",
      "(pndr<0.5 && nlrjet>alrjet)||(padr<0.5 && nlrjet<alrjet)",
      "(pndrbg<0.5 && nlrjet>alrjet)||(padrbg<0.5 && nlrjet<alrjet)");

  // gen jet smear + genp
  AnaFrag mcGenJetSmearNr("mcj1Smear","Near",mcj1t0,mcAna.DJ,mcAna.DJTrk,"log(nljet*njec[10]/ppt)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcGenJetSmearAw("mcj1Smear","Away",mcj1t0,mcAna.DJ,mcAna.DJTrk,"log(aljet*ajec[10]/ppt)","padr<0.5","padrbg<0.5");

  // Gen
  AnaFrag mcGenNr("mcGen","Near",mcj1t0,mcAna.DJ,mcAna.DJTrk,"log(1/zn)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcGenAw("mcGen","Away",mcj1t0,mcAna.DJ,mcAna.DJTrk,"log(1/za)","padr<0.5","padrbg<0.5");
  AnaFrag mcGenNrMat("mcGenMat","Near",mcj2t0,mcMatAna.DJ,mcMatAna.DJTrk,"log(nlrjet/ppt)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcGenAwMat("mcGenMat","Away",mcj2t0,mcMatAna.DJ,mcMatAna.DJTrk,"log(alrjet/ppt)","padr<0.5","padrbg<0.5");
  AnaFrag mcGenNrMatOrder("mcGenMatOrder","Near",mcj1t0,mcMatAna.DJ,mcMatAna.DJTrk,
      "log(1./zn)*(nlrjet>alrjet)+log(1./za)*(nlrjet<alrjet)",
      "(pndr<0.5 && nlrjet>alrjet)||(padr<0.5 && nlrjet<alrjet)",
      "(pndrbg<0.5 && nlrjet>alrjet)||(padrbg<0.5 && nlrjet<alrjet)");

  // Gen-Truth
  TCut djTrkTruthCut = mcAna.DJTrk && "psube==0";
  TCut djTrkTruthMat = mcMatAna.DJTrk && "psube==0";
  AnaFrag mcGenTruthNr("mcGenTruth","Near",mcj1t0,mcAna.DJ,djTrkTruthCut,"log(1/zn)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcGenTruthAw("mcGenTruth","Away",mcj1t0,mcAna.DJ,djTrkTruthCut,"log(1/za)","padr<0.5","padrbg<0.5");
  AnaFrag mcGenTruthNrMat("mcGenTruthMat","Near",mcj2t0,mcMatAna.DJ,djTrkTruthMat,"log(nlrjet/ppt)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcGenTruthAwMat("mcGenTruthMat","Away",mcj2t0,mcMatAna.DJ,djTrkTruthMat,"log(alrjet/ppt)","padr<0.5","padrbg<0.5");

  // All done, save and exit
  outf->Write();
  outf->Close();
}
