// $Id: rootlogon.C,v 1.1 2009/10/21 20:05:34 loizides Exp $

{
  gROOT->Macro("$CMSSW_BASE/src/MitAna/macros/setRootEnv.C+");

  loadLibraries("libMitQcdTracklets*.so");
}
