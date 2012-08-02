#include "CutAndBinCollection2011.h"
#include <TRandom3.h>
#include <time.h>

const int kLjet = 1;
const int kSljet = 2;
const int kRAW = 1;
const int kBKG =2;
const int kSIG =3;

void drawDijetFragSingle( TH1D* htrkPt[3][5]=0,
			  int dataset = kHIDATA,
			  int icent = 1,
			  int irj = 1,
			  int fragMode = 1,   // 1 = pt, 2 = p			 
			  float trackPtCut=4,
			  int doClosure=0,
			  bool usingPara=true
			  );

void drawDijetFragSingleSet(int fragMode = 2, int dataset = kHIDATA, float trackPtCut=4, int doClosure=0, bool usingPara =true) ;


void drawDijetFrag() {
  bool doHIMC = 1;
  bool doHIDATA = 0;
  bool doPPDATA = 0;
  float trackPtCut = 1;
  bool usingPara = false;

  // doClosure  //////
  // 0 : default
  // -1 : only used for pp data : no weighting
  // 100 : eta refelction

  for (int fragMode = 2; fragMode<=2 ; fragMode++) {
    if ( doHIMC ) {
      drawDijetFragSingleSet(fragMode, kHIMC, trackPtCut, 100, usingPara);
      drawDijetFragSingleSet(fragMode, kHIMC, trackPtCut, 101, usingPara);
      drawDijetFragSingleSet(fragMode, kHIMC, trackPtCut, 102, usingPara);
      //      drawDijetFragSingleSet(fragMode, kHIMC, trackPtCut, 0, usingPara);
      //    drawDijetFragSingleSet(fragMode, kHIMC, trackPtCut, 1, usingPara);
      //  drawDijetFragSingleSet(fragMode, kHIMC, trackPtCut, 2, usingPara);
      //      drawDijetFragSingleSet(fragMode, kHIMC, trackPtCut, 10, usingPara);  // 10 => no fake jets in MC
      
    }
    if ( doPPDATA ) {
      drawDijetFragSingleSet(fragMode, kPPDATA, trackPtCut, 0, usingPara);     
      drawDijetFragSingleSet(fragMode, kPPDATA, trackPtCut, -1, usingPara);    // -1 means that you don't reweight by jet spectrum
      drawDijetFragSingleSet(fragMode, kPPDATA, trackPtCut, 100, usingPara);     
    }
    if ( doHIDATA) {
      drawDijetFragSingleSet(fragMode, kHIDATA, trackPtCut, 0, usingPara);
      drawDijetFragSingleSet(fragMode, kHIDATA, trackPtCut, 100, usingPara);
    }
  }
}

void drawDijetFragSingleSet(int fragMode, int dataset, float trackPtCut, int doClosure, bool usingPara) {
  
  TString attPara = ( (usingPara == true) ? "Para" : "" ) ;
  
  TString datasetName = ""; 
  if (dataset == kHIDATA)       datasetName = "hidata";
  else if (dataset == kHIMC)    datasetName = "himc";
  else if (dataset == kPPDATA)  datasetName = "ppdata";
  
  TH1D* htrkFF[5][3][5];
  //  const int nPtBin = 12;
  //  double ptBin[nPtBin+1] = {1, 1.5, 2, 3, 4, 6, 9, 13, 20, 30, 50,100,200};
  //  const int nPtBin = 7;
  //  double ptBin[nPtBin+1] = {1, 2, 4, 8, 16, 32,64,128};
  const int nPtBin = 22;
  double ptBin[nPtBin+1] = { 0,1,1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,4,5,6,8,10,12,14,16,20,25,50};

  const int nZBin = 21;
  double zBin[nZBin+1] = { 0,0.003,0.007,0.012,0.016,0.02,0.03,0.04, 0.06, 0.1, 0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.6,0.7,0.8,1};
  
  /*double ptBin[nPtBin+1];
    for ( int i=0; i<=nPtBin ; i++) { 
    ptBin[i] = i;
    }
  */
  
  for (int icent = 1; icent<=999 ; icent++) {
    for (int irj = 1; irj<=999 ; irj++) {
      // if ( ((irj == 999)&&(icent<=4)) || ((icent == 999)&&(irj<=4)) )  {
      //	if ( (icent == 999) && (irj<=4) ) {
      if ( (irj == 999) && (icent<=4) )  {
	int index = irj;
	if ( irj == 999 ) 
	  index = icent;
	
	///////////////////////////////////////  define what to draw!! ///////////////////////
	TString suffix = Form("%s_icent%d_irj%d_fragMode%d_closure%d%s",datasetName.Data(),icent,irj,fragMode,doClosure,attPara.Data());
	if ( fragMode   == 1 )
	  htrkFF[index][kLjet][kRAW]= new TH1D(Form("hpt_lJet_rawTrk_%s",suffix.Data()),";track p_{T} (GeV/c);dN/dp_{T} (GeV/c)^{-1}",nPtBin,ptBin);
	else if ( fragMode==2)
	  htrkFF[index][kLjet][kRAW]= new TH1D(Form("hpt_lJet_rawTrk_%s",suffix.Data()),";#xi = ln(1/z);dN/d#xi",14,0,7);
	else if ( fragMode==3)
	  htrkFF[index][kLjet][kRAW]= new TH1D(Form("hpt_lJet_rawTrk_%s",suffix.Data()),";z;dN/d#xi;dN/dz",nZBin,zBin);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	htrkFF[index][kSljet][kRAW] = (TH1D*)htrkFF[index][kLjet][kRAW]->Clone(Form( "hpt_slJet_rawTrk_%s",suffix.Data()));
	htrkFF[index][kLjet][kBKG]  = (TH1D*)htrkFF[index][kLjet][kRAW]->Clone(Form( "hpt_lJet_mbTrk_%s",suffix.Data()));
	htrkFF[index][kSljet][kBKG] = (TH1D*)htrkFF[index][kLjet][kRAW]->Clone(Form( "hpt_slJet_mbTrk_%s",suffix.Data()));
	htrkFF[index][kLjet][kSIG] = (TH1D*)htrkFF[index][kLjet][kRAW]->Clone(Form(  "hpt_lJet_sigTrk_%s",suffix.Data()));
	htrkFF[index][kSljet][kSIG] = (TH1D*)htrkFF[index][kSljet][kRAW]->Clone(Form("hpt_slJet_sigTrk_%s",suffix.Data()));
	
	drawDijetFragSingle(htrkFF[index],dataset, icent, irj, fragMode,trackPtCut,doClosure,usingPara);
	
      }
    }
  }
  
  
}

