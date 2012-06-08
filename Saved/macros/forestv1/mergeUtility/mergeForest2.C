#include <TChain.h>
#include <TFile.h>
#include <iostream>

void mergeForest2(){

   const char* fname = "/mnt/hadoop/cms/store/user/frankmalocal/forest/promptskim-hihighpt-hltjet80-pt90-v2_v3/*.root";
   TString outfname="/net/hidsk0001/d00/scratch/frankma/tmp/HiForest-promptskim-hihighpt-hltjet80-pt90-v2_v3_part.root";

   const int N = 46;
   string dir[N] = {
      "hltanalysis",
      "skimanalysis",
      "hcalNoise",
      "icPu5JetAnalyzer",
      "akPu1PFJetAnalyzer",
      "akPu2PFJetAnalyzer",
      "akPu3PFJetAnalyzer",
      "akPu4PFJetAnalyzer",
      "akPu5PFJetAnalyzer",
      "akPu6PFJetAnalyzer",
      "akPu1CaloJetAnalyzer",
      "akPu2CaloJetAnalyzer",
      "akPu3CaloJetAnalyzer",
      "akPu4CaloJetAnalyzer",
      "akPu5CaloJetAnalyzer",
      "akPu6CaloJetAnalyzer",
      "ak1PFJetAnalyzer",
      "ak2PFJetAnalyzer",
      "ak3PFJetAnalyzer",
      "ak4PFJetAnalyzer",
      "ak5PFJetAnalyzer",
      "ak6PFJetAnalyzer",
      "ak1CaloJetAnalyzer",
      "ak2CaloJetAnalyzer",
      "ak3CaloJetAnalyzer",
      "ak4CaloJetAnalyzer",
      "ak5CaloJetAnalyzer",
      "ak6CaloJetAnalyzer",
      "multiPhotonAnalyzer",
      "anaTrack",
      "mergedTrack",
      "pfcandAnalyzer",
      "anaMET",
      "muonTree",
      "hiEvtAnalyzer",
      "hltMuTree",
      "rechitanalyzer",
      "rechitanalyzer",
      "rechitanalyzer",
      "rechitanalyzer",
      "rechitanalyzer",
      "pfTowers",
      "pfTowers",
      "pfTowers",
      "pfTowers",
      "pfTowers"
   };

   string trees[N] = {
      "HltTree",
      "HltTree",
      "hcalNoise",
      "t",
      "t",
      "t",
      "t",
      "t",
      "t",
      "t",
      "t",
      "t",
      "t",
      "t",
      "t",
      "t",
      "t",
      "t",
      "t",
      "t",
      "t",
      "t",
      "t",
      "t",
      "t",
      "t",
      "t",
      "t",
      "photon",
      "trackTree",
      "trackTree",
      "pfTree",
      "metTree",
      "HLTMuTree",
      "HiTree",
      "HLTMuTree",
      "hbhe",
      "hf",
      "ee",
      "eb",
      "tower",
      "hbhe",
      "hf",
      "ee",
      "eb",
      "tower"
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

