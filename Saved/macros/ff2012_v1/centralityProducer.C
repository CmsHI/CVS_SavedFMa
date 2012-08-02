#include "CutAndBinCollection2011.h"

void centralityProducerOne(TString fMBData="mergedFiles/lowHighPtTracks_data_minbias_1MegaEvents.root", bool isDATA = true) ;


void centralityProducer() {
  remove( "percentBins.root" );
  centralityProducerOne("mergedFiles/lowHighPtTracks_data_minbias_1MegaEvents.root", true);
  centralityProducerOne("mergedFiles/lowHighPtTracks_minbias_MC.root", false);
}

void centralityProducerOne(TString fMBData, bool isDATA ) {
  
  TH1D* hhf = new TH1D("hhf",";HF energy (GeV) ; Entries",2000000,0,6000);
  if ( isDATA )
    hhf->SetName("hhf_data");
  else
    hhf->SetName("hhf_mc");
  
  TH1D* hhfc = (TH1D*)hhf->Clone(Form("hhfc_%s",hhf->GetName()));
  hhfc->Reset();
  
  TChain *tdata = new TChain("hiEvtAnalyzer/HiTree");
  tdata->Add(fMBData);
  tdata->AddFriend("ySkim=skimanalysis/HltTree",fMBData.Data());
  if ( isDATA ) 
    tdata->AddFriend("yHlt=hltanalysis/HltTree",fMBData.Data());
  
  
  if ( isDATA ) 
    tdata->Draw(Form("hiHF>>%s",hhf->GetName()),"ySkim.pHBHENoiseFilter>0 && ySkim.pcollisionEventSelection>0 && yHlt.HLT_HIMinBiasHfOrBSC_v1");
  else 
    tdata->Draw(Form("hiHF>>%s",hhf->GetName()),"ySkim.pcollisionEventSelection>0");
  
  
  for ( int i=1; i<= hhf->GetNbinsX() ; i++) {
    if ( i==1) 
      hhfc->SetBinContent(i,     hhfc->GetBinContent(i) );
    else {
      hhfc->SetBinContent(i, hhfc->GetBinContent(i-1) + hhf->GetBinContent(i));
    }
  }

  TCanvas* c1;
  if ( isDATA )
    c1 = new TCanvas("c1_data","data",1200,400);
  else 
    c1 = new TCanvas("c1_mc","mc",1200,400);

  c1->Divide(3,1);
  c1->cd(1);
  hhf->Draw();
  gPad->SetLogy();
  c1->cd(2);
  hhfc->Scale(1./hhf->GetEntries());
  hhfc->SetYTitle("cumulative event fraction");
  hhfc->Draw();
  
  c1->cd(3);
  vector<double> hfs;
  hfs.clear();
  hfs.push_back(0);
  for ( int i=1 ; i<=99; i++) {
    
    float bound = (float)i/100.;
    float theX;
    for (int j=1; j<=hhfc->GetNbinsX() ; j++) {
      if (    hhfc->GetBinContent(j) >= bound ) {
	theX = hhfc->GetBinCenter(j);
	break;
      }
    }
    hfs.push_back(theX);
    
  }
  cout << " size = " << hfs.size() << endl;
  double hfs1[400];
  for ( int i=0; i<hfs.size() ; i++) {
    hfs1[i] = hfs[i];
  }
  TH1D* hres = new TH1D("hresult",";HF energy (GeV) ; Entries",(int)hfs.size()-1, hfs1);
  if ( isDATA )
    hres->SetName("hresult_data");
  else
    hres->SetName("hresult_mc");
  
  if ( isDATA)  
    tdata->Draw(Form("hiHF>>%s",hres->GetName()),"ySkim.pHBHENoiseFilter>0 && ySkim.pcollisionEventSelection>0");
  else 
    tdata->Draw(Form("hiHF>>%s",hres->GetName()),"ySkim.pcollisionEventSelection>0");

  hres->SetAxisRange(0,10000,"Y");
  hres->Draw();
  TFile outf = TFile("percentBins.root","update");
  hres->Write();
  outf.Close();
  
  
}
