#include <iostream>
#include "TTree.h"
#include "TH1D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TCut.h"
#include "TLegend.h"
#include "CmsHi/DiJetAna/ana/aliases_dijet.C"
#include "CmsHi/DiJetAna/ana/selectionCut.h"
using namespace std;

void anaJetEffFake(int doMC=1,
    const char * inFile0Name="../process_aod/outputs/McUqDj120to170_DJes002_10k.root")
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;

  TFile * inFile0 = new TFile(inFile0Name);
  inFile0->ls();
  TTree *mcj2t3, *mcj2t3peri, *mcj2t0, *mcj1t0;
  inFile0->GetObject("dijetAna_mc/djTree",mcj2t3);
  inFile0->GetObject("dijetAna_mc_periph/djTree",mcj2t3peri);
  inFile0->GetObject("dijetAna_mc_genjet_genp/djTree",mcj1t0);


  // Define dijet selection
  selectionCut mcAna(doMC,1,120,170,80);
  cout << "DJ selection: " << TString(mcAna.DJ) << endl;
  cout << "dijetAna_mc/djTree # entries: " << mcj2t3->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj2t3->GetEntries(mcAna.DJ) << endl;
  //cout << "dijetAna_mc_periph/djTree # entries: " << mcj2t3peri->GetEntries() << endl;
  //cout << "# DJ events passed: " << mcj2t3peri->GetEntries(mcAna.DJ) << endl;
  cout << "dijetAna_mc/djTree # entries: " << mcj1t0->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj1t0->GetEntries(mcAna.DJ) << endl;

  // Draw Reco jets
  // Jet Pt
  TCanvas * cJetPt = new TCanvas("cJetPt","cJetPt",500,500);
  cJetPt->SetLogy(0);
  TH1D * hJetPtDJSel = new TH1D("hJetPtDJSel",";p_{T}^{jet};",mcAna.histJetPtBins,mcAna.histJetPtMin,mcAna.histJetPtMax);
  TH1D * hJetPtPreSel = (TH1D*)hJetPtDJSel->Clone("hJetPtPreSel");
  hJetPtPreSel->SetMarkerStyle(kOpenCircle);
  mcj2t3->Draw("nljet>>hJetPtPreSel","","E");
  mcj2t3->Draw("aljet>>+hJetPtPreSel","","Esame");
  mcj2t3->Draw("nljet>>hJetPtDJSel",mcAna.DJ,"Esame");
  mcj2t3->Draw("aljet>>+hJetPtDJSel",mcAna.DJ,"Esame");
  hJetPtPreSel->Draw("hist");
  hJetPtDJSel->Draw("Esame");
  cJetPt->Print("plots/cJetPt.gif");

  // Jet Eta
  TCanvas * cJetEta = new TCanvas("cJetEta","cJetEta",500,500);
  TH1D * hJetEtaDJSel = new TH1D("hJetEtaDJSel",";#eta^{jet};",mcAna.histJetEtaBins,mcAna.histJetEtaMin,mcAna.histJetEtaMax);
  hJetEtaDJSel->SetMinimum(0);
  TH1D * hJetEtaPreSel = (TH1D*)hJetEtaDJSel->Clone("hJetEtaPreSel");
  hJetEtaPreSel->SetMarkerStyle(kOpenCircle);
  mcj2t3->Draw("nljeta>>hJetEtaPreSel","","E");
  mcj2t3->Draw("aljeta>>+hJetEtaPreSel","","Esame");
  mcj2t3->Draw("nljeta>>hJetEtaDJSel",mcAna.DJ,"Esame");
  mcj2t3->Draw("aljeta>>+hJetEtaDJSel",mcAna.DJ,"Esame");
  hJetEtaPreSel->Draw("hist");
  hJetEtaDJSel->Draw("Esame");

  // Jet Phi
  TCanvas * cJetPhi = new TCanvas("cJetPhi","cJetPhi",500,500);
  TH1D * hJetPhiDJSel = new TH1D("hJetPhiDJSel",";#phi^{jet};",mcAna.histJetPhiBins,mcAna.histJetPhiMin,mcAna.histJetPhiMax);
  hJetPhiDJSel->SetMinimum(0);
  TH1D * hJetPhiPreSel = (TH1D*)hJetPhiDJSel->Clone("hJetPhiPreSel");
  hJetPhiPreSel->SetMarkerStyle(kOpenCircle);
  mcj2t3->Draw("nljphi>>hJetPhiPreSel","","E");
  mcj2t3->Draw("aljphi>>+hJetPhiPreSel","","Esame");
  mcj2t3->Draw("nljphi>>hJetPhiDJSel",mcAna.DJ,"Esame");
  mcj2t3->Draw("aljphi>>+hJetPhiDJSel",mcAna.DJ,"Esame");
  hJetPhiPreSel->Draw("hist");
  hJetPhiDJSel->Draw("Esame");

  // DiJet DiPhi
  TCanvas * cJetDjDPhi = new TCanvas("cJetDjDPhi","cJetDjDPhi",500,500);
  TH1D * hJetDjDPhiDJSel = new TH1D("hJetDjDPhiDJSel",";d#phi(j1,j2);",mcAna.histJetPhiBins,0,3.14);
  hJetDjDPhiDJSel->SetMinimum(0);
  TH1D * hJetDjDPhiPreSel = (TH1D*)hJetDjDPhiDJSel->Clone("hJetDjDPhiPreSel");
  hJetDjDPhiPreSel->SetMarkerStyle(kOpenCircle);
  mcj2t3->Draw("jdphi>>hJetDjDPhiPreSel","","E");
  mcj2t3->Draw("jdphi>>hJetDjDPhiDJSel",mcAna.DJ,"Esame");
  hJetDjDPhiPreSel->Draw("hist");
  hJetDjDPhiDJSel->Draw("Esame");
  cJetDjDPhi->Print("plots/cJetDjDPhi.gif");

  // Draw Gen jets
  // GJet Pt
  TCanvas * cGJetPt = new TCanvas("cGJetPt","cGJetPt",500,500);
  cGJetPt->SetLogy(0);
  TH1D * hGJetPtDJSel = new TH1D("hGJetPtDJSel",";p_{T}^{genjet};",mcAna.histJetPtBins,mcAna.histJetPtMin,mcAna.histJetPtMax);
  TH1D * hGJetPtPreSel = (TH1D*)hGJetPtDJSel->Clone("hGJetPtPreSel");
  hGJetPtPreSel->SetMarkerStyle(kOpenCircle);
  mcj1t0->Draw("nljet>>hGJetPtPreSel","","E");
  mcj1t0->Draw("aljet>>+hGJetPtPreSel","","Esame");
  mcj2t3->Draw("nlrjet>>hGJetPtDJSel",mcAna.DJ,"Esame");
  mcj2t3->Draw("alrjet>>+hGJetPtDJSel",mcAna.DJ,"Esame");
  hGJetPtPreSel->Draw("hist");
  hGJetPtDJSel->Draw("Esame");
  cGJetPt->Print("plots/cGJetPt.gif");

  // GJet Eta
  TCanvas * cGJetEta = new TCanvas("cGJetEta","cGJetEta",500,500);
  TH1D * hGJetEtaDJSel = new TH1D("hGJetEtaDJSel",";#eta^{genjet};",mcAna.histJetEtaBins,mcAna.histJetEtaMin,mcAna.histJetEtaMax);
  hGJetEtaDJSel->SetMinimum(0);
  TH1D * hGJetEtaPreSel = (TH1D*)hGJetEtaDJSel->Clone("hGJetEtaPreSel");
  hGJetEtaPreSel->SetMarkerStyle(kOpenCircle);
  mcj1t0->Draw("nljeta>>hGJetEtaPreSel","","E");
  mcj1t0->Draw("aljeta>>+hGJetEtaPreSel","","Esame");
  mcj2t3->Draw("nlrjeta>>hGJetEtaDJSel",mcAna.DJ,"Esame");
  mcj2t3->Draw("alrjeta>>+hGJetEtaDJSel",mcAna.DJ,"Esame");
  hGJetEtaPreSel->Draw("hist");
  hGJetEtaDJSel->Draw("Esame");

  // GJet Phi
  TCanvas * cGJetPhi = new TCanvas("cGJetPhi","cGJetPhi",500,500);
  TH1D * hGJetPhiDJSel = new TH1D("hGJetPhiDJSel",";#phi^{genjet};",mcAna.histJetPhiBins,mcAna.histJetPhiMin,mcAna.histJetPhiMax);
  hGJetPhiDJSel->SetMinimum(0);
  TH1D * hGJetPhiPreSel = (TH1D*)hGJetPhiDJSel->Clone("hGJetPhiPreSel");
  hGJetPhiPreSel->SetMarkerStyle(kOpenCircle);
  mcj1t0->Draw("nljphi>>hGJetPhiPreSel","","E");
  mcj1t0->Draw("aljphi>>+hGJetPhiPreSel","","Esame");
  mcj2t3->Draw("nlrjphi>>hGJetPhiDJSel",mcAna.DJ,"Esame");
  mcj2t3->Draw("alrjphi>>+hGJetPhiDJSel",mcAna.DJ,"Esame");
  hGJetPhiPreSel->Draw("hist");
  hGJetPhiDJSel->Draw("Esame");

  // DiGJet DiPhi
  TCanvas * cGJetDjDPhi = new TCanvas("cGJetDjDPhi","cGJetDjDPhi",500,500);
  TH1D * hGJetDjDPhiDJSel = new TH1D("hGJetDjDPhiDJSel",";d#phi(j1,j2);",mcAna.histJetPhiBins,0,3.14);
  hGJetDjDPhiDJSel->SetMinimum(0);
  TH1D * hGJetDjDPhiPreSel = (TH1D*)hGJetDjDPhiDJSel->Clone("hGJetDjDPhiPreSel");
  hGJetDjDPhiPreSel->SetMarkerStyle(kOpenCircle);
  mcj1t0->Draw("jdphi>>hGJetDjDPhiPreSel","","E");
  mcj1t0->Draw("jdphi>>hGJetDjDPhiDJSel",mcAna.DJ,"Esame");
  hGJetDjDPhiPreSel->Draw("hist");
  hGJetDjDPhiDJSel->Draw("Esame");
}
