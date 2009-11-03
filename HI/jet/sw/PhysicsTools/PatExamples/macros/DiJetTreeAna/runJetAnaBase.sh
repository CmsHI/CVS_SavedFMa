#!/bin/bash -


if [ $# -eq 0 ]; then
   echo "Usage: "
   echo "  $0 <infile_names> <nljetmin> <nljetmax> <aljetmin> <jdphimin> <tag_name>"
   echo "Eg:"
   echo "  ./runJetAnaBase.sh tree_djana_j2_0_t1_RECO_*to*_sw44.root 35 300 30 2.8 testana"
   exit 1;
fi

echo "$@"

indir=/d01/frankma/scratch/data/ana/hydjet_noColl_dijet_2009_10_1
plotbase=plots
infnames=$1
AnaNJetEtMin=$2
AnaNJetEtMax=$3
AnaAJetEtMin=$4
JDPhiMin=$5
tag=$6

#=== analysis vars ===
export NXIBIN=25
export XIMAX=7
export XIYMAX=10
export doCheck="kTRUE"

#== output vars ===
type=`echo $name | grep -o tree.*RECO`
ana=jet_${AnaNJetEtMin}_${AnaNJetEtMax}_${AnaAJetEtMin}_jdphi_${JDPhiMin}_${tag}

#./runJetAnaBase.sh
plotanabase=$plotbase/$job/$type/$ana
plotjFF=$plotanabase/jFF
echo $plotjFF
cmd='root -b -q plotFF.C+\(\"'$indir/$infnames'\",'$AnaNJetEtMin,$AnaNJetEtMax,$AnaAJetEtMin,$JDPhiMin,'\"'$plotjFF'\"',$NXIBIN,$XIMAX,$XIYMAX,$doCheck'\)'
echo $cmd
eval $cmd
