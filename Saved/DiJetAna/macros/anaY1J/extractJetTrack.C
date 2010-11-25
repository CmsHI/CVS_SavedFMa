//------------------------------------------------------------------------------
//  Andre S. Yoon, Nov 24 2010 
//  
//  a root macro that extract information from Frank's ntuple by looping over
//  each event each tracks and save it in a seperate file with histograms for 
//  plotting
// 
//------------------------------------------------------------------------------
#if !defined(__CINT__) || defined(__MAKECINT__)

#include <TROOT.h>
#include <TStyle.h>
#include "TError.h"

#include "TFile.h"
#include "TCanvas.h"

#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TH2D.h"
#include "TH3F.h"
#include "TF1.h"

#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"

#include "TObjArray.h"

#include "TDirectory.h"
#include "TDirectoryFile.h"

#include "TChain.h"
#include "TGraph.h"

#include "TKey.h"
#include "TMath.h"
#include "TRandom.h"

using namespace std;

#endif

// define global variables, histograms, etc

int Nevt=0;
int maxSampling=0;

float trkpt[2500];
float trketa[2500];
float trkphi[2500];  

vector<int> targetEvtNum;
vector<double> ptBins;

TH1F *hdN_dPhiTrkNJet=0;
TH1F *hdN_dPhiTrkAJet=0;
TH1F *hdN_dEtaTrkNJet=0;
TH1F *hdN_dEtaTrkAJet=0;
TH1F *hdN_dRTrkNJet=0;
TH1F *hdN_dRTrkAJet=0;

TH2F *hdN_dPhiTrkTrk_dPhiTrkJet=0;
TH2F *hdN_dEtaTrkTrk_dEtaTrkJet=0;
TH2F *hdN_dRTrkTrk_dRTrkJet=0;
TH2F *hdN_dPhiTrkTrk_dRTrkJet=0;

TH2F *hdN_dEtadPhiTrkNJet=0;
TH2F *hdN_dEtadPhiTrkAJet=0;

TH1F *hdNdPt_NJet=0, *hdNdPt_AJet =0;
TH1F *hdNdPt_NJet_vbin=0, *hdNdPt_AJet_vbin=0;

TH1F *hdNdJetEt_NJet=0, *hdNdJetEt_AJet=0;
TH1F *hdNdTrkSPt_NJet=0, *hdNdTrkSPt_AJet=0;

TH1F *hdNPtEtRatio_NJet=0, *hdNPtEtRatio_AJet=0;
TH1F *hdNPtEtAsymm_NJet=0, *hdNPtEtAsymm_AJet=0;


TH1F *hdNdZ_NJet=0, *hdNdZ_AJet=0;
TH1F *hdNdNR_NJet=0, *hdNdNR_AJet=0;
TH1F *hdNdNRW_NJet=0, *hdNdNRW_AJet=0;

TH2F *hdEtadPhiJet=0, *hdEtadPhiTrk=0, *hdEtadPhiTrkW=0;
TObjArray hdEtadPhiJetArray, hdEtadPhiTrkArray, hdEtadPhiTrkWArray;
TDirectory *dJetDist=0, *dTrkDist=0, *dTrkWDist=0;

TH2F *hdEtadPhiJetTag=0, *hdEtadPhiTrkTag=0, *hdEtadPhiTrkWTag=0;
TObjArray hdEtadPhiJetTagArray, hdEtadPhiTrkTagArray, hdEtadPhiTrkWTagArray;
TDirectory *dJetTagDist=0, *dTrkTagDist=0, *dTrkWTagDist=0;

TRandom rdn;

// functions
//--------------------------------------------
void prepareHist();
void saveHistRoot(const char *cfile);
//-------------------------------------------


