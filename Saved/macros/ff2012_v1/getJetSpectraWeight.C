#include <TStyle.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TNtuple.h>
#include <iostream>
#include <TLine.h>
#include <TMath.h>
#include <math.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TCut.h>
#include <TClonesArray.h>
#include <TH3D.h>
#include "commonUtility.h"
#include <TRandom3.h>
#include "CutAndBinCollection2011.h"
#include "corrFunctionTrk.h"

const int nPtJetBin = 7;
double ptJetBin[nPtJetBin+1] = {20,25,30,35,40,50,70,1000};

void getJetSpectraWeightSub1(TH1D* jSpec[4][4]=0, int cutOpt=9999, int icent=1, int ipt=1, int sampleType=kHIDATA);
TH1D* getJetSpectraWeightDiff(int cutOpt=9999, int icent=1, int ipt=1) ;



void getJetSpectraWeight(int cutOpt=9999) { 
  CutAndBinCollection cab;
  cab.setCuts(cutOpt);

  int nCentBin = cab.getNCentBin();
  int nPtBin   = cab.getNPtBin(); 
  vector<double> centBin_std = cab.getCentBin();
  double centBinArr[100];
  for ( int i = 0 ; i<=nCentBin ;i++) {
    centBinArr[i] = centBin_std[i];
  }
  
  vector<double> ptBinVector = cab.getPtBin();
  double ptBinArr[100];
  for ( int i = 0 ; i<=nPtBin ;i++) {
    ptBinArr[i] = ptBinVector[i];
  }

  TH1D* hRatio[10][10];
  TH3D* hRatio3D = new TH3D("hRatio3D","",nCentBin,centBinArr,nPtBin,ptBinArr,nPtJetBin,ptJetBin);

  TCanvas* c0 = new TCanvas("c0","",1200,800);
  c0->Divide(nCentBin,nPtBin);
  c0->Draw();
  for ( int icent = 1 ; icent<= nCentBin ; icent++) { 
    int lowerCent = centBin_std[icent-1];
    int upperCent = centBin_std[icent]-1;
    for ( int ipt = 1 ; ipt <= nPtBin ; ipt++) { 
      float ptPhoLow = ptBinVector[ipt-1];
      float ptPhoHigh= ptBinVector[ipt];
      c0->cd(icent + (ipt-1)*nCentBin);
      hRatio[icent][ipt] = getJetSpectraWeightDiff(cutOpt, icent, ipt ) ;
      for ( int ij=1;ij<=nPtJetBin;ij++) { 
	hRatio3D->SetBinContent(icent,ipt,ij, hRatio[icent][ipt]->GetBinContent(ij));
	hRatio3D->SetBinError(icent,ipt,ij, 0.00000000001);
	hRatio3D->Sumw2();
      }
    }
  }
  
  TCanvas* cRatio = new TCanvas("cRatio","",1200,800);
  cRatio->Divide(nCentBin,nPtBin);
  cRatio->Draw();
  for ( int icent = 1 ; icent<= nCentBin ; icent++) {
    int lowerCent = centBin_std[icent-1];
    int upperCent = centBin_std[icent]-1;
    for ( int ipt = 1 ; ipt <= nPtBin ; ipt++) {
      float ptPhoLow = ptBinVector[ipt-1];
      float ptPhoHigh= ptBinVector[ipt];
      cRatio->cd(icent + (ipt-1)*nCentBin);
      hRatio[icent][ipt]->SetAxisRange(0,69,"X");
      hRatio[icent][ipt]->SetAxisRange(0,5,"Y");
      hRatio[icent][ipt]->Draw();
    }
  }
  
  TCanvas* cR2 = new TCanvas("cR2","",1200,800);
  cR2->Divide(nCentBin,nPtBin);
  cR2->Draw();
  for ( int icent = 1 ; icent<= nCentBin ; icent++) {
    int lowerCent = centBin_std[icent-1];
    int upperCent = centBin_std[icent]-1;
    for ( int ipt = 1 ; ipt <= nPtBin ; ipt++) {
      cR2->cd(icent + (ipt-1)*nCentBin);
      TH1D* tt = (TH1D*)hRatio3D->ProjectionZ("_pz",icent,icent,ipt,ipt);
      tt->SetAxisRange(0,69,"X");
      tt->SetAxisRange(0,5,"Y");
      tt->DrawCopy();
    }
  }

  TFile outf = TFile(Form("jetReweightingTable_cutOpt%d.root",cutOpt),"recreate");
  hRatio3D->Write();
  outf.Close();

  
  
}


