#include <iostream>
#include "TFile.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TROOT.h"
#include "ana/savedfrankTools.C"
#include "DiJets.h"
using namespace DiJetAna;
using namespace std;

void dijetsDriver(char * infname1 = "/net/pstore01/d00/scratch/frankma/hiroot/pythia100_5k_dijet_000777.root",
            char * infname2 = "/net/pstore01/d00/scratch/frankma/hiroot/pyquen100_5k_dijet_000777.root",
	    // -pythia-
	    Double_t PythiaAnaNJetEtMin = 90,
	    Double_t PythiaAnaNJetEtMax = 110,
	    Double_t PythiaAnaAJetEtMin = 50,
	    // -pyquen-
	    Double_t PyquenAnaNJetEtMin = 90,
	    Double_t PyquenAnaNJetEtMax = 110,
	    Double_t PyquenAnaAJetEtMin = 50,
	    Double_t JDPhiMin = 2.8,
	    char * plotdir = "plots",
	    const Int_t NXIBIN = 10,
	    const Double_t XIMAX = 9.,
	    const Double_t XIYMAX = 10,
	    Bool_t check = kFALSE
      )
{
   //=== Get input files ===
   //---pythia---
   TFile * infile = findFile(infname1);
   TTree * trTruePythia = findTree(infile,"evtTreeTrueFF");
   TTree * trJetPythia = findTree(infile,"evtTreeJetFF");
   //---pyquen---
   TFile * infile2 = findFile(infname2);
   TTree * trTruePyquen = findTree(infile2,"evtTreeTrueFF");
   TTree * trJetPyquen = findTree(infile2,"evtTreeJetFF");

   //=== Creat output ===
   TFile * outfile = new TFile(Form("%s/dijets.root",plotdir),"RECREATE");

   DiJets dj1;
   dj1.SetGenTag("Pythia");
   dj1.SetVerbosity(2);
   dj1.CreateCuts();
   cout << dj1 << endl;

   DiJets dj2("Pyquen","v2",trJetPythia);
//   dj2.SetPartlPtMin(0.5); //0.5 by default
//   dj2.SetNearJetEtMin(PythiaAnaNJetEtMin);
//   dj2.SetNearJetEtMax(PythiaAnaNJetEtMax);
//   dj2.SetAwayJetEtMin(PythiaAnaAJetEtMin);
//   dj2.SetDPhiMin(JDPhiMin);
//   dj2.SetJetPartlDRMax(0.5);
   dj2.CreateCuts();
   dj2.SetVerbosity(2);

   cout << dj2 << endl;

   // === Jet ana ===
   printf("now jet cut: \n  %s\n",dj2.GetCut().GetDiJetCut().Data());
   dj2.CalcNumDiJets();
   printf("# of dijets after cut: %d, normalization: %f\n",dj2.GetNumDiJets(),dj2.GetDiJetsNorm());
   dj2.PlotDiJets();

   // === FF ===
   printf("now near jet_particles cut: \n  %s\n",dj2.GetCut().GetNJetPartlCut().Data());
   printf("now away jet_particles cut: \n  %s\n",dj2.GetCut().GetAJetPartlCut().Data());

   //-------------- Done Save Plots----------------------
   printAllCanvases(plotdir);
   outfile->Write();
   outfile->Close();
}
