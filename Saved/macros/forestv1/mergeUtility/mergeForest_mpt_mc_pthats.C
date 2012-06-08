#include <TChain.h>
#include <TFile.h>
#include <iostream>

void mergeForest_mpt_mc_pthats(
    TString inputDir,
    TString outfname
    )
{
   TString fname = Form("%s/*.root",inputDir.Data());

   const int N = 12;

   string dir[N] = {
      "genpana",
      "skimanalysis",
      "icPu5JetAnalyzer",
      "akPu3PFJetAnalyzer",
      "multiPhotonAnalyzer",
      "anaTrack",
      "pfcandAnalyzer",
      "anaPixTrack",
      "anaMergeTrack",
      "hcalNoise",
      "hiEvtAnalyzer",
      "HiGenParticleAna"
   };

   string trees[N] = {
      "photon",
      "HltTree",
      "t",
      "t",
      "photon",
      "trackTree",
      "pfTree",
      "trackTree",
      "trackTree",
      "hcalNoise",
      "HiTree",
      "hi"
   };


   TChain* ch[N];


   for(int i = 0; i < N; ++i){
      ch[i] = new TChain(string(dir[i]+"/"+trees[i]).data());
      ch[i]->Add(fname);      
   }
   
   cout << fname << endl;
   cout << outfname << endl;
   
   TFile* file = new TFile(outfname, "RECREATE");

   for(int i = 0; i < N; ++i){
      file->cd();
      cout <<string(dir[i]+"/"+trees[i]).data()<<endl;
      if (i==0) {
         file->mkdir(dir[i].data())->cd();
      } else {
         if (TString(dir[i].data())!=TString(dir[i-1].data()))  
           file->mkdir(dir[i].data())->cd();
         else 
           file->cd(dir[i].data());  
      }
      ch[i]->Merge(file,0,"keep");
   }
   cout <<"Good"<<endl;
   //file->Write();
   file->Close();

}

