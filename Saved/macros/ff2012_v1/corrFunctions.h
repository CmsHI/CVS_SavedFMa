#include <TF1.h>
#include <TH1D.h>
#include <TMath.h>
#include "commonUtility.h"

const float awayRange= 3.141592 /8.;
const int nMixing = 40;

int kEvtMix  = 1;
int kDphiJet = 2;

class corrFunctions
{
 public:
  void initAll(bool fIsMC, int icent, int fbkgJetType);
  void calCorr();
  void setNorm();
  double getNorm();
  void calJetPtPure();

  ~corrFunctions(){};

  //      int GetBinNumber(double c);                                                                                   
  TH1D *isoPhotonEt;
  TH1D *nonIsoPhoEt;
  
  TH1D *jetDphi;   //todo : add name welIso
  TH1D *jetDphiMix;
  TH1D *jetDphiPureJ;

  TH1D *jetDphiNIpho;
  TH1D *jetDphiMixNIpho;
  TH1D *jetDphiPureJNIpho; 
  
  TH1D *jetDphiPureJPurePho;

  TH1D *jetDphiPurePho;  // fake jets are not subtracted yet
  TH1D *jetPtAway;
  TH1D *jetPtSBdphi;
  TH1D *jetPtMix;
  TH1D *jetPtPure;
  
  TH1D *xAwayWellIso;
  TH1D *xAwayNonIso;
  TH1D *xsbPhiWellIso;
  TH1D *xsbPhiNonIso;

  TH1D *xMJetWellIso;
  TH1D *xMJetNonIso;
  
  
  TH1D *xPureJetWIpho;
  TH1D *xPureJetNIpho;
  
  TH1D *xFinal;
  TH1D *xFinal2b;
  
  // todo : add MC to ensure that no subtraction on non iso
  
  int nBinsX;
  float xMinX;
  float xMaxX;

  double nfJets;
  double nfJetsErr;
  double nfJetsl;
  double nfJetsErrl;
  double nfJetsh;
  double nfJetsErrh;
  double meanx;
  double meanxErr;

  double rx1;
  double rxErr1;
  double rx0;
  double rxErr0;

 private: 
  bool isMC;
  int icent;
  float purity;
  bool isNormed;
  double nIsoPhos;
  double nPurePhos;
  double nNonIsoPhos;
  int bkgJetType;
};


