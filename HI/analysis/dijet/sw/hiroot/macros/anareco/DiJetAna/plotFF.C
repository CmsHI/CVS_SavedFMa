#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TROOT.h"
#include "ana/savedfrankTools.C"
#include "DiJets.h"
using namespace DiJetAna;
using namespace std;

void plotFF(char * infname1 = "/net/pstore01/d00/scratch/frankma/hiroot/pythia100_5k_dijet_000777.root",
            char * infname2 = "/net/pstore01/d00/scratch/frankma/hiroot/pyquen100_5k_dijet_000777.root",
	    // -pythia-
	    Double_t PythiaAnaNJetEtMin = 90,
	    Double_t PythiaAnaNJetEtMax = 110,
	    Double_t PythiaAnaAJetEtMin = 50,
	    // -pyquen-
	    Double_t PyquenAnaNJetEtMin = 90,
	    Double_t PyquenAnaNJetEtMax = 110,
	    Double_t PyquenAnaAJetEtMin = 50,
	    Double_t JDPhiMin = 3.0,
	    char * plotdir = "plots",
	    const Int_t NXIBIN = 10,
	    const Double_t XIMAX = 9.,
	    const Double_t XIYMAX = 10,
	    Bool_t check = kTRUE
      )
{
   cout << PythiaAnaNJetEtMin << " " << PythiaAnaNJetEtMax << " " << PythiaAnaAJetEtMin << " "
      << PyquenAnaNJetEtMin << " " << PyquenAnaNJetEtMax << " " << PyquenAnaAJetEtMin
      << endl;

   bool NoNorm = false;

   //=== Get input files ===
   //---pythia---
   TFile * infile = findFile(infname1);
   TTree * trTruePythia = findTree(infile,"evtTreeTrueFF");
   TTree * trJetPythia = findTree(infile,"evtTreeJetFF");
   //---pyquen---
   TFile * infile2 = findFile(infname2);
   TTree * trTruePyquen = findTree(infile2,"evtTreeTrueFF");
   TTree * trJetPyquen = findTree(infile2,"evtTreeJetFF");

   //---output---
   TFile * outfile = new TFile(Form("%s/FFHistos.root",plotdir),"RECREATE");

   // === setup cuts ===
   printf("\n=========================Cuts setup======================\n");
   // ==pythia==
   DiJets pyt("Pythia","vjet1",trJetPythia,trJetPythia);
   pyt.SetNearJetEtMin(PythiaAnaNJetEtMin);
   pyt.SetNearJetEtMax(PythiaAnaNJetEtMax);
   pyt.SetAwayJetEtMin(PythiaAnaAJetEtMin);
   pyt.SetDPhiMin(JDPhiMin);
   pyt.SetJetPartlDRMax(0.5);
   pyt.CreateCuts();
   pyt.SetVerbosity(2);
   cout << pyt << endl;

   // -norm-
   printf("now jet cut: \n  %s\n",pyt.GetCut().GetDiJetCut().Data());
   pyt.CalcNumDiJets();
   printf("# of dijets after cut: %d, normalization: %f\n",pyt.GetNumDiJets(),pyt.GetDiJetsNorm());

   // ==pyquen==
   DiJets pyq("Pyquen","vjet1",trJetPyquen,trJetPyquen);
   pyq.SetNearJetEtMin(PyquenAnaNJetEtMin);
   pyq.SetNearJetEtMax(PyquenAnaNJetEtMax);
   pyq.SetAwayJetEtMin(PyquenAnaAJetEtMin);
   pyq.SetDPhiMin(JDPhiMin);
   pyq.SetJetPartlDRMax(0.5);
   pyq.CreateCuts();
   pyq.SetVerbosity(2);
   cout << pyq << endl;

   // -norm-
   printf("now jet cut: \n  %s\n",pyq.GetCut().GetDiJetCut().Data());
   pyq.CalcNumDiJets();
   printf("# of dijets after cut: %d, normalization: %f\n",pyq.GetNumDiJets(),pyq.GetDiJetsNorm());
   // ===

   if (check) {
      printf("\n============================Do Checks======================\n");
      printf("%s\n",infname1);
      printf("%s\n",infname2);
      //---- Check dijet properties ----
      //--- inv mass ---
      drawTree(trJetPythia, "mass>>hMassPPythia","",drsgFF,"hMassPPythia","Pythia: inv mass of dijet",100,0,1800,true,kRed,1,3);
      drawTree(trJetPyquen, "mass>>hMassPPyquen","",drdbFF,"hMassPPyquen","draw Pyquen: inv mass of dijet",100,0,1800,true,kBlue,1,3);
      //--- Et ---
      // -pythia-
      drawTree(trJetPythia, "nljet>>hNLPartonPPythia",pyt.GetCut().GetNJetPartlCut().Data(),drsgFF,"hNLPartonPPythia","Pythia: leading partons",HJETETBINS,HJETETMIN,HJETETMAX,0,kRed,1,3);
      drawTree(trJetPythia, "aljet>>hALPartonPPythia",pyt.GetCut().GetAJetPartlCut().Data(),drdbFF,"hALPartonPPythia","Pythia: leading partons",HJETETBINS,HJETETMIN,HJETETMAX,0,kRed,7,3);
      // -pyquen--
      drawTree(trJetPyquen, "nljet>>hNLPartonPPyquen",pyq.GetCut().GetNJetPartlCut().Data(),drdbFF,"hNLPartonPPyquen","Pyquen: leading partons",HJETETBINS,HJETETMIN,HJETETMAX,0,kBlue,1,3);
      drawTree(trJetPyquen, "aljet>>hALPartonPPyquen",pyq.GetCut().GetAJetPartlCut().Data(),drdbFF,"hALPartonPPyquen","Pyquen: leading partons",HJETETBINS,HJETETMIN,HJETETMAX,0,kBlue,7,3);
   }
   

   //=== Particles ===
   //--- inclusive angular correlations to leading partons---
   // -near-
   drawTree(trTruePyquen, "pndphi>>hnPdPhiPyquen","",drsgFF,"hnPdPhiPyquen",";d#phi(particle,Jet);",100,-0.5*PI,1.5*PI,false,kBlue,1,3);
   drawTree(trTruePythia, "pndphi>>hnPdPhiPythia","",drdbFF,"hnPdPhiPythia","Pythia: dphi to leading partons",100,-0.5*PI,1.5*PI,false,kRed,1,3);
   drawTree(trJetPyquen, "pndphi>>hnPdPhiJetPyquen","",drdbFF,"hnPdPhiJetPyquen","JetPyquen: dphi to leading partons",100,-0.5*PI,1.5*PI,false,kBlue,1,3);
   drawTree(trJetPythia, "pndphi>>hnPdPhiJetPythia","",drdbFF,"hnPdPhiJetPythia","JetPythia: dphi to leading partons",100,-0.5*PI,1.5*PI,false,kRed,1,3);
   // -away-
//   drawTree(trTruePyquen, "padphi>>haPdPhiPyquen","",drdbFF,"haPdPhiPyquen","Pyquen: dphi to leading partons",100,-0.5*PI,1.5*PI,false,kBlue,7,3);
//   drawTree(trTruePythia, "padphi>>haPdPhiPythia","",drdbFF,"haPdPhiPythia","draw Pythia: dphi to leading partons",100,-0.5*PI,1.5*PI,false,kRed,7,3);

   //--- inclusive Pt ---
   drawTree(trTruePythia, "ppt>>hPPtPythia","",drsgFF,"hPPtPythia","Pythia: Pt of final state particles",100,0,50,true,kRed,1,3);
   drawTree(trTruePyquen, "ppt>>hPPtPyquen","",drdbFF,"hPPtPyquen","draw Pyquen: Pt of final state particles",100,0,50,true,kBlue,1,3);


   // === Draw: jet  distributions ===
   // --comp with AnaJet
   drawNormHist(infile, "hJetEtDist",drsgFF,"","Jet Et [GeV]","#",1,true,2,7,1,1,1);
   drawNormHist(infile, "hLeadJetEtDist",drdbFF,"","Jet Et [GeV]","#",1,true,2,7,3,1,1);
   drawTree(trJetPythia, "nljet>>hCheckNearLJetPythia0","",drdbFF,"hCheckNearLJetPythia0",";Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX,true,kBlack,1,1,1,8);
   // --from jet leading--
   drawTree(trJetPythia, "nljet>>hCheckNearLJetPythia","",drsgFF,"hCheckNearLJetPythia",";Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX,true,kBlack,1,1,1,8);
   drawTree(trJetPyquen, "nljet>>hCheckNearLJetPyquen","",drdbFF,"hCheckNearLJetPyquen",";Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX,true,kBlue,1,1,1,8);
   drawTree(trJetPythia, "aljet>>hCheckAwayLJetPythia","",drdbFF,"hCheckAwayLJetPythia",";Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX,true,kBlack,7,1,1,8);
   drawTree(trJetPyquen, "aljet>>hCheckAwayLJetPyquen","",drdbFF,"hCheckAwayLJetPyquen",";Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX,true,kBlue,7,1,1,8);
   //-- *apply cuts* --
   drawTree(trJetPythia, "nljet>>hCheckCutNearLJetPythia",pyt.GetCut().GetDiJetCut().Data(),drdbFF,"hCheckCutNearLJetPythia",";Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX,true,kBlack,1,3,1,8);
   drawTree(trJetPyquen, "nljet>>hCheckCutNearLJetPyquen",pyq.GetCut().GetDiJetCut().Data(),drdbFF,"hCheckCutNearLJetPyquen",";Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX,true,kBlue,1,3,1,8);
   drawTree(trJetPythia, "aljet>>hCheckCutAwayLJetPythia",pyt.GetCut().GetDiJetCut().Data(),drdbFFE,"hCheckCutAwayLJetPythia",";Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX,true,kBlack,1,3,1,4);
   drawTree(trJetPyquen, "aljet>>hCheckCutAwayLJetPyquen",pyq.GetCut().GetDiJetCut().Data(),drdbFFE,"hCheckCutAwayLJetPyquen",";Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX,true,kBlue,1,3,1,4);

   float nJetPythiaNorm = pyt.GetDiJetsNorm();
   if (NoNorm) nJetPythiaNorm = 1.;
   printf("nDiJetPythias: %d,  jnorm: %f\n",pyt.GetNumDiJets(),nJetPythiaNorm);
   float nJetPyquenNorm = pyq.GetDiJetsNorm();
   if (NoNorm) nJetPyquenNorm = 1.;
   printf("nDiJetPyquens: %d,  jnorm: %f\n",pyq.GetNumDiJets(),nJetPyquenNorm);

   //=== Finally: plot fragmentation properties ===
   printf("\n===================== Plot FF =======================\n");
   // -near-
   drawTree(trJetPythia, "log(1/zn)>>hXiNearJetPythia",pyt.GetCut().GetNJetPartlCut().Data(),drsgFFE,"hXiNearJetPythia",";(near) #xi=ln(E_{t}^{Jet}/E_{t}^{Particle});",NXIBIN,0,XIMAX,0,kBlack,1,3,1,8,nJetPythiaNorm,XIYMAX);
   drawTree(trJetPyquen, "log(1/zn)>>hXiNearJetPyquen",pyq.GetCut().GetNJetPartlCut().Data(),drdbFFE,"hXiNearJetPyquen","JetPyquen: FF of near parton",NXIBIN,0,XIMAX,0,kBlue,1,3,1,8,nJetPyquenNorm,XIYMAX);
   drawTree(trTruePyquen, "log(1/zn)>>hXiNearPyquen",pyq.GetCut().GetNJetPartlCut().Data(),drdbFF,"hXiNearPyquen",";(near) #xi=ln(E_{t}^{Jet}/E_{t}^{Particle})",NXIBIN,0,XIMAX,0,kRed-2,1,3,0,0,nJetPyquenNorm,XIYMAX);
   drawTree(trTruePythia, "log(1/zn)>>hXiNearPythia",pyt.GetCut().GetNJetPartlCut().Data(),drdbFF,"hXiNearPythia",";#xi=ln(E_{t}^{Jet}/E_{t}^{Particle};",NXIBIN,0,XIMAX,0,kRed,1,3,0,0,nJetPythiaNorm,XIYMAX);
   // -away-
   drawTree(trJetPythia, "log(1/za)>>hXiAwayJetPythia",pyt.GetCut().GetAJetPartlCut().Data(),drsgFFE,"hXiAwayJetPythia",";(away) #xi=ln(E_{t}^{Jet}/E_{t}^{Particle});",NXIBIN,0,XIMAX,0,kBlack,1,3,1,8,nJetPythiaNorm,XIYMAX);
   drawTree(trJetPyquen, "log(1/za)>>hXiAwayJetPyquen",pyq.GetCut().GetAJetPartlCut().Data(),drdbFFE,"hXiAwayJetPyquen","JetPyquen: FF of near parton",NXIBIN,0,XIMAX,0,kBlue,1,3,1,8,nJetPyquenNorm,XIYMAX);
   drawTree(trTruePyquen, "log(1/za)>>hXiAwayPyquen",pyq.GetCut().GetAJetPartlCut().Data(),drdbFF,"hXiAwayPyquen",";(away) #xi=ln(E_{t}^{Jet}/E_{t}^{Particle})",NXIBIN,0,XIMAX,0,kRed-2,1,3,0,0,nJetPyquenNorm,XIYMAX);
   drawTree(trTruePythia, "log(1/za)>>hXiAwayPythia",pyt.GetCut().GetAJetPartlCut().Data(),drdbFF,"hXiAwayPythia",";#xi=ln(E_{t}^{Jet}/E_{t}^{Particle};",NXIBIN,0,XIMAX,0,kRed,1,3,0,0,nJetPythiaNorm,XIYMAX);

   //---FF ratio2---
   // -parton-
   drawDivHist("hXiNearPyquen","hXiNearPythia",drsgFF,"hNearFFRatio",";#xi_{pyquen}/#xi_{pythia};",NXIBIN,0,XIMAX,0,kRed-2,1,3,0,0,3.);
   drawDivHist("hXiAwayPyquen","hXiAwayPythia",drdbFF,"hAwayFFRatio","draw Away FF ratio: Pyquen/Pythia",NXIBIN,0,XIMAX,0,kRed-2,7,3,1,4);
   // -Jet-
   drawDivHist("hXiNearJetPyquen","hXiNearJetPythia",drdbFFE,"hNearJetFFRatio",";#xi_{pyquen}/#xi_{pythia};",NXIBIN,0,XIMAX,0,kBlue,1,3,1,8);
   drawDivHist("hXiAwayJetPyquen","hXiAwayJetPythia",drdbFFE,"hAwayJetFFRatio","draw Away FF ratio: Pyquen/Pythia",NXIBIN,0,XIMAX,0,kBlue,7,3,1,4);

   //=== Save and exit ===
   printAllCanvases(plotdir);
   outfile->Write();
   outfile->Close();
}
