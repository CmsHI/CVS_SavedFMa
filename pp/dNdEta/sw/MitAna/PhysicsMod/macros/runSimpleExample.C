// $Id: runSimpleExample.C,v 1.12 2009/07/22 19:44:10 loizides Exp $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#include "MitAna/PhysicsMod/interface/SimpleExampleMod.h"
#endif

//--------------------------------------------------------------------------------------------------
void runSimpleExample(
   const char *files = "/home/edwenger/merge_BSC_Tuned_v1_Pythia_MinBias_D6T_900GeV_d20091210.root",
   Bool_t useHlt=1
  )
{
  using namespace mithep;
  gDebugMask  = Debug::kAnalysis;
  gDebugLevel = 1;

  SimpleExampleMod *smod = new SimpleExampleMod;
  smod->SetUseHfOnly(true);

  // set up analysis
  Analysis *ana = new Analysis;
  ana->SetSuperModule(smod);
  ana->AddFile(files);
  ana->SetUseHLT(useHlt);
  if (gROOT->IsBatch()) 
    ana->SetOutputName("mit-example-hist.root");
  
  // run the analysis after successful initialisation
  ana->Run(!gROOT->IsBatch());
}
