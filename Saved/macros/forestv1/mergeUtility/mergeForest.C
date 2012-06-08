#include <TChain.h>
#include <TFile.h>
#include <iostream>

void mergeForest(){

   const char* fname = "/mnt/hadoop/cms/store/user/frankma/HiForest/promptskim-hihighpt-jet115-v0/*.root";
   TString outfname="/net/hidsk0001/d00/scratch/frankma/tmp/HiForest_promptskim-hihighpt-jet115-v0_part.root";

   string dir[17] = {
      "hltanalysis",
      "skimanalysis",
      "icPu5JetAnalyzer",
      "akPu3PFJetAnalyzer",
      "anaTrack",
      "multiPhotonAnalyzer",
      "pfcandAnalyzer",
      "anaMET",
      "muonTree",
      "rechitanalyzer",
      "rechitanalyzer",
      "rechitanalyzer",
      "rechitanalyzer",
      "rechitanalyzer",
      "hiEvtAnalyzer",
      "hcalNoise",
      "genpana"
   };

   string trees[17] = {
      "HltTree",
      "HltTree",
      "t",
      "t",
      "trackTree",
      "photon",
      "pfTree",
      "metTree",
      "HLTMuTree",
      "eb",
      "ee",
      "hbhe",
      "hf",
      "tower",
      "HiTree",
      "hcalNoise",
      "photon"
   };


   TChain* ch[17];

   int N = 17;

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