void extractJetTrack(){

   //TString filename = "dj_RECOPAT_18_1_rhi";
   TString filename = "dj_HCPR-GoodTrk1123_All0";
   //TString filename = "dj_HydjetQ_DJQ80_F10GSR_GoodTrk1123";
   //TString filename = "dj_HydjetQ_DJUQ80_F10GSR_GoodTrk1123";

  TFile *f = new TFile(Form("/home/sungho/sctch101/data/jettrack/%s.root",filename.Data()));

  TTree *djtree = (TTree*) f->Get("djcalo/djTree");

  bool debug = false;
  maxSampling = 10; // max number of event-by-event histogram

  int count=0;

  // variables 
  float minpt = 0.9;
  float centMin = 0, centMax = 10;
  float njet_min = 100; 
  float njeteta_max = 2.0;
  float ajet_min = 50;
  float ajeteta_max = 2.0;


  // event number of interest
  targetEvtNum.push_back(1490824);
  targetEvtNum.push_back(2084186);
  targetEvtNum.push_back(2983992);


  // prepare for output files, histograms, etc;
  prepareHist();

  // event-by-event
  Nevt = djtree->GetEntries();

  float cent = 0;
  djtree->SetBranchAddress("cent",&cent);
  
  int runNum = 0, lumNum = 0, evtNum = 0;
  djtree->SetBranchAddress("run",&runNum);
  djtree->SetBranchAddress("lumi",&lumNum);
  djtree->SetBranchAddress("evt",&evtNum);


  // tracks and jet
  int nTrk = 0; djtree->SetBranchAddress("evtnp",&nTrk);

  // each jet 
  float njeteta=0, njetphi=0, njet=0;  // near side jet
  float ajeteta=0, ajetphi=0, ajet=0;  // away side jet 

  djtree->SetBranchAddress("nljeta",&njeteta);
  djtree->SetBranchAddress("nljphi",&njetphi);
  djtree->SetBranchAddress("nljet",&njet);

  djtree->SetBranchAddress("aljeta",&ajeteta);
  djtree->SetBranchAddress("aljphi",&ajetphi);
  djtree->SetBranchAddress("aljet",&ajet);

  
  // each tracks
  djtree->SetBranchAddress("ppt",&trkpt);
  djtree->SetBranchAddress("peta",&trketa);
  djtree->SetBranchAddress("pphi",&trkphi);


  for(Long_t i=0;i<djtree->GetEntries();i++){

    djtree->GetEntry(i);
    if((i%100)==0) cout<<"counting every 100 events = "<<i<<endl;

    if(debug) cout<<"Evt: "<<evtNum<<" Near side jet Et = "<<njet<<" number of tracks = "<<nTrk<<endl;

    if(cent<centMin || cent>centMax) continue; // centrality 
    if(njet<njet_min || njet>500) continue; // near side jet et cut
    if(fabs(njeteta)>njeteta_max) continue; // near side jet eta cut
    if(ajet<ajet_min) continue;
    if(fabs(ajeteta)>ajeteta_max) continue;
    
    float dphi = nljphi - aljphi;
    if(fabs(dphi)>=(TMath::Pi())) dphi = 2.*TMath::Pi() - fabs(dphi); 
    if(dphi>(TMath::Pi()*(5/6)) continue;  // dphi cut for back-to-back jets

    for(Long_t j=0;j<2;j++){
      
      float jet = (j==0) ? njet : ajet;
      float jeta = (j==0) ? njeteta : ajeteta;
      float jphi = (j==0) ? njetphi : ajetphi;

      // randomize
      //float jeta = (j==0) ? rdn.Uniform(-2,2) : rdn.Uniform(-2,2);
      //float jphi = 0;
      //if(j==0) jphi = rdn.Uniform(0,TMath::Pi());
      //else jphi = -1.*jphi; //back to back
      
      if(hdEtadPhiJetArray.FindObject(Form("hdEtadPhiJet_%d",count)) && count<maxSampling)
	((TH2F*) hdEtadPhiJetArray.FindObject(Form("hdEtadPhiJet_%d",count)))->Fill(jeta,jphi);

      for(int z=0;z<targetEvtNum.size();z++){
	 if(runNum!=151969) continue; // harcoded
	 if(evtNum==targetEvtNum[z] && hdEtadPhiJetTagArray.FindObject(Form("hdEtadPhiJetTag_%d",evtNum))) //redundent
	    ((TH2F*) hdEtadPhiJetTagArray.FindObject(Form("hdEtadPhiJetTag_%d",evtNum)))->Fill(jeta,jphi);
      }

      // pre-loop to determine pt sum ----------------------------------------------------------
      float ptSum_N = 0.0, ptSum_A = 0.0;
      for(Long_t n=0;n<nTrk;n++){

	 if(trkpt[n]<minpt) continue; // low pt cut 

	 float deta_jt_pre = fabs(jeta-trketa[n]);
	 float dphi_jt_pre = fabs(jphi-trkphi[n]);
	 if(dphi_jt_pre>=(TMath::Pi())) dphi_jt_pre  = 2.0*TMath::Pi() - fabs(dphi_jt_pre); 
	 float dR = TMath::Sqrt(deta_jt_pre*deta_jt_pre+dphi_jt_pre*dphi_jt_pre);

	 if(fabs(trketa[n])<5.0){
	    if(j==0 && dR<0.8) ptSum_N = ptSum_N + trkpt[n];
	    if(j==1 && dR<0.8) ptSum_A = ptSum_A + trkpt[n];
	 }
      } // end of pre-loop ---------------------------------------------------------------------

      if(j==0) {
	 hdNdJetEt_NJet->Fill(jet,jet), hdNdTrkSPt_NJet->Fill(jet,ptSum_N);
	 hdNPtEtRatio_NJet->Fill(jet,ptSum_N/jet), hdNPtEtAsymm_NJet->Fill(jet,fabs(jet-ptSum_N)/(jet+ptSum_N));
      }else{
	 hdNdJetEt_AJet->Fill(jet,jet), hdNdTrkSPt_AJet->Fill(jet,ptSum_A);
	 hdNPtEtRatio_AJet->Fill(jet,ptSum_A/jet), hdNPtEtAsymm_AJet->Fill(jet,fabs(jet-ptSum_A)/(jet+ptSum_A));
      }

      // track loop 
      for(Long_t k=0;k<nTrk;k++){
	 if(trkpt[k]<minpt) continue; // low pt cut   

	float deta_jt = fabs(jeta-trketa[k]);
	float dphi_jt = fabs(jphi-trkphi[k]);
	if(fabs(dphi_jt)>=(TMath::Pi())) dphi_jt  = 2.0*TMath::Pi() - dphi_jt; // by convention dPhi < Pi
	float dr_jt = TMath::Sqrt(deta_jt*deta_jt+dphi_jt*dphi_jt);

	float deta_jt_v2 = (jeta-trketa[k]); // can be negative
	float dphi_jt_v2 = (jphi-trkphi[k]);
	

	if(fabs(trketa[k])<5.0){
	   if(j==0 && dphi_jt<(0.5*(TMath::Pi()))) 
	      hdNdPt_NJet->Fill(trkpt[k]), hdNdPt_NJet_vbin->Fill(trkpt[k]), hdNdZ_NJet->Fill(trkpt[k]/ptSum_N);
	   if(j==1 && dphi_jt<(0.5*(TMath::Pi()))) 
	      hdNdPt_AJet->Fill(trkpt[k]), hdNdPt_AJet_vbin->Fill(trkpt[k]), hdNdZ_AJet->Fill(trkpt[k]/ptSum_A);

	   if(j==0 && fabs(dr_jt)<0.8) {
	      hdNdNR_NJet->Fill(dr_jt/0.8), hdNdNRW_NJet->Fill(dr_jt/0.8,trkpt[k]);
	      //hdNPtEtRatio_NJet->Fill(jet,ptSum_N/jet), hdNPtEtAsymm_NJet->Fill(jet,fabs(jet-ptSum_N)/(jet+ptSum_N));
	   }
	   if(j==1 && fabs(dr_jt)<0.8) {
	      hdNdNR_AJet->Fill(dr_jt/0.8), hdNdNRW_AJet->Fill(dr_jt/0.8,trkpt[k]);
	      //hdNPtEtRatio_AJet->Fill(jet,ptSum_A/jet), hdNPtEtAsymm_AJet->Fill(jet,fabs(jet-ptSum_A)/(jet+ptSum_A));
	   }
	}

	//if(trkpt[k]<minpt) continue; // low pt cut
	if(debug) cout<<" trk pt = "<<trkpt[k]<<endl;
	
	if(hdEtadPhiTrkArray.FindObject(Form("hdEtadPhiTrk_%d",count)) && count<50){
	  ((TH2F*) hdEtadPhiTrkArray.FindObject(Form("hdEtadPhiTrk_%d",count)))->Fill(trketa[k],trkphi[k]);
	  ((TH2F*) hdEtadPhiTrkWArray.FindObject(Form("hdEtadPhiTrkW_%d",count)))->Fill(trketa[k],trkphi[k],trkpt[k]);  // with weight
	}

	if(j==0 && dphi_jt<(0.5*(TMath::Pi()))) hdN_dEtadPhiTrkNJet->Fill(deta_jt_v2,dphi_jt_v2);
	if(j==1 && dphi_jt<(0.5*(TMath::Pi()))) hdN_dEtadPhiTrkAJet->Fill(deta_jt_v2,dphi_jt_v2);

	for(int z=0;z<targetEvtNum.size();z++){
	   if(runNum!=151969) continue; // harcoded 
	   if(evtNum==targetEvtNum[z] && hdEtadPhiTrkTagArray.FindObject(Form("hdEtadPhiTrkTag_%d",evtNum))) {
	      ((TH2F*) hdEtadPhiTrkTagArray.FindObject(Form("hdEtadPhiTrkTag_%d",evtNum)))->Fill(trketa[k],trkphi[k]);
	      ((TH2F*) hdEtadPhiTrkWTagArray.FindObject(Form("hdEtadPhiTrkWTag_%d",evtNum)))->Fill(trketa[k],trkphi[k],trkpt[k]);  // with weight
	  }
	}

	if(j==0) 
	  hdN_dPhiTrkNJet->Fill(dphi_jt), hdN_dEtaTrkNJet->Fill(deta_jt), hdN_dRTrkNJet->Fill(dr_jt); 
	else
	  hdN_dPhiTrkAJet->Fill(dphi_jt), hdN_dEtaTrkAJet->Fill(deta_jt), hdN_dRTrkAJet->Fill(dr_jt);

	for(Long_t l=0;l<nTrk;l++){
	  if(trkpt[l]<minpt) continue;
	  if(l==k) continue;  // to remove auto-correlation

	  float deta_tt = fabs(trketa[k]-trketa[l]);
	  float dphi_tt = fabs(trkphi[k]-trkphi[l]);
	  if(fabs(dphi_tt)>=(TMath::Pi())) dphi_tt  = 2.0*TMath::Pi() - fabs(dphi_tt); // by convention dPhi < Pi
	  float dr_tt = TMath::Sqrt(deta_tt*deta_tt+dphi_tt*dphi_tt);

	  
	  hdN_dPhiTrkTrk_dPhiTrkJet->Fill(dphi_tt,dphi_jt);
	  hdN_dEtaTrkTrk_dEtaTrkJet->Fill(deta_tt,deta_jt);
	  hdN_dRTrkTrk_dRTrkJet->Fill(dr_tt,dr_jt);
	  hdN_dPhiTrkTrk_dRTrkJet->Fill(dphi_tt,dr_jt);

	}

      } // end of track loop

    } // end of jet loop

    count++;

  } // end of event loop
  
  // save in output files
  saveHistRoot(Form("./rootOutput/ANAv2_RandomJet_%s_minpT%1.1f_centFrom%1.1fto%1.1f.root",filename.Data(),minpt,centMin,centMax));

}

void saveHistRoot(const char *cfile){

  cout<<"File to be saved = "<<cfile<<endl;
  TFile *outputFile = new TFile(cfile,"recreate");   

  hdN_dPhiTrkNJet->Write(), hdN_dPhiTrkAJet->Write();
  hdN_dEtaTrkNJet->Write(), hdN_dEtaTrkAJet->Write();
  hdN_dRTrkNJet->Write(), hdN_dRTrkAJet->Write();

  hdN_dPhiTrkTrk_dPhiTrkJet->Write();
  hdN_dEtaTrkTrk_dEtaTrkJet->Write();
  hdN_dRTrkTrk_dRTrkJet->Write();
  hdN_dPhiTrkTrk_dRTrkJet->Write();

  hdN_dEtadPhiTrkNJet->Write(), hdN_dEtadPhiTrkAJet->Write();

  hdNdPt_NJet->Write(), hdNdPt_AJet->Write();
  
  // dN/dpT nomalization
  /*
  for(int i=0; i<ptBins.size(); i++){
    float dbin = hdNdPt_NJet_vbin->GetBinWidth(i+1);

    float dn_N = hdNdPt_NJet_vbin->GetBinContent(i+1);
    float dn_A = hdNdPt_AJet_vbin->GetBinContent(i+1);
    
    float edn_N = hdNdPt_NJet_vbin->GetBinError(i+1);
    float edn_A = hdNdPt_AJet_vbin->GetBinError(i+1);

    hdNdPt_NJet_vbin->SetBinContent(i+1,dn_N/dbin);
    hdNdPt_AJet_vbin->SetBinContent(i+1,dn_A/dbin);

    hdNdPt_NJet_vbin->SetBinError(i+1,edn_N/dbin);
    hdNdPt_AJet_vbin->SetBinError(i+1,edn_A/dbin);
  }
  */
  hdNdPt_NJet_vbin->Write(), hdNdPt_AJet_vbin->Write();

  hdNdZ_NJet->Write(), hdNdZ_AJet->Write();
  hdNdNR_NJet->Write(), hdNdNR_AJet->Write();
  hdNdNRW_NJet->Write(), hdNdNRW_AJet->Write();

  hdNdJetEt_NJet->Write(),  hdNdJetEt_AJet->Write();
  hdNdTrkSPt_NJet->Write(), hdNdTrkSPt_AJet->Write();

  hdNPtEtRatio_NJet->Write(), hdNPtEtRatio_AJet->Write();
  hdNPtEtAsymm_NJet->Write(), hdNPtEtAsymm_AJet->Write();

  dJetDist = outputFile->mkdir("jetDist"), dJetDist->cd();
  hdEtadPhiJetArray.Write();
  
  dTrkDist = outputFile->mkdir("trkDist"), dTrkDist->cd();
  hdEtadPhiTrkArray.Write();
  
  dTrkWDist = outputFile->mkdir("trkWDist"), dTrkWDist->cd();
  hdEtadPhiTrkWArray.Write();


  dJetTagDist = outputFile->mkdir("jetTagDist"), dJetTagDist->cd();
  hdEtadPhiJetTagArray.Write();

  dTrkTagDist = outputFile->mkdir("trkTagDist"), dTrkTagDist->cd();
  hdEtadPhiTrkTagArray.Write();

  dTrkWTagDist = outputFile->mkdir("trkWTagDist"), dTrkWTagDist->cd();
  hdEtadPhiTrkWTagArray.Write();


  
}

void prepareHist(){

  TH1::SetDefaultSumw2();
  TH2::SetDefaultSumw2();
  TH3::SetDefaultSumw2();

  // pt bins
  const double small = 1e-3;
  double ptb;

  // simple rebinning possible with a rebinning factor n = 2, 3, 4 !
  for(ptb =   0  ; ptb <   1.2-small; ptb +=  0.05) ptBins.push_back(ptb); // 24 bins
  for(ptb =   1.2; ptb <   2.4-small; ptb +=  0.1 ) ptBins.push_back(ptb); // 12 bins
  for(ptb =   2.4; ptb <   7.2-small; ptb +=  0.2 ) ptBins.push_back(ptb); // 24 bins
  for(ptb =   7.2; ptb <  13.2-small; ptb +=  0.5 ) ptBins.push_back(ptb); // 12 bins
  for(ptb =  13.2; ptb <  25.2-small; ptb +=  1.0 ) ptBins.push_back(ptb); // 12 bins
  for(ptb =  25.2; ptb <  61.2-small; ptb +=  3.0 ) ptBins.push_back(ptb); // 12 bins
  for(ptb =  61.2; ptb < 181.2-small; ptb +=  5.0 ) ptBins.push_back(ptb); // 24 bins
  ptBins.push_back(181.2);

  
  // dn/dpt in a hemisphere
  hdNdPt_NJet = new TH1F("hdNdPt_NJet","track pt;p_{T}^{trk}",200,0,200);
  hdNdPt_AJet = new TH1F("hdNdPt_AJet","track pt;p_{T}^{trk}",200,0,200);

  hdNdPt_NJet_vbin = new TH1F("hdNdPt_NJet_vbin","track pt;p_{T}^{trk}",ptBins.size()-1, &ptBins[0]);
  hdNdPt_AJet_vbin = new TH1F("hdNdPt_AJet_vbin","track pt;p_{T}^{trk}",ptBins.size()-1, &ptBins[0]);

  hdNdZ_NJet = new TH1F("hdNdZ_NJet","track z;p_{T}^{i}/#Sigma p_{T}^{i}",100,0,1.2);
  hdNdZ_AJet = new TH1F("hdNdZ_AJet","track z;p_{T}^{i}/#Sigma p_{T}^{i}",100,0,1.2);

  hdNdNR_NJet = new TH1F("hdNdNR_NJet","track dR normalized;normalized dR_{jet,trk}",100,0,1.2);
  hdNdNR_AJet = new TH1F("hdNdNR_AJet","track dR normalized;normalized dR_{jet,trk}",100,0,1.2);

  hdNdNRW_NJet = new TH1F("hdNdNRW_NJet","track dR normalized and weighted;normalized dR_{jet,trk}",100,0,1.2);
  hdNdNRW_AJet = new TH1F("hdNdNRW_AJet","track dR normalized and weighted;normalized dR_{jet,trk}",100,0,1.2);

  hdNdJetEt_NJet = new TH1F("hdNdJetEt_NJet","jet et distribution;E_{T} (GeV)",30,0,300);
  hdNdJetEt_AJet = new TH1F("hdNdJetEt_AJet","jet et distribution;E_{T} (GeV)",30,0,300);

  hdNdTrkSPt_NJet =new TH1F("hdNdTrkSPt_NJet","pT sum distribution; #Sigma p_{T} (GeV/c)",30,0,300);
  hdNdTrkSPt_AJet =new TH1F("hdNdTrkSPt_AJet","pT sum distribution; #Sigma p_{T} (GeV/c)",30,0,300);

  hdNPtEtRatio_NJet = new TH1F("hdNPtEtRatio_NJet","Ratio of Pt sum to Et",60,0,2.0);
  hdNPtEtRatio_AJet = new TH1F("hdNPtEtRatio_AJet","Ratio of Pt sum to Et",60,0,2.0);
  
  hdNPtEtAsymm_NJet = new TH1F("hdNPtEtAsymm_NJet","Asymmetry of Pt and Et",30,0,1.0);
  hdNPtEtAsymm_AJet = new TH1F("hdNPtEtAsymm_AJet","Asymmetry of Pt and Et",30,0,1.0);

  // track-track vs jet-track

  hdN_dPhiTrkTrk_dPhiTrkJet 
    = new TH2F("hdN_dPhiTrkTrk_dPhiTrkJet","dphi x dphi correlation;#Delta#phi(trk_{i},trk_{j});#Delta#phi(trk_{i},jet)", 100,0,6.4,100,0,6.4);
  hdN_dEtaTrkTrk_dEtaTrkJet 
    = new TH2F("hdN_dEtaTrkTrk_dEtaTrkJet","deta x deta correlation;#Delta#eta(trk_{i},trk_{j});#Delta#eta(trk_{i},jet)",100,0,5.0,100,0,5.0);
  hdN_dRTrkTrk_dRTrkJet 
    = new TH2F("hdN_dRTrkTrk_dRTrkJet","dr x dr correlation;#Delta R(trk_{i},trk_{j});#Delta R(trk_{i},jet)",100,0,6.4,100,0,6.4);
  hdN_dPhiTrkTrk_dRTrkJet 
    = new TH2F("hdN_dPhiTrkTrk_dRTrkJet","dphi x dr correlation;#Delta#phi(trk_{i},trk_{j});#Delta R(trk_{i},jet)",100,0,6.4,100,0,6.4);


  // track-jet
  hdN_dPhiTrkNJet 
    = new TH1F("hdN_dPhiTrkNJet","dphi track- near-side jet;#Delta#phi(trk_{i},jet_{near})",100,0,6.4);
  hdN_dPhiTrkAJet
    = new TH1F("hdN_dPhiTrkAJet","dphi track- away-side jet;#Delta#phi(trk_{i},jet_{away})",100,0,6.4);
  
  hdN_dEtaTrkNJet
    = new TH1F("hdN_dEtaTrkNJet","deta track- near-side jet;#Delta#eta(trk_{i},jet_{near})",100,0,6.4);
  hdN_dEtaTrkAJet
    = new TH1F("hdN_dEtaTrkAJet","deta track- away-side jet;#Delta#eta(trk_{i},jet_{away})",100,0,6.4);
 
  hdN_dRTrkNJet
    = new TH1F("hdN_dRTrkNJet","dr track- near-side jet;#Delta R(trk_{i},jet_{near})",100,0,6.4);
  hdN_dRTrkAJet
    = new TH1F("hdN_dRTrkAJet","dr track- away-side jet;#Delta R(trk_{i},jet_{away})",100,0,6.4);


  hdN_dEtadPhiTrkNJet 
     = new TH2F("hdN_dEtadPhiTrkNJet","deta vs dphi track-near side jet;#Delta#eta(trk_{i},jet_{near});#Delta#phi(trk_{i},jet_{near})",
		80,-4.0,4.0,32,-1.6,1.6);
  hdN_dEtadPhiTrkAJet
     = new TH2F("hdN_dEtadPhiTrkAJet","deta vs dphi track-away side jet;#Delta#eta(trk_{i},jet_{away});#Delta#phi(trk_{i},jet_{away})",
		80,-4.0,4.0,32,-1.6,1.6);

  for(int i=0;i<maxSampling;i++){
    hdEtadPhiJet = new TH2F(Form("hdEtadPhiJet_%d",i),"eta-phi;#eta;#phi",100,-2.4,2.4,100,-3.2,3.2);
    hdEtadPhiJetArray.Add(hdEtadPhiJet);

    hdEtadPhiTrk = new TH2F(Form("hdEtadPhiTrk_%d",i),"eta-phi;#eta;#phi",100,-2.4,2.4,100,-3.2,3.2);
    hdEtadPhiTrkArray.Add(hdEtadPhiTrk);

    hdEtadPhiTrkW = new TH2F(Form("hdEtadPhiTrkW_%d",i),"eta-phi pT weighted;#eta;#phi",100,-2.4,2.4,100,-3.2,3.2);
    hdEtadPhiTrkWArray.Add(hdEtadPhiTrkW);
  }
  
  for(int i=0;i<targetEvtNum.size();i++){
    hdEtadPhiJetTag = new TH2F(Form("hdEtadPhiJetTag_%d",targetEvtNum[i]),"eta-phi;#eta;#phi",100,-2.4,2.4,100,-3.2,3.2);
    hdEtadPhiJetTagArray.Add(hdEtadPhiJetTag);

    hdEtadPhiTrkTag = new TH2F(Form("hdEtadPhiTrkTag_%d",targetEvtNum[i]),"eta-phi;#eta;#phi",100,-2.4,2.4,100,-3.2,3.2);
    hdEtadPhiTrkTagArray.Add(hdEtadPhiTrkTag);

    hdEtadPhiTrkWTag = new TH2F(Form("hdEtadPhiTrkWTag_%d",targetEvtNum[i]),"eta-phi pT weighted;#eta;#phi",100,-2.4,2.4,100,-3.2,3.2);
    hdEtadPhiTrkWTagArray.Add(hdEtadPhiTrkWTag);
  }

  
}

