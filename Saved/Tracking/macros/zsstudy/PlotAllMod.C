#include "ZSCluster.C"

struct StripEvent{
  UInt_t run;
  UInt_t evt;
  UInt_t id;
  UInt_t mod;
  UInt_t nstrip;
};


void PlotAllMod(
                //TString infname="~/scratch01/work/HLT/hi2011zsyueshi_441/src/Saved/TrackerZS/test/sstr_RECOZSHEADyueshi10195pm_.root",
                //TString outdir="out/2011.10.20/zsyueshi_badall"
                TString infname="/home/frankma/work/HI/HLT/sw/hi2011zsyueshi_441/src/Saved/TrackerZS/test/sstr_RECOivan1025_.root",
                TString outdir="out/2011.10.25/ivan_badall"
                )
{
  TFile * inf1 = new TFile(infname);
  
  //TTree * tbase1 = (TTree*)inf1->Get("ssntRRZS/pr");
  TTree * tbase1 = (TTree*)inf1->Get("ssntBad/vr");
  
  //Declaration of leaves types
  StripEvent se;
  
  // Set branch addresses.
  tbase1->SetBranchAddress("module",&se.run);
  
  Long64_t nentries = tbase1->GetEntries();
  Long64_t maxPlot = 1000;
  
  Long64_t nbytes = 0;
  for (Long64_t i=0; i<nentries&&i<maxPlot;++i) {
    nbytes += tbase1->GetEntry(i);
    cout << "run" << se.run << " evt: " << se.evt << " id: " << se.id << " nstrip: " << se.nstrip << endl;
    ZSCluster(infname,se.evt,se.id,0,768,outdir,"zsivan2011.10.24");
  }
}
