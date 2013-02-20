#include "CutAndBinCollection2011.h"
#include <TRandom3.h>
#include <time.h>

// Global Variables --------------------------------------------------------------------
const int kLjet = 1;
const int kSljet = 2;
const int kRAW = 1;
const int kBKG =2;
const int kSIG =3;

const int nptrange = 1;
float ptranges[nptrange+1] = {1,300};

int weightMode = 0; // 0=no weight, 1=trakpt, 2=trakppt/jetpt, 20=trackpt/jetrawpt
bool intPt=true; // whether we want cumulative pt ranges in track, true for fragmenation function

TString tag="";

// Helper Functions --------------------------------------------------------------------
void drawInclJetFragSingle( TH1D* htrkPt[3][5]=0,
			  int dataset = kHIDATA,
			  int icent = 1,
			  int irj = 1,
			  int fragMode = 1,   // 1 = pt, 2 = p			 
			  float trackPtMin=1,
			  float trackPtMax=200,
			  int doClosure=0,
			  bool usingPara=true,
			  int rewtJet=0,
        int ppsm=0
			  );

void drawInclJetFragSingleSet(int fragMode = 2, int dataset = kHIDATA, float trackPtMin=1, float trackPtMax=200, int doClosure=0, bool usingPara =true, int rewtJet=0, int ppsm=0) ;

// Main Function --------------------------------------------------------------------
void drawInclJetFrag_refjetsel() {
  /////////////////////////////////////////////////////////////////
  // This is the entry function that runs the full analysis
  //   * It decides what to run.
  /////////////////////////////////////////////////////////////////
  TH1::SetDefaultSumw2();

  bool doHIMC = 1;
  bool doHIDATA = 0;
  bool doPPDATA = 0;
  bool doPPMC = 1;
  bool usingPara = false;
  
  // doClosure  //////
  // 0 : default
  // -1 : only used for pp data : no weighting
  // 100 : eta refelction
  for (int ip=0; ip<nptrange; ++ip) {
    int begbin=ip;
    if (intPt) begbin=0;
    float trackPtMin = ptranges[begbin];
    float trackPtMax = ptranges[ip+1];
    for (int fragMode = 1; fragMode<=2 ; fragMode++) {
      if ( doPPMC ) {
        vector<pair<int,int> > wt_sm;
        wt_sm.push_back(pair<int,int>(0,0));
        wt_sm.push_back(pair<int,int>(0,2));
        for (int iset=0; iset<wt_sm.size(); ++iset) {
          drawInclJetFragSingleSet(fragMode, kPPMC, trackPtMin,trackPtMax, 100, usingPara, wt_sm[iset].first,wt_sm[iset].second);
          drawInclJetFragSingleSet(fragMode, kPPMC, trackPtMin,trackPtMax, 101, usingPara, wt_sm[iset].first,wt_sm[iset].second);
          drawInclJetFragSingleSet(fragMode, kPPMC, trackPtMin,trackPtMax, 102, usingPara, wt_sm[iset].first,wt_sm[iset].second);
        }
      }
      if ( doHIMC ) {
        vector<pair<int,int> > wt_sm;
        wt_sm.push_back(pair<int,int>(0,0));
        for (int iset=0; iset<wt_sm.size(); ++iset) {
          drawInclJetFragSingleSet(fragMode, kHIMC, trackPtMin,trackPtMax, 100, usingPara, wt_sm[iset].first,wt_sm[iset].second);
          drawInclJetFragSingleSet(fragMode, kHIMC, trackPtMin,trackPtMax, 101, usingPara, wt_sm[iset].first,wt_sm[iset].second);
          drawInclJetFragSingleSet(fragMode, kHIMC, trackPtMin,trackPtMax, 102, usingPara, wt_sm[iset].first,wt_sm[iset].second);
        }
      }
      if ( doPPDATA ) {
        vector<pair<int,int> > wt_sm;
        wt_sm.push_back(pair<int,int>(0,0));
        wt_sm.push_back(pair<int,int>(0,2));
        for (int iset=0; iset<wt_sm.size(); ++iset) {
          drawInclJetFragSingleSet(fragMode, kPPDATA, trackPtMin,trackPtMax, 100, usingPara, wt_sm[iset].first,wt_sm[iset].second);  // no smear
        }
      }
      if ( doHIDATA) {
        vector<pair<int,int> > wt_sm;
        wt_sm.push_back(pair<int,int>(0,0));
        for (int iset=0; iset<wt_sm.size(); ++iset) {
          drawInclJetFragSingleSet(fragMode, kHIDATA, trackPtMin,trackPtMax, 100, usingPara, wt_sm[iset].first,wt_sm[iset].second);
        }
      }
    }
  }
}

