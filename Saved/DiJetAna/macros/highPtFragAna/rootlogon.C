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
      gSystem->Load("libDataFormatsMath.so");
      // JEC
      //gSystem->Load("libCondFormatsJetMETObjects.so");
      AutoLibraryLoader::enable();
   }

   // include paths
   gSystem->AddIncludePath("-I$CMSSW_BASE/src/");

   // styles
   gROOT->Macro("$CMSSW_BASE/src/Saved/Utilities/macros/logon/rootlogon_dNdEtaBase.C+");
   gStyle->SetOptStat(0);   
   //gStyle->SetOptStat(1111);
   //gStyle->SetPadLeftMargin(0.2);
   gStyle->SetOptFit(0);
   gStyle->SetMarkerSize(1.);
   gStyle->SetHistLineWidth(2);
   gStyle->SetErrorX(0); 
}
