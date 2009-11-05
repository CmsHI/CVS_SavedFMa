#!/bin/bash -
if [ $# -eq 0 ]; then
  echo Usage
  echo "  $0 <tag> [jetType] [jetCorrection?] [trackType] <filename> [indir]"
  echo "Eg:"
  echo "  $0 sw45 2 1 1 pyquen_unquendijet_nocoll_pt120to170_d20091025_runs1to40.root rfio:/castor/cern.ch/user/f/frankma/data/pat/CMSSW_3_3_1_fix03/Hydjet_MinBias_noColl_4TeV/pyquen_unquendijet_nocoll_pt120to170_d20091025"
  exit 1
fi

anatag=$1
jetType=$2
jetCorrec=$3
trackType=$4
fileName=$5
indir=$6

# genType: pp(0), HI(1)
genType=0

# HI pat files
#input=/d01/frankma/scratch/data/pat/cmssw331/hydjetNoCollElossFix02/merged/all_50.root
#input=rfio:/castor/cern.ch/user/f/frankma/data/HI/jet/pat/cmssw331/all_300.root
#input=rfio:/castor/cern.ch/user/f/frankma/data/pat/CMSSW_3_3_1_fix03/Hydjet_MinBias_noColl_4TeV/RECO_runs1to40.root
# pp pat files
input=/d01/frankma/scratch/data/pat/CMSSW_3_3_1_fix04/SignalOnly/Hydjet_MinBias_noColl_4TeV/pyquen_unquendijet_nocoll_pt120to170_d20091025/467CB4EC-B6A0-DE11-A0DA-00E08133FF64_numEvent10546.root
if [ -z $indir ]; then indir=/castor/cern.ch/user/f/frankma/data/pat/CMSSW_3_3_1_fix03/Hydjet_MinBias_noColl_4TeV/pyquen_unquendijet_nocoll_pt120to170_d20091025; fi
if [ -z $fileName ]; then fileName=pyquen_unquendijet_nocoll_pt120to170_d20091025_runs1to40.root; fi
input=$indir/$fileName

infname=${input##*/}
inname=${infname%.root}
tag=${inname}_${anatag}

if [ ! -e log ]; then mkdir log; fi
cmd="DiJetFWLiteAnalyzer inputFiles=$input genType=$genType jetType=$jetType jetCorrec=$jetCorrec trackType=$trackType tag=$tag outputevery=1 >& log/${tag}_j${jetType}_${jetCorrec}_t${trackType}.txt"

echo $cmd
eval "time $cmd"

