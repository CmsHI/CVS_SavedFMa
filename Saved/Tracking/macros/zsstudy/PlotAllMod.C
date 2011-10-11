#include "ZSCluster.C"

void PlotAllMod(
                TString outdir="out/2011.10.07/badall_andsavedVR"
                )
{
  TFile * inf1 = new TFile("../ntout/sstr_zshead10_05_23pm_pedsub_allvrzscl_restoreThresh0MeanCM128GTV4_hi2010ped.root");
  
  TTree * tbase1 = (TTree*)inf1->Get("ssntRRZS/pr");
  
  //Declaration of leaves types
  UInt_t          module_run;
  UInt_t          module_evt;
  UInt_t          module_id;
  UInt_t          module_mod;
  UInt_t          module_nstrip;
  
  // Set branch addresses.
  tbase1->SetBranchAddress("module",&module_run);
  tbase1->SetBranchAddress("module",&module_evt);
  tbase1->SetBranchAddress("module",&module_id);
  tbase1->SetBranchAddress("module",&module_mod);
  tbase1->SetBranchAddress("module",&module_nstrip);
  
  Long64_t nentries = tbase1->GetEntries();
  
  Long64_t nbytes = 0;
  for (Long64_t i=0; i<nentries;i++) {
    nbytes += tbase1->GetEntry(i);
    cout << module_id << endl;
    ZSCluster(module_id,outdir);
  }
}