TH1D* getJetSpectraWeightDiff(int cutOpt, int icent, int ipt) { 


  TH1D* jSpecDATA[4][4];
  TH1D* jSpecMC[4][4];
  getJetSpectraWeightSub1(jSpecDATA,cutOpt,icent,ipt,kHIDATA);
  cout << " here 1 " << endl;
  getJetSpectraWeightSub1(jSpecMC,cutOpt,icent,ipt, kHIMCNoJetWeight);
  handsomeTH1(jSpecDATA[0][0],2);
  handsomeTH1(jSpecMC[0][0],1);
  
  TH1D* hTemp = new TH1D("hTemp",";Jet p_{T} (GeV);Entries per trigger photon",1000,0,1000);
  handsomeTH1(hTemp,1);
  hTemp->SetAxisRange(0,90,"X");
  hTemp->SetAxisRange(0,0.35,"Y");
  hTemp->DrawCopy();
  jSpecDATA[0][0]->Draw("same");
  jSpecMC[0][0]->Draw("same");
  
  TString histName9 = Form("jetSpectra_icent%d_ipt%d",icent,ipt);
  TH1D* jSpecRatio = (TH1D*)jSpecDATA[0][0]->Clone(Form("DATA_over_MC_%s",histName9.Data()));
  jSpecRatio->Divide(jSpecMC[0][0]);
  return jSpecRatio;
  
}




