#include <iostream>
#include "CompareTreeL1.C"
#include "TrigEffvJEt.C"
#include "../anant/analyzeTrig.C"
using namespace std;

void jetTrigVal()
{
   TString data,ref;
   // 9x9
   //data = "/d100/velicanu/tmp/hiexp-hirun2011-r181688-reco-v1-collisionEvents_lowerSC_autohlt.root";
   //ref = "/d100/velicanu/tmp/hiexp-hirun2011-r181611-reco-v1-collisionEvents_lowerSC_autohlt.root";

   // 170x170
//   data = "/d100/velicanu/tmp/hiexp-hirun2011-r181688-reco-v1-collisionEvents_lowerSC_autohlt.root";
//   ref = "/d100/velicanu/tmp/hiexp-hirun2011-r181611-reco-v1-collisionEvents_lowerSC_autohlt.root";

//   data = "/d100/velicanu/tmp/hiexp-hirun2011-r181691-reco-v1-collisionEvents_lowerSC_autohlt.root";
//   ref = "/d100/velicanu/tmp/hiexp-hirun2011-r181611-reco-v1-collisionEvents_lowerSC_autohlt.root";

//   data = "/d100/velicanu/tmp/hiexp-hirun2011-r181695-reco-v1-collisionEvents_lowerSC_autohlt.root";
//   ref = "/d100/velicanu/tmp/hiexp-hirun2011-r181611-reco-v1-collisionEvents_lowerSC_autohlt.root";

   // 358x358
//   data = "/d101/kimy/macro/hlt2011/mb2011/openhlt_expressRecoRun181969Full-v2.root";
//   ref = "/d01/velicanu/tmp/hiexp-hirun2011-r181611-reco-v1-collisionEvents_lowerSC_autohlt.root";
//   CompareTreeL1(data,ref,"181969v181611bad","HLT_HIL1CaloMonitor_v1&&!skim.pcollisionEventSelection","HLT_HIMinBiasHfOrBSC_v1&&skim.pcollisionEventSelection","Run 181969","Run 181611");
//   TrigEffvJEt(data,"181969bad","HLT_HIL1CaloMonitor_v1&&!skim.pcollisionEventSelection");

   data = "/d01/velicanu/tmp/hiexp-hirun2011-r182052-reco-v1-allEvents_lowerSC_autohlt.root";
   ref = "/d01/velicanu/tmp/hiexp-hirun2011-r181611-reco-v1-collisionEvents_lowerSC_autohlt.root";
   analyzeTrig("/d01/velicanu/tmp/hiexp-hirun2011-r182052-reco-v1-allEvents_lowerSC_autohlt.root",182052,"../anant");
   CompareTreeL1(data,ref,"182052v181611","HLT_HIMinBiasHfOrBSC_v1&&skim.pcollisionEventSelection","HLT_HIMinBiasHfOrBSC_v1&&skim.pcollisionEventSelection","Run 182052","Run 181611");
   TrigEffvJEt(data,"182052","HLT_HIMinBiasHfOrBSC_v1&&skim.pcollisionEventSelection");
}
