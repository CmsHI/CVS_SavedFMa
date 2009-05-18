// $Id: runJFcompMix.C 3189 2006-08-16 22:00:39Z loizides $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <Varargs.h>
#include <TEnv.h>
#include <TString.h>
#include <TSystem.h>
#include <TObjString.h>
#include <TAModule.h>
#include <THIDebug.h>
#include <THIDiJetTruthAnaMod.h>
#include <THIAnalysis.h>
#include "ana/runAnaGeneric.C"
#endif

// the macro which does the work
void runDijetTruthAnaFrag(Int_t rnum, const char * genIn, const char *hrts, Bool_t useProof=kFALSE)
{
   gDebugMask = THIDebug::kAnalysis;
   gDebugLevel = 1; //Increase this if needed.

   // set up TAM modules to be used
   TAModule *anamod = new 
      TAModule("topmod", "Analysis Top Module");

   THIDiJetTruthAnaMod *anaTruth = new
      THIDiJetTruthAnaMod("anaTruth","Jet Finder Analysis Calo");
   anaTruth->SetStatOnly(kFALSE);
   anamod->Add(anaTruth);

//--------

   // set up top-level analysis class
   THIAnalysis *ana = new THIAnalysis(useProof);
   ana->SetSuperModule(anamod); // pass the top-level module

   // add the files to the analysis
   ana->AddFile(hrts); 

   // set output file name
   //   ana->SetAnaOutputName(Form("ptspec_%06d.root",rnum));
   ana->SetAnaOutputName(Form("%s/%s_truth_%06d.root",getenv("HIROOT_BASEURL"),genIn,rnum));

   // run generic analysis macro
   runAnaGeneric(ana);

   // clean up
   delete anamod;
}
