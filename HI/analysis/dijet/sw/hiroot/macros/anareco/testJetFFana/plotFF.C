#include <iostream>
#include "TFile.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TROOT.h"
#include "ana/savedfrankTools.C"

const char * drsgFF="hist";
const char * drdbFF="hist same";
const char * drsgFFE="E1 ";
const char * drdbFFE="E1 same";

const Int_t NXIBIN = 10;
const Double_t XIMAX = 9.;
//TString AnaJetEtCut = ">70.";
TString PythiaAnaJetEtCut = ">50.";
TString PyquenAnaJetEtCut = ">50.";
TString ParticlePtCut = ">0.5";
TString AnaPConeCut = "<0.5";
TString PythiaNearFFCut = TString("npet")+PythiaAnaJetEtCut+TString(" && ")+TString("abs(pndphi)")+AnaPConeCut+TString(" && ppt")+ParticlePtCut;
TString PythiaAwayFFCut = TString("npet")+PythiaAnaJetEtCut+TString(" && ")+TString("abs(padphi)")+AnaPConeCut+TString(" && ppt")+ParticlePtCut;
TString PyquenNearFFCut = TString("npet")+PyquenAnaJetEtCut+TString(" && ")+TString("abs(pndphi)")+AnaPConeCut+TString(" && ppt")+ParticlePtCut;
TString PyquenAwayFFCut = TString("npet")+PyquenAnaJetEtCut+TString(" && ")+TString("abs(padphi)")+AnaPConeCut+TString(" && ppt")+ParticlePtCut;
bool NoNorm = false;

