#include <iostream>
#include "TFile.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TString.h"
#include "ana/savedfrankTools.C"

//const char * drsg="E1 hist";
const char * drsg="hist";
//const char * drdb="E1 hist same";
const char * drdb="hist same";
TString drsg2("colz");
const Int_t ETAMIN=-4;
const Int_t ETAMAX=4;
const UInt_t NBIN=100;

void plotJetLeading(bool doCheck=true,
	         char * infname1 = "/net/pstore01/d00/scratch/frankma/hiroot/pythia100_5k_dijet_000777.root",
                 char * infname2 = "/net/pstore01/d00/scratch/frankma/hiroot/pyquen100_5k_dijet_000777.root")
{
   printf("%s\n",infname1);
   printf("%s\n",infname2);
   TFile * infile = new TFile(infname1);
//   TNtuple * ntPythia = dynamic_cast<TNtuple*>(infile->Get("NTLPartons"));
//   TNtuple * ntPythia = dynamic_cast<TNtuple*>(infile->Get("NTPartonLeading"));
   TNtuple * ntPythia = dynamic_cast<TNtuple*>(infile->Get("NTJetLeading"));
   TFile * infile2 = new TFile(infname2);
//   TNtuple * ntPyquen = dynamic_cast<TNtuple*>(infile2->Get("NTPartonLeading"));
   TNtuple * ntPyquen = dynamic_cast<TNtuple*>(infile2->Get("NTJetLeading"));
   TFile * outfile = new TFile("LeadingHistos.root","RECREATE");

   //--- If do checks ---
   if (doCheck) {
   //--- inv mass ---
//   drawTree(ntPythia, "mass>>hMassPythia","nlpet>1 && alpet>1",drsg,    "hMassPythia","Pythia: inv mass of dijet",100,0,1800,true,kRed,1,3);
//   drawTree(ntPyquen, "mass>>hMassPyquen","nlpet>1 && alpet>1",drdb,"hMassPyquen","draw Pyquen: inv mass of dijet",100,0,1800,true,kBlue,1,3);
      drawTree2(ntPythia, "nljet:nlpet>>hNearJEtPEtPythia","",drsg2,"hNearJEtPEtPythia",";Parton Et [GeV];Jet Et [GeV]",NBIN,0,200,NBIN,0,200,2);
      drawTree2(ntPythia, "nljphi:nlpphi>>hNearJPhiPPhiPythia","",drsg2,"hNearJPhiPPhiPythia",";Parton Phi [GeV];Jet Phi [GeV]",NBIN,-1*PI,PI,NBIN,-1*PI,PI,2);
      drawTree2(ntPythia, "nljeta:nlpeta>>hNearJEtaPEtaPythia","",drsg2,"hNearJEtaPEtaPythia",";Parton Eta [GeV];Jet Eta [GeV]",NBIN,-1*ETAMAX,ETAMAX,NBIN,-1*ETAMAX,ETAMAX,2);
   }
   
   //--- Et ---
   // -near-
   drawTree(ntPythia, "nlpet>>hNearLPartonPythia","",drsg,    "hNearLPartonPythia",";Et [GeV];",100,0,200,true,kRed,1,3);
   drawTree(ntPythia, "nljet>>hNearLJetPythia","",drdb,    "hNearLJetPythia",";Et [GeV];",100,0,200,true,kBlack,1,3);
   drawTree(ntPyquen, "nlpet>>hNearLPartonPyquen","",drdb,"hNearLPartonPyquen",";Et [GeV];",100,0,200,true,kRed-2,1,3);
   drawTree(ntPyquen, "nljet>>hNearLJetPyquen","",drdb,"hNearLJetPyquen",";Et [GeV];",100,0,200,true,kBlue,1,3);
//   // -away--
   drawTree(ntPythia, "alpet>>hAwayLPartonPythia","",drdb,"hAwayLPartonPythia",";Et [GeV];",100,0,200,true,kRed,7,3);
   drawTree(ntPythia, "aljet>>hAwayLJetPythia","",drdb,"hAwayLJetPythia",";Et [GeV];",100,0,200,true,kBlack,7,3);
   drawTree(ntPyquen, "alpet>>hAwayLPartonPyquen","",drdb,"hAwayLPartonPyquen",";Et [GeV];",100,0,200,true,kRed-2,7,3);
   drawTree(ntPyquen, "aljet>>hAwayLJetPyquen","",drdb,"hAwayLJetPyquen",";Et [GeV];",100,0,200,true,kBlue,7,3);
//
//   //--- dphi ---
//   drawTree(ntPythia, "dphi>>hdPhiPythia","nlpet>1 && alpet>1",drsg,"hdPhiPythia","Pythia: dphi of leading partons",100,0,3.14,true,kRed,1,3);
//   drawTree(ntPyquen, "dphi>>hdPhiPyquen","nlpet>1 && alpet>1",drdb,"hdPhiPyquen","draw Pyquen: dphi of leading partons",100,0,3.14,true,kBlue,1,3);
//
//   //--- Et ratio ---
   drawTree(ntPythia, "(alpet/nlpet)>>hPEtRatioPythia","",drsg,"hPEtRatioPythia",";(away Et)/(near Et);",100,0,1.5,true,kRed,1,3);
   drawTree(ntPyquen, "(alpet/nlpet)>>hPEtRatioPyquen","",drdb,"hPEtRatioPyquen",";(away Et)/(near Et);",100,0,1.5,true,kRed-2,1,3);
   //--- Away/Near Jet Leading ---
   drawTree(ntPythia, "(aljet/nljet)>>hJEtRatioPythia","",drdb,"hJEtRatioPythia",";(away Et)/(near Et);",100,0,1.5,true,kBlack,1,3);
   drawTree(ntPyquen, "(aljet/nljet)>>hJEtRatioPyquen","",drdb,"hJEtRatioPyquen",";(away Et)/(near Et);",100,0,1.5,true,kBlue,1,3);

   //-------------- Done Save Plots----------------------
   printAllCanvases("plots/jleading");
   outfile->Write();
   outfile->Close();
}
