#include <iostream>
#include "TFile.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TString.h"
#include "savedfrankTools.C"

void plotLeading(char * infname1 = "/net/pstore01/d00/scratch/frankma/hiroot/pythia100_5k_truth_000777.root",
                 char * infname2 = "/net/pstore01/d00/scratch/frankma/hiroot/pyquen100_5k_truth_000777.root")
{
   printf("%s\n",infname1);
   printf("%s\n",infname2);
   TFile * infile = new TFile(infname1);
   TNtuple * ntpythia = dynamic_cast<TNtuple*>(infile->Get("NTLPartons"));
   TFile * infile2 = new TFile(infname2);
   TNtuple * ntpyquen = dynamic_cast<TNtuple*>(infile2->Get("NTLPartons"));

//   getHistos(ntpythia, "Pythia");

   //--- inv mass ---
   drawTree(ntpythia, "mass>>hMassPythia","nlpet>1 && alpet>1","",    "hMassPythia","Pythia: inv mass of dijet",100,0,1800,true,kRed,1,3);
   drawTree(ntpyquen, "mass>>hMassPyquen","nlpet>1 && alpet>1","same","hMassPyquen","draw Pyquen: inv mass of dijet",100,0,1800,true,kBlue,1,3);
   
   //--- Et ---
   // -near-
   drawTree(ntpythia, "nlpet>>hNearLPartonPythia","nlpet>1","",    "hNearLPartonPythia","Pythia: leading partons",100,80,200,true,kRed,1,3);
   drawTree(ntpyquen, "nlpet>>hNearLPartonPyquen","nlpet>1","same","hNearLPartonPyquen","Pyquen: leading partons",100,80,200,true,kBlue,1,3);
   // -away--
   drawTree(ntpythia, "alpet>>hAwayLPartonPythia","alpet>1","same","hAwayLPartonPythia","Pythia: leading partons",100,80,200,true,kRed,7,3);
   drawTree(ntpyquen, "alpet>>hAwayLPartonPyquen","alpet>1","same","hAwayLPartonPyquen","draw Pyquen: leading partons",100,80,200,true,kBlue,7,3);

   //--- dphi ---
   drawTree(ntpythia, "dphi>>hdPhiPythia","nlpet>1 && alpet>1","","hdPhiPythia","Pythia: dphi of leading partons",100,0,3.14,true,kRed,1,3);
   drawTree(ntpyquen, "dphi>>hdPhiPyquen","nlpet>1 && alpet>1","same","hdPhiPyquen","draw Pyquen: dphi of leading partons",100,0,3.14,true,kBlue,1,3);

   //--- Et ratio ---
   drawTree(ntpythia, "(alpet/nlpet)>>hEtRatioPythia","","","hEtRatioPythia","Pythia: Et ratio of leading partons",100,0,1.5,true,kRed,1,3);
   drawTree(ntpyquen, "(alpet/nlpet)>>hEtRatioPyquen","","same","hEtRatioPyquen","draw Pyquen: Et ratio of leading partons",100,0,1.5,true,kBlue,1,3);

   printAllCanvases();
}