void plotFF(char * infname1 = "/net/pstore01/d00/scratch/frankma/hiroot/pythia100_5k_dijet_000777.root",
            char * infname2 = "/net/pstore01/d00/scratch/frankma/hiroot/pyquen100_5k_dijet_000777.root",
	    char * plotdir = "plots")
{
   printf("%s\n",infname1);
   printf("%s\n",infname2);
   //---pythia---
   TFile * infile = new TFile(infname1);
   TNtuple * ntPythia = dynamic_cast<TNtuple*>(infile->Get("NTTruePFF"));
   TNtuple * ntJetPythia = dynamic_cast<TNtuple*>(infile->Get("NTJetFF"));
   // -leading-
   TNtuple * ntJetLeadingPythia = dynamic_cast<TNtuple*>(infile->Get("NTJetLeading"));
   //---pyquen---
   TFile * infile2 = new TFile(infname2);
   TNtuple * ntPyquen = dynamic_cast<TNtuple*>(infile2->Get("NTTruePFF"));
   TNtuple * ntJetPyquen = dynamic_cast<TNtuple*>(infile2->Get("NTJetFF"));
   TNtuple * ntJetLeadingPyquen = dynamic_cast<TNtuple*>(infile2->Get("NTJetLeading"));
   //---output---
   TFile * outfile = new TFile("FFHistos.root","RECREATE");

//   TCanvas * cffMcut = new TCanvas("cffMcut","cffMcut",800,500,700,450);
//   cff->SetLogy();
//   ntTruePFF->Draw("log(1/zn)>>hXiNearMcut","abs(pndphi)<0.5 && (mass>200 && mass<280)");
//   ntTruePFF->Draw("log(1/za)>>hXiAwayMcut","abs(padphi)<0.5 && (mass>200 && mass<280)",drdbFF);

   
   Bool_t check = kTRUE;
   if (check) {
      //---- Check dijet properties ----
      //--- inv mass ---
      drawTree(ntPythia, "mass>>hMassPPythia","",drsgFF,    "hMassPPythia","Pythia: inv mass of dijet",100,0,1800,true,kRed,1,3);
      drawTree(ntPyquen, "mass>>hMassPPyquen","",drdbFF,"hMassPPyquen","draw Pyquen: inv mass of dijet",100,0,1800,true,kBlue,1,3);
      //--- Et ---
      // -near-
      drawTree(ntPythia, "npet>>hNLPartonPPythia",PythiaNearFFCut.Data(),drsgFF,    "hNLPartonPPythia","Pythia: leading partons",100,80,200,true,kRed,1,3);
      drawTree(ntPyquen, "npet>>hNLPartonPPyquen",PyquenNearFFCut.Data(),drdbFF,"hNLPartonPPyquen","Pyquen: leading partons",100,80,200,true,kBlue,1,3);
      // -away--
      drawTree(ntPythia, "apet>>hALPartonPPythia",PythiaAwayFFCut.Data(),drdbFF,"hALPartonPPythia","Pythia: leading partons",100,80,200,true,kRed,7,3);
      drawTree(ntPyquen, "apet>>hALPartonPPyquen",PyquenAwayFFCut.Data(),drdbFF,"hALPartonPPyquen","draw Pyquen: leading partons",100,80,200,true,kBlue,7,3);
   }
   

   //---- Particles ----
   //--- inclusive angular correlations to leading partons---
   // -near-
   drawTree(ntPyquen, "pndphi>>hnPdPhiPyquen","",drsgFF,"hnPdPhiPyquen",";d#phi(particle,Jet);",100,-0.5*PI,1.5*PI,false,kBlue,1,3);
   drawTree(ntPythia, "pndphi>>hnPdPhiPythia","",drdbFF,"hnPdPhiPythia","Pythia: dphi to leading partons",100,-0.5*PI,1.5*PI,false,kRed,1,3);
   drawTree(ntJetPyquen, "pndphi>>hnPdPhiJetPyquen","",drdbFF,"hnPdPhiJetPyquen","JetPyquen: dphi to leading partons",100,-0.5*PI,1.5*PI,false,kBlue,1,3);
   drawTree(ntJetPythia, "pndphi>>hnPdPhiJetPythia","",drdbFF,"hnPdPhiJetPythia","JetPythia: dphi to leading partons",100,-0.5*PI,1.5*PI,false,kRed,1,3);
   // -away-
//   drawTree(ntPyquen, "padphi>>haPdPhiPyquen","",drdbFF,"haPdPhiPyquen","Pyquen: dphi to leading partons",100,-0.5*PI,1.5*PI,false,kBlue,7,3);
//   drawTree(ntPythia, "padphi>>haPdPhiPythia","",drdbFF,"haPdPhiPythia","draw Pythia: dphi to leading partons",100,-0.5*PI,1.5*PI,false,kRed,7,3);

   //--- inclusive Pt ---
   drawTree(ntPythia, "ppt>>hPPtPythia","",drsgFF,"hPPtPythia","Pythia: Pt of final state particles",100,0,50,true,kRed,1,3);
   drawTree(ntPyquen, "ppt>>hPPtPyquen","",drdbFF,"hPPtPyquen","draw Pyquen: Pt of final state particles",100,0,50,true,kBlue,1,3);

   //=== Get Normalizations ===
   drawNormHist(infile, "hJetEtDist",drsgFF,"","Jet Et [GeV]","#",1,true,2,7,3,1,1);
   drawTree(ntJetLeadingPythia, "nljet>>hCheckNearLJetPythia","",drdbFF,    "hCheckNearLJetPythia",";Et [GeV];",100,0,200,true,kRed,1,3,1,8);
   drawTree(ntJetLeadingPyquen, "nljet>>hCheckNearLJetPyquen","",drdbFF,"hCheckNearLJetPyquen",";Et [GeV];",100,0,200,true,kRed-2,1,3,1,8);
   Float_t nJetPythia = drawTree(ntJetLeadingPythia, "nljet>>hCheckCutNearLJetPythia",(TString("nljet")+PythiaAnaJetEtCut).Data(),drdbFFE,    "hCheckCutNearLJetPythia",";Et [GeV];",100,0,200,true,kBlack,1,3,1,8);
   Float_t nJetPyquen = drawTree(ntJetLeadingPyquen, "nljet>>hCheckCutNearLJetPyquen",(TString("nljet")+PyquenAnaJetEtCut).Data(),drdbFFE,"hCheckCutNearLJetPyquen",";Et [GeV];",100,0,200,true,kBlue,1,3,1,8);
   drawTree(ntJetLeadingPythia, "aljet>>hCheckCutAwayLJetPythia",(TString("nljet")+PythiaAnaJetEtCut).Data(),drdbFFE,    "hCheckCutAwayLJetPythia",";Et [GeV];",100,0,200,true,kBlack,1,3,1,4);
   drawTree(ntJetLeadingPyquen, "aljet>>hCheckCutAwayLJetPyquen",(TString("nljet")+PyquenAnaJetEtCut).Data(),drdbFFE,"hCheckCutAwayLJetPyquen",";Et [GeV];",100,0,200,true,kBlue,1,3,1,4);
   float nJetPythiaNorm = 1./nJetPythia;
   if (NoNorm) nJetPythiaNorm = 1.;
   printf("nJetPythias: %f,  jnorm: %f\n",nJetPythia,nJetPythiaNorm);
   float nJetPyquenNorm = 1./nJetPyquen;
   if (NoNorm) nJetPyquenNorm = 1.;
   printf("nJetPyquens: %f,  jnorm: %f\n",nJetPyquen,nJetPyquenNorm);

   //=== Finally: plot fragmentation properties ===
   printf("Near Pythia FF cut: %s\n",PythiaNearFFCut.Data());
   printf("Away Pythia FF cut: %s\n",PythiaAwayFFCut.Data());
   printf("Near Pyquen FF cut: %s\n",PyquenNearFFCut.Data());
   printf("Away Pyquen FF cut: %s\n",PyquenAwayFFCut.Data());
   // -near-
   drawTree(ntJetPythia, "log(1/zn)>>hXiNearJetPythia",PythiaNearFFCut.Data(),drsgFFE,"hXiNearJetPythia",";(near) #xi=ln(E_{t}^{Jet}/E_{t}^{Particle});",NXIBIN,0,XIMAX,true,kBlack,1,3,1,8,nJetPythiaNorm);
   drawTree(ntJetPyquen, "log(1/zn)>>hXiNearJetPyquen",PyquenNearFFCut.Data(),drdbFFE,"hXiNearJetPyquen","JetPyquen: FF of near parton",NXIBIN,0,XIMAX,true,kBlue,1,3,1,8,nJetPyquenNorm);
   drawTree(ntPyquen, "log(1/zn)>>hXiNearPyquen",PyquenNearFFCut.Data(),drdbFF,"hXiNearPyquen",";(near) #xi=ln(E_{t}^{Jet}/E_{t}^{Particle})",NXIBIN,0,XIMAX,true,kRed-2,1,3,0,0,nJetPyquenNorm);
   drawTree(ntPythia, "log(1/zn)>>hXiNearPythia",PythiaNearFFCut.Data(),drdbFF,"hXiNearPythia",";#xi=ln(E_{t}^{Jet}/E_{t}^{Particle};",NXIBIN,0,XIMAX,true,kRed,1,3,0,0,nJetPythiaNorm);
   // -away-
   drawTree(ntJetPythia, "log(1/za)>>hXiAwayJetPythia",PythiaAwayFFCut.Data(),drsgFFE,"hXiAwayJetPythia",";(away) #xi=ln(E_{t}^{Jet}/E_{t}^{Particle});",NXIBIN,0,XIMAX,true,kBlack,1,3,1,8,nJetPythiaNorm);
   drawTree(ntJetPyquen, "log(1/za)>>hXiAwayJetPyquen",PyquenAwayFFCut.Data(),drdbFFE,"hXiAwayJetPyquen","JetPyquen: FF of near parton",NXIBIN,0,XIMAX,true,kBlue,1,3,1,8,nJetPyquenNorm);
   drawTree(ntPyquen, "log(1/za)>>hXiAwayPyquen",PyquenAwayFFCut.Data(),drdbFF,"hXiAwayPyquen",";(away) #xi=ln(E_{t}^{Jet}/E_{t}^{Particle})",NXIBIN,0,XIMAX,true,kRed-2,1,3,0,0,nJetPyquenNorm);
   drawTree(ntPythia, "log(1/za)>>hXiAwayPythia",PythiaAwayFFCut.Data(),drdbFF,"hXiAwayPythia",";#xi=ln(E_{t}^{Jet}/E_{t}^{Particle};",NXIBIN,0,XIMAX,true,kRed,1,3,0,0,nJetPythiaNorm);

   //---FF ratio1---
   // -parton-
//   drawDivHist("hXiNearPyquen","hXiNearPythia",drsgFF,"hNearFFRatio",";#xi_{pyquen}/#xi_{pythia};",100,0,XIMAX,1,kRed-2,1,3);
//   drawDivHist("hXiAwayPyquen","hXiAwayPythia",drdbFF,"hAwayFFRatio","draw Away FF ratio: Pyquen/Pythia",100,0,XIMAX,1,kRed-2,7,3,1,4);
//   // -Jet-
//   drawDivHist("hXiNearJetPyquen","hXiNearPythia",drdbFFE,"hNearJetFFRatio",";#xi_{pyquen}/#xi_{pythia};",100,0,XIMAX,1,kBlue,1,3,1,8);
//   drawDivHist("hXiAwayJetPyquen","hXiAwayPythia",drdbFFE,"hAwayJetFFRatio","draw Away FF ratio: Pyquen/Pythia",100,0,XIMAX,1,kBlue,7,3,1,4);
   //---FF ratio2---
   // -parton-
   drawDivHist("hXiNearPyquen","hXiNearPythia",drsgFF,"hNearFFRatio",";#xi_{pyquen}/#xi_{pythia};",NXIBIN,0,XIMAX,0,kRed-2,1,3,0,0,2.5);
   drawDivHist("hXiAwayPyquen","hXiAwayPythia",drdbFF,"hAwayFFRatio","draw Away FF ratio: Pyquen/Pythia",NXIBIN,0,XIMAX,0,kRed-2,7,3,1,4);
   // -Jet-
   drawDivHist("hXiNearJetPyquen","hXiNearJetPythia",drdbFFE,"hNearJetFFRatio",";#xi_{pyquen}/#xi_{pythia};",NXIBIN,0,XIMAX,0,kBlue,1,3,1,8);
   drawDivHist("hXiAwayJetPyquen","hXiAwayJetPythia",drdbFFE,"hAwayJetFFRatio","draw Away FF ratio: Pyquen/Pythia",NXIBIN,0,XIMAX,0,kBlue,7,3,1,4);

   printAllCanvases(plotdir);
   outfile->Write();
   outfile->Close();
}
