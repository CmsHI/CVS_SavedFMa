// $Id: runPixelTreeMaker.C,v 1.9 2009/12/09 16:37:04 loizides Exp $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TString.h>
#include <TSystem.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#include "MitAna/TreeMod/interface/HLTMod.h"
#include "MitQcd/TrackletsAna/interface/PixelsMakerMod.h"
#endif

using namespace mithep;
using namespace mithig;

//--------------------------------------------------------------------------------------------------


void runPixelTreeMaker(Bool_t doMC         = 0,
                       const char *files   = 0,
                       const char *outfile = 0,
                       UInt_t nev          = 0,
		       Float_t SDRelFrac   = 1.)
{
  gDebugMask  = Debug::kAnalysis;
  gDebugLevel = 1;
  gErrorIgnoreLevel = kInfo;

  // setup analysis object
  Analysis *ana = new Analysis;
  ana->SetUseHLT(1);
  if (nev>0)
    ana->SetProcessNEvents(nev);
  TString ofname(gSystem->Getenv("MIT_OUTPUTFILE"));
  if (!ofname.IsNull()) 
    ana->SetOutputName(ofname);

  if (files) {
    TString dummy(files);
    TString suffix(dummy(dummy.Length()-4,dummy.Length()));
    if (suffix.CompareTo("root")==0)
      ana->AddFile(files);
    else
      ana->AddFiles(files);
  }

  HLTMod *hmod = new HLTMod;
  hmod->SetPrintTable(1);
  ana->AddSuperModule(hmod);

  // setup modules
  PixelsMakerMod *mod = new PixelsMakerMod;
  mod->SetIsMC(doMC);
  mod->AddVertexName("Pixel3Vertexes");
  mod->AddVertexName("TrackletVertexes");
  mod->AddVertexName("ClusterVertexes");
  mod->AddVertexName("PixelVertexes");
  mod->AddVertexName("PrimaryVertexes");
  mod->AddVertexName("PrimaryVertexesBeamSpot");
  // - diffractive study -
  mod->SetUseHfOnly(true);
  mod->SetSDRelFrac(SDRelFrac);

  if (outfile)
    mod->SetFileName(Form("%s_SDRelFrac%.1f.root",outfile,SDRelFrac));
  else
    mod->SetFileName("pixeltree.root");

  ana->AddSuperModule(mod);
  
  // run the analysis after successful initialisation
  ana->Run(!gROOT->IsBatch());
}
