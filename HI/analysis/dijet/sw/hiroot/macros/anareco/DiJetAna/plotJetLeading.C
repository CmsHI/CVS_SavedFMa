#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TString.h"
#include "ana/savedfrankTools.C"
#include "DiJets.h"
using namespace DiJetAna;

void plotJetLeading(bool doCheck=true,
	         char * infname1 = "/net/pstore01/d00/scratch/frankma/hiroot/pythia100_5k_dijet_000777.root",
                 char * infname2 = "/net/pstore01/d00/scratch/frankma/hiroot/pyquen100_5k_dijet_000777.root",
		 char * plotdir = "plots")
{
   printf("%s\n",infname1);
   printf("%s\n",infname2);
   TFile * infile = new TFile(infname1);
//   TNtuple * trJetPythia = findTree(infile,"NTPartonLeading");
   TTree * trJetPythia = findTree(infile,"evtTreeJetFF");
   TFile * infile2 = new TFile(infname2);
//   TNtuple * trJetPyquen = findTree(infile2,"NTPartonLeading");
   TTree * trJetPyquen = findTree(infile2,"evtTreeJetFF");
   TFile * outfile = new TFile("LeadingHistos.root","RECREATE");

   //--- If do checks ---
   if (doCheck) {
   //--- inv mass ---
//   drawTree(trJetPythia, "mass>>hMassPythia","nlpet>1 && alpet>1",drsg,    "hMassPythia","Pythia: inv mass of dijet",100,0,1800,true,kRed,1,3);
//   drawTree(trJetPyquen, "mass>>hMassPyquen","nlpet>1 && alpet>1",drdb,"hMassPyquen","draw Pyquen: inv mass of dijet",100,0,1800,true,kBlue,1,3);
      drawTree2(trJetPythia, "nljet:nlpet>>hNearJEtPEtPythia","",drsg2,"hNearJEtPEtPythia",";Parton Et [GeV];Jet Et [GeV]",NBIN,0,200,NBIN,0,200,2);
      drawTree2(trJetPythia, "nljphi:nlpphi>>hNearJPhiPPhiPythia","",drsg2,"hNearJPhiPPhiPythia",";Parton Phi [GeV];Jet Phi [GeV]",NBIN,-1*PI,PI,NBIN,-1*PI,PI,2);
      drawTree2(trJetPythia, "nljeta:nlpeta>>hNearJEtaPEtaPythia","",drsg2,"hNearJEtaPEtaPythia",";Parton Eta [GeV];Jet Eta [GeV]",NBIN,-1*ETAMAX,ETAMAX,NBIN,-1*ETAMAX,ETAMAX,2);
      drawNormHist(infile, "hJetEtDist",drsg,"","Jet Et [GeV]","#",1,true,2,7,3,1,1);
      drawTree(trJetPythia, "nljet>>hCheckNearLJetPythia","",drdb,    "hCheckNearLJetPythia",";Et [GeV];",100,0,200,true,kRed,1,3,1,8);
      drawTree(trJetPyquen, "nljet>>hCheckNearLJetPyquen","",drdb,"hCheckNearLJetPyquen",";Et [GeV];",100,0,200,true,kRed-2,1,3,1,8);
      drawTree(trJetPythia, "nljet>>hCheckCutNearLJetPythia","nljet>70",drdbE,    "hCheckCutNearLJetPythia",";Et [GeV];",100,0,200,true,kBlack,1,3,1,8);
      drawTree(trJetPyquen, "nljet>>hCheckCutNearLJetPyquen","nljet>70",drdbE,"hCheckCutNearLJetPyquen",";Et [GeV];",100,0,200,true,kBlue,1,3,1,8);
   }
   
   //--- Et ---
   // -near-
   drawTree(trJetPythia, "nlpet>>hNearLPartonPythia","",drsg,    "hNearLPartonPythia",";(near jet) Et [GeV];",100,0,200,true,kRed,1,3);
   drawTree(trJetPythia, "nljet>>hNearLJetPythia","",drdbE,    "hNearLJetPythia",";Et [GeV];",100,0,200,true,kBlack,1,3,1,8);
   drawTree(trJetPyquen, "nlpet>>hNearLPartonPyquen","",drdb,"hNearLPartonPyquen",";Et [GeV];",100,0,200,true,kRed-2,1,3);
   drawTree(trJetPyquen, "nljet>>hNearLJetPyquen","",drdbE,"hNearLJetPyquen",";Et [GeV];",100,0,200,true,kBlue,1,3,1,8);
   // -away--
   drawTree(trJetPythia, "alpet>>hAwayLPartonPythia","",drsg,"hAwayLPartonPythia",";(away jet) Et [GeV];",100,0,200,true,kRed,1,3);
   drawTree(trJetPythia, "aljet>>hAwayLJetPythia","",drdbE,"hAwayLJetPythia",";Et [GeV];",100,0,200,true,kBlack,1,3,1,8);
   drawTree(trJetPyquen, "alpet>>hAwayLPartonPyquen","",drdb,"hAwayLPartonPyquen",";Et [GeV];",100,0,200,true,kRed-2,1,3);
   drawTree(trJetPyquen, "aljet>>hAwayLJetPyquen","",drdbE,"hAwayLJetPyquen",";Et [GeV];",100,0,200,true,kBlue,1,3,1,8);

   //--- dphi ---
//   drawTree(trJetPythia, "dphi>>hdPhiPythia","nlpet>1 && alpet>1",drsg,"hdPhiPythia","Pythia: dphi of leading partons",100,0,3.14,true,kRed,1,3);
//   drawTree(trJetPyquen, "dphi>>hdPhiPyquen","nlpet>1 && alpet>1",drdb,"hdPhiPyquen","draw Pyquen: dphi of leading partons",100,0,3.14,true,kBlue,1,3);

   //--- near/away Et ratio ---
   drawTree(trJetPythia, "(alpet/nlpet)>>hPEtRatioPythia","",drsg,"hPEtRatioPythia",";(away Et)/(near Et);",100,0,1.5,true,kRed,1,3);
   drawTree(trJetPyquen, "(alpet/nlpet)>>hPEtRatioPyquen","",drdb,"hPEtRatioPyquen",";(away Et)/(near Et);",100,0,1.5,true,kRed-2,1,3);
   //--- Away/Near Jet Leading ---
   drawTree(trJetPythia, "(aljet/nljet)>>hJEtRatioPythia","",drdb,"hJEtRatioPythia",";(away Et)/(near Et);",100,0,1.5,true,kBlack,1,3);
   drawTree(trJetPyquen, "(aljet/nljet)>>hJEtRatioPyquen","",drdb,"hJEtRatioPyquen",";(away Et)/(near Et);",100,0,1.5,true,kBlue,1,3);

   //=== jet/parton Et ratio ===
   // -near-
   drawTree(trJetPythia, "(nljet/nlpet)>>hNJEtPEtRatioPythia","",drsgE,"hNJEtPEtRatioPythia",";E_{t}^{Jet}/E_{t}^{Parton};",100,0,1.5,true,kBlack,1,3,1,8);
   drawTree(trJetPyquen, "(nljet/nlpet)>>hNJEtPEtRatioPyquen","",drdbE,"hNJEtPEtRatioPyquen",";E_{t}^{Jet}/E_{t}^{Parton};",100,0,1.5,true,kBlue,1,3,1,8);
   // -away-
   drawTree(trJetPythia, "(aljet/alpet)>>hAJEtPEtRatioPythia","",drdbE,"hAJEtPEtRatioPythia",";(away) E_{t}^{Jet}/E_{t}^{Parton};",100,0,1.5,true,kBlack,1,3,1,4);
   drawTree(trJetPyquen, "(aljet/alpet)>>hAJEtPEtRatioPyquen","",drdbE,"hAJEtPEtRatioPyquen",";E_{t}^{Jet}/E_{t}^{Parton};",100,0,1.5,true,kBlue,1,3,1,4);

   //--- as a function of parton Et---
   // -near-
   drawTree2(trJetPythia, "(nljet/nlpet):nlpet>>hNearJEtPEtRatPythia2","",drsg2,"hNearJEtPEtRatPythia2",";Parton Et [GeV];E_{t}^{Jet}/E_{t}^{Parton}",NBIN,80,200,NBIN,0,1.5,2);

   // === Cone info ===
   // -- # particles --
   drawTree(trJetPythia, "nljCone5NP>>hNJCone5NPPythia","",drsgE,"hNJCone5NPPythia",";# charged particles in 0.5 cone;",NBIN,0,40,0,kBlack,1,3,1,8);
   drawTree(trJetPythia, "aljCone5NP>>hAJCone5NPPythia","",drdbE,"hAJCone5NPPythia",";# charged particles in 0.5 cone;",NBIN,0,40,0,kBlack,1,3,1,4);
   drawTree(trJetPyquen, "nljCone5NP>>hNJCone5NPPyquen","",drdbE,"hNJCone5NPPyquen",";# charged particles in 0.5 cone;",NBIN,0,40,0,kBlue,1,3,1,8);
   drawTree(trJetPyquen, "aljCone5NP>>hAJCone5NPPyquen","",drdbE,"hAJCone5NPPyquen",";# charged particles in 0.5 cone;",NBIN,0,40,0,kBlue,1,3,1,4);

   // --Et--
   drawTree(trJetPythia, "nljCone5Et>>hNJCone5EtPythia","",drsgE,"hNJCone5EtPythia",";charged particles Et_{tot} in 0.5 cone;",NBIN,0,100,0,kBlack,1,3,1,8);
   drawTree(trJetPythia, "aljCone5Et>>hAJCone5EtPythia","",drdbE,"hAJCone5EtPythia",";charged particles Et_{tot} in 0.5 cone;",NBIN,0,100,0,kBlack,1,3,1,4);
   drawTree(trJetPyquen, "nljCone5Et>>hNJCone5EtPyquen","",drdbE,"hNJCone5EtPyquen",";charged particles Et_{tot} in 0.5 cone;",NBIN,0,100,0,kBlue,1,3,1,8);
   drawTree(trJetPyquen, "aljCone5Et>>hAJCone5EtPyquen","",drdbE,"hAJCone5EtPyquen",";charged particles Et_{tot} in 0.5 cone;",NBIN,0,100,0,kBlue,1,3,1,4);

   //-------------- Done Save Plots----------------------
   printAllCanvases(plotdir);
   outfile->Write();
   outfile->Close();
}
