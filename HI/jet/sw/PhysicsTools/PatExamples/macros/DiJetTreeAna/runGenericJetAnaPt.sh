#!/bin/bash -
if [ $# -eq 0 ]; then
   echo "Usage: "
   echo "  $0 <tag_name>"
   echo "Eg:"
   echo "  ./runGenericJetAnaPt.sh sw46_2"
   exit 1;
fi

echo "$@"

#== input vars==
tag=$1
#signal=pyquen_unquendijet_nocoll_pt120to170_d20091025

AnaNJetEtMin=135
AnaNJetEtMax=300
AnaAJetEtMin=100
JDPhiMin=2.85

cmssw=CMSSW_3_3_1_fix03
indir=/d01/frankma/scratch/data/ana/$cmssw/Hydjet_MinBias_noColl_4TeV
for signal in pyquen_quendijet_nocoll_pt120to170_d20091024 pyquen_unquendijet_nocoll_pt120to170_d20091025; do
  for anatype in j2_1_t1 j2_0_t1 j1_1_t0 j0_1_t0; do
    #for anatype in j2_1_t1; do
    export plotbase=plots/$cmssw/Hydjet_MinBias_noColl_4TeV/$signal/$tag
    ./runJetAnaBase.sh tree_djana_${anatype}_${signal}_runs*to*_${tag}.root  $AnaNJetEtMin $AnaNJetEtMax $AnaAJetEtMin $JDPhiMin ${tag}_${anatype} $indir
  done
done

cmssw=CMSSW_3_3_1_fix04
indir=/d01/frankma/scratch/data/ana/$cmssw/SignalOnly
for signal in  pyquen_unquendijet_nocoll_pt120to170_d20091025; do
  for anatype in j2_1_t1 j2_0_t1 j1_1_t0 j0_1_t0; do
    #for anatype in j2_1_t1; do
    export plotbase=plots/$cmssw/SignalOnly/$signal/$tag
    ./runJetAnaBase.sh tree_djana_${anatype}_${signal}_runs*to*_${tag}.root  $AnaNJetEtMin $AnaNJetEtMax $AnaAJetEtMin $JDPhiMin ${tag}_${anatype} $indir
  done
done
