#include "CutAndBinCollection2011.h"



int kEvtMix  = 1;
int kDphiTrk = 2;

//combinations
const int kIsoPho=1;
const int kDecay=2;
const int kPurePho = 3;

const int kRawTrk=1;
const int kHydTrk=2;
const int kHardTrk =3;
const int kMBJetTrk=4;
const int kMBxTrk=5;
const int kHardMBTrk =6;
const int kFinalGoodTrk =0;

const int kRawJet=1;
const int kHydJet=2;
const int kFinalJet=3;

const float fixedPPPurity = 0.80;
//kPure is shared with photon

class corrFunctionTrk
{
 public:
  void initAll(int fsampleType, int ficent, int fipt, int cutOpt, int iJetPt, int iptAssoc);
  void initSingleSet(TH1D* singleCorr[4][7], TString histName, int nXbinning, double* xbinnging);
  
  //  void initSingleSet2D(TH2D* singleCorr[4][7], TString histName, int nXbinning, double* xbinnging,int nYbinning, double* ybinnging);
  
  void calCorr();
  void calCorrDiff(TH1D* corrF[4][7]);
  //  void calCorrDiff2D(TH2D* corrF[4][7]);
  void clear();
  void setNorm();

  double getNorm();

  ~corrFunctionTrk(){};

  //      int GetBinNumber(double c);                                                                                   

  TH1D *sigPhotonEt;
  TH1D *sbPhotonEt;

  TH1D *awayJetSpectra[4][7];

  TH1D *hTrkDphi[4][7];
  TH1D *hTrkPtInFragJet[4][7];
  TH1D *hTrkX[4][7];
  TH1D *hTrkXi[4][7];
  TH1D *hTrkXij[4][7];
  TH1D *hIncXi05[4][7];
  TH1D *hIncXi10[4][7];
  TH1D *hIncKt05[4][7];
  TH1D *hIncKt10[4][7];

  //  TH1D *hDphiCorr[4][7];

  //  TH2D *h2dCorr[4][7];
  TH2D *hPurity2D;
  
 private: 
  int sampleType;
  int icent;
  int ipt;
  int iJetPt;
  float purity;
  bool isNormed;
  double nIsoPhos;
  double nPurePhos;
  double nNonIsoPhos;
  
  
};

