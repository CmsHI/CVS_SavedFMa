{
  TString inDir = "/d01/frankma/scratch/pp/dNdEta/anaCMSSW_3_5_6/diffractionAna/pixel_trees/collbx/";
  TString inF = "pixelTree_run132440_PromptReco-v7_veryloosecuts_v4.root";
  TFile * inFile0 = new TFile(inDir+inF);
  cout << endl << inF << endl;
  gROOT->Macro("checkMonster.C(PixelTree,0)");
  gROOT->Macro("checkHFnHit.C(PixelTree,0)");

  inDir = "/d01/frankma/scratch/pp/dNdEta/anaCMSSW_3_5_6/diffractionAna/pixel_trees/mc//";
  inF = "pixelTree_pythiaD6t_MB7TeV_356ReRecov1_1M.root";
  TFile * inFile1 = new TFile(inDir+inF);
  cout << endl << inF << endl;
  gROOT->Macro("checkMonster.C(PixelTree,1)");
  gROOT->Macro("checkHFnHit.C(PixelTree,1)");

  inDir = "/d01/frankma/scratch/pp/dNdEta/anaCMSSW_3_5_6/diffractionAna/pixel_trees/emptybx/";
  inF = "pixelTree_emptyBx_132422-veryloosecuts_v2.root";
  TFile * inFile2 = new TFile(inDir+inF);
  cout << endl << inF << endl;
  gROOT->Macro("checkMonster.C(PixelTree,2)");
  gROOT->Macro("checkHFnHit.C(PixelTree,2)");
}