void drawDijetFragSingle( TH1D* htrkPt[3][5],
			  int dataset,
			  int icent,
			  int irj,
			  int fragMode,
			  float trackPtCut,
			  int doClosure,
			  bool usingPara
			  ) {
  
  float finalEtaCut = 2;
  /// Inclusive ////////////////////////
  //  TCut dijetSelCut = Form("lJetPt>100 && slJetPt>100 && acos(cos(lJetPhi - slJetPhi))>%f",jetDPhiCut);
  TCut dijetSelCut = "lJetPt>100";
  TCut dijetSBCut  = "lJetPt>100 && slJetPt>40 && acos(cos(lJetPhi - slJetPhi))>0.5 && acos(cos(lJetPhi - slJetPhi))<2";

  TString datasetName = "";
  if (dataset == kHIDATA)       datasetName = "hidata";
  else if (dataset == kHIMC)    datasetName = "himc";
  else if (dataset == kPPDATA)  datasetName = "ppdata";
  TString attPara = ( (usingPara == true) ? "Para" : "" ) ;
  
  TString suffix = Form("%s_icent%d_irj%d_fragMode%d_closure%d",datasetName.Data(),icent,irj,fragMode,doClosure);
  
  TString clsText = "evtMixing";
  TString clsText_ = "event mixing method";
  if ( doClosure == 100 ) {
    clsText = "etaRefl";
    clsText_ = "#eta reflection method";
  }
  
  TH1::SetDefaultSumw2();

  TCut centCut = "1==1";
  if ( (icent >= 1) && (icent<=4) )
    centCut = Form("cBin>=%d && cBin<%d", (int)centBin1[icent-1], (int)centBin1[icent]);
  else if ( icent == 999 )
    centCut = "cBin>=0 && cBin<12";
  
  TCut rjCut = "2==2";
  if ( (irj >= 1) && (irj<=4) )
    rjCut = Form("(slJetPt/lJetPt)>%f && (slJetPt/lJetPt)<=%f", (float)rjBin1[irj-1], (float)rjBin1[irj]);
  
  
  multiTreeUtil* dj  = new multiTreeUtil();
  if ( dataset == kHIDATA) {
    dj->addFile("jskim_HiForest-promptskim-hihighpt-hltjet80-pt90-v3_part2_akPu3PF_July-v6-8VtxBin-24PlnBin-40CentBin-trackingV2-02-14_jetPt_100_-1_jetEtaCut_2.00_yesPbin_smearCentBin0_useGenJet0.root",
		"tdj", dijetSelCut && centCut && rjCut,1);
  }
  else if ( dataset == kHIMC) {
    //    dj->addFile("jskim_mergedFile100_nMix10_akPu3PF_July-v4-4VtxBin-24PlnBin-40CentBin-EtaReflectionAdded_jetPt_100_40_jetEtaCut_2.00_dphi_2.748893_yesPbin_doSmear0_useGenJet0.root", 
    dj->addFile("jskim_Dijet100_HydjetDrum_v27_mergedV1_nMix10_akPu3PF_July-v6-8VtxBin-24PlnBin-40CentBin-trackingV2-02-14_jetPt_100_-1_jetEtaCut_2.00_yesPbin_smearCentBin0_useGenJet0.root",
		"tdj", dijetSelCut &&  centCut && rjCut,1);
    
  }
  else if ( dataset == kPPDATA) {
    //    dj->addFile(Form("jskim_forest2_2760GeV_ppJet_full_July14_akPu3Calo_July-v6-8VtxBin-24PlnBin-40CentBin-trackingV2-02-14_jetPt_100_-1_jetEtaCut_2.00_yesPbin_smearCentBin%d_useGenJet0_jetWeightAdded.root",icent),
    dj->addFile(Form("jskim_forest2_2760GeV_ppJet_full_July14_akPu3PF_July-v6-8VtxBin-24PlnBin-40CentBin-EtaReflectionAdded_jetPt_100_-1_jetEtaCut_2.00_yesPbin_smearCentBin%d_useGenJet0_jetWeightAdded.root",icent),
		"tdj", dijetSelCut, 1); // no centrality cut or aj cut
    
  }
  
  dj->AddFriend("mTrk");
  dj->AddFriend("yTrk=yongsunTrack");
  if ( (doClosure > 0) && ( doClosure != 100 ) ) {
    dj->AddFriend("genPar");
    dj->AddFriend("mGenPar");
  }
  
  cout << " ================================================= " << endl;
  cout << " working on dataset, icent, irj = " << datasetName << ", "<<icent<<", "<<irj<<endl;
  
  float jetDrCut = 0.3;
  TString theWeight = "";
  TString ppWeightLjet= "";
  TString ppWeightSljet= "";
  TString ppWeightLjet_= "";
  TString ppWeightSljet_= "";
  if (dataset == kHIMC) theWeight = "*1" ;//"*reweight";
  else if ( (dataset == kPPDATA)&&(doClosure==0) ) {
    if (icent == 999) {
      ppWeightLjet = Form(" *ljetWeightAj%d",irj);
      ppWeightLjet_ = Form(" ljetWeightAj%d",irj);
      // inclusive
      //ppWeightSljet = Form(" *sljetWeightAj%d",irj);
      //ppWeightSljet_ = Form(" sljetWeightAj%d",irj);
      ppWeightSljet = Form(" *ljetWeightAj%d",irj);
      ppWeightSljet_ = Form(" ljetWeightAj%d",irj);
    }
    if ( irj == 999) {
      ppWeightLjet = Form(" *ljetWeightCent%d",icent);
      ppWeightLjet_ = Form(" ljetWeightCent%d",icent);
      // inclusive
      //      ppWeightSljet = Form(  "*sljetWeightCent%d",icent);
      //  ppWeightSljet_ = Form( "sljetWeightCent%d",icent);
      ppWeightSljet = Form(  "*ljetWeightCent%d",icent);
      ppWeightSljet_ = Form( "ljetWeightCent%d",icent);
    }
  }
  else if ( (dataset == kPPDATA)&&(doClosure>99) ) {
    if (icent == 999) {
      ppWeightLjet = Form(" *ljetWeightAj%der",irj);
      ppWeightLjet_ = Form(" ljetWeightAj%der",irj);
      // inclusive
      //ppWeightSljet = Form(" *sljetWeightAj%der",irj);
      //ppWeightSljet_ = Form(" sljetWeightAj%der",irj);
      ppWeightSljet = Form(" *ljetWeightAj%der",irj);
      ppWeightSljet_ = Form(" ljetWeightAj%der",irj);
    }
    if ( irj == 999) {
      ppWeightLjet = Form(" *ljetWeightCent%der",icent);
      ppWeightLjet_ = Form(" ljetWeightCent%der",icent);
      // inclusive
      //ppWeightSljet = Form(" *sljetWeightCent%der",icent);
      //ppWeightSljet_ = Form(" sljetWeightCent%der",icent);
      ppWeightSljet = Form(" *ljetWeightCent%der",icent);
      ppWeightSljet_ = Form(" ljetWeightCent%der",icent);
    }
  }
  
  // inclusive /////////
  //  TH1D* hLjetPt = new TH1D(Form("hLjetPt_%s",suffix.Data()),";Jet p_{T} (GeV/c);",100,0,500);
  //  TH1D* hSljetPt = new TH1D(Form("hSljetPt_%s",suffix.Data()),";Jet p_{T} (GeV/c);",100,0,500);
  TH1D* hLjetPt = new TH1D(Form("hLjetPt_%s",suffix.Data()),";Jet p_{T} (GeV/c);Entries;",50,0,500);
  TH1D* hSljetPt = new TH1D(Form("hSljetPt_%s",suffix.Data()),";Jet p_{T} (GeV/c);Entries;",50,0,500);
  if ( (dataset==kHIMC) || (dataset==kHIDATA) ){ 
    
    if ( doClosure < 99 ) { // eta reflection
      dj->Draw2(hLjetPt,"lJetPt",Form("abs(lJetEta)<%f",finalEtaCut), ""); //( (dataset == kHIMC) ? "reweight" : "") );
      //// inclusive
      //      dj->Draw2(hSljetPt,"slJetPt","", ""); //( (dataset == kHIMC) ? "reweight" : "") );
      dj->Draw2(hSljetPt,"slJetPt",Form("slJetPt>100 && abs(slJetEta)<%f",finalEtaCut), ""); //( (dataset == kHIMC) ? "reweight" : "") );
    }
    else {   // 100 series is the eta reflection method
      dj->Draw2(hLjetPt,"lJetPt",Form("abs(lJetEta)<%f && abs(lJetEta)>0.3",finalEtaCut), ""); //( (dataset == kHIMC) ? "reweight" : "") );
      //// inclusive
      //      dj->Draw2(hSljetPt,"slJetPt","abs(slJetEta)>0.3", ""); //( (dataset == kHIMC) ? "reweight" : "") );
      dj->Draw2(hSljetPt,"slJetPt",Form("abs(slJetEta)<%f && slJetPt>100 && abs(slJetEta)>0.3",finalEtaCut), ""); //( (dataset == kHIMC) ? "reweight" : "") );
    }
    
  }
  else if ( dataset==kPPDATA) {
    
    if ( doClosure < 99 ) { // eta reflection                                                                                      
      dj->Draw2(hLjetPt, "lJetPt" ,Form("abs(lJetEta)<%f",finalEtaCut) ,ppWeightLjet_);
      // inclusive
      //   dj->Draw2(hSljetPt,"slJetPt","" ,ppWeightSljet_);
      dj->Draw2(hSljetPt,"slJetPt",Form("slJetPt>100 && abs(slJetEta)<%f",finalEtaCut) ,ppWeightSljet_);
    }
    else {                  // eta reflection.. 
      dj->Draw2(hLjetPt, "lJetPt" ,Form("abs(lJetEta)<%f && abs(lJetEta)>0.3",finalEtaCut) ,ppWeightLjet_);
      // inclusive
      //dj->Draw2(hSljetPt,"slJetPt","abs(slJetEta)>0.3" ,ppWeightSljet_);
      dj->Draw2(hSljetPt,"slJetPt",Form("abs(slJetEta)<%f && slJetPt>100 && abs(slJetEta)>0.3",finalEtaCut) ,ppWeightSljet_);
    }
  }
  // Inclusive////////////////
  hLjetPt->Add(hSljetPt);
  ////////////////////////////

  double NoE1 = hLjetPt->Integral();  // now NoE1 is esstentially the number of inclusive jets
  double NoE2 = hSljetPt->Integral();

  if ( fragMode   == 1 ) {
    if (doClosure <= 0) {
      dj->Draw2(htrkPt[kLjet][kRAW],"yTrk.pt"  ,Form("abs(lJetEta)<%f && yTrk.jetMatch==1 && yTrk.jetDr<%f && yTrk.pt>%f", finalEtaCut, jetDrCut,trackPtCut),finalEtaCut, "yTrk.trkWeight"+attPara +theWeight + ppWeightLjet);
      dj->Draw2(htrkPt[kSljet][kRAW],"yTrk.pt",Form("abs(slJetEta)<%f &&  slJetPt>100 && yTrk.jetMatch==2 && yTrk.jetDr<%f && yTrk.pt>%f", finalEtaCut, jetDrCut,trackPtCut),"yTrk.trkWeight"+attPara +theWeight + ppWeightSljet);
      dj->Draw2(htrkPt[kLjet][kBKG],"mTrk.pt"  ,Form("abs(lJetEta)<%f &&  mTrk.jetMatch==1 && mTrk.jetDr<%f && mTrk.pt>%f", finalEtaCut, jetDrCut,trackPtCut), "mTrk.trkWeight"+attPara +theWeight + ppWeightLjet);
      dj->Draw2(htrkPt[kSljet][kBKG],"mTrk.pt",Form("abs(slJetEta)<%f &&  slJetPt>100 && mTrk.jetMatch==2 && mTrk.jetDr<%f && mTrk.pt>%f", finalEtaCut, jetDrCut,trackPtCut), "mTrk.trkWeight"+attPara +theWeight + ppWeightSljet);
    }
    else if ( doClosure == 100 ) {
      dj->Draw2(htrkPt[kLjet][kRAW],"yTrk.pt"  ,Form("abs(lJetEta)<%f &&  abs(lJetEta)>0.3  && yTrk.jetMatch==1   && yTrk.jetDr<%f && yTrk.pt>%f", finalEtaCut, jetDrCut,trackPtCut), "yTrk.trkWeight"+attPara +theWeight+ppWeightSljet );
      dj->Draw2(htrkPt[kSljet][kRAW],"yTrk.pt",Form("abs(slJetEta)<%f && slJetPt>100 && abs(slJetEta)>0.3 && yTrk.jetMatch==2   && yTrk.jetDr<%f && yTrk.pt>%f", finalEtaCut, jetDrCut,trackPtCut), "yTrk.trkWeight"+attPara +theWeight+ppWeightSljet);
      dj->Draw2(htrkPt[kLjet][kBKG],"yTrk.pt"  ,Form("abs(lJetEta)<%f && abs(lJetEta)>0.3  && yTrk.jetMatch==-1  && yTrk.jetDr<%f && yTrk.pt>%f", finalEtaCut, jetDrCut,trackPtCut), "yTrk.trkWeight"+attPara +theWeight+ppWeightSljet);
      dj->Draw2(htrkPt[kSljet][kBKG],"yTrk.pt",Form("abs(slJetEta)<%f && slJetPt>100 && abs(slJetEta)>0.3  && yTrk.jetMatch==-2 && yTrk.jetDr<%f && yTrk.pt>%f", finalEtaCut, jetDrCut,trackPtCut), "yTrk.trkWeight"+attPara +theWeight+ppWeightSljet);
    } 
    else if ( doClosure == 101 ) {
      dj->Draw2(htrkPt[kLjet][kRAW],"genPar.pt"  ,Form("abs(lJetEta)<%f && abs(lJetEta)>0.3   && (abs(genPar.chg)>0) && (genPar.jetMatch==1) && (genPar.jetDr<%f) && (genPar.pt>%f)", finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kSljet][kRAW],"genPar.pt",Form("abs(slJetEta)<%f && slJetPt>100 && abs(slJetEta)>0.3 && (abs(genPar.chg)>0) && (genPar.jetMatch==2) && (genPar.jetDr<%f) && (genPar.pt>%f)", finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kLjet][kBKG],"genPar.pt"  ,Form("abs(lJetEta)<%f && abs(lJetEta)>0.3 && (genPar.sube!=0) && (abs(genPar.chg)>0) && (genPar.jetMatch==1) && (genPar.jetDr<%f) && (genPar.pt>%f)", finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kSljet][kBKG],"genPar.pt",Form("abs(slJetEta)<%f && slJetPt>100 && abs(slJetEta)>0.3 && (genPar.sube!=0) && (abs(genPar.chg)>0) && (genPar.jetMatch==2) && (genPar.jetDr<%f) && (genPar.pt>%f)", finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
    } 
    else if ( doClosure == 102 ) {
      dj->Draw2(htrkPt[kLjet][kRAW],"genPar.pt"  ,Form("abs(lJetEta)<%f && abs(lJetEta)>0.3 && (abs(genPar.chg)>0) && (genPar.jetMatch==1)    && (genPar.jetDr<%f) && (genPar.pt>%f)", finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kSljet][kRAW]," genPar.pt",Form("abs(slJetEta)<%f && slJetPt>100 && abs(slJetEta)>0.3 && (abs(genPar.chg)>0) && (genPar.jetMatch==2)   && (genPar.jetDr<%f) && (genPar.pt>%f)", finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kLjet][kBKG]," genPar.pt"  ,Form("abs(lJetEta)<%f && abs(lJetEta)>0.3 && (abs(genPar.chg)>0) && (genPar.jetMatch==-1)   && (genPar.jetDr<%f) && (genPar.pt>%f)", finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kSljet][kBKG]," genPar.pt",Form("abs(slJetEta)<%f && slJetPt>100 && abs(slJetEta)>0.3 && (abs(genPar.chg)>0) && (genPar.jetMatch==-2) && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      
    }
    
    else if ( doClosure == 1 ) {
      dj->Draw2(htrkPt[kLjet][kRAW]," genPar.pt"  ,Form("abs(lJetEta)<%f &&     (abs(genPar.chg)>0) && (genPar.jetMatch==1) && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kSljet][kRAW]," genPar.pt",Form("abs(slJetEta)<%f && slJetPt>100 &&     (abs(genPar.chg)>0) && (genPar.jetMatch==2) && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kLjet][kBKG]," genPar.pt"  ,Form("abs(lJetEta)<%f && (genPar.sube!=0) && (abs(genPar.chg)>0) && (genPar.jetMatch==1) && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kSljet][kBKG]," genPar.pt",Form("abs(slJetEta)<%f && slJetPt>100 && (genPar.sube!=0) && (abs(genPar.chg)>0) && (genPar.jetMatch==2) && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
    }
    else if ( doClosure == 2 ) {
      dj->Draw2(htrkPt[kLjet][kRAW]," genPar.pt"  ,Form("abs(lJetEta)<%f &&                     (abs(genPar.chg)>0) && (genPar.jetMatch==1) && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kSljet][kRAW]," genPar.pt",Form("abs(slJetEta)<%f && slJetPt>100 &&                     (abs(genPar.chg)>0) && (genPar.jetMatch==2) && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kLjet][kBKG], "mGenPar.pt" ,Form("abs(lJetEta)<%f &&  (abs(mGenPar.chg)>0) && (mGenPar.jetMatch==1) && (mGenPar.jetDr<%f) && (mGenPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kSljet][kBKG],"mGenPar.pt",Form("abs(slJetEta)<%f && slJetPt>100 &&  (abs(mGenPar.chg)>0) && (mGenPar.jetMatch==2) && (mGenPar.jetDr<%f) && (mGenPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
    }
  }
  
  else if ( fragMode   == 2 ) {
    if (doClosure <= 0) {
      dj->Draw2(htrkPt[kLjet][kRAW],"-log(yTrk.pt * cos(yTrk.jetDr)  *(cosh(yTrk.eta)/cosh(lJetEta)) /lJetPt )"  ,Form("abs(lJetEta)<%f && yTrk.jetMatch==1 && yTrk.jetDr<%f && yTrk.pt>%f",finalEtaCut, jetDrCut,trackPtCut), "yTrk.trkWeight"+attPara +theWeight + ppWeightLjet);
      dj->Draw2(htrkPt[kSljet][kRAW],"-log(yTrk.pt * cos(yTrk.jetDr) * (cosh(yTrk.eta)/cosh(slJetEta)) /slJetPt )",Form("abs(slJetEta)<%f &&slJetPt>100 && yTrk.jetMatch==2 && yTrk.jetDr<%f && yTrk.pt>%f",finalEtaCut, jetDrCut,trackPtCut), "yTrk.trkWeight"+attPara +theWeight + ppWeightSljet);
      dj->Draw2(htrkPt[kLjet][kBKG],"-log(mTrk.pt * cos(mTrk.jetDr) * (cosh(mTrk.eta)/cosh(lJetEta)) /lJetPt )"  ,Form("abs(lJetEta)<%f &&mTrk.jetMatch==1 && mTrk.jetDr<%f && mTrk.pt>%f",finalEtaCut, jetDrCut,trackPtCut), "mTrk.trkWeight"+attPara +theWeight + ppWeightLjet);
      dj->Draw2(htrkPt[kSljet][kBKG],"-log(mTrk.pt * cos(mTrk.jetDr) *(cosh(mTrk.eta)/cosh(slJetEta)) /slJetPt  )",Form("abs(slJetEta)<%f &&slJetPt>100 && mTrk.jetMatch==2 && mTrk.jetDr<%f && mTrk.pt>%f",finalEtaCut, jetDrCut,trackPtCut), "mTrk.trkWeight"+attPara +theWeight + ppWeightSljet);
    }
    else if ( doClosure == 100 ) {
      dj->Draw2(htrkPt[kLjet][kRAW],"-log(yTrk.pt * cos(yTrk.jetDr) *(cosh(yTrk.eta)/cosh(lJetEta)) /lJetPt)"  ,Form("abs(lJetEta)<%f &&abs(lJetEta)>0.3  && yTrk.jetMatch==1   && yTrk.jetDr<%f && yTrk.pt>%f",finalEtaCut, jetDrCut,trackPtCut), "yTrk.trkWeight"+attPara +theWeight+ppWeightSljet );
      dj->Draw2(htrkPt[kSljet][kRAW],"-log(yTrk.pt * cos(yTrk.jetDr) * (cosh(yTrk.eta)/cosh(slJetEta)) /slJetPt)",Form("abs(slJetEta)<%f &&slJetPt>100 && abs(slJetEta)>0.3 && yTrk.jetMatch==2   && yTrk.jetDr<%f && yTrk.pt>%f",finalEtaCut, jetDrCut,trackPtCut), "yTrk.trkWeight"+attPara +theWeight+ppWeightSljet);
      dj->Draw2(htrkPt[kLjet][kBKG],"-log(yTrk.pt * cos(yTrk.jetDr) * (cosh(yTrk.eta)/cosh(lJetEta)) /lJetPt)"  ,Form("abs(lJetEta)<%f &&abs(lJetEta)>0.3  && yTrk.jetMatch==-1  && yTrk.jetDr<%f && yTrk.pt>%f",finalEtaCut, jetDrCut,trackPtCut), "yTrk.trkWeight"+attPara +theWeight+ppWeightSljet);
      dj->Draw2(htrkPt[kSljet][kBKG],"-log(yTrk.pt * cos(yTrk.jetDr)  *(cosh(yTrk.eta)/cosh(slJetEta)) /slJetPt)",Form("abs(slJetEta)<%f && slJetPt>100 && abs(slJetEta)>0.3  && yTrk.jetMatch==-2 && yTrk.jetDr<%f && yTrk.pt>%f",finalEtaCut, jetDrCut,trackPtCut), "yTrk.trkWeight"+attPara +theWeight+ppWeightSljet);
    } 
    else if ( doClosure == 101 ) {
      dj->Draw2(htrkPt[kLjet][kRAW],"-log( genPar.pt * cos(genPar.jetDr)  *(cosh(genPar.eta)/cosh(lJetEta)) /lJetPt)"  ,Form("abs(lJetEta)<%f &&abs(lJetEta)>0.3   && (abs(genPar.chg)>0) && (genPar.jetMatch==1) && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kSljet][kRAW],"-log( genPar.pt * cos(genPar.jetDr) *(cosh(genPar.eta)/cosh(slJetEta))  /slJetPt)",Form("abs(slJetEta)<%f &&slJetPt>100 && abs(slJetEta)>0.3 && (abs(genPar.chg)>0) && (genPar.jetMatch==2) && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kLjet][kBKG],"-log( genPar.pt * cos(genPar.jetDr)  *(cosh(genPar.eta)/cosh(lJetEta))  /lJetPt)"  ,Form("abs(lJetEta)<%f &&abs(lJetEta)>0.3 && (genPar.sube!=0) && (abs(genPar.chg)>0) && (genPar.jetMatch==1) && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kSljet][kBKG],"-log( genPar.pt * cos(genPar.jetDr)  *(cosh(genPar.eta)/cosh(slJetEta))  /slJetPt)",Form("abs(slJetEta)<%f &&slJetPt>100 && abs(slJetEta)>0.3 && (genPar.sube!=0) && (abs(genPar.chg)>0) && (genPar.jetMatch==2) && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
    } 
    else if ( doClosure == 102 ) {
      dj->Draw2(htrkPt[kLjet][kRAW],"-log( genPar.pt * cos(genPar.jetDr)  *(cosh(genPar.eta)/cosh(lJetEta)) /lJetPt)"  ,Form("abs(lJetEta)<%f &&abs(lJetEta)>0.3 && (abs(genPar.chg)>0) && (genPar.jetMatch==1)    && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kSljet][kRAW],"-log( genPar.pt * cos(genPar.jetDr)  *(cosh(genPar.eta)/cosh(slJetEta)) /slJetPt)",Form("abs(slJetEta)<%f &&slJetPt>100 && abs(slJetEta)>0.3 && (abs(genPar.chg)>0) && (genPar.jetMatch==2)   && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kLjet][kBKG],"-log( genPar.pt * cos(genPar.jetDr)  *(cosh(genPar.eta)/cosh(lJetEta)) /lJetPt)"  ,Form("abs(lJetEta)<%f &&abs(lJetEta)>0.3 && (abs(genPar.chg)>0) && (genPar.jetMatch==-1)   && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kSljet][kBKG],"-log( genPar.pt * cos(genPar.jetDr)  *(cosh(genPar.eta)/cosh(slJetEta)) /slJetPt)",Form("abs(slJetEta)<%f &&slJetPt>100 && abs(slJetEta)>0.3 && (abs(genPar.chg)>0) && (genPar.jetMatch==-2) && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      
    }
    
    
    
    else if ( doClosure == 1 ) {
      dj->Draw2(htrkPt[kLjet][kRAW],"-log( genPar.pt * cos(genPar.jetDr)  *(cosh(genPar.eta)/cosh(lJetEta)) /lJetPt)"  ,Form("abs(lJetEta)<%f &&    (abs(genPar.chg)>0) && (genPar.jetMatch==1) && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kSljet][kRAW],"-log( genPar.pt * cos(genPar.jetDr)  *(cosh(genPar.eta)/cosh(slJetEta)) /slJetPt)",Form("abs(slJetEta)<%f &&slJetPt>100 &&     (abs(genPar.chg)>0) && (genPar.jetMatch==2) && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kLjet][kBKG],"-log( genPar.pt * cos(genPar.jetDr)  *(cosh(genPar.eta)/cosh(lJetEta)) /lJetPt)"  ,Form("abs(lJetEta)<%f &&(genPar.sube!=0) && (abs(genPar.chg)>0) && (genPar.jetMatch==1) && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kSljet][kBKG],"-log( genPar.pt * cos(genPar.jetDr)  *(cosh(genPar.eta)/cosh(slJetEta)) /slJetPt)",Form("abs(slJetEta)<%f &&slJetPt>100 && (genPar.sube!=0) && (abs(genPar.chg)>0) && (genPar.jetMatch==2) && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
    }
    else if ( doClosure == 2 ) {
      dj->Draw2(htrkPt[kLjet][kRAW],"-log( genPar.pt * cos(genPar.jetDr)  *(cosh(genPar.eta)/cosh(lJetEta)) /lJetPt)"  ,Form("  abs(lJetEta)<%f &&                  (abs(genPar.chg)>0) && (genPar.jetMatch==1) && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kSljet][kRAW],"-log( genPar.pt * cos(genPar.jetDr)  *(cosh(genPar.eta)/cosh(slJetEta)) /slJetPt)",Form("abs(slJetEta)<%f &&slJetPt>100 &&                     (abs(genPar.chg)>0) && (genPar.jetMatch==2) && (genPar.jetDr<%f) && (genPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kLjet][kBKG], "-log(mGenPar.pt * cos(mGenPar.jetDr)  *(cosh(mGenPar.eta)/cosh(lJetEta)) /lJetPt)" ,Form("abs(lJetEta)<%f && (abs(mGenPar.chg)>0) && (mGenPar.jetMatch==1) && (mGenPar.jetDr<%f) && (mGenPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
      dj->Draw2(htrkPt[kSljet][kBKG],"-log(mGenPar.pt * cos(mGenPar.jetDr)  *(cosh(mGenPar.eta)/cosh(slJetEta)) /slJetPt)",Form("abs(slJetEta)<%f &&slJetPt>100 &&  (abs(mGenPar.chg)>0) && (mGenPar.jetMatch==2) && (mGenPar.jetDr<%f) && (mGenPar.pt>%f)",finalEtaCut, jetDrCut,trackPtCut), "1"+theWeight);
    }
    
    
  }

   
  // inclusive ////////////////////////////////////
  htrkPt[kLjet][kRAW]->Add(htrkPt[kSljet][kRAW]);   // now kLjet is all inclusive ones
  htrkPt[kLjet][kBKG]->Add(htrkPt[kSljet][kBKG]);
  ////////////////////////////////////////////////
  TH1ScaleByWidth(htrkPt[kLjet][kRAW]);
  TH1ScaleByWidth(htrkPt[kSljet][kRAW]);
  TH1ScaleByWidth(htrkPt[kLjet][kBKG]);
  TH1ScaleByWidth(htrkPt[kSljet][kBKG]);
  if ( (doClosure == 1)  || (doClosure>99) ) {
    htrkPt[kLjet][kBKG]->Scale(1./(NoE1));
    htrkPt[kSljet][kBKG]->Scale(1./(NoE2));
  }
  else {
    htrkPt[kLjet][kBKG]->Scale(1./(NoE1*nMixing1));
    htrkPt[kSljet][kBKG]->Scale(1./(NoE2*nMixing1));
  }

  if ( dataset == kPPDATA ) {
    cout << " it's pp data.. So DO background subtraction!!! " << endl;
    //  htrkPt[kLjet][kBKG]->Reset();
    //  htrkPt[kSljet][kBKG]->Reset();
  }
  htrkPt[kLjet][kRAW]->Scale(1./NoE1);
  htrkPt[kSljet][kRAW]->Scale(1./NoE2);
  
  htrkPt[kLjet][kSIG]->Add(htrkPt[kLjet][kRAW]);
  htrkPt[kSljet][kSIG]->Add(htrkPt[kSljet][kRAW]);
  htrkPt[kLjet][kSIG]->Add(htrkPt[kLjet][kBKG],-1);
  htrkPt[kSljet][kSIG]->Add(htrkPt[kSljet][kBKG],-1);

  
  handsomeTH1(htrkPt[kLjet][kRAW],1,1,24);
  handsomeTH1(htrkPt[kLjet][kBKG],1);
  handsomeTH1(htrkPt[kLjet][kSIG],1,1,20);
  handsomeTH1(htrkPt[kSljet][kRAW],2,1,24);
  handsomeTH1(htrkPt[kSljet][kBKG],2);
  handsomeTH1(htrkPt[kSljet][kSIG],2,1,20);
  
  
  TCanvas* c1 = new TCanvas(Form("c1_icent%d_irj%d",icent,irj),"",900,450);
  c1->Divide(2,1);
  c1->cd(1);
  htrkPt[kLjet][kRAW]->SetAxisRange(1e-5,10000,"Y");
  htrkPt[kLjet][kRAW]->Draw();
  htrkPt[kLjet][kBKG]->Draw("same hist");
  htrkPt[kLjet][kSIG]->Draw("same");
  gPad->SetLogy();
  TLegend* l1 = new TLegend(0.3678213,0.7702352,0.9162483,0.9296714,NULL,"brNDC");
  easyLeg(l1,clsText_.Data()) ;
  l1->AddEntry(htrkPt[kLjet][kRAW],"All tracks in jet cone","p");
  l1->AddEntry(htrkPt[kLjet][kBKG], "tracks from underlying events","l");
  l1->AddEntry(htrkPt[kLjet][kSIG], "After subtraction","p");
  l1->Draw();
  drawText("jet p_{T} > 100GeV/c,  |#eta|<2", 0.26,0.3,1,17);
  drawText(Form("%d entries",(int)NoE1), 0.26,0.23,1,17);
  if ( fragMode   == 1 )      gPad->SetLogx();
  
  //Incluse
  /*
  c1->cd(2);
  htrkPt[kSljet][kRAW]->SetAxisRange(1e-5,10000,"Y");
  htrkPt[kSljet][kRAW]->Draw();
  htrkPt[kSljet][kBKG]->Draw("same hist");
  htrkPt[kSljet][kSIG]->Draw("same");
  gPad->SetLogy();
  drawText("Sublead. jet > 40GeV", 0.26,0.3,1,17);
  drawText(Form("%d entries",(int)NoE2), 0.26,0.23,1,17);
  if ( fragMode   == 1 )      gPad->SetLogx();
  */
  if       ( dataset == kHIDATA) 
    drawText("PbPb DATA", 0.2,0.8,1,15);
  else  if ( dataset == kHIMC)
    drawText("MC", 0.2,0.8,1);
  else if ( dataset == kPPDATA)
    drawText("pp DATA", 0.2,0.8,1,15);
  
  if ( (icent>=1) && (icent<=4) ) {
    int lowCent = centBin1[icent-1];
    int highCent = centBin1[icent]-1;
    drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)), 0.2,0.73,1);
  }
  if ( (irj>=1) && (irj<=4) ) {
    drawText("0-30%", 0.2,0.73,1);
    drawText(Form("%.2f < pt_{2}/pt_{1} < %.2f             p_{T}^{track}>%.0f", float(rjBin1[irj-1]), float(rjBin1[irj]),float(trackPtCut)), 0.2,0.66,1);
    
  }
  // Inclusive 
  //  c1->cd(3);
  c1->cd(2);
  cleverRange(hLjetPt,10,1e-1);
  handsomeTH1(hLjetPt,1);
  handsomeTH1(hSljetPt,2);
  hLjetPt->Draw();
  //  hSljetPt->Draw("same");
  gPad->SetLogy();
  drawText("Jet p_{T}", 0.55,0.63,1);
  

  
  if ( fragMode==2) {
    c1->SaveAs(Form("plotsOfDijetFF/dijetFF_xi_icent%d_irj%d_%s_%s.pdf",icent,irj,datasetName.Data(),clsText.Data()));
  }
  else if ( fragMode==1) {
    c1->SaveAs(Form("plotsOfDijetFF/dijetFF_pt_icent%d_irj%d_%s_%s.pdf",icent,irj,datasetName.Data(),clsText.Data()));
  }
  TCanvas* c0 = new TCanvas("ctempTemp","",100,100);

  
  TFile outf = TFile(Form("dijetFF_output_histograms_trkPtProjectOnJetAxis_trackPtCut%.0f_FinaletaCut%.2f.root",(float)trackPtCut,(float)finalEtaCut),"update");
  hLjetPt->Write();
  hSljetPt->Write();
  htrkPt[kSljet][kRAW]->Write();
  htrkPt[kLjet][kRAW]->Write();
  htrkPt[kSljet][kSIG]->Write();
  htrkPt[kLjet][kSIG]->Write();
  htrkPt[kSljet][kBKG]->Write();
  htrkPt[kLjet][kBKG]->Write();
  outf.Close();
}
