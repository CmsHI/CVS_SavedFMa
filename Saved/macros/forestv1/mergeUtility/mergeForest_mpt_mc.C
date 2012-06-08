#include <TChain.h>
#include <TFile.h>
#include <iostream>

void mergeForest_mpt_mc(){
   //const char* fname = "/net/hisrv0001/home/frankma/hadoopfranklocal/forest/Hydjet18/DiJet120_MergeTrk_v1_SimTrack/*.root";
   //TString outfname="/net/hidsk0001/d00/scratch/frankma/tmp/DiJet120_MergeTrk_v1_SimTrack.root";
   //const char* fname = "/net/hisrv0001/home/frankma/hadoopfranklocal/forest/Hydjet18/DiJet80_MergeTrkAllGenpAllCh_v1/*.root";
   //TString outfname="/net/hidsk0001/d00/scratch/frankma/tmp/DiJet80_MergeTrkAllGenpAllCh_v1.root";
   //const char* fname = "/net/hisrv0001/home/frankma/hadoopfranklocal/forest/Hydjet18/DiJet50_MergeTrkQual_v1_SimTrack/*.root";
   //TString outfname="/net/hidsk0001/d00/scratch/frankma/tmp/DiJet50_MergeTrkQual_v1_SimTrack_part.root";
   const char* fname = "/net/hisrv0001/home/frankma/hadoopfranklocal/forest/Hydjet18/DiJet120_iterTrk_v1/*.root";
   TString outfname="/net/hidsk0001/d00/scratch/frankma/tmp/DiJet120_iterTrk_v1.root";

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

