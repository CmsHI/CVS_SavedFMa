#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TROOT.h"
#include "/net/hisrv0001/home/frankma/UserCode/SavedFMa/HI/analysis/dijet/sw/hiroot/macros/ana/savedfrankTools.C"
#include "/net/hisrv0001/home/frankma/UserCode/SavedFMa/HI/analysis/dijet/sw/hiroot/macros/ana/HistMath.C"
#include "DiJets.h"
#include <vector>
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

   // === Ana cone Size ===
   UInt_t dRDig = 10;
   Float_t dRMax = dRDig*0.1;
   printf("==== Cone size: %f ====\n", dRMax);

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
   // ==pyquen==
   DiJets pyq("Pyquen","vjet1",trJetPyquen);
   pyq.SetNearJetEtMin(PyquenAnaNJetEtMin);
   pyq.SetNearJetEtMax(PyquenAnaNJetEtMax);
   pyq.SetAwayJetEtMin(PyquenAnaAJetEtMin);
   // == common cuts, actions==
   vector<DiJets*> vgen;
   vgen.push_back(&pyt);
   vgen.push_back(&pyq);
   for (UInt_t ig=0; ig<vgen.size(); ++ig) {
      vgen[ig]->SetDPhiMin(JDPhiMin);
      vgen[ig]->SetJetPartlDRMax(dRMax);
      // make cut
      vgen[ig]->CreateCuts();
      vgen[ig]->SetVerbosity(2);
      // output
      cout << *(vgen[ig]) << endl;

      // -norm-
      printf("now jet cut: \n  %s\n",vgen[ig]->GetCut().GetDiJetCut().Data());
      vgen[ig]->CalcNumDiJets();
      printf("# of dijets after cut: %d, normalization: %f\n",vgen[ig]->GetNumDiJets(),vgen[ig]->GetDiJetsNorm());
      printf("===================\n\n");
   }
   // ===


   // === Jet Shape Ana Properties ===
   // -- sizes of cone to perfrom ana --
   vector<Int_t> vcone;
   vcone.push_back(dRDig);

   // -- Study different Shape denominators --
   vector<TString> vcbase;
   //  -for fixed cone et base-
   vcbase.push_back(TString(Form("nljCone%dEt",dRDig)));
   //  -parton et based-
   vcbase.push_back(TString("nlpet"));

   // -- plotting properties --
   //  - jet level -
   Double_t coneNPMAX = 80;
   UInt_t coneEtNBIN = NBIN/5;
   Double_t coneEtMAX = 120;
   //  - shape level -
   Double_t HBEtaCell = 0.087; // phi is the same
   UInt_t rNBIN = 13;
   Double_t rMAX = HBEtaCell*rNBIN; // ~1.1 for now
   Double_t shapeMsz = 1.3;
   Double_t intShpMax = 1.1;

   //
   // Plot Jet shapes for different cones (one for now) and cone et bases
   // - Note
   //   * We can only loop on one cone size now for techincal reasons
   //     1. need to figure out a way to change the dRMax cut
   //     2. need to figure out a way to find the fixed cone size et base
   //        that corresponds to the cone size in loop
   //        
   for ( UInt_t ic=0; ic<vcone.size(); ++ic) {
      // === Jet Cone info ===
      // -- # particles --
      drawTree(trJetPythia, Form("nljCone%dNP>>hNJCone%dNPPythia",vcone[ic],vcone[ic]),pyt.GetCut().GetDiJetCut().Data(),drsgE,
	    Form("hNJCone%dNPPythia",vcone[ic]),Form(";# charged particles in 0.%d cone;",vcone[ic]),NBIN,0,coneNPMAX,0,kBlack,1,3,1,8);
      drawTree(trJetPythia, Form("aljCone%dNP>>hAJCone%dNPPythia",vcone[ic],vcone[ic]),pyt.GetCut().GetDiJetCut().Data(),drdbE,
	    Form("hAJCone%dNPPythia",vcone[ic]),Form(";# charged particles in 0.%d cone;",vcone[ic]),NBIN,0,coneNPMAX,0,kBlack,1,3,1,4);
      drawTree(trJetPyquen, Form("nljCone%dNP>>hNJCone%dNPPyquen",vcone[ic],vcone[ic]),pyq.GetCut().GetDiJetCut().Data(),drdbE,
	    Form("hNJCone%dNPPyquen",vcone[ic]),Form(";# charged particles in 0.%d cone;",vcone[ic]),NBIN,0,coneNPMAX,0,kBlue,1,3,1,8);
      drawTree(trJetPyquen, Form("aljCone%dNP>>hAJCone%dNPPyquen",vcone[ic],vcone[ic]),pyq.GetCut().GetDiJetCut().Data(),drdbE,
	    Form("hAJCone%dNPPyquen",vcone[ic]),Form(";# charged particles in 0.%d cone;",vcone[ic]),NBIN,0,coneNPMAX,0,kBlue,1,3,1,4);
      // --Et--
      drawTree(trJetPythia, Form("nljCone%dEt>>hNJCone%dEtPythia",vcone[ic],vcone[ic]),pyt.GetCut().GetDiJetCut().Data(),drsgE,
	    Form("hNJCone%dEtPythia",vcone[ic]),Form(";# charged particles in 0.%d cone;",vcone[ic]),coneEtNBIN,0,coneEtMAX,0,kBlack,1,3,1,8);
      drawTree(trJetPythia, Form("aljCone%dEt>>hAJCone%dEtPythia",vcone[ic],vcone[ic]),pyt.GetCut().GetDiJetCut().Data(),drdbE,
	    Form("hAJCone%dEtPythia",vcone[ic]),Form(";# charged particles in 0.%d cone;",vcone[ic]),coneEtNBIN,0,coneEtMAX,0,kBlack,1,3,1,4);
      drawTree(trJetPyquen, Form("nljCone%dEt>>hNJCone%dEtPyquen",vcone[ic],vcone[ic]),pyq.GetCut().GetDiJetCut().Data(),drdbE,
	    Form("hNJCone%dEtPyquen",vcone[ic]),Form(";# charged particles in 0.%d cone;",vcone[ic]),coneEtNBIN,0,coneEtMAX,0,kBlue,1,3,1,8);
      drawTree(trJetPyquen, Form("aljCone%dEt>>hAJCone%dEtPyquen",vcone[ic],vcone[ic]),pyq.GetCut().GetDiJetCut().Data(),drdbE,
	    Form("hAJCone%dEtPyquen",vcone[ic]),Form(";# charged particles in 0.%d cone;",vcone[ic]),coneEtNBIN,0,coneEtMAX,0,kBlue,1,3,1,4);

      // === Now plot jet shapes for each cone et base ===
      for (UInt_t ib=0; ib<vcbase.size(); ++ib) {
	 // === Add Jet Shape Weights ===
	 printf("===== Add Weights =====\n");
	 pyt.cut_.SetWeightParticles(Form("ppt/%s",vcbase[ib].Data()));
	 cout << pyt << endl;
	 pyq.cut_.SetWeightParticles(Form("ppt/%s",vcbase[ib].Data()));
	 cout << pyq << endl;

	 // -- label aliases --
	 TString nbkey(vcbase[ib]);
	 TString abkey(vcbase[ib].ReplaceAll("nl","al"));
	 printf("Label aliases:\n");
	 printf("  near: %s, away: %s\n",nbkey.Data(),abkey.Data());
	 // === Differential Jet Shapes ===
	 drawTree(trJetPythia, Form("pndr>>%sDiffShapePythia",nbkey.Data()),pyt.GetCut().GetNJetPartlCutW().Data(),drsgE,
	       Form("%sDiffShapePythia",nbkey.Data()),";r;#rho(r)",rNBIN,0,rMAX,1,kBlack,1,3,1,8,pyt.GetDiJetsNorm());
	 drawTree(trJetPythia, Form("padr>>%sDiffShapePythia",abkey.Data()),pyt.GetCut().GetAJetPartlCutW().Data(),drdbE,
	       Form("%sDiffShapePythia",abkey.Data()),";r;#rho(r)",rNBIN,0,rMAX,1,kBlack,1,3,1,4,pyt.GetDiJetsNorm());
	 drawTree(trJetPyquen, Form("pndr>>%sDiffShapePyquen",nbkey.Data()),pyq.GetCut().GetNJetPartlCutW().Data(),drdbE,
	       Form("%sDiffShapePyquen",nbkey.Data()),";r;#rho(r)",rNBIN,0,rMAX,1,kBlue,1,3,1,8,pyq.GetDiJetsNorm());
	 drawTree(trJetPyquen, Form("padr>>%sDiffShapePyquen",abkey.Data()),pyq.GetCut().GetAJetPartlCutW().Data(),drdbE,
	       Form("%sDiffShapePyquen",abkey.Data()),";r;#rho(r)",rNBIN,0,rMAX,1,kBlue,1,3,1,4,pyq.GetDiJetsNorm());

	 // -- ratio --
	 drawDivHist(Form("%sDiffShapePyquen",nbkey.Data()),Form("%sDiffShapePythia",nbkey.Data()),drsgFFE,
	       Form("%sDiffShapeRatio",nbkey.Data()),";r;#rho_{pythia}(r)/#rho_{pyquen}(r)",rNBIN,0,rMAX,0,kBlue,1,3,1,8,3.);
	 drawDivHist(Form("%sDiffShapePyquen",abkey.Data()),Form("%sDiffShapePythia",abkey.Data()),drdbFFE,
	       Form("%sDiffShapeRatio",abkey.Data()),";r;#rho(r)",rNBIN,0,rMAX,0,kBlue,1,3,1,4,3.);

	 // === Integral Jet Shapes ===
	 // -- Ref R cone based --
	 drawIntHist(Form("%sDiffShapePythia",nbkey.Data()),Form("%sIntShapePythia",nbkey.Data()),drsgP,"Integrated Jet Shape","r",
	       Form("#sum_{0.%d cone}E_{T}/E_{T}^{1.0 cone}",vcone[ic]),-1,0,kBlack,1,3,shapeMsz,kFullCircle,intShpMax);
	 drawIntHist(Form("%sDiffShapePythia",abkey.Data()),Form("%sIntShapePythia",abkey.Data()),drdbP,"Integrated Jet Shape","r",
	       Form("#sum_{0.%d cone}E_{T}/E_{T}^{1.0 cone}",vcone[ic]),-1,0,kBlack,1,3,shapeMsz,kOpenCircle,intShpMax);
	 drawIntHist(Form("%sDiffShapePyquen",nbkey.Data()),Form("%sIntShapePyquen",nbkey.Data()),drdbP,"Integrated Jet Shape","r",
	       Form("#sum_{0.%d cone}E_{T}/E_{T}^{1.0 cone}",vcone[ic]),-1,0,kBlue,1,3,shapeMsz,kFullCircle,intShpMax);
	 drawIntHist(Form("%sDiffShapePyquen",abkey.Data()),Form("%sIntShapePyquen",abkey.Data()),drdbP,"Integrated Jet Shape","r",
	       Form("#sum_{0.%d cone}E_{T}/E_{T}^{1.0 cone}",vcone[ic]),-1,0,kBlue,1,3,shapeMsz,kOpenCircle,intShpMax);
      }
   }
   
   //=== Save and exit ===
   printAllCanvases(plotdir);
   outfile->Write();
   outfile->Close();
}