void corrFunctionTrk::initAll(int fsampleType, int ficent, int fipt, int cutOpt, int fiJetPt, int iptAssoc){ 
  sampleType = fsampleType;
  icent = ficent;
  ipt   = fipt;
  iJetPt = fiJetPt;
  isNormed = false;
  nIsoPhos    = 0;
  nPurePhos   = 0;
  nNonIsoPhos = 0;

  TString fPurityName = Form("photonPurityCollection_cutOption%d.root",cutOpt);
  cout << "output file name = " << fPurityName.Data() << endl;
  TFile *fpurity = new TFile(fPurityName.Data());
  hPurity2D = (TH2D*)fpurity->Get("hPurity2D");

  if ( sampleType == kHIMC )   
    purity = 1 ;
  else if ( sampleType == kHIDATA) {   // Purity with 50GeV
    purity = hPurity2D->GetBinContent(ipt, icent);
  }
  else if ( sampleType == kPPDATA) {   // Purity with 50GeV  
    purity = fixedPPPurity; 
  }

  else 
    cout << " no such sample type!!! "<< endl << endl << endl << endl ;
  
  TString hName1;
  if ( sampleType == kHIMC ) 
    hName1 = Form("himc_icent%d_ipt%d_iJetPt%d_iptAssoc%d",icent,ipt,iJetPt,iptAssoc);
  else if ( sampleType == kHIDATA) {
    hName1 = Form("hidata_icent%d_ipt%d_iJetPt%d_iptAssoc%d",icent,ipt,iJetPt,iptAssoc);
  }
  else if ( sampleType == kPPDATA) {
    hName1 = Form("ppdata_icent%d_ipt%d_iJetPt%d_iptAssoc%d",icent,ipt,iJetPt,iptAssoc); // icent will be 614 for a while
  }


  
  sigPhotonEt = new TH1D( Form("%s_sigPhotonEt",hName1.Data()),";photon E_{T} (GeV); Entries",20,0,200);
  sbPhotonEt = (TH1D*)sigPhotonEt->Clone(Form("%s_sbPhotonEt",hName1.Data()));
  
  const int nJetPtBin = 40;
  double jetPtBin[nJetPtBin+1];
  for ( int ii=0 ; ii<=nJetPtBin ; ii++) {
    jetPtBin[ii] = (float)ii * 5. ;
  }
  initSingleSet( awayJetSpectra, Form("%s_awayJetSpectra",hName1.Data()), nJetPtBin, jetPtBin);
  
  
  
  
  const int nXBinTrk = 16;
  double xBinTrk[nXBinTrk+1] = {0, 0.025, 0.05, 0.075, 0.1, 0.125, 0.15, 0.175, 0.2, 0.25, 0.3, 0.4, 0.5, 0.7, 1, 1.5, 2};
  initSingleSet(hTrkX,Form("%s_x",hName1.Data()),nXBinTrk, xBinTrk);

  const int nPtBinTrk = 17;
  double ptBinTrk[nPtBinTrk+1] = {0, 0.5, 0.7, 0.9, 1.2, 1.5, 2, 3, 4, 5, 7, 9, 11, 14, 17, 22, 30, 1000};
  initSingleSet(hTrkPtInFragJet,Form("%s_Pt",hName1.Data()),nPtBinTrk, ptBinTrk);
  
  const int nDphiBin = 40;
  double dphiBin[nDphiBin+1];
  for ( int ii=0 ; ii<=nDphiBin ; ii++) {
    dphiBin[ii] = float(ii)/float(nDphiBin) * 3.141592;
  }
  initSingleSet(hTrkDphi,Form("%s_dphiPtWeight",hName1.Data()),nDphiBin, dphiBin);
  //  initSingleSet(hDphiCorr,Form("%s_dphi",hName1.Data()),nDphiBin, dphiBin);

  const int nDetaBin = 10;
  double detaBin[nDetaBin+1];
  for ( int ii=0 ; ii<=nDetaBin ; ii++) {
    detaBin[ii] = float(ii)/float(nDetaBin) * 5.;
  }


  //  initSingleSet2D( h2dCorr, Form("%s_2dCorr",hName1.Data()),nDphiBin, dphiBin, nDetaBin, detaBin);
  

  const int nXiBinTrk = 20;
  double xiBinTrk[nXBinTrk+1];
  for ( int ii=0 ; ii<=nXiBinTrk ; ii++) {
    xiBinTrk[ii] = (float)ii * 0.4 - 0.4;
  }
  initSingleSet(hTrkXi,Form("%s_xi",hName1.Data()),nXiBinTrk, xiBinTrk);
  initSingleSet(hTrkXij,Form("%s_xij",hName1.Data()),nXiBinTrk, xiBinTrk);
  initSingleSet(hIncXi05,Form("%s_xiInc05",hName1.Data()),nXiBinTrk, xiBinTrk);
  initSingleSet(hIncXi10,Form("%s_xiInc10",hName1.Data()),nXiBinTrk, xiBinTrk);
 
 
  const int nKtBinTrk = 100;
  double ktBinTrk[nKtBinTrk+1];
  for ( int ii=0 ; ii<=nKtBinTrk ; ii++) {
    ktBinTrk[ii] = (float)ii * 0.2;
  }
  initSingleSet(hIncKt05,Form("%s_incKt05",hName1.Data()),nKtBinTrk, ktBinTrk);
  initSingleSet(hIncKt10,Form("%s_incKt10",hName1.Data()),nKtBinTrk, ktBinTrk);



}