// Helper Functions --------------------------------------------------------------------
void drawInclJetFragSingleSet(int fragMode, int dataset, float trackPtMin, float trackPtMax, int doClosure, bool usingPara,int rewtJet, int ppsm) {
  /////////////////////////////////////////////////////////////////
  // This is the function that sets up analysis for each
  // dataset, and booking the needed histograms
  /////////////////////////////////////////////////////////////////  
  TString datasetName; 
  if (dataset == kHIDATA)       datasetName = "hidata";
  else if (dataset == kHIMC)    datasetName = "himc";
  else if (dataset == kPPDATA)  datasetName = "ppdata";
  else if (dataset == kPPMC)    datasetName = "ppmc";
  TString attPara = ( (usingPara == true) ? "Para" : "" ) ; // Do we want to use parametrization (Krisztian) version of tracking correction?
  
  /////////////////////////////////////////////////////////////////
  // Here The main result histograms are booked.
  /////////////////////////////////////////////////////////////////
  TH1D* htrkFF[5][3][5];
  // const int nPtBin = 22;
  // double ptBin[nPtBin+1] = { 0,1,1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,4,5,6,8,10,12,14,16,20,25,50};
  const int nPtBin = 27;
  double ptBin[nPtBin+1] = {0, 0.5, 1, 1.203915, 1.449412, 1.74497, 2.100796, 2.529181, 3.04492, 3.665826, 4.413344, 5.313293, 6.396755, 7.701152, 9.271536, 11.16214, 13.43828, 16.17855, 19.47761, 23.44939, 28.23108, 33.98783, 40.91848, 49.26238, 59.30774, 71.40151, 85.96137, 103.4902}; 


  const int nZBin = 21;
  double zBin[nZBin+1] = { 0,0.003,0.007,0.012,0.016,0.02,0.03,0.04, 0.06, 0.1, 0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.6,0.7,0.8,1};
  
  for (int icent = 1; icent<=4; icent++) { // centrality bins in the analysis
    for (int irj = 999; irj<=999 ; irj++) { // asymmetry bins in the analysis, we don't bin here for QM12 analysis
      int index = icent;
      for (int j=0; j<1; ++j) {
        ///////////////////////////////////////  define what to draw!! ///////////////////////
        TString suffix = Form("%s_icent%d_irj%d_fragMode%d_closure%d%s%s_rewt%d_ppsm%d",datasetName.Data(),icent,irj,fragMode,doClosure,attPara.Data(),tag.Data(),rewtJet,ppsm);
        suffix += Form("_wtmode%d_pt%dto%d",weightMode,(int)trackPtMin,(int)trackPtMax);
        if ( fragMode   == 1 ) // Track pt analysis
          htrkFF[index][j][kRAW]= new TH1D(Form("hpt_jet_rawTrk_%s",suffix.Data()),";track p_{T} (GeV/c);dN/dp_{T} (GeV/c)^{-1}",nPtBin,ptBin);
        else if ( fragMode==2) // Xi analysis
          htrkFF[index][j][kRAW]= new TH1D(Form("hpt_jet_rawTrk_%s",suffix.Data()),";#xi = ln(1/z);dN/d#xi",14,0,7);
        else if ( fragMode==3) // z analysis
          htrkFF[index][j][kRAW]= new TH1D(Form("hpt_jet_rawTrk_%s",suffix.Data()),";z;dN/d#xi;dN/dz",nZBin,zBin);
        else if ( fragMode==10) // rocket analysis
          htrkFF[index][j][kRAW]= new TH1D(Form("hpt_jet_rawTrk_%s",suffix.Data()),";r;#Sigma p_{T} (GeV/c);",6,0,0.3);
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////      

        // Needed for background subtraction
        htrkFF[index][j][kBKG]  = (TH1D*)htrkFF[index][j][kRAW]->Clone(Form( "hpt_jet_mbTrk_%s",suffix.Data()));
        htrkFF[index][j][kSIG] = (TH1D*)htrkFF[index][j][kRAW]->Clone(Form(  "hpt_jet_sigTrk_%s",suffix.Data()));

        // All histograms booked, now we do the real work
        drawInclJetFragSingle(htrkFF[index],dataset, icent, irj, fragMode,trackPtMin,trackPtMax,doClosure,usingPara,rewtJet,ppsm);
      }  
    }
  }
  
  
}

