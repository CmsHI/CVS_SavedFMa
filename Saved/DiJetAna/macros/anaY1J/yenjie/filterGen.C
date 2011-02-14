#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

void filterGen(std::string inputFile_,
    Double_t nearMinEt=100,
    Double_t awayMinEt=50)
{
  // get the correct tree from input file
   TFile *inf = new TFile(inputFile_.data());
   TTree *fChain = (TTree*)inf->Get("djcalo_genp/djTree");
   cout << "Input file is " << inputFile_ << endl;

   // rename the output file
   std::string remword=".root";
   size_t pos = inputFile_.find(remword);
   std::string forOutput = inputFile_;  
   if(pos!= std::string::npos)
     forOutput.swap(forOutput.erase(pos,remword.length()));   
   std::string endfix = Form("_djcaloGenParticle_%.0f_%.0f.root",nearMinEt,awayMinEt);
   std::string outputFile = forOutput + endfix;

   // now open new root file
   TFile* newfile_data = new TFile(outputFile.data(),"recreate");
   cout << "Output file " << outputFile << endl;

   // clone tree
   TTree* newtree = fChain->CloneTree(0);
   newtree->SetMaxTreeSize(4000000000);
   cout << "Saving "  << endfix << " tree" << endl;

   Long64_t nentries = fChain->GetEntries();
   cout << "nentries = " << nentries << endl;

   Float_t nljet;
   Float_t aljet;
   fChain->SetBranchAddress("nljet",&nljet);
   fChain->SetBranchAddress("aljet",&aljet);

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     if (jentry% 10000 == 0) cout <<jentry<<" / "<<nentries<<" "<<setprecision(2)<<(double)jentry/nentries*100<<endl;
     fChain->GetEntry(jentry);
     if(nljet>nearMinEt&&aljet>awayMinEt) {
       newtree->Fill();
     }
   }
 
   newtree->Print();
   newtree->AutoSave();
   delete newfile_data;
}