void corrFunctionTrk::initSingleSet(TH1D* singleCorr[4][7], TString histName, int nXbinning, double* xbinnging) {
  TH1::SetDefaultSumw2();

  TH1D* hSingleCorrTemp = new TH1D( Form("%s_Template",histName.Data()), "", nXbinning, xbinnging ) ;
  
  singleCorr[kIsoPho][kRawTrk]       = (TH1D*)hSingleCorrTemp->Clone(Form  ("%s_candPho_rawTrk_allCone", histName.Data()));
  singleCorr[kIsoPho][kHydTrk]       = (TH1D*)hSingleCorrTemp->Clone(Form  ("%s_candPho_xTrk_allCone", histName.Data()));
  singleCorr[kIsoPho][kHardTrk]      = (TH1D*)hSingleCorrTemp->Clone(Form ("%s_candPho_pureTrk_allCone", histName.Data()));
  singleCorr[kIsoPho][kMBJetTrk]     = (TH1D*)hSingleCorrTemp->Clone(Form("%s_candPho_rawTrk_mbCone", histName.Data()));
  singleCorr[kIsoPho][kMBxTrk]       = (TH1D*)hSingleCorrTemp->Clone(Form  ("%s_candPho_xTrk_mbCone", histName.Data()));
  singleCorr[kIsoPho][kHardMBTrk]    = (TH1D*)hSingleCorrTemp->Clone(Form("%s_candPho_pureTrk_mbCone", histName.Data()));
  singleCorr[kIsoPho][kFinalGoodTrk] = (TH1D*)hSingleCorrTemp->Clone(Form("%s_candPho_pureTrk_pureCone", histName.Data()));
 
  singleCorr[kDecay][kRawTrk]        = (TH1D*)hSingleCorrTemp->Clone(Form  ("%s_decayPho_rawTrk_allCone", histName.Data()));
  singleCorr[kDecay][kHydTrk]        = (TH1D*)hSingleCorrTemp->Clone(Form  ("%s_decayPho_xTrk_allCone", histName.Data()));
  singleCorr[kDecay][kHardTrk]       = (TH1D*)hSingleCorrTemp->Clone(Form ("%s_decayPho_pureTrk_allCone", histName.Data()));
  singleCorr[kDecay][kMBJetTrk]      = (TH1D*)hSingleCorrTemp->Clone(Form("%s_decayPho_rawTrk_mbCone", histName.Data()));
  singleCorr[kDecay][kMBxTrk]        = (TH1D*)hSingleCorrTemp->Clone(Form  ("%s_decayPho_xTrk_mbCone", histName.Data()));
  singleCorr[kDecay][kHardMBTrk]     = (TH1D*)hSingleCorrTemp->Clone(Form("%s_decayPho_pureTrk_mbCone", histName.Data()));
  singleCorr[kDecay][kFinalGoodTrk]  = (TH1D*)hSingleCorrTemp->Clone(Form("%s_decayPho_pureTrk_pureCone", histName.Data()));

  singleCorr[kPurePho][kFinalGoodTrk]= (TH1D*)hSingleCorrTemp->Clone(Form("%s_purePho_pureTrk_pureCone", histName.Data()));
  
  //  for ( int ii=0; ii<4; ii++) {
  //  for ( int jj=0; jj<7; jj++) {
  //    if (singleCorr[ii][jj] != 0)  singleCorr[ii][jj]->Sumw2();
  //  }
  // }

}





