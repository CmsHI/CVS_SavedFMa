#!/bin/bash -
anaV=v19DeadForPix

#for BkgSub in "EtaReflSingle" "EtaRefl" "None"; do
for BkgSub in "EtaReflSingle"; do
#for BkgSub in "EtaReflSingleEtaPos" "EtaReflSingleEtaNeg"; do
  for module in djcalo; do
    #./runAnaJetFragHI.sh dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0_${module}_120_50.root HCPR_J50U $anaV $module 0 $BkgSub 1
    ./runAnaJetFragHI.sh dj_HyUQ80v4_${module}_100_50.root HydjetAll ${anaV}ReWt $module 1 $BkgSub 1
    #./runAnaJetFragHI.sh dj_HCPR-J50U_OfficialSelv3_${module}_100_50.root HCPR_J50U_PFPhoton $anaV $module 0 $BkgSub 1
    #./runAnaJetFragHI.sh dj_HyUQ80v5_${module}_100_50.root HydjetAll_PFPhoton ${anaV}ReWt $module 1 $BkgSub 1
  done

  for module in djcalo_genp; do
    ./runAnaJetFragHI.sh dj_HyUQ80v3_${module}_100_50.root HydjetSig ${anaV}ReWt $module 1 $BkgSub 1
#    ./runAnaJetFragHI.sh dj_HyUQ80v4_${module}_120_50.root HydjetAll ${anaV}ReWt $module 1 $BkgSub 1
  done
#
#  for module in djgen; do
#    ./runAnaJetFragHI.sh dj_HyUQ80v3_${module}_120_50.root HydjetSig ${anaV}ReWt $module 1 $BkgSub 0
#    ./runAnaJetFragHI.sh dj_HyUQ80v4_${module}_120_50.root HydjetAll ${anaV}ReWt $module 1 $BkgSub 0
#  done
done
