#include "Saved/DiJetAna/macros/anaY1J/JetFragAna.h"
#include <TCanvas.h>
#include <iostream>
using namespace std;

void anaJetFrag(TString infile="dj_HCPR-GoodTrkAndPixel_CleanEvt1130.root")
{
  TChain * dj = new TChain("djcalo/djTree");
  dj->Add(infile);

  JetFragAna jana(dj);
  jana.cut.SetDJEt(200,500,50,500,2.5);
  jana.cut.BaseCutType="S1";
  jana.cut.DJCutType = "Ana";
  jana.cut.TrkCutType = "Ana";
  jana.cut.SetCut();

  // -- analysis selections --
  cout << endl << "====== DJ Selection: " << jana.cut.DJCutType << " ======" << endl;
  cout << " DJ selection: " << TString(jana.cut.FinDJCut()) << endl;
  cout << "# DJ events passed: " << dj->GetEntries(jana.cut.FinDJCut()) << endl;


  // do ana
  jana.Loop();
}
