{
  // stole from Ed

  //for FWLite
  gSystem->Load("libFWCoreFWLite.so");
  AutoLibraryLoader::enable();
  gSystem->Load("libDataFormatsFWLite.so");
  // for PAT
  // - cf
  //   * http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/PhysicsTools/PatExamples/bin/BuildFile?view=markup
  //   * $CMSSW_RELEASE_BASE/lib/slc4_ia32_gcc345/libDataFormatsPatCandidates.so
  gSystem->Load("libDataFormatsPatCandidates.so");
  printf("root set for fwlight. happy rooting :)\n");

  //gROOT->SetStyle("Pub");
  gROOT->SetStyle("Plain");
  gStyle->SetErrorX(0);
  gStyle->SetNdivisions(505,"x");
  gStyle->SetNdivisions(505,"y");
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetTitleBorderSize(0);

  gStyle->SetPalette(1,0);
  //gStyle->SetOptStat(0);
  //gStyle->SetStatColor(0);
  gStyle->SetLabelColor(1,"xyz");
  gStyle->SetLineWidth(1);
  gStyle->SetOptTitle(1);
  gStyle->SetTitleColor(1);
  gStyle->SetTitleTextColor(1);
  gStyle->SetTitleYOffset(1.1);

}
