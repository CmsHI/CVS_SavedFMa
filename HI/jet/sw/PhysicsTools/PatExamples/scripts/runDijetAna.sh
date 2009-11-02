#!/bin/bash -
if [ $# -eq 0 ]; then
  echo Usage
  echo "  $0 <tag> [jetType] [trackType] [jetCorrection?]"
  echo "  defaults: 2 1 true"
  exit 1
fi

anatag=$1
jetType=$2
trackType=$3
jetCorrec=$4
#input=/d01/frankma/scratch/data/pat/cmssw331/hydjetNoCollElossFix02/merged/all_50.root
#input=rfio:/castor/cern.ch/user/f/frankma/data/HI/jet/pat/cmssw331/all_300.root
#input=rfio:/castor/cern.ch/user/f/frankma/data/pat/CMSSW_3_3_1_fix03/Hydjet_MinBias_noColl_4TeV/RECO_runs1to40.root
#input=rfio:/castor/cern.ch/user/f/frankma/data/pat/CMSSW_3_3_1_fix03/Hydjet_MinBias_noColl_4TeV/RECO_runs41to80.root
#input=rfio:/castor/cern.ch/user/f/frankma/data/pat/CMSSW_3_3_1_fix03/Hydjet_MinBias_noColl_4TeV/RECO_runs81to120.root
input=rfio:/castor/cern.ch/user/f/frankma/data/pat/CMSSW_3_3_1_fix03/Hydjet_MinBias_noColl_4TeV/$5

infname=${input##*/}
inname=${infname%.root}
tag=${inname}_${anatag}

cmd="DiJetFWLiteAnalyzer inputFiles=$input jetType=$jetType jetCorrec=$jetCorrec trackType=$trackType tag=$tag outputevery=1 >& log/${tag}_j${jetType}_${jetCorrec}_t${trackType}.txt"

echo $cmd
eval "time $cmd"

