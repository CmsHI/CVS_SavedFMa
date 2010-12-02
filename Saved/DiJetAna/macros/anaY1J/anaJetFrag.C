#include "Saved/DiJetAna/macros/anaY1J/JetFragAna.h"
#include <TCanvas.h>
#include <iostream>
using namespace std;

void anaJetFrag(TString infile="dj_HCPR-GoodTrkAndPixel_CleanEvt1130.root")
{
  TChain * dj = new TChain("djcalo/djTree");
  dj->Add(infile);

  JetFragAna jfa(dj);
  jfa.Loop();
}
