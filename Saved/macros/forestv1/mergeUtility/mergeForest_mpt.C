#include <TChain.h>
#include <TFile.h>
#include <iostream>

void mergeForest_mpt(){

   const char* fname = "/mnt/hadoop/cms/store/user/frankma/HiForest/promptskim-hihighpt-jet115-v2MgTrk/*.root";
   TString outfname="/net/hidsk0001/d00/scratch/frankma/tmp/HiForest_promptskim-hihighpt-jet115-v2MgTrk.root";

   //const int N = 10;
   const int N = 9;

   string dir[N] = {
      "hltanalysis",
      "skimanalysis",
      "icPu5JetAnalyzer",
      "akPu3PFJetAnalyzer",
      "multiPhotonAnalyzer",
      "anaTrack",
      "pfcandAnalyzer",
      //"anaPixTrack",
      "hcalNoise",
      "hiEvtAnalyzer"
   };

   string trees[N] = {
      "HltTree",
      "HltTree",
      "t",
      "t",
      "photon",
      "trackTree",
      "pfTree",
      //"trackTree",
      "hcalNoise",
      "HiTree",
   };


   TChain* ch[N];


   for(int i = 0; i < N; ++i){
      ch[i] = new TChain(string(dir[i]+"/"+trees[i]).data());
      ch[i]->Add(fname);      
   }
   

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