void corrFunctions::initAll(bool fIsMC, int ficent,int fbkgJetType)   {
  bkgJetType = fbkgJetType;
  isNormed = false;
  nIsoPhos    = 0;
  nPurePhos   = 0;
  nNonIsoPhos = 0;
  isMC = fIsMC;
  nBinsX = 20; // Should be even!!!
  xMinX  = 0;
  xMaxX  = 1.99999;
  
  icent = ficent;
  
  if ( isMC )   
    purity = 1 ;
  else    { 
    if      ( icent == 1 )
      purity = 0.73;
    else if ( icent == 2)
      purity = 0.77;
    else if ( icent == 3)
      purity = 0.79;
    else if ( icent == 4)
      purity = 0.84;
  }
  
  TString hName1;
  if ( isMC ) 
    hName1 = Form("mc_icent%d",icent);
  else 
    hName1 = Form("data_icent%d",icent);
  
  isoPhotonEt  = new TH1D(Form("%s_isoPhotonEt",hName1.Data()),";Selected Photon E_{T} (GeV);Entries per 10GeV",30,0,300);
  nonIsoPhoEt  = (TH1D*)isoPhotonEt->Clone(Form("%s_nonIsoPhoEt",hName1.Data()));
  jetDphi      = new TH1D(Form("%s_jetDphi"  ,hName1.Data()),";#gamma-jet #Delta#phi ",20,0,3.141592);
  jetDphiMix   = new TH1D(Form("%s_jetDphiMix"  ,hName1.Data()),";#gamma-jet #Delta#phi ",20,0,3.141592);
  jetDphiPureJ   = new TH1D(Form("%s_jetDphiPureJ"  ,hName1.Data()),";#gamma-jet #Delta#phi ",20,0,3.141592);

  jetDphiNIpho   = (TH1D*)jetDphiPureJ->Clone(Form("%s_jetDphiNIpho"  ,hName1.Data()));
  jetDphiMixNIpho   = (TH1D*)jetDphiPureJ->Clone(Form("%s_jetDphiMixNIpho"  ,hName1.Data()));
  jetDphiPureJNIpho  = (TH1D*)jetDphiPureJ->Clone(Form("%s_jetDphiPureJNIpho"  ,hName1.Data()));
  jetDphiPureJPurePho   = (TH1D*)jetDphiPureJ->Clone(Form("%s_jetDphiPureJPurePho"  ,hName1.Data()));
  
  jetDphiPurePho = (TH1D*)jetDphiPureJ->Clone(Form("%s_jetDphiPurePho"  ,hName1.Data()));
  
  
  
  jetPtAway   = new TH1D(Form("%s_jetPtAway",hName1.Data()),";Jet p_{T};Entries / N_{#gamma}  ",40,0,400);
  jetPtSBdphi = new TH1D(Form("%s_jetPtSBdphi",hName1.Data()),";Jet p_{T};Entries / N_{#gamma}",40,0,400);
  jetPtMix    = new TH1D(Form("%s_jetPtMix",hName1.Data()),";Jet p_{T};Entries / N_{#gamma}",40,0,400);
  jetPtPure   = new TH1D(Form("%s_jetPtPure",hName1.Data()),";Jet p_{T};Entreis / N_{#gamma} ",40,0,400);
  
 
  isoPhotonEt->Sumw2();
  nonIsoPhoEt->Sumw2();
  jetDphi->Sumw2();
  jetDphiMix->Sumw2();
  jetPtAway->Sumw2();
  jetPtSBdphi->Sumw2();
  jetPtPure->Sumw2();
  
  xAwayWellIso  = new TH1D(Form("%s_xAwayWellIso",hName1.Data()), ";x_{#gammaj};Normalized per trigger",nBinsX, xMinX, xMaxX);
  handsomeTH1(xAwayWellIso);
  xAwayNonIso   = (TH1D*)xAwayWellIso->Clone(Form("%s_xAwayNonIso",hName1.Data()));
  xsbPhiWellIso = (TH1D*)xAwayWellIso->Clone(Form("%s_xsbPhiWellIso",hName1.Data()));
  xsbPhiNonIso  = (TH1D*)xAwayWellIso->Clone(Form("%s_xsbPhiNonIso",hName1.Data()));
  xMJetWellIso  = (TH1D*)xAwayWellIso->Clone(Form("%s_xMJetWellIso",hName1.Data()));
  xMJetNonIso  = (TH1D*)xAwayWellIso->Clone(Form("%s_xMJetNonIso",hName1.Data()));
  xPureJetWIpho = (TH1D*)xAwayWellIso->Clone(Form("%s_xPureJetWIpho",hName1.Data()));
  xPureJetNIpho = (TH1D*)xAwayWellIso->Clone(Form("%s_xPureJetNIpho",hName1.Data()));
  xFinal        = (TH1D*)xAwayWellIso->Clone(Form("%s_xFinal",hName1.Data()));




  
  xAwayWellIso->Sumw2();
  xAwayNonIso->Sumw2();
  xsbPhiWellIso->Sumw2();
  xsbPhiNonIso->Sumw2();
  xPureJetWIpho->Sumw2();
  xPureJetNIpho->Sumw2();
  xFinal->Sumw2();
    
}

void corrFunctions::setNorm() {
  if (isNormed) {
    cout << "WARNING!!! IT's already normalized!!!!!! setNorm() error"<<endl;
    return;
  }

  nIsoPhos = isoPhotonEt->Integral();
  if (nIsoPhos==0)  cout << " strange n_photon = 0 " << endl;
  if (isMC) {
    nPurePhos= nIsoPhos;
    cout << " it's mc, so nIsoPhos = " << nIsoPhos << " and  nPurePhos = " << nPurePhos <<endl;
  }
  else {
    nPurePhos = nIsoPhos * purity;
    cout << " it's data, so nIsoPhos = " << nIsoPhos << " and  nphots = " << nPurePhos << endl;
  }
 
  // Non Iso phos
  nNonIsoPhos =  nonIsoPhoEt->Integral();
  cout << " Number of Sideband Non-Iso Photons = " << nNonIsoPhos << endl;
  

}
double corrFunctions::getNorm() {
  return nPurePhos;
}

