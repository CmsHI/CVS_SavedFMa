#include <iostream>
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
using namespace std;

void PrintRate(
               //TString infname="../ntout/L1Tree_mb_mctag_l1hlt_masterv1.root"
               //TString infname="../ntout/L1Tree_mb_mctag_l1hlt_v3.root"
               TString infname="/net/hisrv0001/home/frankma/scratch01/data/HCRaw/hcraw-rerunl1hlt-masterhil1mctagv1/merge/all.root"
               )
{
  TFile * inf = new TFile(infname);
  
  TTree * t = (TTree*)inf->Get("hltbitnew/HltTree");
  float numTot = t->GetEntries();
  
  cout << "Total events: " << numTot << endl;

  cout << "l1" << endl;
  cout << "L1_SingleJet36_BptxAND: " << t->GetEntries("L1_SingleJet36_BptxAND")/numTot << endl;
  cout << "L1_SingleJet52_BptxAND: " << t->GetEntries("L1_SingleJet52_BptxAND")/numTot << endl;
  cout << "L1_SingleJet68_BptxAND: " << t->GetEntries("L1_SingleJet68_BptxAND")/numTot << endl;
  cout << "L1_SingleJet80_BptxAND: " << t->GetEntries("L1_SingleJet80_BptxAND")/numTot << endl;
  cout << "L1_SingleJet92_BptxAND: " << t->GetEntries("L1_SingleJet92_BptxAND")/numTot << endl;
  cout << "L1_SingleJet128_BptxAND: " << t->GetEntries("L1_SingleJet128_BptxAND")/numTot << endl;
  
  TString hltv="_v1";
  //TString hltv="";
  cout << "hlt version " << hltv << endl;
  cout << "HLT_HIJet55: " << t->GetEntries("HLT_HIJet55"+hltv)/numTot << endl;
  cout << "HLT_HIJet65: " << t->GetEntries("HLT_HIJet65"+hltv)/numTot << endl;
  cout << "HLT_HIJet80: " << t->GetEntries("HLT_HIJet80"+hltv)/numTot << endl;
  cout << "HLT_HIJet95: " << t->GetEntries("HLT_HIJet95"+hltv)/numTot << endl;
  cout << "HLT_HIDiJet55: " << t->GetEntries("HLT_HIDiJet55"+hltv)/numTot << endl;
  cout << "HLT_HIJet65_Jet55: " << t->GetEntries("HLT_HIJet65_Jet55"+hltv)/numTot << endl;
  //cout << "HLT_HIDiJet55: " << t->GetEntries("HLT_HIDiJet55and55"+hltv)/numTot << endl;
  //cout << "HLT_HIJet65_Jet55: " << t->GetEntries("HLT_HIDiJet65and55"+hltv)/numTot << endl;
}
