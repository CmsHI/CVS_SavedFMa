#include <iostream>
#include "TFile.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TROOT.h"
#include "savedfrankTools.C"

void plotFF(char * infname1 = "/net/pstore01/d00/scratch/frankma/hiroot/pythia100_5k_truth_000777.root",
            char * infname2 = "/net/pstore01/d00/scratch/frankma/hiroot/pyquen100_5k_truth_000777.root")
{
   printf("%s\n",infname1);
   printf("%s\n",infname2);
   TFile * infile = new TFile(infname1);
   TNtuple * ntPythia = dynamic_cast<TNtuple*>(infile->Get("NTTruePFF"));
   TFile * infile2 = new TFile(infname2);
   TNtuple * ntPyquen = dynamic_cast<TNtuple*>(infile2->Get("NTTruePFF"));

//   TH1F * hXiNearMcut = new TH1F("hXiNearMcut","FF for near (away) side jet",100,0,10);
//   hXiNearMcut->SetLineColor(2);
//   hXiNearMcut->SetXTitle("Xi=ln(1/z)");
//   TH1F * hXiAwayMcut = new TH1F("hXiAwayMcut","FF for away side jet",100,0,10);
//   hXiAwayMcut->SetLineColor(4);
//   
//
//
//   //--- now look at fragmentation properties --
//   // Plot xi, with various cuts
//   TCanvas * cff = new TCanvas("cff","cff",800,0,700,450);
//   cff->SetLogy();
//   ntTruePFF->Draw("log(1/zn)>>hXiNear","abs(pndphi)<0.5");
//   ntTruePFF->Draw("log(1/za)>>hXiAway","abs(padphi)<0.5","same");
//   // with some mass window
//   TCanvas * cffMcut = new TCanvas("cffMcut","cffMcut",800,500,700,450);
//   cff->SetLogy();
//   ntTruePFF->Draw("log(1/zn)>>hXiNearMcut","abs(pndphi)<0.5 && (mass>200 && mass<280)");
//   ntTruePFF->Draw("log(1/za)>>hXiAwayMcut","abs(padphi)<0.5 && (mass>200 && mass<280)","same");

   //---- Check dijet properties ----
   //--- inv mass ---
   drawTree(ntPythia, "mass>>hMassPPythia","","",    "hMassPPythia","Pythia: inv mass of dijet",100,0,1800,true,kRed,1,3);
   drawTree(ntPyquen, "mass>>hMassPPyquen","","same","hMassPPyquen","draw Pyquen: inv mass of dijet",100,0,1800,true,kBlue,1,3);
   //--- Et ---
   // -near-
   drawTree(ntPythia, "npet>>hNLPartonPPythia","","",    "hNLPartonPPythia","Pythia: leading partons",100,80,200,true,kRed,1,3);
   drawTree(ntPyquen, "npet>>hNLPartonPPyquen","","same","hNLPartonPPyquen","Pyquen: leading partons",100,80,200,true,kBlue,1,3);
   // -away--
   drawTree(ntPythia, "apet>>hALPartonPPythia","","same","hALPartonPPythia","Pythia: leading partons",100,80,200,true,kRed,7,3);
   drawTree(ntPyquen, "apet>>hALPartonPPyquen","","same","hALPartonPPyquen","draw Pyquen: leading partons",100,80,200,true,kBlue,7,3);

   //---- Particles ----
   //--- inclusive angular correlations to leading partons---
   // -near-
   drawTree(ntPyquen, "pndphi>>hnPdPhiPyquen","","","hnPdPhiPyquen","Pyquen: dphi to leading partons",100,-0.5*PI,1.5*PI,false,kBlue,1,3);
   drawTree(ntPythia, "pndphi>>hnPdPhiPythia","","same","hnPdPhiPythia","Pythia: dphi to leading partons",100,-0.5*PI,1.5*PI,false,kRed,1,3);
   // -away-
   drawTree(ntPyquen, "padphi>>haPdPhiPyquen","","same","haPdPhiPyquen","Pyquen: dphi to leading partons",100,-0.5*PI,1.5*PI,false,kBlue,7,3);
   drawTree(ntPythia, "padphi>>haPdPhiPythia","","same","haPdPhiPythia","draw Pythia: dphi to leading partons",100,-0.5*PI,1.5*PI,false,kRed,7,3);

   //--- inclusive Pt ---
   drawTree(ntPythia, "ppt>>hPPtPythia","","","hPPtPythia","Pythia: Pt of final state particles",100,0,300,true,kRed,1,3);
   drawTree(ntPyquen, "ppt>>hPPtPyquen","","same","hPPtPyquen","draw Pyquen: Pt of final state particles",100,0,300,true,kBlue,1,3);

   //--- Finally: plot fragmentation properties ---
   // -near-
   drawTree(ntPythia, "log(1/zn)>>hXiNearPythia","abs(pndphi)<0.5","","hXiNearPythia","Pythia: FF of near parton",100,0,10,true,kRed,1,3);
   drawTree(ntPyquen, "log(1/zn)>>hXiNearPyquen","abs(pndphi)<0.5","same","hXiNearPyquen","Pyquen: FF of near parton",100,0,10,true,kBlue,1,3);
   // -away-
   drawTree(ntPythia, "log(1/za)>>hXiAwayPythia","abs(padphi)<0.5","same","hXiAwayPythia","Pythia: FF of away parton",100,0,10,true,kRed,7,3);
   drawTree(ntPyquen, "log(1/za)>>hXiAwayPyquen","abs(padphi)<0.5","same","hXiAwayPyquen","draw Pyquen: FF of away parton",100,0,10,true,kBlue,7,3);
   // -FF ratio-
   //TH1F * hNearFFRatio = new TH1F("hFFRatio","FF ratio", 100,0,10);
   drawDivHist("hXiNearPyquen","hXiNearPythia","","hNearFFRatio","Near FF ratio: Pyquen/Pythia",100,0,10,false,kBlue+2,1,3);
   drawDivHist("hXiAwayPyquen","hXiAwayPythia","same","hAwayFFRatio","draw Away FF ratio: Pyquen/Pythia",100,0,10,false,kGreen+2,7,3);

   printAllCanvases();
}
