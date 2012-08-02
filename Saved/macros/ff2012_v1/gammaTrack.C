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
#include "commonUtility.h"
#include <TRandom3.h>
#include "CutAndBinCollection2011.h"
#include "corrFunctionTrk.h"

void drawDetail(TH1D* corr[4][7]=0, TString mcLabel="", TString fname="",int icent=1, int ipt=1, int cutOpt=1, int iJetPt=1);

void gammaTrkDiff(corrFunctionTrk* corr=0, int icent=1, int ipt=1, int sampleType = kHIDATA, int cutOpt=1, int iJetPt=1, int iptAssoc=1);
void gammaTrackPtCent(int sampleType = kHIDATA, int icent = 1, int ipt = 1, int cutOpt=1);

void gammaTrack(int cutOpt=602003, int icent =1, int ipt=1) {
  

  if ( icent == 614 ) { 
    cout <<"=====icent = 614, it's pp ======"  << endl;
    gammaTrackPtCent(kPPDATA, icent,ipt,cutOpt);
  }
  cout << "%%%%% working on centrality = " << icent << endl << endl;
  
  CutAndBinCollection cab;
  cab.setCuts(cutOpt);
  int nCentBin = cab.getNCentBin();
  if ( icent > nCentBin ) { 
    cout << "End of centrality bin!!" << endl; 
    return; 
  }
  gammaTrackPtCent(kHIDATA, icent,ipt,cutOpt);
  
}

void gammaTrackPtCent(int sampleType, int icent , int ipt , int cutOpt) {
  
  
  cout << " Let's see how fast if we don't close file manually " << endl <<endl << endl;
  
  CutAndBinCollection cab;
  cab.setCuts(cutOpt);
  int  nJetPtBin = cab.getNJetPtBin();
  
  TString outputFName = Form("photonTrackOutput_cutOpt%d.root",cutOpt);
  corrFunctionTrk* corrFunc;

  
  
    
  int iptAssoc = 1 ;
  for ( int iJetPt = 1; iJetPt <= nJetPtBin ; iJetPt++) {
    cout << "######## working on iJetPt = " << iJetPt << endl;
    cout << "#####################################################" << endl; 
    corrFunc = new corrFunctionTrk();
    gammaTrkDiff( corrFunc , icent, ipt, sampleType,  cutOpt,iJetPt, iptAssoc) ;
    
    TFile outf = TFile(outputFName,"update");
    for ( int ii=0; ii<4; ii++) {
      for ( int jj=0; jj<7; jj++) {
	
	if ( corrFunc->hTrkX[ii][jj] != 0) {
	  if ( cab.doJetSpec() )
	    corrFunc->awayJetSpectra[ii][jj]->Write();
	  
	  if ( cab.doFragPt() ) 
	    corrFunc->hTrkPtInFragJet[ii][jj]->Write();
	  if ( cab.doFragX() )
	    corrFunc->hTrkX[ii][jj]->Write();
	  if ( cab.doFragXi() ) 
	    corrFunc->hTrkXi[ii][jj]->Write();
	  if ( cab.doFragXij() ) 
	    corrFunc->hTrkXij[ii][jj]->Write();
	  
	  if ( cab.doIncXi05() )
	    corrFunc->hIncXi05[ii][jj]->Write();
	  if ( cab.doIncXi10() ) 
	    corrFunc->hIncXi10[ii][jj]->Write();
	  
	  if ( cab.doIncKt05() ) 
	    corrFunc->hIncKt05[ii][jj]->Write();
	  if ( cab.doIncKt10() ) 
	    corrFunc->hIncKt10[ii][jj]->Write();
	}
      }
    }
    
    cout << " closing file...... ";
    outf.Close();
    cout << " closed!" << endl;
    delete corrFunc; 
  }
  
}



