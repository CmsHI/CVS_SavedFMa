#!/bin/bash -


if [ $# -eq 0 ]; then
   echo "Usage: "
   echo "  $0 <infile_names> <nljetmin> <nljetmax> <aljetmin> <jdphimin> <tag_name> [indir]"
   echo "Eg:"
   echo "  ./runJetAnaBase.sh tree_djana_j2_0_t1_RECO_*to*_sw44.root 35 300 30 2.8 testana /d01/frankma/scratch/data/ana/hydjet_noColl_dijet_2009_10_1"
   exit 1;
fi

echo "$@"

infnames=$1
AnaNJetEtMin=$2
AnaNJetEtMax=$3
AnaAJetEtMin=$4
JDPhiMin=$5
tag=$6
indir=$7
if [ -z $indir ]; then indir=/d01/frankma/scratch/data/ana/hydjet_noColl_dijet_2009_10_1; fi
if [ -z $plotbase ]; then plotbase=plots; fi

#=== analysis vars ===
export NXIBIN=25
export XIMAX=7
export XIYMAX=10
export doCheck="kTRUE"

#== output vars ===
type=`echo $name | grep -o tree.*run`
ana=jet_${AnaNJetEtMin}_${AnaNJetEtMax}_${AnaAJetEtMin}_jdphi_${JDPhiMin}_${tag}

#./runJetAnaBase.sh
plotanabase=$plotbase/$type/$ana
plotjFF=$plotanabase/jFF
cmd='root -b -q plotFF.C+\(\"'$indir/$infnames'\",'$AnaNJetEtMin,$AnaNJetEtMax,$AnaAJetEtMin,$JDPhiMin,'\"'$plotjFF'\"',$NXIBIN,$XIMAX,$XIYMAX,$doCheck'\)'
echo $cmd
eval $cmd
cmd1='root -b -q plotFinalFF.C+\(\"'$plotjFF'/FFHistos.root\",\"'$plotanabase'\"\)'
echo $cmd1
eval $cmd1
