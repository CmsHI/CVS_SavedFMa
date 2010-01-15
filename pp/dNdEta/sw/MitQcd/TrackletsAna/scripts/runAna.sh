#!/bin/bash -
if [ $# -lt 1 ]; then
  echo Usage
  echo "  $0 <source> [tag]"
  exit
fi

source=$1
tag=$2

if [ $source -eq 4 ]; then
  # high stat 900 GeV, first run made in Dec 2009
  inDir=~cer/bambu/MinBias/r124022_rr
  inFile='bambu_000_*.root'
  isMC=0
  outFile=pixelTree_124022a3a4_v1_MB.root
elif [ $source -eq 5 ]; then
  # for final 900 GeV
  # contains also 023, 024
  # Yenjie: use 41- 96 run124023
  inDir=/d100/data/MinimumBias-ReReco/124022a3a4-vtxcomp
  inFile='bambu_000_*.root'
  isMC=0
  outFile=pixelTree_124022a3a4-vtxcomp_MB.root
elif [ $source -eq 6 ]; then
  # for final 2.36 TeV
  inDir=/d100/data/MinimumBias-ReReco/124120-vtxcmp
  inFile='bambu_000*.root'
  isMC=0
  outFile=pixelTree_124120-vtxcmp_MB.root
fi

outDir=.

mkdir -p $outDir/log
root -b -q runPixelTreeMaker.C+\($isMC,\"$inDir/$inFile\",\"$outDir/${outFile}\"\) > $outDir/log/${outFile%.root}_$tag.txt