void gammaTrkDiff(corrFunctionTrk* corr, int icent, int ipt, int sampleType,  int cutOpt, int iJetPt, int iptAssoc) {
  CutAndBinCollection cab;
  cab.setCuts(cutOpt);
  int lowerCent = 0;
  int upperCent = 100;
  int nCentBin = cab.getNCentBin();
  vector<double> centBin_std = cab.getCentBin();
  if ( icent != 614 ) { 
    lowerCent = centBin_std[icent-1];
    upperCent = centBin_std[icent]-1;
  }
  TCut centCut = Form("cBin >= %d && cBin<= %d",lowerCent,upperCent);
  cout <<" centrality : " << centCut.GetTitle() << endl;
  TCut evtPlaneCut = "" ;  // pBin > 70 " ;
  cout <<" event plane cut :  " << evtPlaneCut.GetTitle() << "maybe nothing"<<endl;
  
  
  ///////// General Event cut //////////////////////////////////////////////////////////////////////////////
  TCut generalEvtCut  = centCut && evtPlaneCut ;       // && (ySkim.pHBHENoiseFilter==1)"; HBHE noise cut and pCollision cut is done in forest2yskim
  //  TCut hoePhoCot = "hovere < 0.1";                 // && hovere cut is done in forest2yskim                                               
  
  ///////// Photon cut //////////////////////////////////////////////////////////////////////////////
  vector<double> ptBinVector = cab.getPtBin();
  if (ptBinVector.size() ==1  ) 
      cout << " error!!!" << endl <<" error!!!!"<< endl<<" error!!!!!"<< endl<<" error!!!!"<< endl<< " only one element in photon pt Bin" << endl;
  float ptPhoLow = ptBinVector[ipt-1];
  float ptPhoHigh= ptBinVector[ipt];
  cout <<" photon pt : " << ptPhoLow <<" ~ "<<ptPhoHigh<<endl;
  TCut ptPhoCut  = Form("photonEt>%.1f && photonEt<%.1f",ptPhoLow,ptPhoHigh);
  TCut caloIso = "sumIso < 1";
  TCut tightPhoCut = caloIso && "sigmaIetaIeta<0.010";  //todo => should be flexible
  TCut sbPhoCut  = caloIso && "(sigmaIetaIeta>0.011) && (sigmaIetaIeta<0.017)"; 
  TCut sigPhotonCut = ptPhoCut && tightPhoCut;
  TCut sbPhotonCut = ptPhoCut && sbPhoCut;
  if ( (sampleType == kHIMC ) || (sampleType == kPPMC ) )
    sigPhotonCut = sigPhotonCut && "genIso<5 && abs(genMomId)<=22";
  
  //Jet cuts  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////                        
  vector<double> jetPtBin = cab.getJetPtBin();
  if (jetPtBin.size() <2  ) 
    cout << " error!!!" << endl <<" error!!!!"<< endl<<" error!!!!!"<< endl<<" error!!!!"<< endl<< " only one or less element in jet pt Bin" << endl;
  float jetPtCutLow  = jetPtBin[iJetPt-1];
  float jetPtCutHigh = jetPtBin[iJetPt];
  cout <<" jet pt : " << jetPtCutLow<<" ~ "<<jetPtCutHigh<<endl;
  TCut jetPtCut    =  Form(" yJet.jtpt > %.1f &&  yJet.jtpt < %.1f", jetPtCutLow, jetPtCutHigh );
  TCut mjetPtCut   =  Form("ymJet.pt > %.1f && ymJet.pt < %.1f", jetPtCutLow, jetPtCutHigh );
  TCut jetEtaCut   =  Form("abs(yJet.jteta)< %f", (float)cutjetEta ) ;
  TCut mjetEtaCut  =  Form("abs(ymJet.eta) < %f", (float)cutjetEta ) ;
  
  
  //Track cuts  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
  vector<double> assoPtBin = cab.getPtBinAssoc();
  
  // confused with track and jet pt binning... clear this!!!!!!
  if ( assoPtBin.size() <2  ) 
    cout << " error!     !!" << endl <<" error!!!!"<< endl<<" error!!!!!"<< endl<<" error!!!!"<< endl<< " only zero or one element in jet pt Bin" << endl;
  
  float trkPtCutLow = assoPtBin[iptAssoc-1];
  float trkPtCutHigh= assoPtBin[iptAssoc];
  cout << " track pt : "<< trkPtCutLow<<" ~ "<<trkPtCutHigh<<endl;
    
  TCut  trkPtCut        = Form("yTrk.pt>%.2f  && yTrk.pt<%.2f ",  (float)trkPtCutLow, (float)trkPtCutHigh);
  TCut  mTrk1PtCut      = Form("mTrk1.pt>%.2f && mTrk1.pt<%.2f",  (float)trkPtCutLow, (float)trkPtCutHigh);    
  TCut  mTrk2PtCut      = Form("mTrk2.pt>%.2f && mTrk2.pt<%.2f",  (float)trkPtCutLow, (float)trkPtCutHigh);


  // eta cut
  TCut trkEtaCut       = Form("abs(yTrk.eta)<%.2f",(float)cuttrkEtaForAna);
  TCut mTrk1EtaCut     = Form("abs(mTrk1.eta)<%.2f",(float)cuttrkEtaForAna);
  TCut mTrk2EtaCut     = Form("abs(mTrk2.eta)<%.2f",(float)cuttrkEtaForAna);
  
  // injet cut has jet-track dR, jet dphi, jet eta and jet pt cut
  float coneSize     = cab.getConeSize();

  TCut trkInjetCut   =Form("yTrk.injetCR<%.2f && yTrk.injetDphi>%.2f && yTrk.injetPt>%d  &&  yTrk.injetPt<=%d  && abs(yTrk.injetEta)<%.2f", 
			   (float)coneSize, (float)awayRange,  (int)jetPtCutLow, (int)jetPtCutHigh, (float)cutjetEta );
  TCut m1xjetCut   =  Form("mTrk1.xjetCR<%.2f && mTrk1.xjetDphi>%.2f && mTrk1.xjetPt>%d  &&  mTrk1.xjetPt<=%d  && abs(mTrk1.xjetEta)<%.2f",
			   (float)coneSize, (float)awayRange,  (int)jetPtCutLow, (int)jetPtCutHigh, (float)cutjetEta );
  TCut m2injetCut   = Form("mTrk2.injetCR<%.2f &&  mTrk2.injetDphi>%.2f && mTrk2.injetPt>%d && mTrk2.injetPt<=%d && abs(mTrk2.injetEta)<%.2f",
			   (float)coneSize, (float)awayRange,  (int)jetPtCutLow, (int)jetPtCutHigh, (float)cutjetEta );
  TCut m1xmjetCut   = Form("mTrk1.xmjetCR<%.2f && mTrk1.xmjetDphi>%.2f && mTrk1.xmjetPt>%d && mTrk1.xmjetPt<=%d  && abs(mTrk1.xmjetEta)<%.2f",
			   (float)coneSize, (float)awayRange,  (int)jetPtCutLow, (int)jetPtCutHigh, (float)cutjetEta );
  
  
  // Frag. tracks
  TCut trkForFragCut  = trkEtaCut   && trkInjetCut;
  TCut m1xTrkFragCut  = mTrk1EtaCut && m1xjetCut;
  TCut m2inTrkFragCut = mTrk2EtaCut && m2injetCut;
  TCut m1xmTrkFragCut = mTrk1EtaCut && m1xmjetCut;

  // dphi cut
  TCut trkInc05Cut     = trkEtaCut   &&  "yTrk.dphi>2.641592";
  TCut m1tInc05Cut     = mTrk1EtaCut && "mTrk1.dphi>2.641592";
  TCut trkInc10Cut     = trkEtaCut   &&  "yTrk.dphi>2.141592";
  TCut m1tInc10Cut     = mTrk1EtaCut && "mTrk1.dphi>2.141592";

  ///////////////////////////////////////////////////////////////////////////////
  corr->initAll(sampleType, icent,ipt,cutOpt,iJetPt,iptAssoc);
  multiTreeUtil* photon1 = new multiTreeUtil();
  setupMTU(photon1,sampleType);
  
  //////////////////////////////////// Jet Cuts /////////////////////////////////
  TString dphi_gj = "acos(cos(photonPhi-yJet.jtphi))";
  TCut jetNotInPhoCut = Form("(%s >0.5)",dphi_gj.Data());
  TCut awayJetCut    =  Form("(%s > %f)",dphi_gj.Data(), (float)awayRange ); 
  TCut jetGeneralCut = jetNotInPhoCut && jetEtaCut && jetPtCut && awayJetCut;

  TString dphi_gmj = "acos(cos(photonPhi-ymJet.phi))";
  TCut mjetNotInPhoCut = Form("(%s >0.5)",dphi_gmj.Data());
  TCut mawayJetCut    = Form("(%s > %f)",dphi_gmj.Data(), (float)awayRange );    
  TCut mjetGeneralCut = mjetNotInPhoCut && mjetEtaCut && mjetPtCut && mawayJetCut;

  ////////////////////////////////////////////////////////////////////////////////
  
  
  TString centWeight;
  
  if ( sampleType == kHIMC ) {  // for centrality reweighting
    centWeight = "reweight";
  }
  else  {
    centWeight  = "1";
  }
  
  photon1->Draw2(corr->sigPhotonEt, "photonEt", generalEvtCut && sigPhotonCut, centWeight);
  photon1->Draw2(corr->sbPhotonEt, "photonEt", generalEvtCut && sbPhotonCut, centWeight);
  corr->setNorm();
 
  
  if ( cab.doFragPt() ) {
    photon1->Draw2(corr->hTrkPtInFragJet[kIsoPho][kRawTrk],   "yTrk.pt",
		   generalEvtCut && sigPhotonCut &&  trkForFragCut      , centWeight + " *yTrk.trkWeight");
    photon1->Draw2(corr->hTrkPtInFragJet[kIsoPho][kHydTrk],  "mTrk1.pt",
		   generalEvtCut && sigPhotonCut &&  m1xTrkFragCut     , centWeight + " *mTrk1.trkWeight");
    photon1->Draw2(corr->hTrkPtInFragJet[kIsoPho][kMBJetTrk],"mTrk2.pt",
		   generalEvtCut && sigPhotonCut &&  m2inTrkFragCut     , centWeight + " *mTrk2.trkWeight");
    photon1->Draw2(corr->hTrkPtInFragJet[kIsoPho][kMBxTrk],  "mTrk1.pt",
		   generalEvtCut && sigPhotonCut && m1xmTrkFragCut    , centWeight + " *mTrk1.trkWeight");
    
    photon1->Draw2(corr->hTrkPtInFragJet[kDecay][kRawTrk],   "yTrk.pt",
		   generalEvtCut && sbPhotonCut &&  trkForFragCut      , centWeight +  " *yTrk.trkWeight");
    photon1->Draw2(corr->hTrkPtInFragJet[kDecay][kHydTrk],  "mTrk1.pt",
		   generalEvtCut && sbPhotonCut &&  m1xTrkFragCut     , centWeight + " *mTrk1.trkWeight");
    photon1->Draw2(corr->hTrkPtInFragJet[kDecay][kMBJetTrk],"mTrk2.pt",
                  generalEvtCut && sbPhotonCut &&  m2inTrkFragCut     , centWeight + " *mTrk2.trkWeight");
    photon1->Draw2(corr->hTrkPtInFragJet[kDecay][kMBxTrk],  "mTrk1.pt",
                  generalEvtCut && sbPhotonCut && m1xmTrkFragCut    , centWeight  + " *mTrk1.trkWeight");
    TH1ScaleByWidth(corr->hTrkPtInFragJet[kIsoPho][kRawTrk]);
    TH1ScaleByWidth(corr->hTrkPtInFragJet[kIsoPho][kHydTrk]);
    TH1ScaleByWidth(corr->hTrkPtInFragJet[kIsoPho][kMBJetTrk]);
    TH1ScaleByWidth(corr->hTrkPtInFragJet[kIsoPho][kMBxTrk]);
    TH1ScaleByWidth(corr->hTrkPtInFragJet[kDecay][kRawTrk]);
    TH1ScaleByWidth(corr->hTrkPtInFragJet[kDecay][kHydTrk]);
    TH1ScaleByWidth(corr->hTrkPtInFragJet[kDecay][kMBJetTrk]);
    TH1ScaleByWidth(corr->hTrkPtInFragJet[kDecay][kMBxTrk]);
  }
  
  if ( cab.doFragX() ) {
    photon1->Draw2(corr->hTrkX[kIsoPho][kRawTrk],   "yTrk.pt/photonEt",
		   generalEvtCut && sigPhotonCut &&  trkForFragCut      , centWeight + " *yTrk.trkWeight");
    photon1->Draw2(corr->hTrkX[kIsoPho][kHydTrk],  "mTrk1.pt/photonEt",
		   generalEvtCut && sigPhotonCut &&  m1xTrkFragCut     , centWeight + " *mTrk1.trkWeight");
    photon1->Draw2(corr->hTrkX[kIsoPho][kMBJetTrk],"mTrk2.pt/photonEt",
                  generalEvtCut && sigPhotonCut &&  m2inTrkFragCut     , centWeight + " *mTrk2.trkWeight");
    photon1->Draw2(corr->hTrkX[kIsoPho][kMBxTrk],  "mTrk1.pt/photonEt",
                  generalEvtCut && sigPhotonCut && m1xmTrkFragCut    , centWeight  + " *mTrk1.trkWeight");
    
    photon1->Draw2(corr->hTrkX[kDecay][kRawTrk],   "yTrk.pt/photonEt",
                  generalEvtCut && sbPhotonCut &&  trkForFragCut      , centWeight +  " *yTrk.trkWeight");
    photon1->Draw2(corr->hTrkX[kDecay][kHydTrk],  "mTrk1.pt/photonEt",
                  generalEvtCut && sbPhotonCut &&  m1xTrkFragCut     , centWeight + " *mTrk1.trkWeight");
    photon1->Draw2(corr->hTrkX[kDecay][kMBJetTrk],"mTrk2.pt/photonEt",
                  generalEvtCut && sbPhotonCut &&  m2inTrkFragCut     , centWeight + " *mTrk2.trkWeight");
    photon1->Draw2(corr->hTrkX[kDecay][kMBxTrk],  "mTrk1.pt/photonEt",
                  generalEvtCut && sbPhotonCut && m1xmTrkFragCut    , centWeight  + " *mTrk1.trkWeight");
    TH1ScaleByWidth(corr->hTrkX[kIsoPho][kRawTrk]);
    TH1ScaleByWidth(corr->hTrkX[kIsoPho][kHydTrk]);
    TH1ScaleByWidth(corr->hTrkX[kIsoPho][kMBJetTrk]);
    TH1ScaleByWidth(corr->hTrkX[kIsoPho][kMBxTrk]);
    TH1ScaleByWidth(corr->hTrkX[kDecay][kRawTrk]);
    TH1ScaleByWidth(corr->hTrkX[kDecay][kHydTrk]);
    TH1ScaleByWidth(corr->hTrkX[kDecay][kMBJetTrk]);
    TH1ScaleByWidth(corr->hTrkX[kDecay][kMBxTrk]);
  }
  if ( cab.doFragXi() ) {
    photon1->Draw2(corr->hTrkXi[kIsoPho][kRawTrk],  "-log(yTrk.pt/photonEt)",
		   generalEvtCut && sigPhotonCut &&  trkForFragCut      , centWeight + " *yTrk.trkWeight");
    photon1->Draw2(corr->hTrkXi[kIsoPho][kHydTrk],  "-log(mTrk1.pt/photonEt)",
		   generalEvtCut && sigPhotonCut &&  m1xTrkFragCut     , centWeight + " *mTrk1.trkWeight");
    photon1->Draw2(corr->hTrkXi[kIsoPho][kMBJetTrk],"-log(mTrk2.pt/photonEt)",
                  generalEvtCut && sigPhotonCut &&  m2inTrkFragCut     , centWeight + " *mTrk2.trkWeight");
    photon1->Draw2(corr->hTrkXi[kIsoPho][kMBxTrk],  "-log(mTrk1.pt/photonEt)",
                  generalEvtCut && sigPhotonCut && m1xmTrkFragCut    , centWeight  + " *mTrk1.trkWeight");
    
    photon1->Draw2(corr->hTrkXi[kDecay][kRawTrk],  "-log(yTrk.pt/photonEt)",
                  generalEvtCut && sbPhotonCut &&  trkForFragCut      , centWeight +  " *yTrk.trkWeight");
    photon1->Draw2(corr->hTrkXi[kDecay][kHydTrk],  "-log(mTrk1.pt/photonEt)",
                  generalEvtCut && sbPhotonCut &&  m1xTrkFragCut     , centWeight + " *mTrk1.trkWeight");
    photon1->Draw2(corr->hTrkXi[kDecay][kMBJetTrk],"-log(mTrk2.pt/photonEt)",
                  generalEvtCut && sbPhotonCut &&  m2inTrkFragCut     , centWeight + " *mTrk2.trkWeight");
    photon1->Draw2(corr->hTrkXi[kDecay][kMBxTrk],  "-log(mTrk1.pt/photonEt)",
                  generalEvtCut && sbPhotonCut && m1xmTrkFragCut    , centWeight  + " *mTrk1.trkWeight");
    TH1ScaleByWidth(corr->hTrkXi[kIsoPho][kRawTrk]);
    TH1ScaleByWidth(corr->hTrkXi[kIsoPho][kHydTrk]);
    TH1ScaleByWidth(corr->hTrkXi[kIsoPho][kMBJetTrk]);
    TH1ScaleByWidth(corr->hTrkXi[kIsoPho][kMBxTrk]);
    TH1ScaleByWidth(corr->hTrkXi[kDecay][kRawTrk]);
    TH1ScaleByWidth(corr->hTrkXi[kDecay][kHydTrk]);
    TH1ScaleByWidth(corr->hTrkXi[kDecay][kMBJetTrk]);
    TH1ScaleByWidth(corr->hTrkXi[kDecay][kMBxTrk]);
  }
  if ( cab.doFragXij() ) {
    photon1->Draw2(corr->hTrkXij[kIsoPho][kRawTrk],  "-log(yTrk.pt/yTrk.injetPt)",
		   generalEvtCut && sigPhotonCut &&  trkForFragCut      , centWeight + " *yTrk.trkWeight");
    photon1->Draw2(corr->hTrkXij[kIsoPho][kHydTrk],  "-log(mTrk1.pt/mTrk1.xjetPt)",
		   generalEvtCut && sigPhotonCut &&  m1xTrkFragCut     , centWeight + " *mTrk1.trkWeight");
    photon1->Draw2(corr->hTrkXij[kIsoPho][kMBJetTrk],"-log(mTrk2.pt/mTrk2.injetPt)",
                  generalEvtCut && sigPhotonCut &&  m2inTrkFragCut     , centWeight + " *mTrk2.trkWeight");
    photon1->Draw2(corr->hTrkXij[kIsoPho][kMBxTrk],  "-log(mTrk1.pt/mTrk1.xmjetPt)",
                  generalEvtCut && sigPhotonCut && m1xmTrkFragCut    , centWeight  + " *mTrk1.trkWeight");
    
    photon1->Draw2(corr->hTrkXij[kDecay][kRawTrk],  "-log(yTrk.pt/yTrk.injetPt)",
                  generalEvtCut && sbPhotonCut &&  trkForFragCut      , centWeight +  " *yTrk.trkWeight");
    photon1->Draw2(corr->hTrkXij[kDecay][kHydTrk],  "-log(mTrk1.pt/mTrk1.xjetPt)",
                  generalEvtCut && sbPhotonCut &&  m1xTrkFragCut     , centWeight + " *mTrk1.trkWeight");
    photon1->Draw2(corr->hTrkXij[kDecay][kMBJetTrk],"-log(mTrk2.pt/mTrk2.injetPt)",
                  generalEvtCut && sbPhotonCut &&  m2inTrkFragCut     , centWeight + " *mTrk2.trkWeight");
    photon1->Draw2(corr->hTrkXij[kDecay][kMBxTrk],  "-log(mTrk1.pt/mTrk1.xmjetPt)",
                  generalEvtCut && sbPhotonCut && m1xmTrkFragCut    , centWeight  + " *mTrk1.trkWeight");
    TH1ScaleByWidth(corr->hTrkXij[kIsoPho][kRawTrk]);
    TH1ScaleByWidth(corr->hTrkXij[kIsoPho][kHydTrk]);
    TH1ScaleByWidth(corr->hTrkXij[kIsoPho][kMBJetTrk]);
    TH1ScaleByWidth(corr->hTrkXij[kIsoPho][kMBxTrk]);
    TH1ScaleByWidth(corr->hTrkXij[kDecay][kRawTrk]);
    TH1ScaleByWidth(corr->hTrkXij[kDecay][kHydTrk]);
    TH1ScaleByWidth(corr->hTrkXij[kDecay][kMBJetTrk]);
    TH1ScaleByWidth(corr->hTrkXij[kDecay][kMBxTrk]);
  }
  

  
  if ( cab.doJetSpec() ) {
    photon1->Draw2(corr->awayJetSpectra[kIsoPho][kRawTrk],  "yJet.jtpt",
		   generalEvtCut && sigPhotonCut && jetGeneralCut,   centWeight ) ;
    photon1->Draw2(corr->awayJetSpectra[kIsoPho][kHydTrk],  "ymJet.pt",
		   generalEvtCut && sigPhotonCut && mjetGeneralCut,   centWeight ) ;
    photon1->Draw2(corr->awayJetSpectra[kDecay][kRawTrk],  "yJet.jtpt",
		   generalEvtCut && sbPhotonCut  && jetGeneralCut,   centWeight ) ;
    photon1->Draw2(corr->awayJetSpectra[kDecay][kHydTrk],  "ymJet.pt",
		   generalEvtCut && sbPhotonCut  && mjetGeneralCut,   centWeight ) ;
    
  }
  
  
  
  
  if ( cab.doIncXi05() ) {
    photon1->Draw2(corr->hIncXi05[kIsoPho][kRawTrk],  "-log(yTrk.pt/photonEt)",
		   generalEvtCut && sigPhotonCut && trkInc05Cut    , centWeight + "*yTrk.trkWeight");
    photon1->Draw2(corr->hIncXi05[kIsoPho][kHydTrk],  "-log(mTrk1.pt/photonEt)",
		   generalEvtCut && sigPhotonCut && m1tInc05Cut   , centWeight +  "*mTrk1.trkWeight");
    photon1->Draw2(corr->hIncXi05[kDecay][kRawTrk],  "-log(yTrk.pt/photonEt)",
                   generalEvtCut && sbPhotonCut &&  trkInc05Cut     , centWeight + "*yTrk.trkWeight");
    photon1->Draw2(corr->hIncXi05[kDecay][kHydTrk],  "-log(mTrk1.pt/photonEt)",
		   generalEvtCut && sbPhotonCut &&  m1tInc05Cut     , centWeight + "*mTrk1.trkWeight");
    corr->hIncXi05[kIsoPho][kMBJetTrk]->Reset();   
    corr->hIncXi05[kIsoPho][kMBxTrk]->Reset();   
    corr->hIncXi05[kDecay][kMBJetTrk]->Reset();   
    corr->hIncXi05[kDecay][kMBxTrk]->Reset();   
    
    TH1ScaleByWidth(corr->hIncXi05[kIsoPho][kRawTrk]);
    TH1ScaleByWidth(corr->hIncXi05[kIsoPho][kHydTrk]);
    TH1ScaleByWidth(corr->hIncXi05[kDecay][kRawTrk]);
    TH1ScaleByWidth(corr->hIncXi05[kDecay][kHydTrk]);
  }


  if ( cab.doIncXi10() ) {
    photon1->Draw2(corr->hIncXi10[kIsoPho][kRawTrk],  "-log(yTrk.pt/photonEt)",
		   generalEvtCut && sigPhotonCut && trkInc10Cut    , centWeight + " *yTrk.trkWeight");
    photon1->Draw2(corr->hIncXi10[kIsoPho][kHydTrk],  "-log(mTrk1.pt/photonEt)",
		   generalEvtCut && sigPhotonCut && m1tInc10Cut    , centWeight + " *mTrk1.trkWeight");
    photon1->Draw2(corr->hIncXi10[kDecay][kRawTrk],  "-log(yTrk.pt/photonEt)",
                   generalEvtCut && sbPhotonCut &&  trkInc10Cut     , centWeight + " *yTrk.trkWeight");
    photon1->Draw2(corr->hIncXi10[kDecay][kHydTrk],  "-log(mTrk1.pt/photonEt)",
		   generalEvtCut && sbPhotonCut &&  m1tInc10Cut     , centWeight + " *mTrk1.trkWeight");
    corr->hIncXi10[kIsoPho][kMBJetTrk]->Reset();   
    corr->hIncXi10[kIsoPho][kMBxTrk]->Reset();   
    corr->hIncXi10[kDecay][kMBJetTrk]->Reset();   
    corr->hIncXi10[kDecay][kMBxTrk]->Reset();   
    
    TH1ScaleByWidth(corr->hIncXi10[kIsoPho][kRawTrk]);
    TH1ScaleByWidth(corr->hIncXi10[kIsoPho][kHydTrk]);
    TH1ScaleByWidth(corr->hIncXi10[kDecay][kRawTrk]);
    TH1ScaleByWidth(corr->hIncXi10[kDecay][kHydTrk]);
  }

  if ( cab.doIncKt05() ) {
    photon1->Draw2(corr->hIncKt05[kIsoPho][kRawTrk], "yTrk.pt*sin(yTrk.dphi)",
		   generalEvtCut && sigPhotonCut && trkInc05Cut   , centWeight + " *yTrk.trkWeight");
    photon1->Draw2(corr->hIncKt05[kIsoPho][kHydTrk], "mTrk1.pt*sin(mTrk1.dphi)",
		   generalEvtCut && sigPhotonCut && m1tInc05Cut    , centWeight + " *mTrk1.trkWeight");
    photon1->Draw2(corr->hIncKt05[kDecay][kRawTrk],  "yTrk.pt*sin(yTrk.dphi)",
                   generalEvtCut && sbPhotonCut &&  trkInc05Cut     , centWeight + " *yTrk.trkWeight");
    photon1->Draw2(corr->hIncKt05[kDecay][kHydTrk],  "mTrk1.pt*sin(mTrk1.dphi)",
                  generalEvtCut && sbPhotonCut &&  m1tInc05Cut     , centWeight + " *mTrk1.trkWeight");
    corr->hIncKt05[kIsoPho][kMBJetTrk]->Reset();   
    corr->hIncKt05[kIsoPho][kMBxTrk]->Reset();   
    corr->hIncKt05[kDecay][kMBJetTrk]->Reset();   
    corr->hIncKt05[kDecay][kMBxTrk]->Reset();   
    
    TH1ScaleByWidth(corr->hIncKt05[kIsoPho][kRawTrk]);
    TH1ScaleByWidth(corr->hIncKt05[kIsoPho][kHydTrk]);
    TH1ScaleByWidth(corr->hIncKt05[kDecay][kRawTrk]);
    TH1ScaleByWidth(corr->hIncKt05[kDecay][kHydTrk]);
  }
  if ( cab.doIncKt10()  ) {
    photon1->Draw2(corr->hIncKt10[kIsoPho][kRawTrk],  "yTrk.pt*sin(yTrk.dphi)",
		   generalEvtCut && sigPhotonCut && trkInc10Cut   , centWeight +   " *yTrk.trkWeight");
    photon1->Draw2(corr->hIncKt10[kIsoPho][kHydTrk],  "mTrk1.pt*sin(mTrk1.dphi)",
		   generalEvtCut && sigPhotonCut && m1tInc10Cut    , centWeight +  " *mTrk1.trkWeight");
    photon1->Draw2(corr->hIncKt10[kDecay][kRawTrk],  "yTrk.pt*sin(yTrk.dphi)",
                   generalEvtCut && sbPhotonCut &&  trkInc10Cut     , centWeight + " *yTrk.trkWeight");
    photon1->Draw2(corr->hIncKt10[kDecay][kHydTrk],  "mTrk1.pt*sin(mTrk1.dphi)",
                  generalEvtCut && sbPhotonCut &&  m1tInc10Cut     , centWeight +  " *mTrk1.trkWeight");
    corr->hIncKt10[kIsoPho][kMBJetTrk]->Reset();   
    corr->hIncKt10[kIsoPho][kMBxTrk]->Reset();   
    corr->hIncKt10[kDecay][kMBJetTrk]->Reset();   
    corr->hIncKt10[kDecay][kMBxTrk]->Reset();   
    
    TH1ScaleByWidth(corr->hIncKt10[kIsoPho][kRawTrk]);
    TH1ScaleByWidth(corr->hIncKt10[kIsoPho][kHydTrk]);
    TH1ScaleByWidth(corr->hIncKt10[kDecay][kRawTrk]);
    TH1ScaleByWidth(corr->hIncKt10[kDecay][kHydTrk]);
  }

  
  
  TString centWeightj, centWeightjMx, centWeightjMi, centWeightjMM;
  
  if ( sampleType == kHIMC ) {  // for centrality reweighting
    centWeightj  = "reweight * yTrk.trkWeight * yTrk.injetWeight";
    centWeightjMx = "reweight * ymTrk.trkWeight * ymTrk.xjetWeight";
    centWeightjMi = "reweight * ymTrk.trkWeight * ymTrk.injetWeight";
    centWeightjMM = "reweight * ymmTrk.trkWeight * ymmTrk.xjetWeight";
  }
  else if ( (sampleType==kHIDATA) || (sampleType==kPPDATA) ) {
    centWeightj  = "yTrk.trkWeight";
    centWeightjMx = "ymTrk.trkWeight";
    centWeightjMi = "ymTrk.trkWeight";
    centWeightjMM = "ymmTrk.trkWeight";
  }
  
  corr->calCorr();
  TString mcLabel = "";
   
  if ( sampleType == kHIMC ) mcLabel = "PbPb_mc";
  else if ( sampleType == kHIDATA ) mcLabel = "PbPb_data";
  else if ( sampleType == kPPMC ) mcLabel = "pp_mc";
  else if ( sampleType == kPPDATA ) mcLabel = "pp_data";
  else {
    cout << " no such sample "  << endl << endl << endl;
    return ;
  }
  
  if ( cab.doJetSpec() ) 
    drawDetail(corr->awayJetSpectra,   mcLabel, "awaySideJetPt", icent, ipt, cutOpt, iJetPt);

  if ( cab.doFragPt() )
    drawDetail(corr->hTrkPtInFragJet,   mcLabel, "FragPt", icent, ipt, cutOpt, iJetPt);
  if ( cab.doFragXi() )
    drawDetail(corr->hTrkXi, mcLabel, "xi",   icent, ipt, cutOpt, iJetPt);
  if ( cab.doFragXij() )
    drawDetail(corr->hTrkXij,    mcLabel, "xij",  icent, ipt, cutOpt, iJetPt);
  if ( cab.doIncXi05() )
    drawDetail(corr->hIncXi05,   mcLabel, "Incxi05",  icent, ipt, cutOpt, iJetPt);
  if ( cab.doIncXi10() )
    drawDetail(corr->hIncXi10,   mcLabel, "Incxi10",  icent, ipt, cutOpt, iJetPt);
  if ( cab.doIncKt05() )
    drawDetail(corr->hIncKt05,   mcLabel, "IncKt05",  icent, ipt, cutOpt, iJetPt);
  if ( cab.doIncKt10() )
    drawDetail(corr->hIncKt10,   mcLabel, "IncKt10",  icent, ipt, cutOpt, iJetPt);

}


