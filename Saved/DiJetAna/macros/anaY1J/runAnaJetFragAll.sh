#!/bin/bash -
anaV=v18

for BkgSub in "EtaReflSingle" "EtaRefl"; do
  for module in djcalo; do
    ./runAnaJetFragHI.sh dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0_${module}_120_50.root HCPR_J50U $anaV $module 0 $BkgSub 1
    ./runAnaJetFragHI.sh dj_HyUQ80v4_${module}_120_50.root HydjetAll ${anaV}ReWt $module 1 $BkgSub 1
  done

  for module in djcalo_genp; do
    ./runAnaJetFragHI.sh dj_HyUQ80v3_${module}_120_50.root HydjetSig ${anaV}ReWt $module 1 $BkgSub 1
    ./runAnaJetFragHI.sh dj_HyUQ80v4_${module}_120_50.root HydjetAll ${anaV}ReWt $module 1 $BkgSub 1
  done

  for module in djgen; do
    #./runAnaJetFragHI.sh dj_HyUQ80v3_${module}_120_50.root HydjetSig ${anaV}ReWt $module 1 $BkgSub 0
    ./runAnaJetFragHI.sh dj_HyUQ80v4_${module}_120_50.root HydjetAll ${anaV}ReWt $module 1 $BkgSub 0
  done
done

#for BkgSub in "None"; do
#  for module in djcalo_genp; do
#    ./runAnaJetFragHI.sh dj_HyUQ80v3_${module}_120_50.root HydjetSig ${anaV}ReWt $module 1 $BkgSub 1
#  done
#done