/* Not used anymore
   void corrFunctionTrk::initSingleSet2D(TH2D* singleCorr[4][7], TString histName, int nXbinning, double* xbinnging, int nYbinning, double* ybinnging) {
   TH2D* hSingleCorrTemp = new TH2D( Form("%s_Template",histName.Data()), "", nXbinning, xbinnging, nYbinning, ybinnging ) ;
   singleCorr[kIsoPho][kRawTrk] = (TH2D*)hSingleCorrTemp->Clone(Form  ("%s_candPho_rawTrk_allCone", histName.Data()));
   singleCorr[kIsoPho][kHydTrk] = (TH2D*)hSingleCorrTemp->Clone(Form  ("%s_candPho_xTrk_allCone", histName.Data()));
   singleCorr[kIsoPho][kHardTrk] = (TH2D*)hSingleCorrTemp->Clone(Form ("%s_candPho_pureTrk_allCone", histName.Data()));
   singleCorr[kIsoPho][kMBJetTrk] = (TH2D*)hSingleCorrTemp->Clone(Form("%s_candPho_rawTrk_mbCone", histName.Data()));
   singleCorr[kIsoPho][kMBxTrk] = (TH2D*)hSingleCorrTemp->Clone(Form  ("%s_candPho_xTrk_mbCone", histName.Data()));
   singleCorr[kIsoPho][kHardMBTrk] =(TH2D*)hSingleCorrTemp->Clone(Form("%s_candPho_pureTrk_mbCone", histName.Data()));
   singleCorr[kIsoPho][kFinalGoodTrk] =(TH2D*)hSingleCorrTemp->Clone(Form("%s_candPho_pureTrk_pureCone", histName.Data()));
   
   singleCorr[kDecay][kRawTrk] = (TH2D*)hSingleCorrTemp->Clone(Form  ("%s_decayPho_rawTrk_allCone", histName.Data()));
   singleCorr[kDecay][kHydTrk] = (TH2D*)hSingleCorrTemp->Clone(Form  ("%s_decayPho_xTrk_allCone", histName.Data()));
   singleCorr[kDecay][kHardTrk] = (TH2D*)hSingleCorrTemp->Clone(Form ("%s_decayPho_pureTrk_allCone", histName.Data()));
   singleCorr[kDecay][kMBJetTrk] = (TH2D*)hSingleCorrTemp->Clone(Form("%s_decayPho_rawTrk_mbCone", histName.Data()));
   singleCorr[kDecay][kMBxTrk] = (TH2D*)hSingleCorrTemp->Clone(Form  ("%s_decayPho_xTrk_mbCone", histName.Data()));
   singleCorr[kDecay][kHardMBTrk] =(TH2D*)hSingleCorrTemp->Clone(Form("%s_decayPho_pureTrk_mbCone", histName.Data()));
   singleCorr[kDecay][kFinalGoodTrk] =(TH2D*)hSingleCorrTemp->Clone(Form("%s_decayPho_pureTrk_pureCone", histName.Data()));
   
   singleCorr[kPurePho][kFinalGoodTrk] =(TH2D*)hSingleCorrTemp->Clone(Form("%s_purePho_pureTrk_pureCone", histName.Data()));
   
   for ( int ii=0; ii<4; ii++) {
   for ( int jj=0; jj<7; jj++) {
   if (singleCorr[ii][jj] != 0)  singleCorr[ii][jj]->Sumw2();
   }
   }
   }
*/


void corrFunctionTrk::setNorm() {
  if (isNormed) {
    cout << "WARNING!!! IT's already normalized!!!!!! setNorm() error"<<endl;
    return;
  }

  cout << " ============= Photon counting ============================ " << endl;
  
  nIsoPhos    = sigPhotonEt->Integral();
  nNonIsoPhos =  sbPhotonEt->Integral();

  if (nIsoPhos==0)  cout << " strange!!! n_photon = 0 " << endl;

  if ( sampleType == kHIMC) {
    nPurePhos= nIsoPhos;
    cout << " it's PbPb mc, so pure photons  = " << nPurePhos  << " out of  candidates = " << nIsoPhos << endl;
  }
  else if (sampleType == kHIDATA ) {
    nPurePhos = nIsoPhos * purity;
    cout << " it's PbPb data, so pure photons  = " << nPurePhos  << " out of  candidates = " << nIsoPhos << endl;
  }
  else if (sampleType == kPPDATA ) {
    nPurePhos = nIsoPhos * purity;
    cout << " it's pp data, so pure photons  = " << nPurePhos  << " out of  candidates = " << nIsoPhos << endl;
  }

  cout << " nPurePhos = " <<nPurePhos << "    : purity = " << purity << "   out of  nIsoPhos = " << nIsoPhos <<endl;
  cout << " Number of Sideband Photons = " << nNonIsoPhos << endl;
  cout << " ========================================================== " << endl;
  
  
  
  
  
}
double corrFunctionTrk::getNorm() {
  return nPurePhos;
}



void corrFunctionTrk::calCorr() {
  if (isNormed) {
    cout << "WARNING!!! IT's already normalized!!!!!!"  <<endl;
      return;
  }
  
  isNormed = true;
  
  calCorrDiff(hTrkPtInFragJet);
  calCorrDiff(hTrkX);
  calCorrDiff(hTrkXi);
  calCorrDiff(hTrkXij);
  calCorrDiff(hIncXi05);
  calCorrDiff(hIncXi10);
  calCorrDiff(hTrkDphi);
  calCorrDiff(hIncKt05);
  calCorrDiff(hIncKt10);

  //  calCorrDiff(hDphiCorr);
  //  calCorrDiff2D(h2dCorr);
}

