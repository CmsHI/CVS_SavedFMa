#!/bin/bash -
anaV=v20RandomCone

#for BkgSub in "EtaReflSingle" "EtaRefl" "None"; do # systematic check on different bkg sub
#for BkgSub in "EtaReflSingleEtaPos" "EtaReflSingleEtaNeg"; do # systematic check on eta region
for BkgSub in "EtaReflSingle"; do # paper: EtaReflSingle
  for module in djcalo; do
    ./runAnaJetFragHI.sh dj_data50v2_${module}_120_50.root HCPR_J50U $anaV $module 0 $BkgSub 1
    ./runAnaJetFragHI.sh dj_HyUQ80v4_${module}_100_50.root HydjetAll ${anaV}ReWt $module 1 $BkgSub 1
    #./runAnaJetFragHI.sh dj_HCPR-J50U_OfficialSelv3_${module}_100_50.root HCPR_J50U_PFPhoton $anaV $module 0 $BkgSub 1
    #./runAnaJetFragHI.sh dj_HyUQ80v5_${module}_100_50.root HydjetAll_PFPhoton ${anaV}ReWt $module 1 $BkgSub 1
    # == Random Cone Study ==
    #./runAnaJetFragHI.sh dj_datambv3_${module}_c0to30.root HCPR_MBMixJ50 ${anaV}ReWt $module 1 $BkgSub 1
    #./runAnaJetFragHI.sh dj_data50v2_${module}_100_50_c0to30.root HCPR_J50MixJ50 ${anaV} $module 0 $BkgSub 1
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