void drawInclJetFragSingle( TH1D* htrkPt[3][5],
			  int dataset,
			  int icent,
			  int irj,
			  int fragMode,
			  float trackPtMin,
			  float trackPtMax,
			  int doClosure,
			  bool usingPara,
        int rewtJet,
        int ppsm
        ) {
  /////////////////////////////////////////////////////////////////
  // This function is given the booked histograms
  // and does the real work of filling the histograms
  // for each dataset and each analysis setup
  /////////////////////////////////////////////////////////////////
  TString datasetName;
  if (dataset == kHIDATA)       datasetName = "hidata";
  else if (dataset == kHIMC)    datasetName = "himc";
  else if (dataset == kPPDATA)  datasetName = "ppdata";
  else if (dataset == kPPMC)  datasetName = "ppmc";
  TString attPara = ( (usingPara == true) ? "Para" : "" ) ;
  
  TString suffix = Form("%s_icent%d_irj%d_fragMode%d_closure%d_rewt%d_ppsm%d_%s",datasetName.Data(),icent,irj,fragMode,doClosure,rewtJet,ppsm,tag.Data());
  cout << endl << endl << " ** Start to analize: " << suffix << endl;
  
  TString clsText = "evtMixing";
  TString clsText_ = "event mixing method";
  if ( doClosure >= 100 ) {
    clsText = "etaRefl";
    clsText_ = "#eta reflection method";
  }

  ///////////////////////////////////////////////////////
  // Book histograms for Intermediate checks
  ///////////////////////////////////////////////////////
  TH1D* hjetPt = new TH1D(Form("hjetPt_%s",suffix.Data()),";Jet p_{T} (GeV/c);Entries;",50,0,500);

  ///////////////////////////////////////////////////////
  // Setup cuts
  ///////////////////////////////////////////////////////
  float finalEtaCut = 2.0;
  float finalJetPtMin = 100;
  float finalJetPtMax = 300;
  // TCut jetSelCut = Form("finalJetPt>=%.0f&&finalJetPt<%.0f",finalJetPtMin,finalJetPtMax);
  TCut jetSelCut = Form("jetPtGM>=%.0f&&jetPtGM<%.0f",finalJetPtMin,finalJetPtMax);
  // TCut jetSelCut = Form("finalJetPt>=%.0f&&finalJetPt<%.0f&&jetPtGM>100",finalJetPtMin,finalJetPtMax);
  // jetSelCut=jetSelCut&&"abs(vz)<4";
  
  TCut centCut = "1==1";
  if ( (icent >= 1) && (icent<=4) )
    centCut = Form("cBin>=%d && cBin<%d", (int)centBin1[icent-1], (int)centBin1[icent]);

  ////////////////////////////////////////////////////////  
  // Jet Selection
  ////////////////////////////////////////////////////////  
  // inclusive /////////
  TString jet1Cut = Form("abs(jetEta)<%.2f",finalEtaCut);
  // if (doClosure>=100&&doClosure!=101) {
  if (doClosure>=100) {
    jet1Cut = Form("%s && abs(jetEta)>0.3",jet1Cut.Data());
  }

  ////////////////////////////////////////////////////////  
  // Weights
  ////////////////////////////////////////////////////////  
  float jetDrCut = 0.3;
  TString jetRewt= "1";
  if ( rewtJet==1 ) {
    jetRewt = Form("rewtPt");
    // jetRewt = Form("(1+0.5*(rewtPt-1))");
  }
  
  ///////////////////////////////////////////////////////
  // Input trees
  ///////////////////////////////////////////////////////
  int ppsmCent=icent;
  if (ppsm==0) ppsmCent=0;
  multiTreeUtil* dj  = new multiTreeUtil();
  if ( dataset == kHIDATA) {
    // dj->addFile("../ntout/jskim_hltjet80-pt90-v20_akPu3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0_addedReweight.root","tdj", jetSelCut && centCut,1);
    // dj->addFile("../ntout/jskim_hltjet80-pt90-v20_akPu3PF_Jan26_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm3bin0_akPu3PF_gj0.root","tdj", jetSelCut && centCut,1);
    // Feb 14 ntuples
    dj->addFile("../ntout/jskim_hltjet80-pt90-v20_akPu3PF_Feb14v2_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root","tdj", jetSelCut && centCut,1);
  } else if ( dataset == kHIMC) {
    // arc paper meeting Feb 05
    // dj->addFile("../ntout/jskim_hydj80_akPu3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root", "tdj", jetSelCut && centCut && "pthat>=80&&pthat<100",9.913e-5*1.e9);
    // dj->addFile("../ntout/jskim_hydj100_akPu3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root", "tdj", jetSelCut && centCut && "pthat>=100&&pthat<9999",3.069e-5*1.e9);
    // Feb 14 ntuples
    dj->addFile("../ntout/jskim_hydj80_akPu3PF_Feb14v2_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root", "tdj", jetSelCut && centCut && "pthat>=80&&pthat<100",9.913e-5*1.e9);
    dj->addFile("../ntout/jskim_hydj100_akPu3PF_Feb14v2_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root", "tdj", jetSelCut && centCut && "pthat>=100&&pthat<170",3.069e-5*1.e9);
    dj->addFile("../ntout/jskim_hydj170_akPu3PF_Feb14v2_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root", "tdj", jetSelCut && centCut && "pthat>=170&&pthat<9999",1.470e-6*1.e9);
  } else if ( dataset == kPPDATA) {
      // archive
    // dj->addFile(Form("../ntout/jskim_pp-full_ak3PF_Dec5newsmgt60steps_jetPt_60_jetEtaCut_2.00_noPbin_sm2bin%d_ak3PF_gj0.root",icent),
      // arc paper meeting Feb 05
    // dj->addFile(Form("../ntout/jskim_pp-full_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm2bin%d_ak3PF_gj0_addedReweight.root",icent),
    // Feb 14 ntuples
    // dj->addFile(Form("../ntout/jskim_pp-full_ak3PF_Feb14v2_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm2bin%d_ak3PF_gj0.root",icent),
    dj->addFile(Form("../ntout/jskim_pp-full_ak3PF_Feb14v2_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm%dbin%d_ak3PF_gj0.root",ppsm,ppsmCent),
    "tdj", jetSelCut, 1); // no centrality cut
  } else if ( dataset == kPPMC) {
    // dj->addFile(Form("../ntout/jskim_dj80_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root"),"tdj", jetSelCut && "pthat>=80&&pthat<9999", 9.913e-5*1.e9); // no centrality cut
    // dj->addFile(Form("../ntout/jskim_dj80_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm2bin%d_ak3PF_gj0.root",icent),"tdj", jetSelCut, 1); // no centrality cut
    // dj->addFile(Form("../ntout/jskim_dj80_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm2bin%d_ak3PF_gj0_addedReweight.root",icent),"tdj", jetSelCut, 1); // no centrality cut
    // arc paper meeting Feb 05
    // dj->addFile(Form("../ntout/jskim_dj80_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm2bin%d_ak3PF_gj0.root",icent),"tdj", jetSelCut && "pthat>=80&&pthat<120", 9.913e-5*1.e9); // no centrality cut
    // dj->addFile(Form("../ntout/jskim_dj120_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm2bin%d_ak3PF_gj0.root",icent),"tdj", jetSelCut && "pthat>=120&&pthat<9999", 1.128e-5*1.e9); // no centrality cut
    // Feb 14 ntuples
    dj->addFile(Form("../ntout/jskim_dj80_ak3PF_Feb14v2_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm%dbin%d_ak3PF_gj0.root",ppsm,ppsmCent), "tdj", jetSelCut && "pthat>=80&&pthat<120",9.913e-5*1.e9); // no centrality cut!
    dj->addFile(Form("../ntout/jskim_dj120_ak3PF_Feb14v2_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm%dbin%d_ak3PF_gj0.root",ppsm,ppsmCent), "tdj", jetSelCut && "pthat>=120&&pthat<170",1.128e-5*1.e9);
    dj->addFile(Form("../ntout/jskim_dj170_ak3PF_Feb14v2_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm%dbin%d_ak3PF_gj0.root",ppsm,ppsmCent), "tdj", jetSelCut && "pthat>=170&&pthat<9999",1.470e-6*1.e9);
  }
  // pthat scaling normalization
  if ( dataset == kHIMC || dataset == kPPMC) dj->NormalizeScales(2,"","hEvtCentNoSkim");
  
  // Friend Trees
  dj->AddFriend("yTrk=yongsunTrack");
//   dj->AddFriend("mTrk");
  if ( (doClosure > 0) && ( doClosure != 100 ) && doClosure!=200 && doClosure!=210 && doClosure!=240 && doClosure!=250) {
    dj->AddFriend("genPar");
//     dj->AddFriend("mGenPar");
  }
  dj->SetAlias("finalJetPt","jetPt");
  // dj->SetAlias("finalJetPt","(jetPt*(0.935+0.065*cBin/40.))"); // systematic check for uncertainty in jes
  cout << "finalJetPt = " << dj->trees_[0]->GetAlias("finalJetPt") << endl;
  
  cout << " ================================================= " << endl;
  cout << " working on dataset, icent, irj = " << datasetName << ", "<<icent<<", "<<irj<<endl;
  cout << " doClosure: " << doClosure << " rewtJet: " << rewtJet << endl;
  cout << " ================================================= " << endl;

  ////////////////////////////////////////////////////////  
  // Draw From Tree
  ////////////////////////////////////////////////////////  
  dj->Draw(hjetPt, "finalJetPt" ,jet1Cut.Data() ,jetRewt);
  
  double NoE1 = hjetPt->Integral();
  
  ////////////////////////////////////////////////////////  
  // Track Selection
  ////////////////////////////////////////////////////////  
  TString trkCutJet1 = Form("yTrk.jetMatch==1 && yTrk.jetDr<%.2f && yTrk.pt>%.2f && yTrk.pt<%.2f", jetDrCut,trackPtMin,trackPtMax);
  TString bkgTrkCutJet1;
  if (doClosure==0) {
    bkgTrkCutJet1 = Form("mTrk.jetMatch==1 && mTrk.jetDr<%.2f && mTrk.pt>%.2f && mTrk.pt<%.2f", jetDrCut,trackPtMin,trackPtMax);
  } else if (doClosure==100) {
    bkgTrkCutJet1 = Form("yTrk.jetMatch==-1 && yTrk.jetDr<%.2f && yTrk.pt>%.2f && yTrk.pt<%.2f", jetDrCut,trackPtMin,trackPtMax);
  } else if (doClosure==1 || doClosure==101) {
    trkCutJet1    = Form("(abs(genPar.chg)>0) && (genPar.jetMatch==1) && (genPar.jetDr<%.2f) && (genPar.pt>%.2f && genPar.pt<%.2f)", jetDrCut,trackPtMin,trackPtMax);
    bkgTrkCutJet1 = Form("(genPar.sube!=0) && (abs(genPar.chg)>0) && (genPar.jetMatch==1) && (genPar.jetDr<%.2f) && (genPar.pt>%.2f && genPar.pt<%.2f)", jetDrCut,trackPtMin,trackPtMax);
  } else if (doClosure==102) {
    trkCutJet1    = Form("(abs(genPar.chg)>0) && (genPar.jetMatch==1) && (genPar.jetDr<%.2f) && (genPar.pt>%.2f && genPar.pt<%.2f)", jetDrCut,trackPtMin,trackPtMax);
    bkgTrkCutJet1 = Form("(abs(genPar.chg)>0) && (genPar.jetMatch==-1) && (genPar.jetDr<%.2f) && (genPar.pt>%.2f && genPar.pt<%.2f)", jetDrCut,trackPtMin,trackPtMax);
  } else if (doClosure ==2) {
    trkCutJet1    = Form("(abs(genPar.chg)>0) && (genPar.jetMatch==1) && (genPar.jetDr<%.2f) && (genPar.pt>%.2f && genPar.pt<%.2f)", jetDrCut,trackPtMin,trackPtMax);
    bkgTrkCutJet1 = Form("(abs(mGenPar.chg)>0) && (mGenPar.jetMatch==1) && (mGenPar.jetDr<%.2f) && (mGenPar.pt>%.2f && mGenPar.pt<%.2f)", jetDrCut,trackPtMin,trackPtMax);
  }

  ////////////////////////////////////////////////////////  
  // Track Weights
  ////////////////////////////////////////////////////////  
  TString trackWeight[3];
  TString matchedTrackWeight[3];
  TString genpWeight[3];
  // initialize
  for (int j=0; j<3; ++j) {
    // trackWeight[j] = jetRewt;
    // matchedTrackWeight[j] = jetRewt;
    trackWeight[j] = jetRewt+"*(yTrk.trkWeight*(yTrk.trkWeight<3.5)+1.47*(yTrk.trkWeight>=3.5))";
    matchedTrackWeight[j] = jetRewt+"*(mTrk.trkWeight*(mTrk.trkWeight<3.5)+1.47*(mTrk.trkWeight>=3.5))";
    genpWeight[j] = jetRewt;
    if (weightMode==1) {
      trackWeight[j]+="*yTrk.pt*";
      matchedTrackWeight[j]+="*mTrk.pt";
      genpWeight[j] += "*genPar.pt";
    } else if (weightMode==2) {
      trackWeight[j]+="*yTrk.pt/finalJetPt";
      matchedTrackWeight[j]+="*mTrk.pt/finalJetPt";
      genpWeight[j] += "*genPar.pt/finalJetPt";
    }
  }

  for (int j=0; j<1; ++j) {
    if ( fragMode   == 1 ) {
      if (doClosure == 0) {
        dj->Draw(htrkPt[j][kRAW],"yTrk.pt",    Form("%s && %s", jet1Cut.Data(), trkCutJet1.Data()), trackWeight[j] );
        dj->Draw(htrkPt[j][kBKG],"mTrk.pt",    Form("%s && %s", jet1Cut.Data(), bkgTrkCutJet1.Data()), matchedTrackWeight[j]);
      }
      else if ( doClosure == 100 ) {
        dj->Draw(htrkPt[j][kRAW],"yTrk.pt",    Form("%s && %s", jet1Cut.Data(), trkCutJet1.Data()), trackWeight[j]);
        dj->Draw(htrkPt[j][kBKG], "yTrk.pt",   Form("%s && %s", jet1Cut.Data(), bkgTrkCutJet1.Data()), trackWeight[j]);
      } 
      else if ( doClosure == 1 || doClosure == 101 || doClosure == 102) {
        dj->Draw(htrkPt[j][kRAW], "genPar.pt", Form("%s && %s", jet1Cut.Data(), trkCutJet1.Data()), genpWeight[j]);
        dj->Draw(htrkPt[j][kBKG], "genPar.pt", Form("%s && %s", jet1Cut.Data(), bkgTrkCutJet1.Data()), genpWeight[j]);
      }
//       else if ( doClosure == 2 ) {
//         dj->Draw(htrkPt[j][kRAW], "genPar.pt", Form("%s && %s", jet1Cut.Data(), trkCutJet1.Data()), genpWeight[j]);
//         dj->Draw(htrkPt[j][kBKG], "mGenPar.pt",Form("%s && %s", jet1Cut.Data(), bkgTrkCutJet1.Data()), genpWeight[j]);
//       }
    } else if ( fragMode   == 2 ) {
      if (doClosure == 0) {
        dj->Draw(htrkPt[j][kRAW],"-log(yTrk.pt * cos(yTrk.jetDr)  *(cosh(yTrk.eta)/cosh(jetEta)) /finalJetPt )",
          Form("%s && %s", jet1Cut.Data(), trkCutJet1.Data()), trackWeight[j]);
        dj->Draw(htrkPt[j][kBKG],"-log(mTrk.pt * cos(mTrk.jetDr) * (cosh(mTrk.eta)/cosh(jetEta)) /finalJetPt )",
          Form("%s && %s", jet1Cut.Data(), bkgTrkCutJet1.Data()), matchedTrackWeight[j]);
      }
      else if ( doClosure == 100 ) {
        dj->Draw(htrkPt[j][kRAW],"-log(yTrk.pt * cos(yTrk.jetDr) *(cosh(yTrk.eta)/cosh(jetEta)) /finalJetPt)",
          Form("%s && %s", jet1Cut.Data(), trkCutJet1.Data()), trackWeight[j]);
        dj->Draw(htrkPt[j][kBKG], "-log(yTrk.pt * cos(yTrk.jetDr) * (cosh(yTrk.eta)/cosh(jetEta)) /finalJetPt)",
          Form("%s && %s", jet1Cut.Data(), bkgTrkCutJet1.Data()), trackWeight[j]);
      }
      else if ( doClosure == 1|| doClosure == 101 || doClosure == 102) {
        dj->Draw(htrkPt[j][kRAW], "-log( genPar.pt * cos(genPar.jetDr)  *(cosh(genPar.eta)/cosh(jetEta)) /finalJetPt)", Form("%s && %s",
          jet1Cut.Data(), trkCutJet1.Data()), genpWeight[j]);
        dj->Draw(htrkPt[j][kBKG], "-log( genPar.pt * cos(genPar.jetDr)  *(cosh(genPar.eta)/cosh(jetEta))  /finalJetPt)", Form("%s && %s",
          jet1Cut.Data(), bkgTrkCutJet1.Data()), genpWeight[j]);
      }
//       else if ( doClosure == 2 ) {
//         dj->Draw(htrkPt[j][kRAW], "-log( genPar.pt * cos(genPar.jetDr)  *(cosh(genPar.eta)/cosh(jetEta)) /finalJetPt)", Form("%s && %s",
//           jet1Cut.Data(), trkCutJet1.Data()), genpWeight[j]);
//         dj->Draw(htrkPt[j][kBKG], "-log(mGenPar.pt * cos(mGenPar.jetDr)  *(cosh(mGenPar.eta)/cosh(jetEta)) /finalJetPt)", Form("%s && %s",
//           jet1Cut.Data(), bkgTrkCutJet1.Data()), genpWeight[j]);
//       }
    }
    // Rockets!
//     else if ( fragMode   == 10 ) {
//       if (doClosure <= 0) {
//         dj->Draw(htrkPt[j][kRAW],"yTrk.jetDr",    Form("%s && %s", jet1Cut.Data(), trkCutJet1.Data()), trackWeight[j]);
//         dj->Draw(htrkPt[j][kBKG],"mTrk.jetDr",    Form("%s && %s", jet1Cut.Data(), bkgTrkCutJet1.Data()), matchedTrackWeight[j]);
//       }
//       else if ( doClosure == 100 ) {
//         dj->Draw(htrkPt[j][kRAW],"yTrk.jetDr",    Form("%s && %s", jet1Cut.Data(), trkCutJet1.Data()), trackWeight[j]);
//         dj->Draw(htrkPt[j][kBKG], "yTrk.jetDr",   Form("%s && %s", jet1Cut.Data(), bkgTrkCutJet1.Data()), trackWeight[j]);
//       } 
//       else if ( doClosure == 1 || doClosure == 101 || doClosure == 102) {
//         dj->Draw(htrkPt[j][kRAW], "genPar.jetDr", Form("%s && %s", jet1Cut.Data(), trkCutJet1.Data()), genpWeight[j]);
//         dj->Draw(htrkPt[j][kBKG], "genPar.jetDr", Form("%s && %s", jet1Cut.Data(), bkgTrkCutJet1.Data()), genpWeight[j]);
//       }
//       else if ( doClosure == 2 ) {
//         dj->Draw(htrkPt[j][kRAW], "genPar.jetDr", Form("%s && %s", jet1Cut.Data(), trkCutJet1.Data()), genpWeight[j]);
//         dj->Draw(htrkPt[j][kBKG], "mGenPar.jetDr",Form("%s && %s", jet1Cut.Data(), bkgTrkCutJet1.Data()), genpWeight[j]);
//       }  
//     }

    // Normalize Histograms
    if (fragMode<10) {
       TH1ScaleByWidth(htrkPt[j][kRAW]);
       TH1ScaleByWidth(htrkPt[j][kBKG]);
    }
    // Signal
    htrkPt[j][kRAW]->Scale(1./NoE1);
    // Background
    if ( (doClosure == 1)  || (doClosure>99) ) {
      htrkPt[j][kBKG]->Scale(1./(NoE1));
    }
    else {
      htrkPt[j][kBKG]->Scale(1./(NoE1*nMixing1));
    }
    htrkPt[j][kSIG]->Add(htrkPt[j][kRAW]);
    htrkPt[j][kSIG]->Add(htrkPt[j][kBKG],-1);
    handsomeTH1(htrkPt[j][kRAW],1,1,24);
    handsomeTH1(htrkPt[j][kBKG],1);
    handsomeTH1(htrkPt[j][kSIG],1,1,20);
  }

  // Inspect
  TCanvas* c1 = new TCanvas(Form("c1_icent%d_irj%d",icent,irj),"",900,450);
  int iJetInsp=0;
  c1->Divide(2,1);
  c1->cd(1);
  if (fragMode<10) {
    gPad->SetLogy();
    htrkPt[iJetInsp][kRAW]->SetAxisRange(1e-5,10000,"Y");
  } else {
    gPad->SetLogy();
    if (weightMode==0) htrkPt[iJetInsp][kRAW]->SetAxisRange(1e-4,100,"Y");
    if (weightMode==1) htrkPt[iJetInsp][kRAW]->SetAxisRange(1e-4,1e5,"Y");
    if (weightMode==2) htrkPt[iJetInsp][kRAW]->SetAxisRange(1e-4,10,"Y");
  }
  htrkPt[iJetInsp][kRAW]->Draw();
  htrkPt[iJetInsp][kBKG]->Draw("same hist");
  htrkPt[iJetInsp][kSIG]->Draw("same");
  TLegend* l1 = new TLegend(0.3678213,0.7702352,0.9162483,0.9296714,NULL,"brNDC");
  easyLeg(l1,clsText_.Data()) ;
  l1->AddEntry(htrkPt[iJetInsp][kRAW],"All tracks in jet cone","p");
  l1->AddEntry(htrkPt[iJetInsp][kBKG], "tracks from underlying events","l");
  l1->AddEntry(htrkPt[iJetInsp][kSIG], "After subtraction","p");
  l1->Draw();
  drawText("jet p_{T} > 100GeV/c,  |#eta|<2", 0.26,0.3,1,17);
  drawText(Form("%d jets",(int)hjetPt->GetEntries()), 0.26,0.23,1,17);
  if ( fragMode   == 1 )      gPad->SetLogx();
  
  if       ( dataset == kHIDATA) 
    drawText("PbPb DATA", 0.2,0.8,1,15);
  else  if ( dataset == kHIMC)
    drawText("MC", 0.2,0.8,1);
  else if ( dataset == kPPDATA)
    drawText("pp DATA", 0.2,0.8,1,15);
  else if ( dataset == kPPMC)
    drawText("pp MC", 0.2,0.8,1,15);
  
  if ( (icent>=1) && (icent<=4) ) {
    int lowCent = centBin1[icent-1];
    int highCent = centBin1[icent]-1;
    drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)), 0.2,0.73,1);
  }
  if ( (irj>=1) && (irj<=4) ) {
    drawText("0-30%", 0.2,0.73,1);
    drawText(Form("%.2f < pt_{2}/pt_{1} < %.2f             %.0f < p_{T}^{track} < %.0f", float(rjBin1[irj-1]), float(rjBin1[irj]),trackPtMin,trackPtMax), 0.2,0.66,1);    
  }
  if (fragMode==10) {
    drawText(Form("%.0f < p_{T}^{track} < %.0f GeV/c", trackPtMin,trackPtMax), 0.5,0.66,1);
  }
  c1->cd(2);
  cleverRange(hjetPt,10,1e-1);
  handsomeTH1(hjetPt,1);
  hjetPt->Draw();
  gPad->SetLogy();
  drawText("Jet p_{T}", 0.55,0.63,1);
  
  TString outnameTag=Form("trackPtCut%.0f_FinalJetPt%.0fto%.0feta%.2f_Feb14v2_mc80to170_refjetsel_hi_pp",ptranges[0],finalJetPtMin,finalJetPtMax,finalEtaCut);

  if ( fragMode==2) {
    c1->SaveAs(Form("plotsOfInclJetFF/inclJetFF_xi_doClosure%d_icent%d_irj%d_%s_%s%s_%s.pdf",doClosure,icent,irj,datasetName.Data(),clsText.Data(),tag.Data(),outnameTag.Data()));
  }
  else if ( fragMode==1) {
    c1->SaveAs(Form("plotsOfInclJetFF/inclJetFF_pt_doClosure%d_icent%d_irj%d_%s_%s%s_%s.pdf",doClosure,icent,irj,datasetName.Data(),clsText.Data(),tag.Data(),outnameTag.Data()));
  }
  else if ( fragMode==10) {
    c1->SaveAs(Form("plotsOfInclJetFF/inclJetFF_dr_pt%.0fto%.0f_doClosure%d_wtMode%d_icent%d_irj%d_%s_%s%s_%s.pdf",trackPtMin,trackPtMax,doClosure,weightMode,icent,irj,datasetName.Data(),clsText.Data(),tag.Data(),outnameTag.Data()));
  }

  // All Done, write output  
  TFile outf = TFile(Form("inclJetFF_output_%s.root",outnameTag.Data()),"update");
  hjetPt->Write();
  htrkPt[0][kRAW]->Write();
  htrkPt[0][kSIG]->Write();
  htrkPt[0][kBKG]->Write();
  outf.Close();

  // clean up
  delete dj;
}