void corrFunctions::calCorr() {
  if (isNormed) {
    cout << "WARNING!!! IT's already normalized!!!!!!"  <<endl;
      return;
  }
  
  isNormed = true;
  
  //  First
  // Todo : automate the scaling factor ... currently it's set as 5                                                                        
  
  // First Divide by repetition number of data mixing
  jetPtMix->Scale(1./nMixing);
  jetDphiMix->Scale(1./nMixing);
  xMJetWellIso->Scale(1./nMixing);
  xMJetNonIso->Scale(1./nMixing);
  jetDphiMixNIpho->Scale(1./nMixing);
  

  // per isolated photon
  jetDphi->Scale(1./nIsoPhos);
  jetDphiMix->Scale(1. /nIsoPhos);
  jetPtAway->Scale(          1. / ( nIsoPhos)   );
  jetPtSBdphi->Scale( awayRange / ( (PI/2.-0.7) * nIsoPhos )   );  //todo : integrate calJetPtPure into this funciton
  jetPtMix->Scale(  1. / ( nIsoPhos)  ) ;
 
    
  
  cout << " nPurePhos = " <<nPurePhos << "    and purity = " << purity << "   and nIsoPhos = " << nIsoPhos <<endl;
  
  // Per Isolated Photon Candidate
  xAwayWellIso->Scale( 1.  / nIsoPhos );
  xsbPhiWellIso->Scale( (awayRange / (PI/2.-0.7)) / nIsoPhos );
  xMJetWellIso->Scale( 1.  / nIsoPhos );

  xPureJetWIpho->Reset();
  xPureJetWIpho->Add(xAwayWellIso,1);
  if ( bkgJetType ==kEvtMix )
    xPureJetWIpho->Add(xMJetWellIso,-1);
  else if ( bkgJetType == kDphiJet)
    xPureJetWIpho->Add(xsbPhiWellIso,-1);
 
  // Per Pi0s..
  xAwayNonIso->Scale( 1. / nNonIsoPhos );
  xsbPhiNonIso->Scale( ( awayRange / (PI/2.-0.7) ) / nNonIsoPhos );
  xMJetNonIso->Scale(1. / nNonIsoPhos );

  jetDphiNIpho->Scale(1. / nNonIsoPhos );
  jetDphiMixNIpho->Scale(1. / nNonIsoPhos );



  // Calculate dN/dPhi!!!
  jetDphiPureJ->Reset();
  jetDphiPureJ->Add(jetDphi);
  jetDphiPureJ->Add(jetDphiMix,-1);
  
  jetDphiPureJNIpho->Reset();
  jetDphiPureJNIpho->Add(jetDphiNIpho);
  jetDphiPureJNIpho->Add(jetDphiMixNIpho,-1);
  // jetDphiPureJPurePho will be calculated below.

  

  xPureJetNIpho->Reset();
  xPureJetNIpho->Add(xAwayNonIso,1);
  if ( bkgJetType ==kEvtMix )
    xPureJetNIpho->Add(xMJetNonIso,-1);
  else if ( bkgJetType == kDphiJet)
    xPureJetNIpho->Add(xsbPhiNonIso,-1);
  
  
  // We have nIsoPhos Candidates -  nIsoPhos*(1-purity) of pi0s
  // Then should divide by nPurePhos = nIsoPhos * purity
  
  // DPhi first
  jetDphiPureJPurePho->Reset();
  jetDphiPureJPurePho->Add(jetDphiPureJ,nIsoPhos);
  jetDphiPureJPurePho->Add(jetDphiPureJNIpho, -1 * nIsoPhos*(1-purity));
  jetDphiPureJPurePho->Scale(1./nPurePhos);


  //// Just for the dphi sideband study                                                                              
  jetDphiPurePho->Reset();
  jetDphiPurePho->Add(jetDphi,nIsoPhos);
  jetDphiPurePho->Add(jetDphiNIpho, -1 * nIsoPhos*(1-purity));
  jetDphiPurePho->Scale(1./nPurePhos);
  ////////////////                                                                                                                         



  // x    
  xFinal->Reset();
  xFinal->Add(xPureJetWIpho, nIsoPhos);
  xFinal->Add(xPureJetNIpho, -1 * nIsoPhos*(1-purity));
  xFinal->Scale(1./nPurePhos);

  xFinal2b = (TH1D*)xFinal->Clone(Form("%s_2bin",xFinal->GetName()));
  if ( nBinsX%2 == 1)  
    cout << "nBinsX is not even!!!!" << endl;
  xFinal2b->Rebin(nBinsX/2);

  nfJetsl = xFinal2b->GetBinContent(1);
  nfJetsErrl = xFinal2b->GetBinError(1);
  nfJetsh = xFinal2b->GetBinContent(2);
  nfJetsErrh = xFinal2b->GetBinError(2);

  nfJets = nfJetsl + nfJetsh;
  nfJetsErr = sqrt( nfJetsErrl*nfJetsErrl + nfJetsErrh*nfJetsErrh);
  
  meanx   = xFinal->GetMean();
  meanxErr= xFinal->GetMeanError();
  
  rx0      = nfJets;
  rxErr0   = nfJetsErr;

  rx1      = nfJetsh/nfJets;
  rxErr1   = sqrt( nfJetsl*nfJetsl*nfJetsErrh*nfJetsErrh + nfJetsh*nfJetsh*nfJetsErrl*nfJetsErrl ) / (nfJets*nfJets);
  // cout << " low : " << nfJetsl << " +-"<<nfJetsErrl << endl;
  //  cout << " high : " << nfJetsh << " +-"<<nfJetsErrh << endl;
  //  cout << " Total : " << nfJets << " +-"<<nfJetsErr << endl;
  cout << "meanX = " << meanx << "+- " << meanxErr << endl;
  cout << "rx = " << rx1 << "+- " << rxErr1 << endl;

  // Done!!!
}



void corrFunctions::calJetPtPure() {
  jetPtPure->Reset();  
  jetPtPure->Add(jetPtAway);
  if ( bkgJetType ==kEvtMix )
    jetPtPure->Add(jetPtMix,-1);
  else if ( bkgJetType == kDphiJet)
    jetPtPure->Add(jetPtSBdphi,-1);
}
