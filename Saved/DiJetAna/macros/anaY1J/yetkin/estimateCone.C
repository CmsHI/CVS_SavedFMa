#include <string>
#include <vector>
#include <iostream>
#include <TH2D.h>
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TTree.h>

void estimateCone(){

  //  TFile* inf = new TFile("tuneD6T_Pt120/results.root");
  //  TFile* inf2 = new TFile("tuneA_Pt120/results.root");
  //  TFile* inf = new TFile("combined_spectrum.root");

  TChain* t = new TChain("jets/t");
  t->Add("test1/treefile*.root");

  TFile* outf = new TFile("test_dijets_d1209.root","recreate");

  TH1::SetDefaultSumw2();

  TH1D* h = new TH1D("hConeEstimate",";estimated p_{T}^{cone}",200,-80,320);
  TH1D* hb = new TH1D("hConeEstimate2",";estimated p_{T}^{cone}",200,-80,320);

  TH1D* hTop100a = new TH1D("hTop100a",";estimated p_{T}^{cone}",300,100,400);
  TH1D* hTop100b = new TH1D("hTop100b",";estimated p_{T}^{cone}",300,100,400);
  TH1D* hTop100aO = new TH1D("hTop100aO",";estimated p_{T}^{cone}",300,100,400);
  TH1D* hTop100bO = new TH1D("hTop100bO",";estimated p_{T}^{cone}",300,100,400);
  TH1D* hTop100aM = new TH1D("hTop100aM",";estimated p_{T}^{cone}",300,100,400);
  TH1D* hTop100bM = new TH1D("hTop100bM",";estimated p_{T}^{cone}",300,100,400);


  TF1* f1 = new TF1("f1","gaus",-150,150);
  f1->SetParameter(0,1);
  f1->SetParameter(2,18.7786);
  f1->SetParameter(1,0);

  double top100a = 204;
  double top100b = 206;

  TCanvas * cf = new TCanvas("cf1","",400,400);
  f1->Draw();

  h1->SetDirectory(0);
  h2->SetDirectory(0);
  outf->cd();

  h1->GetXaxis()->SetRange(1,h1->GetNbinsX()-1);
  h1->Write();
  h2->Write();

  h->Write();

  TH1D* hTest = (TH1D*)h->Clone("hTest");
  hTest->Reset();
  
  int nSample = 5000000;

  double eta[MAXJETS],phi[MAXJETS],pt[MAXJETS];



  for(int j = 0; j < nSample; ++j){
    double pu = f1->GetRandom();


    double pt1 = h1->GetRandom();


    h->Fill(pt1+pu);
    hb->Fill(pt2+pu);

    if(pt1+pu > top100a) hTop100a->Fill(pt1);
    if(pt2+pu > top100b) hTop100b->Fill(pt2);
    
  }

  h->SetLineColor(4);
  hb->SetLineColor(4);
  hTop100b->SetLineColor(2);
  hTop100b->SetMarkerColor(2);

  h->Scale(h1->Integral("width")/h->Integral("width"));
  hb->Scale(h2->Integral("width")/hb->Integral("width"));

  TCanvas* c1 = new TCanvas("c1","c1",400,400);

  h->GetXaxis()->SetRange(hb->FindBin(160),hb->FindBin(350));
  h1->GetXaxis()->SetRange(h1->FindBin(160),h1->FindBin(350));
  h->Draw();
  h1->Draw("same");

  TLine * l1 = new TLine(204,0,204,15);
  TLine * l1s = new TLine(top100a,0,top100a,15);
  l1s->SetLineColor(4);
  l1->Draw();
  //  l1s->Draw();

  for(int i = 0; i < hTop100a->GetNbinsX(); ++i){
    if(i > hTop100aO->FindBin(204)){
      hTop100aO->SetBinContent(i,h1->GetBinContent(i));
      hTop100aO->SetBinError(i,h1->GetBinError(i));
    }
    if(i > hTop100bO->FindBin(206)){
      hTop100bO->SetBinContent(i,h2->GetBinContent(i));
      hTop100bO->SetBinError(i,h2->GetBinError(i));
    }

    if(i > hTop100aM->FindBin(top100a)){
      hTop100aM->SetBinContent(i,hTop100a->GetBinContent(i));
      hTop100aM->SetBinError(i,hTop100a->GetBinError(i));
    }
    if(i > hTop100bM->FindBin(top100b)){
      hTop100bM->SetBinContent(i,hTop100b->GetBinContent(i));
      hTop100bM->SetBinError(i,hTop100b->GetBinError(i));
    }


  }


  TCanvas* c2 = new TCanvas("c2","c2",400,400);
  hb->GetXaxis()->SetRange(hb->FindBin(160),hb->FindBin(350));
  h2->GetXaxis()->SetRange(h2->FindBin(160),h2->FindBin(350));
	hb->Draw();
	h2->Draw("same");

	TLine * l2 = new TLine(206,0,206,15);
	TLine * l2s = new TLine(top100b,0,top100b,15);
	l2s->SetLineColor(4);
	l2->Draw();
	//	l2s->Draw();


    double tot = 0;

    for(int i = h1->GetNbinsX()-1; i >=0; --i){
      tot+= h1->GetBinContent(i)*h1->GetBinWidth(i);
      if(tot > 100){
	cout<<"Real Distribution D6T "<<j<<endl;
	cout<<"PT : "<<h1->GetBinLowEdge(i)<<endl;
	break;
      }
    }
    tot = 0;

	for(int i = h->GetNbinsX()-1; i >=0; --i){
		tot+= h->GetBinContent(i)*h->GetBinWidth(i);
		if(tot > 100){
			cout<<"Smeared Distribution D6T "<<j<<endl;
			cout<<"PT : "<<h->GetBinLowEdge(i)<<endl;
			break;
		}
    }

	tot = 0;
	for(int i = h2->GetNbinsX()-1; i >=0; --i){
	  tot+= h2->GetBinContent(i)*h2->GetBinWidth(i);
	  if(tot > 100){
	    cout<<"Real Distribution Tune A "<<j<<endl;
	    cout<<"PT : "<<h2->GetBinLowEdge(i)<<endl;
	    break;
	  }
	}
	tot = 0;

	for(int i = hb->GetNbinsX()-1; i >=0; --i){
		tot+= hb->GetBinContent(i)*hb->GetBinWidth(i);
		if(tot > 100){
			cout<<"Smeared Distribution Tune A "<<j<<endl;
			cout<<"PT : "<<hb->GetBinLowEdge(i)<<endl;
			break;
		}
    }
	
	
	TCanvas* c3 = new TCanvas("c3","",400,400);		

	hTop100a->Draw();
        hTop100b->Draw("same");

	double meanD6T = hTop100aO->GetMean();
        double rmsD6T = hTop100aO->GetRMS();
	double meanD6Tsmeared = hTop100a->GetMean();
        double rmsD6Tsmeared = hTop100a->GetRMS();
	double meanD6Tmixed = hTop100aM->GetMean();
        double rmsD6Tmixed = hTop100aM->GetRMS();

	double meanA = hTop100bO->GetMean();
        double rmsA = hTop100bO->GetRMS();
        double meanAsmeared = hTop100b->GetMean();
        double rmsAsmeared = hTop100b->GetRMS();
	double meanAmixed = hTop100bM->GetMean();
        double rmsAmixed = hTop100bM->GetRMS();

	cout<<"MEAN D6T : "<<meanD6T<<endl;
        cout<<"RMS D6T : "<<rmsD6T<<endl;
	cout<<"MEAN D6T smeared : "<<meanD6Tsmeared<<endl;
        cout<<"RMS D6T smeared : "<<rmsD6Tsmeared<<endl;
	cout<<"MEAN D6T mixed : "<<meanD6Tmixed<<endl;
        cout<<"RMS D6T mixed : "<<rmsD6Tmixed<<endl;
	cout<<"================================"<<endl;
	cout<<"MEAN A : "<<meanA<<endl;
        cout<<"RMS A : "<<rmsA<<endl;
        cout<<"MEAN A smeared : "<<meanAsmeared<<endl;
        cout<<"RMS A smeared : "<<rmsAsmeared<<endl;
        cout<<"MEAN A mixed : "<<meanAmixed<<endl;
        cout<<"RMS A mixed : "<<rmsAmixed<<endl;




	outf->Write();



}
