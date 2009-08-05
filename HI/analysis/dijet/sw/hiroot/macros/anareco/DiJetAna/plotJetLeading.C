#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TString.h"
#include "ana/savedfrankTools.C"

const Int_t ETAMIN=-4;
const Int_t ETAMAX=4;
const UInt_t NBIN=100;

const char * drsgE="E1";
const char * drdbE="E1 same";
const char * drsg="hist";
//const char * drdb="E1 hist same";
const char * drdb="hist same";
TString drsg2("colz");
TString drsg2nc("BOX");
TString drdb2nc("same");

void plotJetLeading(bool doCheck=true,
	         char * infname1 = "/net/pstore01/d00/scratch/frankma/hiroot/pythia100_5k_dijet_000777.root",
                 char * infname2 = "/net/pstore01/d00/scratch/frankma/hiroot/pyquen100_5k_dijet_000777.root",
		 char * plotdir = "plots")
{
   printf("%s\n",infname1);
   printf("%s\n",infname2);
   TFile * infile = new TFile(infname1);
//   TNtuple * ntPythia = findTree(infile,"NTLPartons");
//   TNtuple * ntPythia = findTree(infile,"NTPartonLeading");
   TTree * ntPythia = findTree(infile,"evtTreeJetFF");
   TFile * infile2 = new TFile(infname2);
//   TNtuple * ntPyquen = findTree(infile2,"NTPartonLeading");
//   TNtuple * ntPyquen = findTree(infile2,"NTJetLeading");
   TTree * ntPyquen = findTree(infile2,"evtTreeJetFF");
   TFile * outfile = new TFile("LeadingHistos.root","RECREATE");

   //--- If do checks ---
   if (doCheck) {
   //--- inv mass ---
//   drawTree(ntPythia, "mass>>hMassPythia","nlpet>1 && alpet>1",drsg,    "hMassPythia","Pythia: inv mass of dijet",100,0,1800,true,kRed,1,3);
//   drawTree(ntPyquen, "mass>>hMassPyquen","nlpet>1 && alpet>1",drdb,"hMassPyquen","draw Pyquen: inv mass of dijet",100,0,1800,true,kBlue,1,3);
      drawTree2(ntPythia, "nljet:nlpet>>hNearJEtPEtPythia","",drsg2,"hNearJEtPEtPythia",";Parton Et [GeV];Jet Et [GeV]",NBIN,0,200,NBIN,0,200,2);
      drawTree2(ntPythia, "nljphi:nlpphi>>hNearJPhiPPhiPythia","",drsg2,"hNearJPhiPPhiPythia",";Parton Phi [GeV];Jet Phi [GeV]",NBIN,-1*PI,PI,NBIN,-1*PI,PI,2);
      drawTree2(ntPythia, "nljeta:nlpeta>>hNearJEtaPEtaPythia","",drsg2,"hNearJEtaPEtaPythia",";Parton Eta [GeV];Jet Eta [GeV]",NBIN,-1*ETAMAX,ETAMAX,NBIN,-1*ETAMAX,ETAMAX,2);
      drawNormHist(infile, "hJetEtDist",drsg,"","Jet Et [GeV]","#",1,true,2,7,3,1,1);
      drawTree(ntPythia, "nljet>>hCheckNearLJetPythia","",drdb,    "hCheckNearLJetPythia",";Et [GeV];",100,0,200,true,kRed,1,3,1,8);
      drawTree(ntPyquen, "nljet>>hCheckNearLJetPyquen","",drdb,"hCheckNearLJetPyquen",";Et [GeV];",100,0,200,true,kRed-2,1,3,1,8);
      drawTree(ntPythia, "nljet>>hCheckCutNearLJetPythia","nljet>70",drdbE,    "hCheckCutNearLJetPythia",";Et [GeV];",100,0,200,true,kBlack,1,3,1,8);
      drawTree(ntPyquen, "nljet>>hCheckCutNearLJetPyquen","nljet>70",drdbE,"hCheckCutNearLJetPyquen",";Et [GeV];",100,0,200,true,kBlue,1,3,1,8);
   }
   
   //--- Et ---
   // -near-
   drawTree(ntPythia, "nlpet>>hNearLPartonPythia","",drsg,    "hNearLPartonPythia",";(near jet) Et [GeV];",100,0,200,true,kRed,1,3);
   drawTree(ntPythia, "nljet>>hNearLJetPythia","",drdbE,    "hNearLJetPythia",";Et [GeV];",100,0,200,true,kBlack,1,3,1,8);
   drawTree(ntPyquen, "nlpet>>hNearLPartonPyquen","",drdb,"hNearLPartonPyquen",";Et [GeV];",100,0,200,true,kRed-2,1,3);
   drawTree(ntPyquen, "nljet>>hNearLJetPyquen","",drdbE,"hNearLJetPyquen",";Et [GeV];",100,0,200,true,kBlue,1,3,1,8);
//   // -away--
   drawTree(ntPythia, "alpet>>hAwayLPartonPythia","",drsg,"hAwayLPartonPythia",";(away jet) Et [GeV];",100,0,200,true,kRed,1,3);
   drawTree(ntPythia, "aljet>>hAwayLJetPythia","",drdbE,"hAwayLJetPythia",";Et [GeV];",100,0,200,true,kBlack,1,3,1,8);
   drawTree(ntPyquen, "alpet>>hAwayLPartonPyquen","",drdb,"hAwayLPartonPyquen",";Et [GeV];",100,0,200,true,kRed-2,1,3);
   drawTree(ntPyquen, "aljet>>hAwayLJetPyquen","",drdbE,"hAwayLJetPyquen",";Et [GeV];",100,0,200,true,kBlue,1,3,1,8);
//
//   //--- dphi ---
//   drawTree(ntPythia, "dphi>>hdPhiPythia","nlpet>1 && alpet>1",drsg,"hdPhiPythia","Pythia: dphi of leading partons",100,0,3.14,true,kRed,1,3);
//   drawTree(ntPyquen, "dphi>>hdPhiPyquen","nlpet>1 && alpet>1",drdb,"hdPhiPyquen","draw Pyquen: dphi of leading partons",100,0,3.14,true,kBlue,1,3);
//
//   //--- near/away Et ratio ---
   drawTree(ntPythia, "(alpet/nlpet)>>hPEtRatioPythia","",drsg,"hPEtRatioPythia",";(away Et)/(near Et);",100,0,1.5,true,kRed,1,3);
   drawTree(ntPyquen, "(alpet/nlpet)>>hPEtRatioPyquen","",drdb,"hPEtRatioPyquen",";(away Et)/(near Et);",100,0,1.5,true,kRed-2,1,3);
   //--- Away/Near Jet Leading ---
   drawTree(ntPythia, "(aljet/nljet)>>hJEtRatioPythia","",drdb,"hJEtRatioPythia",";(away Et)/(near Et);",100,0,1.5,true,kBlack,1,3);
   drawTree(ntPyquen, "(aljet/nljet)>>hJEtRatioPyquen","",drdb,"hJEtRatioPyquen",";(away Et)/(near Et);",100,0,1.5,true,kBlue,1,3);

   //=== jet/parton Et ratio ===
   // -near-
   drawTree(ntPythia, "(nljet/nlpet)>>hNJEtPEtRatioPythia","",drsgE,"hNJEtPEtRatioPythia",";E_{t}^{Jet}/E_{t}^{Parton};",100,0,1.5,true,kBlack,1,3,1,8);
   drawTree(ntPyquen, "(nljet/nlpet)>>hNJEtPEtRatioPyquen","",drdbE,"hNJEtPEtRatioPyquen",";E_{t}^{Jet}/E_{t}^{Parton};",100,0,1.5,true,kBlue,1,3,1,8);
   // -away-
   drawTree(ntPythia, "(aljet/alpet)>>hAJEtPEtRatioPythia","",drdbE,"hAJEtPEtRatioPythia",";(away) E_{t}^{Jet}/E_{t}^{Parton};",100,0,1.5,true,kBlack,1,3,1,4);
   drawTree(ntPyquen, "(aljet/alpet)>>hAJEtPEtRatioPyquen","",drdbE,"hAJEtPEtRatioPyquen",";E_{t}^{Jet}/E_{t}^{Parton};",100,0,1.5,true,kBlue,1,3,1,4);
  //--- as a function of parton Et---
   // -near-
   drawTree2(ntPythia, "(nljet/nlpet):nlpet>>hNearJEtPEtRatPythia2","",drsg2,"hNearJEtPEtRatPythia2",";Parton Et [GeV];E_{t}^{Jet}/E_{t}^{Parton}",NBIN,80,200,NBIN,0,1.5,2);
   //=== Now plot the FF ===
   //plotFF(infname1,infname2);

   //-------------- Done Save Plots----------------------
   printAllCanvases(plotdir);
   outfile->Write();
   outfile->Close();
}
