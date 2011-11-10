#include <iostream>
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
using namespace std;

void PrintRate(
               //TString infname="../ntout/L1Tree_mb_mctag_l1hlt_masterv1.root"
               //TString infname="../ntout/L1Tree_mb_mctag_l1hlt_v3.root"
               //TString infname="/net/hisrv0001/home/frankma/scratch01/data/HCRaw/hcraw-rerunl1hlt-masterhil1mctagv1/merge/all.root"
               //TString infname="/net/hisrv0001/home/frankma/scratch01/data/HCRaw/hcraw-rerunl1hlt-masterhil1mctagv2/merge/all.root"
               //TString infname="/net/hisrv0001/home/davidlw/scratch1/HLTStudies/CMSSW_4_4_2_HIProd/src/openhlt_mc.root"
               //TString infname="/net/hidsk0001/d00/scratch/dav2105/HIHLT_Validation_Test_GRIF_v10.root"
               //TString infname="../trees/HIHLT_Validation_Test_GRIF_v10.root"
               //TString infname="/home/frankma/work/HI/HLT/sw/hi2011trigana_442p2/src/CmsHi/HiHLTAlgos/test/openhlt_data_raw_180892_full.root"
               // mc
               TString infname="/net/hisrv0001/home/davidlw/scratch1/openhlt/merged_openhlt_mc_hydjet.root"
               )
{
   TFile * inf = new TFile(infname);
   
   TTree * t = (TTree*)inf->Get("hltbitnew/HltTree");
   //TTree * t = (TTree*)inf->Get("hltana/HltTree");
   TString trigMB = "HLT_HIMinBiasHfOrBSC_v1";
   float nMB = t->GetEntries(trigMB);
   
   cout << "Total events: " << t->GetEntries() << endl;
   cout << "Total " << trigMB << " events: " << nMB << endl;

   cout << "Fraction out of " << trigMB << " events: " << endl;
   cout << "l1jet" << endl;
   cout << "L1_SingleJet36_BptxAND: " << t->GetEntries("L1_SingleJet36_BptxAND&&"+trigMB)/nMB << endl;
   cout << "L1_SingleJet52_BptxAND: " << t->GetEntries("L1_SingleJet52_BptxAND&&"+trigMB)/nMB << endl;
   cout << "L1_SingleJet68_BptxAND: " << t->GetEntries("L1_SingleJet68_BptxAND&&"+trigMB)/nMB << endl;
   cout << "L1_SingleJet80_BptxAND: " << t->GetEntries("L1_SingleJet80_BptxAND&&"+trigMB)/nMB << endl;
   cout << "L1_SingleJet92_BptxAND: " << t->GetEntries("L1_SingleJet92_BptxAND&&"+trigMB)/nMB << endl;
   cout << "L1_SingleJet128_BptxAND: " << t->GetEntries("L1_SingleJet128_BptxAND&&"+trigMB)/nMB << endl;
   cout << "photon" << endl;
   cout << "L1_SingleEG5_BptxAND: " << t->GetEntries("L1_SingleEG5_BptxAND&&"+trigMB)/nMB << endl;
   cout << "L1_SingleEG8_BptxAND: " << t->GetEntries("L1_SingleEG8_BptxAND&&"+trigMB)/nMB << endl;
   cout << "l1muon" << endl;
   cout << "L1_DoubleMuOpen_BptxAND: " << t->GetEntries("L1_DoubleMuOpen_BptxAND&&"+trigMB)/nMB << endl;
   cout << "l1Ett" << endl;
   cout << "L1_ETT100_BptxAND: " << t->GetEntries("L1_ETT100_BptxAND&&"+trigMB)/nMB << endl;
   cout << "L1_ETT140_BptxAND: " << t->GetEntries("L1_ETT140_BptxAND&&"+trigMB)/nMB << endl;
   cout << "L1_ETT220_BptxAND: " << t->GetEntries("L1_ETT220_BptxAND&&"+trigMB)/nMB << endl;
   cout << "L1_ETT2000: " << t->GetEntries("L1_ETT2000&&"+trigMB)/nMB << endl;
   cout << "l1 met" << endl;
   cout << "L1_ETM30_BptxAND: " << t->GetEntries("L1_ETM30_BptxAND&&"+trigMB)/nMB << endl;
   cout << "L1_ETM50_BptxAND: " << t->GetEntries("L1_ETM50_BptxAND&&"+trigMB)/nMB << endl;
   
   TString hltv="_v1";
   cout << "hlt version " << hltv << endl;
   cout << "HLT_HIJet55: " << t->GetEntries("HLT_HIJet55"+hltv+"&&"+trigMB)/nMB << endl;
   cout << "HLT_HIJet65: " << t->GetEntries("HLT_HIJet65"+hltv+"&&"+trigMB)/nMB << endl;
   cout << "HLT_HIJet80: " << t->GetEntries("HLT_HIJet80"+hltv+"&&"+trigMB)/nMB << endl;
   cout << "HLT_HIJet95: " << t->GetEntries("HLT_HIJet95"+hltv+"&&"+trigMB)/nMB << endl;
   cout << "HLT_HIDiJet55: " << t->GetEntries("HLT_HIDiJet55"+hltv+"&&"+trigMB)/nMB << endl;
   cout << "HLT_HIJet65_Jet55: " << t->GetEntries("HLT_HIJet65_Jet55"+hltv+"&&"+trigMB)/nMB << endl;
}