void corrFunctionTrk::calCorrDiff(TH1D* corrF[4][7]) {

  // First Divide by repetition number of data mixing                                                
  corrF[kIsoPho][kHydTrk]->Scale(1./nMixing1);
  corrF[kIsoPho][kMBJetTrk]->Scale(1./nMixing1);
  corrF[kIsoPho][kMBxTrk]->Scale(1./nMixing1);
  corrF[kDecay][kHydTrk]->Scale(1./nMixing1);
  corrF[kDecay][kMBJetTrk]->Scale(1./nMixing1);
  corrF[kDecay][kMBxTrk]->Scale(1./nMixing1);


  // Per Isolated Photon Candidate                                                                               
  corrF[kIsoPho][kRawTrk]->Scale( 1.  / nIsoPhos );
  corrF[kIsoPho][kHydTrk]->Scale( 1.  / nIsoPhos );
  corrF[kIsoPho][kMBJetTrk]->Scale( 1.  / nIsoPhos );
  corrF[kIsoPho][kMBxTrk]->Scale( 1.  / nIsoPhos );
  
  corrF[kDecay][kRawTrk]->Scale( 1.  /nNonIsoPhos);
  corrF[kDecay][kHydTrk]->Scale( 1.  / nNonIsoPhos );
  corrF[kDecay][kMBJetTrk]->Scale( 1.  / nNonIsoPhos );
  corrF[kDecay][kMBxTrk]->Scale( 1.  / nNonIsoPhos );
  
  // pure tracks                                                                                                           
  corrF[kIsoPho][kHardTrk]->Reset();
  corrF[kIsoPho][kHardTrk]->Add(corrF[kIsoPho][kRawTrk]);
  corrF[kIsoPho][kHardTrk]->Add(corrF[kIsoPho][kHydTrk],-1);
  corrF[kIsoPho][kHardMBTrk]->Reset();
  corrF[kIsoPho][kHardMBTrk]->Add(corrF[kIsoPho][kMBJetTrk]);
  corrF[kIsoPho][kHardMBTrk]->Add(corrF[kIsoPho][kMBxTrk],-1);
  corrF[kIsoPho][kFinalGoodTrk]->Reset();
  corrF[kIsoPho][kFinalGoodTrk]->Add(corrF[kIsoPho][kHardTrk]);
  corrF[kIsoPho][kFinalGoodTrk]->Add(corrF[kIsoPho][kHardMBTrk],-1);

  corrF[kDecay][kHardTrk]->Reset();
  corrF[kDecay][kHardTrk]->Add(corrF[kDecay][kRawTrk]);
  corrF[kDecay][kHardTrk]->Add(corrF[kDecay][kHydTrk],-1);
  corrF[kDecay][kHardMBTrk]->Reset();
  corrF[kDecay][kHardMBTrk]->Add(corrF[kDecay][kMBJetTrk]);
  corrF[kDecay][kHardMBTrk]->Add(corrF[kDecay][kMBxTrk],-1);
  corrF[kDecay][kFinalGoodTrk]->Reset();
  corrF[kDecay][kFinalGoodTrk]->Add(corrF[kDecay][kHardTrk]);
  corrF[kDecay][kFinalGoodTrk]->Add(corrF[kDecay][kHardMBTrk],-1);

  // We have nIsoPhos Candidates -  nIsoPhos*(1-purity) of pi0s                                                               
  // Then should divide by nPurePhos = nIsoPhos * purity                                         
  // pure photons and pure tracks
  corrF[kPurePho][kFinalGoodTrk]->Reset();
  corrF[kPurePho][kFinalGoodTrk]->Add(corrF[kIsoPho][kFinalGoodTrk], nIsoPhos);
  corrF[kPurePho][kFinalGoodTrk]->Add(corrF[kDecay][kFinalGoodTrk] , -1 * nIsoPhos*(1-purity) );
  corrF[kPurePho][kFinalGoodTrk]->Scale(1./nPurePhos);

}

