#include <TFile.h>
#include <TTree.h>
#include <TH2D.h>
#include <TCut.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
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
#include <TClonesArray.h>
#include <TCut.h>

#include "/home/kimy/script/commonUtility.h"


void getpTBins(
	  TString inFileName="jfhCorrEtaPtBin200RBin20_HCPR_J50U_djcalo_Cent10to30_Aj0to100_SubEtaRefl.root" ) {
   
   TFile *f = new TFile(inFileName);
   TH2D * hPtPNDR = (TH2D*) f->Get("hPtPNDR");
   TH2D * hPtPNDRBg = (TH2D*) f->Get("hPtPNDRBg");
   TH2D * hPtPNDRBgSub = (TH2D*)hPtPNDR->Clone("hPtPNDRBgSub");
   hPtPNDRBgSub->Add(hPtPNDR,hPtPNDRBg,1,-1);
   TH1D * hPt = (TH1D*)hPtPNDRBgSub->ProjectionX("hPt");
   getNiceBins(hPt,4);
}
