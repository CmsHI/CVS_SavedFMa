#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

void addOrderOpenHLT(std::string inputFile_, std::string outputFile = "openhlt_run181913Sorted.root", float etaCut=1.45)
{
  const int nMaxPho = 3000;
  
  TFile *inf = new TFile(inputFile_.data());
  
  TTree *fChainPhoton = (TTree*)inf->Get("simpleSCTree/superCluster");
  fChainPhoton->SetName("photon1111");
  TTree* newtree = fChainPhoton->CloneTree(0);
  newtree->SetName("yongsunPhotonTree");
  
  TTree *fChainHLT    = (TTree*)inf->Get("hltanalysis/HltTree");
  fChainHLT->SetName("fChainHLT111");
  TTree* newtreehlt = fChainHLT->CloneTree(0);
  newtreehlt->SetName("yongsunHltTree");

  TTree *fChainColl   = (TTree*)inf->Get("skimanalysis/HltTree");
  fChainColl->SetName("fChainColl111");
  TTree* newtreeSkim = fChainColl->CloneTree(0);
  newtreeSkim->SetName("yongsunSkimTree");
  
  // rename the output file
  //  std::string remword=".root";
  //  size_t pos = inputFile_.find(remword);
  //  std::string forOutput = inputFile_;  
  //  if(pos!= std::string::npos)
  //   forOutput.swap(forOutput.erase(pos,remword.length()));   
  // std::string endfix = "_orderAttached.root";

  
   // now open new root file
   TFile* newfile_data = new TFile(outputFile.data(),"recreate");
   cout << "Output file " << outputFile << endl;
   
   // clone tree

        
      //   TTree* newtreejet = fChainJet->CloneTree(0);
   
   
   newtree->SetMaxTreeSize(4000000000);

   Long64_t nentries = fChainPhoton->GetEntries();
   cout << "nentries = " << nentries << endl;
   
   Float_t et[nMaxPho];
   Float_t eta[nMaxPho];
   TBranch *b_et;
   TBranch *b_eta;

   int leading[nMaxPho];
   
   Int_t           pcollisionEventSelection;
   TBranch        *b_pcollisionEventSelection;   
   fChainColl->SetBranchAddress("pcollisionEventSelection", &pcollisionEventSelection, &b_pcollisionEventSelection);
   
   
   Int_t cBin;
   Int_t nPar;
   TBranch *b_nPar;
   fChainPhoton->SetBranchAddress("et",et,&b_et);
   fChainPhoton->SetBranchAddress("eta",eta,&b_eta);
   fChainPhoton->SetBranchAddress("nPar", &nPar, &b_nPar);
   newtree->Branch("leading",leading,"leading[nPar]/I");
   
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      if (jentry% 10000 == 0) cout <<jentry<<" / "<<nentries<<" "<<setprecision(2)<<(double)jentry/nentries*100<<endl;
      fChainPhoton->GetEntry(jentry);
      fChainColl->GetEntry(jentry);
      
      for (int j=0;j<nPar;j++) {
	 leading[j] = 0;
      }	

      int theLeadingJ = -1;
      int maxEt       = -100;
      for (int j=0;j<nPar;j++) {
	 if ( fabs(eta[j]) > etaCut ) 
	    continue;
         if ( et[j] > maxEt ) {
	    maxEt = et[j];
	    theLeadingJ = j;
	 }
      }
     
      if ( maxEt > 0 ) 
	 leading[theLeadingJ] = 1;

      newtree->Fill();
   }
   
   newtreehlt->SetMaxTreeSize(4000000000);

   for (Long64_t jentry=0; jentry< fChainHLT->GetEntries();jentry++) {
      if (jentry% 10000 == 0) cout <<jentry<<" / "<<nentries<<" "<<setprecision(2)<<(double)jentry/nentries*100<<endl;
      fChainHLT->GetEntry(jentry);
      newtreehlt->Fill();
   }
   
   for (Long64_t jentry=0; jentry< fChainColl->GetEntries();jentry++) {
      if (jentry% 10000 == 0) cout <<jentry<<" / "<<nentries<<" "<<setprecision(2)<<(double)jentry/nentries*100<<endl;
      fChainColl->GetEntry(jentry);
      newtreehlt->Fill();
   }


   /*   
   newtreejet->SetMaxTreeSize(4000000000);
   for (Long64_t jentry=0; jentry< fChainJet->GetEntries();jentry++) {
      if (jentry% 10000 == 0) cout <<jentry<<" / "<<nentries<<" "<<setprecision(2)<<(double)jentry/nentries*100<<endl;
      fChainJet->GetEntry(jentry);
      fChainColl->GetEntry(jentry);
      if ( !pcollisionEventSelection )
         continue;
      
      newtreejet->Fill();
   }
   */
   
   
   
   //  newtree2->Print();
   newtree->AutoSave();
   newtreehlt->AutoSave();
   //   newtreejet->AutoSave();
   //   fChainColl->AutoSave();
   //   delete newfile_data;
   
   
}
