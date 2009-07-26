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
   TNtuple * ntPythia = dynamic_cast<TNtuple*>(infile->Get("NTTruePFF"));
   TNtuple * ntJetPythia = dynamic_cast<TNtuple*>(infile->Get("NTJetFF"));
   TNtuple * ntJetLeadingPythia = dynamic_cast<TNtuple*>(infile->Get("NTJetLeading"));
   //---pyquen---
   TFile * infile2 = findFile(infname2);
   TNtuple * ntPyquen = dynamic_cast<TNtuple*>(infile2->Get("NTTruePFF"));
   TNtuple * ntJetPyquen = dynamic_cast<TNtuple*>(infile2->Get("NTJetFF"));
   TNtuple * ntJetLeadingPyquen = dynamic_cast<TNtuple*>(infile2->Get("NTJetLeading"));

   //=== Creat output ===
   TFile * outfile = new TFile(Form("%s/dijets.root",plotdir),"RECREATE");

   DiJets dj1;
   dj1.SetGenTag("Pythia");
   cout << dj1 << endl;

   DiJets dj2("Pyquen","v2",ntJetLeadingPyquen,ntJetPyquen);
   dj2.SetNearJetEtMin(90);
   dj2.SetNearJetEtMax(110);
   dj2.SetAwayJetEtMin(50);
   dj2.SetDPhiMin(2.8);
   dj2.SetPartlPtMin(0.5); //0.5 by default
   dj2.SetJetPartlDRMax(0.5);
   dj2.CreateCuts();

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
