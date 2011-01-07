#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

void filter_chain(TString inputDir,
    TString inputFile,
    TString outputName,
    Double_t nearMinEt=120,
    Double_t awayMinEt=50,
    TString module="djcalo")
{
  // get the correct tree from input file
  cout << "Input file is " << inputFile << endl;
  TChain *fChain = new TChain(module+"/djTree");
  fChain->Add(inputDir+"/"+inputFile);

  // rename the output file
  TString endfix = Form("_%s_%.0f_%.0f.root",module.Data(),nearMinEt,awayMinEt);
  TString outputFile = outputName + endfix;

  // now open new root file
  TFile* newfile_data = new TFile(outputFile,"recreate");
  cout << "Output file " << outputFile << endl;

  // clone tree
  TTree* newtree = fChain->CloneTree(0);
  newtree->SetMaxTreeSize(7000000000.);
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
