{
  // Set up FW Lite for automatic loading of CMS libraries
  // and data formats.   As you may have other user-defined setup
  // in your rootlogon.C, the CMS setup is executed only if the CMS
  // environment is set up.
  //
  bool doFWLite = false;
  TString cmsswbase = getenv("CMSSW_BASE");
  if (doFWLite && cmsswbase.Length() > 0) {
    //
    // The CMSSW environment is defined (this is true even for FW Lite)
    // so set up the rest.
    //
    cout << "Loading FW Lite setup." << endl;
    gSystem->Load("libFWCoreFWLite.so");
    gSystem->Load("libDataFormatsFWLite.so");
    gSystem->Load("libDataFormatsPatCandidates.so");
    gSystem->Load("libDataFormatsCommon.so");

    // JEC
    gSystem->Load("libCondFormatsJetMETObjects.so");

    AutoLibraryLoader::enable();
  }

  // include paths
  gSystem->AddIncludePath("-I$CMSSW_BASE/src/");

  // styles
  gROOT->Macro("$CMSSW_BASE/src/Saved/Utilities/macros/logon/rootlogon_dNdEtaBase.C+");
  //gStyle->SetOptStat(1111);
  //gStyle->SetPadLeftMargin(0.2);
  //gStyle->SetMarkerSize(1.3);


  // helper classes
  gROOT->Macro("$CMSSW_BASE/src/Saved/Utilities/macros/cplot/CPlot.cc+");
}
