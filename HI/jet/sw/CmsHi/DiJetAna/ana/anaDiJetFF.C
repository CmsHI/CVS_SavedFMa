#include <iostream>
#include "TTree.h"
#include "TH1D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TCut.h"
#include "TLegend.h"
#include "CmsHi/DiJetAna/ana/aliases_dijet.C"
#include "CmsHi/DiJetAna/ana/selectionCut.h"
using namespace std;

void anaDiJetFF(int doMC=1,
    const char * inFile0Name="../process_aod/outputs/dijetAna_anaJet_Mc1_2k.root")
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;

  TFile * inFile0 = new TFile(inFile0Name);
  inFile0->ls();
  TTree *mcj2t3, *mcj2t3peri, *mcj2t0, *mcj1t0;
  inFile0->GetObject("dijetAna_mc/djTree",mcj2t3);
  inFile0->GetObject("dijetAna_mc_periph/djTree",mcj2t3peri);

  cout << "dijetAna_mc/mcj2t3 # entries: " << mcj2t3->GetEntries() << endl;
  cout << "dijetAna_mc_periph/mcj2t3 # entries: " << mcj2t3peri->GetEntries() << endl;

  // Define dijet selection
  selectionCut mcAna(1,1);
  cout << "DJ selection: " << TString(mcAna.DJ) << endl;
  cout << "dijetAna_mc/mcj2t3 # entries: " << mcj2t3->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj2t3->GetEntries(mcAna.DJ) << endl;
  cout << "dijetAna_mc_periph/mcj2t3 # entries: " << mcj2t3peri->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj2t3peri->GetEntries(mcAna.DJ) << endl;

  //  - DiJet FF -
}
