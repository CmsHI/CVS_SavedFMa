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

void plotShapes(char * infname1 = "/net/pstore01/d00/scratch/frankma/hiroot/pythia100_5k_dijet_000777.root",
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
   TFile * outfile = new TFile(Form("%s/ShapeHistos.root",plotdir),"RECREATE");

   // === setup cuts ===
   printf("\n=========================Cuts setup======================\n");
   // ==pythia==
   DiJets pyt("Pythia","vjet1",trJetPythia);
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
   DiJets pyq("Pyquen","vjet1",trJetPyquen);
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


   // === Cone info ===
   // -- # particles --
   drawTree(trJetPythia, "nljCone5NP>>hNJCone5NPPythia",pyt.GetCut().GetDiJetCut().Data(),drsgE,"hNJCone5NPPythia",";# charged particles in 0.5 cone;",NBIN,0,40,1,kBlack,1,3,1,8);
   drawTree(trJetPythia, "aljCone5NP>>hAJCone5NPPythia",pyt.GetCut().GetDiJetCut().Data(),drdbE,"hAJCone5NPPythia",";# charged particles in 0.5 cone;",NBIN,0,40,1,kBlack,1,3,1,4);
   drawTree(trJetPyquen, "nljCone5NP>>hNJCone5NPPyquen",pyq.GetCut().GetDiJetCut().Data(),drdbE,"hNJCone5NPPyquen",";# charged particles in 0.5 cone;",NBIN,0,40,1,kBlue,1,3,1,8);
   drawTree(trJetPyquen, "aljCone5NP>>hAJCone5NPPyquen",pyq.GetCut().GetDiJetCut().Data(),drdbE,"hAJCone5NPPyquen",";# charged particles in 0.5 cone;",NBIN,0,40,1,kBlue,1,3,1,4);

   // --Et--
   drawTree(trJetPythia, "nljCone5Et>>hNJCone5EtPythia",pyt.GetCut().GetDiJetCut().Data(),drsgE,"hNJCone5EtPythia",";#sum_{in 0.5 cone} E_{T}^{charged particles} [GeV];",NBIN/2,0,120,1,kBlack,1,3,1,8);
   drawTree(trJetPythia, "aljCone5Et>>hAJCone5EtPythia",pyt.GetCut().GetDiJetCut().Data(),drdbE,"hAJCone5EtPythia",";#sum_{in 0.5 cone} E_{T}^{charged particles} [GeV];",NBIN/2,0,120,1,kBlack,1,3,1,4);
   drawTree(trJetPyquen, "nljCone5Et>>hNJCone5EtPyquen",pyq.GetCut().GetDiJetCut().Data(),drdbE,"hNJCone5EtPyquen",";#sum_{in 0.5 cone} E_{T}^{charged particles} [GeV];",NBIN/2,0,120,1,kBlue,1,3,1,8);
   drawTree(trJetPyquen, "aljCone5Et>>hAJCone5EtPyquen",pyq.GetCut().GetDiJetCut().Data(),drdbE,"hAJCone5EtPyquen",";#sum_{in 0.5 cone} E_{T}^{charged particles} [GeV];",NBIN/2,0,120,1,kBlue,1,3,1,4);


   //=== Save and exit ===
   printAllCanvases(plotdir);
   outfile->Write();
   outfile->Close();
}
