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
               //TString infname="/d101/kimy/macro/hlt2011/run181530/openhlt_run181531.root"
               TString infname="/d100/velicanu/hiexp-hirun2011-r181611-reco-v1-collisionEvents_lowerSC_autohlt.root"
               // mc
               //TString infname="/net/hisrv0001/home/davidlw/scratch1/openhlt/merged_openhlt_mc_hydjet.root"
               )
{
   TFile * inf = new TFile(infname);
   
   //TTree * t = (TTree*)inf->Get("hltbitnew/HltTree");
   TTree * t = (TTree*)inf->Get("hltanalysis/HltTree");
   t->AddFriend("skim=skimanalysis/HltTree",infname);
   TString trigMB = "HLT_HIMinBiasHfOrBSC_v1";
   //TString trigMB = "HLT_HIMinBiasHfOrBSC_v1&&skim.pcollisionEventSelection";
   float nMB = t->GetEntries(trigMB);
   
   cout << "Total events: " << t->GetEntries() << endl;
   cout << "Total " << trigMB << " events: " << nMB << endl;
   cout << "Fraction out of " << trigMB << " events: " << endl;
   cout << " L1 Jet" << endl;
   cout << "L1_SingleJet36_BptxAND: " << t->GetEntries("L1_SingleJet36_BptxAND&&"+trigMB)/nMB << endl;
   cout << "L1_SingleJet52_BptxAND: " << t->GetEntries("L1_SingleJet52_BptxAND&&"+trigMB)/nMB << endl;
   cout << "L1_SingleJet68_BptxAND: " << t->GetEntries("L1_SingleJet68_BptxAND&&"+trigMB)/nMB << endl;
   cout << "L1_SingleJet80_BptxAND: " << t->GetEntries("L1_SingleJet80_BptxAND&&"+trigMB)/nMB << endl;
   cout << "L1_SingleJet92_BptxAND: " << t->GetEntries("L1_SingleJet92_BptxAND&&"+trigMB)/nMB << endl;
   cout << "L1_SingleJet128_BptxAND: " << t->GetEntries("L1_SingleJet128_BptxAND&&"+trigMB)/nMB << endl;
   cout << " L1 Photon" << endl;
   cout << "L1_SingleEG5_BptxAND: " << t->GetEntries("L1_SingleEG5_BptxAND&&"+trigMB)/nMB << endl;
   cout << "L1_SingleEG8_BptxAND: " << t->GetEntries("L1_SingleEG8_BptxAND&&"+trigMB)/nMB << endl;
   cout << " L1 Muon" << endl;
   cout << "L1_DoubleMuOpen_BptxAND: " << t->GetEntries("L1_DoubleMuOpen_BptxAND&&"+trigMB)/nMB << endl;
   cout << " L1 ETT" << endl;
   cout << "L1_ETT100_BptxAND: " << t->GetEntries("L1_ETT100_BptxAND&&"+trigMB)/nMB << endl;
   cout << "L1_ETT140_BptxAND: " << t->GetEntries("L1_ETT140_BptxAND&&"+trigMB)/nMB << endl;
   cout << "L1_ETT220_BptxAND: " << t->GetEntries("L1_ETT220_BptxAND&&"+trigMB)/nMB << endl;
   cout << "L1_ETT2000: " << t->GetEntries("L1_ETT2000&&"+trigMB)/nMB << endl;
   cout << " L1 MET" << endl;
   cout << "L1_ETM30_BptxAND: " << t->GetEntries("L1_ETM30_BptxAND&&"+trigMB)/nMB << endl;
   cout << "L1_ETM50_BptxAND: " << t->GetEntries("L1_ETM50_BptxAND&&"+trigMB)/nMB << endl;
   cout << endl << " HLT Jet" << endl;
   cout << "HLT_HIJet55_v1: " << t->GetEntries("HLT_HIJet55_v1&&"+trigMB)/nMB << endl;
   cout << "HLT_HIJet65_v1: " << t->GetEntries("HLT_HIJet65_v1&&"+trigMB)/nMB << endl;
   cout << "HLT_HIJet80_v1: " << t->GetEntries("HLT_HIJet80_v1&&"+trigMB)/nMB << endl;
   cout << "HLT_HIJet95_v1: " << t->GetEntries("HLT_HIJet95_v1&&"+trigMB)/nMB << endl;
   cout << "HLT_HIJet65_Jet55_v1: " << t->GetEntries("HLT_HIJet65_Jet55_v1&&"+trigMB)/nMB << endl;
   cout << " HLT Photon" << endl;
   cout << "HLT_HISinglePhoton15_v2: " << t->GetEntries("HLT_HISinglePhoton15_v1&&"+trigMB)/nMB << endl;
   cout << "HLT_HISinglePhoton20_v2: " << t->GetEntries("HLT_HISinglePhoton20_v2&&"+trigMB)/nMB << endl;
   cout << "HLT_HISinglePhoton30_v2: " << t->GetEntries("HLT_HISinglePhoton30_v2&&"+trigMB)/nMB << endl;
   cout << "HLT_HISinglePhoton40_v2: " << t->GetEntries("HLT_HISinglePhoton40_v2&&"+trigMB)/nMB << endl;
   cout << "HLT_HIDoublePhoton20_v2: " << t->GetEntries("HLT_HIDoublePhoton20_v1&&"+trigMB)/nMB << endl;
   cout << " HLT Single Track" << endl;
   cout << "HLT_HIFullTrack12_L1Central_v1: " << t->GetEntries("HLT_HIFullTrack12_L1Central_v1&&"+trigMB)/nMB << endl;
   cout << "HLT_HIFullTrack12_L1Peripheral_v1: " << t->GetEntries("HLT_HIFullTrack12_L1Peripheral_v1&&"+trigMB)/nMB << endl;
   cout << "HLT_HIFullTrack14_L1Central_v1: " << t->GetEntries("HLT_HIFullTrack14_L1Central_v1&&"+trigMB)/nMB << endl;
   cout << "HLT_HIFullTrack14_L1Peripheral_v1: " << t->GetEntries("HLT_HIFullTrack14_L1Peripheral_v1&&"+trigMB)/nMB << endl;
   cout << "HLT_HIFullTrack20_L1Central_v1: " << t->GetEntries("HLT_HIFullTrack20_L1Central_v1&&"+trigMB)/nMB << endl;
   cout << "HLT_HIFullTrack20_L1Peripheral_v1: " << t->GetEntries("HLT_HIFullTrack20_L1Peripheral_v1&&"+trigMB)/nMB << endl;
   cout << "HLT_HIFullTrack25_L1Central_v1: " << t->GetEntries("HLT_HIFullTrack25_L1Central_v1&&"+trigMB)/nMB << endl;
   cout << "HLT_HIFullTrack25_L1Peripheral_v1: " << t->GetEntries("HLT_HIFullTrack25_L1Peripheral_v1&&"+trigMB)/nMB << endl;
   cout << " HLT Muon" << endl;
   cout << "HLT_HIL1DoubleMuOpen_v1: " << t->GetEntries("HLT_HIL1DoubleMuOpen_v1&&"+trigMB)/nMB << endl;
   cout << "HLT_HIL2DoubleMu3_v1: " << t->GetEntries("HLT_HIL2DoubleMu3_v1&&"+trigMB)/nMB << endl;
   cout << "HLT_HIL3DoubleMuOpen_v1: " << t->GetEntries("HLT_HIL3DoubleMuOpen_v1&&"+trigMB)/nMB << endl;
   cout << "HLT_HIL2Mu7_v1: " << t->GetEntries("HLT_HIL2Mu7_v1&&"+trigMB)/nMB << endl;
}
