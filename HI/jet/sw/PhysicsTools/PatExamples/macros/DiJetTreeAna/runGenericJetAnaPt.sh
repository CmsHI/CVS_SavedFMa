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
indir=/d01/frankma/scratch/data/ana/CMSSW_3_3_1_fix03/Hydjet_MinBias_noColl_4TeV/pyquen_unquendijet_nocoll_pt120to170_d20091025/$tag
export plotbase=plots/CMSSW_3_3_1_fix03/Hydjet_MinBias_noColl_4TeV/pyquen_unquendijet_nocoll_pt120to170_d20091025/$tag

AnaNJetEtMin=100
AnaNJetEtMax=300
AnaAJetEtMin=80
JDPhiMin=2.85

for anatype in j2_1_t1 j2_0_t1 j1_1_t0 j0_1_t0; do
  ./runJetAnaBase.sh tree_djana_${anatype}_pyquen_unquendijet_nocoll_pt120to170_d20091025_runs*to*.root  $AnaNJetEtMin $AnaNJetEtMax $AnaAJetEtMin $JDPhiMin ${tag}_${anatype} $indir
done

