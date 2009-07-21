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
	    TString PythiaAnaNJetEtMin = "90",
	    TString PythiaAnaNJetEtMax = "",
	    TString PythiaAnaAJetEtMin = "50",
	    // -pyquen-
	    TString PyquenAnaNJetEtMin = "90",
	    TString PyquenAnaNJetEtMax = "",
	    TString PyquenAnaAJetEtMin = "50",
	    TString JDPhiMin = "3.0",
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


   DiJets dj1;
   dj1.SetGenTag("pythia");
   dj1.SetCutTag("v1");
   cout << dj1 << endl;

   DiJets dj2("pyquen","v1",ntJetPyquen,ntJetLeadingPyquen);
   cout << dj2 << endl;
}