/* deprecated
   void corrFunctionTrk::calCorrDiff2D(TH2D* corrF[4][7]) {

// First Divide by repetition number of data mixing                                                                                         
  corrF[kIsoPho][kHydTrk]->Scale(1./nMixing1);
  corrF[kIsoPho][kMBJetTrk]->Scale(1./nMixing1);
  corrF[kIsoPho][kMBxTrk]->Scale(1./nMixing1);
  corrF[kDecay][kHydTrk]->Scale(1./nMixing1);
  corrF[kDecay][kMBJetTrk]->Scale(1./nMixing1);
  corrF[kDecay][kMBxTrk]->Scale(1./nMixing1);

  // Per Isolated Photon Candidate                                                                                                            
  corrF[kIsoPho][kRawTrk]->Scale( 1.  / nIsoPhos );
  corrF[kIsoPho][kHydTrk]->Scale( 1.  / nIsoPhos );
  corrF[kIsoPho][kMBJetTrk]->Scale( 1.  / nIsoPhos );
  corrF[kIsoPho][kMBxTrk]->Scale( 1.  / nIsoPhos );

  corrF[kDecay][kRawTrk]->Scale( 1.  /nNonIsoPhos);
  corrF[kDecay][kHydTrk]->Scale( 1.  / nNonIsoPhos );
  corrF[kDecay][kMBJetTrk]->Scale( 1.  / nNonIsoPhos );
  corrF[kDecay][kMBxTrk]->Scale( 1.  / nNonIsoPhos );

  // pure tracks                                                                                                                              
  corrF[kIsoPho][kHardTrk]->Reset();
  corrF[kIsoPho][kHardTrk]->Add(corrF[kIsoPho][kRawTrk]);
  corrF[kIsoPho][kHardTrk]->Add(corrF[kIsoPho][kHydTrk],-1);
  corrF[kIsoPho][kHardMBTrk]->Reset();
  corrF[kIsoPho][kHardMBTrk]->Add(corrF[kIsoPho][kMBJetTrk]);
  corrF[kIsoPho][kHardMBTrk]->Add(corrF[kIsoPho][kMBxTrk],-1);
  corrF[kIsoPho][kFinalGoodTrk]->Reset();
  corrF[kIsoPho][kFinalGoodTrk]->Add(corrF[kIsoPho][kHardTrk]);
  corrF[kIsoPho][kFinalGoodTrk]->Add(corrF[kIsoPho][kHardMBTrk],-1);

  corrF[kDecay][kHardTrk]->Reset();
  corrF[kDecay][kHardTrk]->Add(corrF[kDecay][kRawTrk]);
  corrF[kDecay][kHardTrk]->Add(corrF[kDecay][kHydTrk],-1);
  corrF[kDecay][kHardMBTrk]->Reset();
  corrF[kDecay][kHardMBTrk]->Add(corrF[kDecay][kMBJetTrk]);
  corrF[kDecay][kHardMBTrk]->Add(corrF[kDecay][kMBxTrk],-1);
  corrF[kDecay][kFinalGoodTrk]->Reset();
  corrF[kDecay][kFinalGoodTrk]->Add(corrF[kDecay][kHardTrk]);
  corrF[kDecay][kFinalGoodTrk]->Add(corrF[kDecay][kHardMBTrk],-1);

  // We have nIsoPhos Candidates -  nIsoPhos*(1-purity) of pi0s                                                                               
  // Then should divide by nPurePhos = nIsoPhos * purity                                                                                      
  // pure photons and pure tracks                                                                                                             
  corrF[kPurePho][kFinalGoodTrk]->Reset();
  corrF[kPurePho][kFinalGoodTrk]->Add(corrF[kIsoPho][kFinalGoodTrk], nIsoPhos);
  corrF[kPurePho][kFinalGoodTrk]->Add(corrF[kDecay][kFinalGoodTrk] , -1 * nIsoPhos*(1-purity) );
  corrF[kPurePho][kFinalGoodTrk]->Scale(1./nPurePhos);

}


*/
