#include "Saved/DiJetAna/macros/aliases_dijet.C"

void anaFragShapePtBins(TString infile1="/net/hisrv0001/home/frankma/scratch01/ana/merge/dj_HCPR-J50U-JSON_hiGoodMergedTrksRuns152562to152643-v1_StdAna1204v2.root",
		  TString infile2="/net/hisrv0001/home/frankma/scratch01/ana/merge/dj_HCPR-GoodTrk1123_All0.root"
)
{
  gStyle->SetMarkerStyle(20);

  TChain * djcalo = new TChain("djcalo/djTree");
  djcalo->Add(infile1);
  djcalo->Add(infile2);
  aliases_dijet(djcalo);
  TString evtSel("(cent<10 && nljet>120 && abs(nljeta)<2 && aljet>50 && abs(aljeta)<2 && jdphi>2.5 && Aj>0.24)");
  TH1::SetDefaultSumw2();

  //cones reflected in eta. avoid jets *right* at mid-rapidity to avoid overlap
  djcalo->SetAlias("pndrrefl","(sqrt(pndphi*pndphi+(peta+nljeta)*(peta+nljeta)))");
  djcalo->SetAlias("padrrefl","(sqrt(padphi*padphi+(peta+aljeta)*(peta+aljeta)))");
  
  TString evtSelNS = evtSel + " && (abs(nljeta)>0.5)";
  TString evtSelAS = evtSel + " && (abs(aljeta)>0.5)";

  //jet energy imbalance
  TCanvas * c0 = new TCanvas("c0","c0",500,500);
  djcalo->Draw("Aj>>hAj(20,0,1)",evtSel);

  //back-to-back correlation
  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  TH1D * hJDPhi = new TH1D("hJDPhi","hJDPhi",50,0,3.1416);
  TH1D * hJDPhi2 = new TH1D("hJDPhi2","hJDPhi",50,0,3.1416);
  Float_t numDJNS = djcalo->Draw("jdphi>>hJDPhi",evtSelNS);
  Float_t numDJAS = djcalo->Draw("jdphi>>hJDPhi",evtSelAS,"same");
  cout << "num djs: " << numDJNS << " (near-side) \t" << numDJAS << " (away-side)" << endl;

  //track energy density by dR in pt bins
  TH1D * hPNDRTrk0 = new TH1D("hPNDRTrk0",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  TH1D * hPADRTrk0 = new TH1D("hPADRTrk0",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  djcalo->Draw("pndr>>hPNDRTrk0",Form("(%s&&ppt<1.0)*(ppt/(TMath::TwoPi()*pndr))",evtSelNS.Data()),"goff");
  djcalo->Draw("padr>>hPADRTrk0",Form("(%s&&ppt<1.0)*(ppt/(TMath::TwoPi()*padr))",evtSelAS.Data()),"goff");
  TH1D * hPNDRReflTrk0 = new TH1D("hPNDRReflTrk0",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  TH1D * hPADRReflTrk0 = new TH1D("hPADRReflTrk0",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  djcalo->Draw("pndrrefl>>hPNDRReflTrk0",Form("(%s&&ppt<1.0)*(ppt/(TMath::TwoPi()*pndrrefl))",evtSelNS.Data()),"goff");
  djcalo->Draw("padrrefl>>hPADRReflTrk0",Form("(%s&&ppt<1.0)*(ppt/(TMath::TwoPi()*padrrefl))",evtSelAS.Data()),"goff");

  TH1D * hPNDRTrk1 = new TH1D("hPNDRTrk1",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  TH1D * hPADRTrk1 = new TH1D("hPADRTrk1",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  djcalo->Draw("pndr>>hPNDRTrk1",Form("(%s&&ppt<2.0&&ppt>=1.0)*(ppt/(TMath::TwoPi()*pndr))",evtSelNS.Data()),"goff");
  djcalo->Draw("padr>>hPADRTrk1",Form("(%s&&ppt<2.0&&ppt>=1.0)*(ppt/(TMath::TwoPi()*padr))",evtSelAS.Data()),"goff");
  TH1D * hPNDRReflTrk1 = new TH1D("hPNDRReflTrk1",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  TH1D * hPADRReflTrk1 = new TH1D("hPADRReflTrk1",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  djcalo->Draw("pndrrefl>>hPNDRReflTrk1",Form("(%s&&ppt<2.0&&ppt>=1.0)*(ppt/(TMath::TwoPi()*pndrrefl))",evtSelNS.Data()),"goff");
  djcalo->Draw("padrrefl>>hPADRReflTrk1",Form("(%s&&ppt<2.0&&ppt>=1.0)*(ppt/(TMath::TwoPi()*padrrefl))",evtSelAS.Data()),"goff");

  TH1D * hPNDRTrk2 = new TH1D("hPNDRTrk2",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  TH1D * hPADRTrk2 = new TH1D("hPADRTrk2",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  djcalo->Draw("pndr>>hPNDRTrk2",Form("(%s&&ppt<4.0&&ppt>=2.0)*(ppt/(TMath::TwoPi()*pndr))",evtSelNS.Data()),"goff");
  djcalo->Draw("padr>>hPADRTrk2",Form("(%s&&ppt<4.0&&ppt>=2.0)*(ppt/(TMath::TwoPi()*padr))",evtSelAS.Data()),"goff");
  TH1D * hPNDRReflTrk2 = new TH1D("hPNDRReflTrk2",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  TH1D * hPADRReflTrk2 = new TH1D("hPADRReflTrk2",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  djcalo->Draw("pndrrefl>>hPNDRReflTrk2",Form("(%s&&ppt<4.0&&ppt>=2.0)*(ppt/(TMath::TwoPi()*pndrrefl))",evtSelNS.Data()),"goff");
  djcalo->Draw("padrrefl>>hPADRReflTrk2",Form("(%s&&ppt<4.0&&ppt>=2.0)*(ppt/(TMath::TwoPi()*padrrefl))",evtSelAS.Data()),"goff");

  TH1D * hPNDRTrk4 = new TH1D("hPNDRTrk4",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  TH1D * hPADRTrk4 = new TH1D("hPADRTrk4",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  djcalo->Draw("pndr>>hPNDRTrk4",Form("(%s&&ppt<8.0&&ppt>=4.0)*(ppt/(TMath::TwoPi()*pndr))",evtSelNS.Data()),"goff");
  djcalo->Draw("padr>>hPADRTrk4",Form("(%s&&ppt<8.0&&ppt>=4.0)*(ppt/(TMath::TwoPi()*padr))",evtSelAS.Data()),"goff");
  TH1D * hPNDRReflTrk4 = new TH1D("hPNDRReflTrk4",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  TH1D * hPADRReflTrk4 = new TH1D("hPADRReflTrk4",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  djcalo->Draw("pndrrefl>>hPNDRReflTrk4",Form("(%s&&ppt<8.0&&ppt>=4.0)*(ppt/(TMath::TwoPi()*pndrrefl))",evtSelNS.Data()),"goff");
  djcalo->Draw("padrrefl>>hPADRReflTrk4",Form("(%s&&ppt<8.0&&ppt>=4.0)*(ppt/(TMath::TwoPi()*padrrefl))",evtSelAS.Data()),"goff");

  TH1D * hPNDRTrk8 = new TH1D("hPNDRTrk8",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  TH1D * hPADRTrk8 = new TH1D("hPADRTrk8",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  djcalo->Draw("pndr>>hPNDRTrk8",Form("(%s&&ppt>=8.0)*(ppt/(TMath::TwoPi()*pndr))",evtSelNS.Data()),"goff");
  djcalo->Draw("padr>>hPADRTrk8",Form("(%s&&ppt>=8.0)*(ppt/(TMath::TwoPi()*padr))",evtSelAS.Data()),"goff");
  TH1D * hPNDRReflTrk8 = new TH1D("hPNDRReflTrk8",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  TH1D * hPADRReflTrk8 = new TH1D("hPADRReflTrk8",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  djcalo->Draw("pndrrefl>>hPNDRReflTrk8",Form("(%s&&ppt>=8.0)*(ppt/(TMath::TwoPi()*pndrrefl))",evtSelNS.Data()),"goff");
  djcalo->Draw("padrrefl>>hPADRReflTrk8",Form("(%s&&ppt>=8.0)*(ppt/(TMath::TwoPi()*padrrefl))",evtSelAS.Data()),"goff");

  hPNDRTrk0->Scale(1./(numDJNS*hPNDRTrk0->GetBinWidth(1)));
  hPADRTrk0->Scale(1./(numDJAS*hPADRTrk0->GetBinWidth(1)));
  hPNDRReflTrk0->Scale(1./(numDJNS*hPNDRReflTrk0->GetBinWidth(1)));
  hPADRReflTrk0->Scale(1./(numDJAS*hPADRReflTrk0->GetBinWidth(1)));

  hPNDRTrk1->Scale(1./(numDJNS*hPNDRTrk1->GetBinWidth(1)));
  hPADRTrk1->Scale(1./(numDJAS*hPADRTrk1->GetBinWidth(1)));
  hPNDRReflTrk1->Scale(1./(numDJNS*hPNDRReflTrk1->GetBinWidth(1)));
  hPADRReflTrk1->Scale(1./(numDJAS*hPADRReflTrk1->GetBinWidth(1)));

  hPNDRTrk2->Scale(1./(numDJNS*hPNDRTrk2->GetBinWidth(1)));
  hPADRTrk2->Scale(1./(numDJAS*hPADRTrk2->GetBinWidth(1)));
  hPNDRReflTrk2->Scale(1./(numDJNS*hPNDRReflTrk2->GetBinWidth(1)));
  hPADRReflTrk2->Scale(1./(numDJAS*hPADRReflTrk2->GetBinWidth(1)));

  hPNDRTrk4->Scale(1./(numDJNS*hPNDRTrk4->GetBinWidth(1)));
  hPADRTrk4->Scale(1./(numDJAS*hPADRTrk4->GetBinWidth(1)));
  hPNDRReflTrk4->Scale(1./(numDJNS*hPNDRReflTrk4->GetBinWidth(1)));
  hPADRReflTrk4->Scale(1./(numDJAS*hPADRReflTrk4->GetBinWidth(1)));

  hPNDRTrk8->Scale(1./(numDJNS*hPNDRTrk8->GetBinWidth(1)));
  hPADRTrk8->Scale(1./(numDJAS*hPADRTrk8->GetBinWidth(1)));
  hPNDRReflTrk8->Scale(1./(numDJNS*hPNDRReflTrk8->GetBinWidth(1)));
  hPADRReflTrk8->Scale(1./(numDJAS*hPADRReflTrk8->GetBinWidth(1)));

  hPNDRTrk0->SetMinimum(0.1);
  hPNDRTrk0->SetMaximum(1.3*hPNDRTrk0->GetBinContent(1));
  hPNDRTrk0->SetMarkerStyle(kFullSquare);
  hPNDRTrk0->SetMarkerColor(kRed);
  hPADRTrk0->SetMarkerStyle(kOpenSquare);
  hPADRTrk0->SetMarkerColor(kBlue);
  hPNDRReflTrk0->SetLineColor(kRed);
  hPADRReflTrk0->SetLineColor(kBlue);

  hPNDRTrk1->SetMinimum(0.1);
  hPNDRTrk1->SetMaximum(1.3*hPNDRTrk1->GetBinContent(1));
  hPNDRTrk1->SetMarkerStyle(kFullSquare);
  hPNDRTrk1->SetMarkerColor(kRed);
  hPADRTrk1->SetMarkerStyle(kOpenSquare);
  hPADRTrk1->SetMarkerColor(kBlue);
  hPNDRReflTrk1->SetLineColor(kRed);
  hPADRReflTrk1->SetLineColor(kBlue);

  hPNDRTrk2->SetMinimum(0.1);
  hPNDRTrk2->SetMaximum(1.3*hPNDRTrk2->GetBinContent(1));
  hPNDRTrk2->SetMarkerStyle(kFullSquare);
  hPNDRTrk2->SetMarkerColor(kRed);
  hPADRTrk2->SetMarkerStyle(kOpenSquare);
  hPADRTrk2->SetMarkerColor(kBlue);
  hPNDRReflTrk2->SetLineColor(kRed);
  hPADRReflTrk2->SetLineColor(kBlue);

  hPNDRTrk4->SetMinimum(0.1);
  hPNDRTrk4->SetMaximum(1.3*hPNDRTrk4->GetBinContent(1));
  hPNDRTrk4->SetMarkerStyle(kFullSquare);
  hPNDRTrk4->SetMarkerColor(kRed);
  hPADRTrk4->SetMarkerStyle(kOpenSquare);
  hPADRTrk4->SetMarkerColor(kBlue);
  hPNDRReflTrk4->SetLineColor(kRed);
  hPADRReflTrk4->SetLineColor(kBlue);

  hPNDRTrk8->SetMinimum(0.1);
  hPNDRTrk8->SetMaximum(1.3*hPNDRTrk8->GetBinContent(1));
  hPNDRTrk8->SetMarkerStyle(kFullSquare);
  hPNDRTrk8->SetMarkerColor(kRed);
  hPADRTrk8->SetMarkerStyle(kOpenSquare);
  hPADRTrk8->SetMarkerColor(kBlue);
  hPNDRReflTrk8->SetLineColor(kRed);
  hPADRReflTrk8->SetLineColor(kBlue);

  TCanvas * c3 = new TCanvas("c3","c3",800,500);
  c3->Divide(3,2);
  c3->cd(1);
  hPNDRTrk0->Draw("E");
  hPADRTrk0->Draw("Esame");
  hPNDRReflTrk0->Draw("histsame");
  hPADRReflTrk0->Draw("histsame");
  c3->cd(2);
  hPNDRTrk1->Draw("E");
  hPADRTrk1->Draw("Esame");
  hPNDRReflTrk1->Draw("histsame");
  hPADRReflTrk1->Draw("histsame");
  c3->cd(3);
  hPNDRTrk2->Draw("E");
  hPADRTrk2->Draw("Esame");
  hPNDRReflTrk2->Draw("histsame");
  hPADRReflTrk2->Draw("histsame");
  c3->cd(4);
  hPNDRTrk4->Draw("E");
  hPADRTrk4->Draw("Esame");
  hPNDRReflTrk4->Draw("histsame");
  hPADRReflTrk4->Draw("histsame");
  c3->cd(5);
  hPNDRTrk8->Draw("E");
  hPADRTrk8->Draw("Esame");
  hPNDRReflTrk8->Draw("histsame");
  hPADRReflTrk8->Draw("histsame");

  TCanvas * c4 = new TCanvas("c4","c4",600,500);
  c4->Divide(3,2);
  c4->cd(1);
  TH1D* hPNDRSigTrk0 = (TH1D*) hPNDRTrk0->Clone("hPNDRSigTrk0");
  hPNDRSigTrk0->Add(hPNDRReflTrk0,-1);
  hPNDRSigTrk0->Draw("E");
  TH1D* hPADRSigTrk0 = (TH1D*) hPADRTrk0->Clone("hPADRSigTrk0");
  hPADRSigTrk0->Add(hPADRReflTrk0,-1);
  hPADRSigTrk0->Draw("Esame");
  c4->cd(2);
  TH1D* hPNDRSigTrk1 = (TH1D*) hPNDRTrk1->Clone("hPNDRSigTrk1");
  hPNDRSigTrk1->Add(hPNDRReflTrk1,-1);
  hPNDRSigTrk1->Draw("E");
  TH1D* hPADRSigTrk1 = (TH1D*) hPADRTrk1->Clone("hPADRSigTrk1");
  hPADRSigTrk1->Add(hPADRReflTrk1,-1);
  hPADRSigTrk1->Draw("Esame");
  c4->cd(3);
  TH1D* hPNDRSigTrk2 = (TH1D*) hPNDRTrk2->Clone("hPNDRSigTrk2");
  hPNDRSigTrk2->Add(hPNDRReflTrk2,-1);
  hPNDRSigTrk2->Draw("E");
  TH1D* hPADRSigTrk2 = (TH1D*) hPADRTrk2->Clone("hPADRSigTrk2");
  hPADRSigTrk2->Add(hPADRReflTrk2,-1);
  hPADRSigTrk2->Draw("Esame");
  c4->cd(4);
  TH1D* hPNDRSigTrk4 = (TH1D*) hPNDRTrk4->Clone("hPNDRSigTrk4");
  hPNDRSigTrk4->Add(hPNDRReflTrk4,-1);
  hPNDRSigTrk4->Draw("E");
  TH1D* hPADRSigTrk4 = (TH1D*) hPADRTrk4->Clone("hPADRSigTrk4");
  hPADRSigTrk4->Add(hPADRReflTrk4,-1);
  hPADRSigTrk4->Draw("Esame");
  c4->cd(5);
  TH1D* hPNDRSigTrk8 = (TH1D*) hPNDRTrk8->Clone("hPNDRSigTrk8");
  hPNDRSigTrk8->Add(hPNDRReflTrk8,-1);
  hPNDRSigTrk8->Draw("E");
  TH1D* hPADRSigTrk8 = (TH1D*) hPADRTrk8->Clone("hPADRSigTrk8");
  hPADRSigTrk8->Add(hPADRReflTrk8,-1);
  hPADRSigTrk8->Draw("Esame");

  TCanvas * c5 = new TCanvas("c5","c5",600,500);
  TH1D* hDiff0 = (TH1D*) hPADRSigTrk0->Clone("hDiff0");
  hDiff0->Add(hPNDRSigTrk0,-1);
  hDiff0->SetMarkerColor(kBlue);
  hDiff0->SetMarkerStyle(kFullSquare);
  hDiff0->SetMaximum(14);
  hDiff0->SetMinimum(-4);
  hDiff0->SetStats(0);
  TH1D* hDiff1 = (TH1D*) hPADRSigTrk1->Clone("hDiff1");
  hDiff1->Add(hPNDRSigTrk1,-1);
  hDiff1->SetMarkerColor(kGreen-2);
  hDiff1->SetMarkerStyle(kFullCircle);
  TH1D* hDiff2 = (TH1D*) hPADRSigTrk2->Clone("hDiff2");
  hDiff2->Add(hPNDRSigTrk2,-1);
  hDiff2->SetMarkerColor(kOrange-2);
  hDiff2->SetMarkerSize(1.3);
  hDiff2->SetMarkerStyle(kFullStar);
  TH1D* hDiff4 = (TH1D*) hPADRSigTrk4->Clone("hDiff4");
  hDiff4->Add(hPNDRSigTrk4,-1);
  hDiff4->SetMarkerColor(kOrange+8);
  hDiff4->SetMarkerStyle(kFullTriangleUp);
  TH1D* hDiff8 = (TH1D*) hPADRSigTrk8->Clone("hDiff8");
  hDiff8->Add(hPNDRSigTrk8,-1);
  hDiff8->SetMarkerColor(kRed);
  hDiff8->SetMarkerStyle(kFullTriangleDown);

  hDiff0->Draw("l");
  hDiff1->Draw("lsame");
  hDiff2->Draw("lsame");
  hDiff4->Draw("lsame");
  hDiff8->Draw("lsame");

  TFile *fout = new TFile("out.root","RECREATE");
  fout->cd();

  hPNDRSigTrk0->Write();
  hPADRSigTrk0->Write();
  hPNDRSigTrk1->Write();
  hPADRSigTrk1->Write();
  hPNDRSigTrk2->Write();
  hPADRSigTrk2->Write();
  hPNDRSigTrk4->Write();
  hPADRSigTrk4->Write();
  hPNDRSigTrk8->Write();
  hPADRSigTrk8->Write();

  /*
  hPNDRTrk0->Write();
  hPADRTrk0->Write();
  hPNDRReflTrk0->Write();
  hPADRReflTrk0->Write();
  hPNDRTrk1->Write();
  hPADRTrk1->Write();
  hPNDRReflTrk1->Write();
  hPADRReflTrk1->Write();
  hPNDRTrk2->Write();
  hPADRTrk2->Write();
  hPNDRReflTrk2->Write();
  hPADRReflTrk2->Write();
  hPNDRTrk4->Write();
  hPADRTrk4->Write();
  hPNDRReflTrk4->Write();
  hPADRReflTrk4->Write();
  hPNDRTrk8->Write();
  hPADRTrk8->Write();
  hPNDRReflTrk8->Write();
  hPADRReflTrk8->Write();
  */

  fout->Close();
}
