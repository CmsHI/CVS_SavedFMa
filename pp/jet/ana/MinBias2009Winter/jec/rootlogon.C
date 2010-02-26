{
  // libs
  gROOT->Macro("$CMSSW_BASE/src/MitAna/macros/setRootEnv.C+");
  loadLibraries("libMitQcdTracklets*.so");
  loadLibraries("libCondFormatsJetMETObjects.so");
  gSystem->AddIncludePath("-I$CMSSW_RELEASE_BASE/src/");

  // styles
  gROOT->Macro("$SavedFMa/analysis/root/logon/rootlogon_dNdEtaBase.C");
}
