{
  // Set up FW Lite for automatic loading of CMS libraries
  // and data formats.   As you may have other user-defined setup
  // in your rootlogon.C, the CMS setup is executed only if the CMS
  // environment is set up.
  //
  TString cmsswbase = getenv("CMSSW_BASE");
  if (cmsswbase.Length() > 0) {
    //
    // The CMSSW environment is defined (this is true even for FW Lite)
    // so set up the rest.
    //
    cout << "Loading FW Lite setup." << endl;
    gSystem->Load("libFWCoreFWLite.so");
    //gSystem->Load("libDataFormatsFWLite.so");
    //gSystem->Load("libDataFormatsPatCandidates.so");
    //gSystem->Load("libDataFormatsCommon.so");
    gSystem->Load("libDataFormatsMath.so");
    //gSystem->Load("pluginSavedDiJetAna.so");

    // JEC
    //gSystem->Load("libCondFormatsJetMETObjects.so");

    AutoLibraryLoader::enable();
  }

  // include paths
  gSystem->AddIncludePath("-I$CMSSW_BASE/src/");

  // styles
  gROOT->Macro("$CMSSW_BASE/src/MNguyen/JetAnalysisMacros/InclusiveJetAnalyzer/rootlogon.C+");
  //gROOT->Macro("$CMSSW_BASE/src/Saved/Utilities/macros/logon/rootlogon_dNdEtaBase.C+");
  //gROOT->Macro("$CMSSW_BASE/src/Saved/Utilities/macros/logon/rootcolors.C+");
  gStyle->SetOptStat(0);   
  //gStyle->SetOptStat(1111);
  //gStyle->SetPadLeftMargin(0.2);
  gStyle->SetOptFit(0);
  //gStyle->SetMarkerSize(0.8);
  gStyle->SetHistLineWidth(3);


  // helper classes
  gROOT->Macro("$CMSSW_BASE/src/Saved/Utilities/macros/cplot/CPlot.cc+");
  gROOT->Macro("$CMSSW_BASE/src/Saved/DiJetAna/macros/anaY1J/JetFragAna.C+");
}