void getJetSpectraWeightSub1(TH1D* jSpec[4][4], int cutOpt, int icent, int ipt, int sampleType) {

  CutAndBinCollection cab;
  cab.setCuts(cutOpt);

  int nCentBin = cab.getNCentBin();
  vector<double> centBin_std = cab.getCentBin();
  int lowerCent = centBin_std[icent-1];
  int upperCent = centBin_std[icent]-1;
  TCut centCut = Form("cBin >= %d && cBin<= %d",lowerCent,upperCent);



  vector<double> ptBinVector = cab.getPtBin();
  float ptPhoLow = ptBinVector[ipt-1];
  float ptPhoHigh= ptBinVector[ipt];




  multiTreeUtil* photon1 = new multiTreeUtil();
  setupMTU(photon1,sampleType);
  
  TCut generalEvtCut  = centCut && "(ySkim.pcollisionEventSelection==1)"; // && (ySkim.pHBHENoiseFilter==1)";
  if ( sampleType != kHIMC )
    generalEvtCut = generalEvtCut && "ySkim.pHBHENoiseFilter==1";
  
      
  TCut ptPhoCut  = Form("photonEt>%.1f && photonEt<%.1f",ptPhoLow,ptPhoHigh);
  TCut hoePhoCot = "hovere < 0.1";
  TCut caloIso = "(cc4 + cr4 + ct4PtCut20)/0.9 < 1";
  TCut isoPhoCut = caloIso && "sigmaIetaIeta<0.010";  //todo => should be flexible
  TCut sbPhoCut  = caloIso && "(sigmaIetaIeta>0.011) && (sigmaIetaIeta<0.017)";
  TCut genIsoPhoton = "";
  TCut isoPhotonCut = ptPhoCut && hoePhoCot && isoPhoCut;
  TCut nonIsoPhoCut = ptPhoCut && hoePhoCot && sbPhoCut;
  
  if ( (sampleType == kHIMC ) || (sampleType == kPPMC ) )
    isoPhotonCut =  isoPhotonCut && "genIso<5 && abs(genMomId)<=22 ";
  
  //////////////////////////////////// Jet Cuts /////////////////////////////////
  TString dphi_gj = "acos(cos(photonPhi-yJet.jtphi))";
  TCut jetEtaCut   =  Form("abs(yJet.jteta) < %f", (float)cutjetEta ) ;
  TCut awayJetCut    =  Form("(%s > %f)",dphi_gj.Data(), (float)awayRange ); 
  TCut jetGeneralCut = jetEtaCut && awayJetCut ;

  TString dphi_gmj = "acos(cos(photonPhi-ymJet.phi))";
  TCut mawayJetCut    = Form("(%s > %f)",dphi_gmj.Data(), (float)awayRange );
  TCut mjetEtaCut   =  Form("abs(ymJet.eta) < %f", (float)cutjetEta ) ;
  TCut mjetGeneralCut = mawayJetCut && mjetEtaCut;


  TString theWeightBit;
  
  if ( sampleType == kHIMC ) {  // for centrality reweighting
    theWeightBit = "reweight";
  }
  else  {
    theWeightBit  = "";
  }
  
  TString histName9 = Form("jetSpectra_icent%d_ipt%d_sampleType%d",icent,ipt,sampleType);
  jSpec[0][0] = new TH1D(Form("%s_final",histName9.Data()),";p_{T} (GeV)",nPtJetBin,ptJetBin);
  jSpec[kIsoPho][kRawJet] = (TH1D*)jSpec[0][0]->Clone(Form("%s_kIsoPho_kRawJet",histName9.Data()));
  jSpec[kDecay][kRawJet] = (TH1D*)jSpec[0][0]->Clone( Form("%s_kDecay_kRawJet",histName9.Data()));
  jSpec[kIsoPho][kHydJet] = (TH1D*)jSpec[0][0]->Clone(Form("%s_kIsoPho_kHydJet",histName9.Data()));
  jSpec[kDecay][kHydJet] = (TH1D*)jSpec[0][0]->Clone( Form("%s_kDecay_kHydJet",histName9.Data()));
  jSpec[kIsoPho][kFinalJet] = (TH1D*)jSpec[0][0]->Clone(Form("%s_kIsoPho_kFinalJet",histName9.Data()));
  jSpec[kDecay][kFinalJet] = (TH1D*)jSpec[0][0]->Clone( Form("%s_kDecay_kFinalJet",histName9.Data()));

  TH1D* isoPhotonEt = new TH1D("isoPhotonEt","",20,0,1000);
  TH1D* nonIsoPhoEt = (TH1D*)isoPhotonEt->Clone("nonIsoPhoEt");
  
  /// Purity 
  TString fPurityName = Form("photonPurityCollection_cutOption%d.root",cutOpt);
  TFile *fpurity = new TFile(fPurityName.Data());
  TH2D* hPurity2D = (TH2D*)fpurity->Get("hPurity2D");
  double purity(0);
  if ( sampleType == kHIMC )
    purity = 1 ;
  else if ( sampleType == kHIDATA) {
    purity = hPurity2D->GetBinContent(ipt, icent);
  }
  cout << " purity = " << purity << endl;
  ///////////////////////////


  photon1->Draw2(isoPhotonEt,  "photonEt", generalEvtCut && isoPhotonCut, theWeightBit);
  photon1->Draw2(nonIsoPhoEt, "photonEt", generalEvtCut && nonIsoPhoCut, theWeightBit);
  double nIsoPhos    =  isoPhotonEt->Integral();
  double nNonIsoPhos =  nonIsoPhoEt->Integral();
  double nPurePhos(0);
  if (nIsoPhos==0)  cout << " strange!!! n_photon = 0 " << endl;
  if ( sampleType == kHIMC) {
    nPurePhos= nIsoPhos;
    cout << " it's PbPb mc, so pure photons  = " << nPurePhos  << " out of  candidates = " << nIsoPhos << endl;
  }
  else if (sampleType == kHIDATA ) {
    nPurePhos = nIsoPhos * purity;
    cout << " it's PbPb data, so pure photons  = " << nPurePhos  << " out of  candidates = " << nIsoPhos << endl;
  }
  

  photon1->Draw2(jSpec[kIsoPho][kRawJet], "yJet.jtpt",
		  generalEvtCut && isoPhotonCut && jetGeneralCut, theWeightBit);                                       
  photon1->Draw2(jSpec[kIsoPho][kHydJet], "ymJet.pt",
		  generalEvtCut && isoPhotonCut &&  mjetGeneralCut, theWeightBit);                                     
  photon1->Draw2(jSpec[kDecay][kRawJet], "yJet.jtpt",
		  generalEvtCut && nonIsoPhoCut && jetGeneralCut, theWeightBit);                                       
  photon1->Draw2(jSpec[kDecay][kHydJet], "ymJet.pt",
		  generalEvtCut && nonIsoPhoCut &&  mjetGeneralCut, theWeightBit);                                     

  jSpec[kIsoPho][kHydJet]->Scale(1./nMixing1);
  jSpec[kDecay][kHydJet]->Scale(1./nMixing1);
  jSpec[kIsoPho][kRawJet]->Scale(1. / nIsoPhos );
  jSpec[kIsoPho][kHydJet]->Scale(1. / nIsoPhos );
  jSpec[kDecay][kRawJet]->Scale(1. / nNonIsoPhos );
  jSpec[kDecay][kHydJet]->Scale(1. / nNonIsoPhos );
  
  jSpec[kIsoPho][kFinalJet]->Add(jSpec[kIsoPho][kRawJet]);
  jSpec[kIsoPho][kFinalJet]->Add(jSpec[kIsoPho][kHydJet],-1);
  jSpec[kDecay][kFinalJet]->Add(jSpec[kDecay][kRawJet]);
  jSpec[kDecay][kFinalJet]->Add(jSpec[kDecay][kHydJet],-1);
    
 
  jSpec[0][0]->Reset();
  jSpec[0][0]->Add(jSpec[kIsoPho][kFinalJet], nIsoPhos);
  jSpec[0][0]->Add(jSpec[kDecay][kFinalJet],  -1 * nIsoPhos*(1-purity) );
  jSpec[0][0]->Scale(1./nPurePhos);
  
  //  gPad->SetLogx();
  
}

