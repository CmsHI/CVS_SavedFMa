#!/bin/bash -
if [ $# -eq 0 ]; then
   echo "Usage: "
   echo "  $0 <tag_name>"
   echo "Eg:"
   echo "  ./runGenericJetAnaPt.sh sw45_j2_1_t1"
   exit 1;
fi

echo "$@"

#== input vars==
indir=/d01/frankma/scratch/data/ana/pyquen_unquendijet_nocoll_2009_11_4
export plotbase=plots/CMSSW_3_3_1_fix03/Hydjet_MinBias_noColl_4TeV/pyquen_unquendijet_nocoll_pt120to170_d20091025
tag=$1

AnaNJetEtMin=100
AnaNJetEtMax=300
AnaAJetEtMin=80
JDPhiMin=2.85

./runJetAnaBase.sh tree_djana_j2_1_t1_pyquen_unquendijet_nocoll_pt120to170_d20091025_runs*to*.root  $AnaNJetEtMin $AnaNJetEtMax $AnaAJetEtMin $JDPhiMin $tag $indir