void drawDetail(TH1D* corr[4][7], TString mcLabel, TString fname, int icent, int ipt, int cutOpt, int iJetPt) 
{
  CutAndBinCollection cab;
  cab.setCuts(cutOpt);

  vector<double> ptBinVector = cab.getPtBin();
  float ptPhoLow = ptBinVector[ipt-1];
  float ptPhoHigh= ptBinVector[ipt];

  vector<double> jetPtBin = cab.getJetPtBin();
  float jetPtCutLow  = jetPtBin[iJetPt-1];
  float jetPtCutHigh = jetPtBin[iJetPt];
  
  int lowerCent = 0;
  int upperCent = 100;
  int nCentBin = cab.getNCentBin();
  vector<double> centBin_std = cab.getCentBin();
  if ( icent != 614 ) {
    lowerCent = centBin_std[icent-1];
    upperCent = centBin_std[icent]-1;
  }
  
  
  TLegend* legIsoP =  new TLegend(0.194905,0.6240079,0.6965772,0.9030258,NULL,"brNDC");
  TLegend* legIsoP2 =  new TLegend(0.194905,0.6240079,0.6965772,0.9030258,NULL,"brNDC");
  TLegend* legDecP =  new TLegend(0.194905,0.6240079,0.6965772,0.9030258,NULL,"brNDC");

  float xLeftEdge =  corr[kIsoPho][kRawTrk]->GetBinLowEdge(1);
  float xRightEdge = corr[kIsoPho][kRawTrk]->GetBinLowEdge(corr[kIsoPho][kRawTrk]->GetNbinsX()) + corr[kIsoPho][kRawTrk]->GetBinWidth(corr[kIsoPho][kRawTrk]->GetNbinsX() ) ;
  
  float yhigh = 1.3 * getCleverRange(corr[kIsoPho][kRawTrk]);
  float ylow  = -yhigh/20.;
  
  TCanvas* c0 = new TCanvas(Form("c0_%s_%s_icent%d",mcLabel.Data(), fname.Data(),icent),"",1200,700);
  c0->Divide(3,2);
  c0->cd(1);
  corr[kIsoPho][kRawTrk]->SetAxisRange(ylow,yhigh,"Y");
  corr[kIsoPho][kRawTrk]->Draw();
  handsomeTH1(corr[kIsoPho][kHydTrk],2);
  corr[kIsoPho][kHydTrk]->Draw("same");
  jumSun( xLeftEdge,0, xRightEdge,0);
  drawText(Form("%s %.0f %% - %.0f %%",mcLabel.Data(), float((float)lowerCent*2.5), float((float)(upperCent+1)*2.5)),0.3680963,0.4568);
  easyLeg(legIsoP,"#gamma cand. - Tracks");
  legIsoP->AddEntry(corr[kIsoPho][kRawTrk],"from gamma-jets","p");
  legIsoP->AddEntry(corr[kIsoPho][kHydTrk],"from MB track","p");
  legIsoP->Draw();

  c0->cd(4);
  corr[kIsoPho][kHardTrk]->Draw();
  corr[kIsoPho][kHardTrk]->SetAxisRange(ylow,yhigh,"Y");
  jumSun( xLeftEdge,0, xRightEdge,0);
  drawText("#gamma cand.- Hard tracks",0.2680963,0.4569118);


  c0->cd(2);
  corr[kIsoPho][kMBJetTrk]->SetAxisRange(ylow,yhigh,"Y");
  corr[kIsoPho][kMBJetTrk]->Draw();
  handsomeTH1(corr[kIsoPho][kMBxTrk],2);
  corr[kIsoPho][kMBxTrk]->Draw("same");
  jumSun( xLeftEdge,0, xRightEdge,0);
  
  easyLeg(legIsoP2,"MB jets");
  legIsoP2->AddEntry(corr[kIsoPho][kMBJetTrk],"from MB jets","p");
  legIsoP2->AddEntry(corr[kIsoPho][kMBxTrk],"from MB track","p");
  legIsoP2->Draw();

  c0->cd(5);
  corr[kIsoPho][kHardMBTrk]->Draw();
  corr[kIsoPho][kHardMBTrk]->SetAxisRange(ylow,yhigh,"Y");
  jumSun( xLeftEdge,0, xRightEdge,0);
  drawText("#gamma cand.- Hard tracks",0.2680963,0.4569118);

  c0->cd(3);
  if (ptPhoHigh > 200)                                                                                                         
    drawText(Form("p_{T}^{#gamma} > %dGeV", (int)ptPhoLow), 0.15,0.85,1,18);                                                    
  else                                                                                                                        
    drawText(Form("p_{T}^{#gamma} : %d - %dGeV", (int)ptPhoLow, (int)ptPhoHigh), 0.3,0.85,1,18);                                 
  drawText("|#eta^{#gamma}| < 1.44", 0.6,0.85,1,16);                                                                             
  // drawText(Form("p_{T}^{jet}: %dGeV - %dGeV", (int)jetPtCutLow, (int)jetPtCutHigh) );
  drawText("|#eta^{jet}| < 1.6", 0.6,0.72,1,18);                  
  
  // handsomeTH1(corr[kIsoPho][kHardMBTrk],2);
  // corr[kIsoPho][kHardTrk]->Draw();
  //  corr[kIsoPho][kHardMBTrk]->DrawCopy("same");

  c0->cd(6);
  corr[kIsoPho][kFinalGoodTrk]->SetAxisRange(ylow,yhigh,"Y");
  corr[kIsoPho][kFinalGoodTrk]->Draw();
  jumSun( xLeftEdge,0, xRightEdge,0);
  
  
  
  c0->SaveAs(Form("plotsGammaTrack/%s_%s_photonEt%dto%d_jetPt%dto%dGeV_icent%d_iJetPt%d.gif",fname.Data(), mcLabel.Data(), (int)ptPhoLow, (int)ptPhoHigh, (int)jetPtCutLow, (int)jetPtCutHigh, icent, iJetPt));
  TCanvas* cNull = new TCanvas("cNull","",100,100);
}


/*
  //  corr[1][3]->Draw();
  //  corr[2][3]->Draw("same");
  
  c0->cd(6);
  corr[3][3]->SetAxisRange(ylow,yhigh,"Y");
  corr[3][3]->DrawCopy();
  drawText("Pure #gamma - Pure tracks",0.2680963,0.4569118);
  jumSun( xLeftEdge,0, xRightEdge,0);
  
  c0->cd(3);
  if (ptPhoHigh > 200)
    drawText(Form("p_{T}^{#gamma} > %dGeV", (int)ptPhoLow), 0.15,0.85,1,18);
  else
    drawText(Form("p_{T}^{#gamma} : %d - %dGeV", (int)ptPhoLow, (int)ptPhoHigh), 0.3,0.85,1,18);
  drawText("|#eta^{#gamma}| <1.44", 0.6,0.85,1,16);

  drawText(Form("p_{T}^{jet} : %dGeV - %dGeV", (int)jetPtCutLow, (int)jetPtCutHigh), 0.15,0.72,1,18);
  drawText("|#eta^{jet}| <1.6", 0.6,0.72,1,18);

 }
*/
