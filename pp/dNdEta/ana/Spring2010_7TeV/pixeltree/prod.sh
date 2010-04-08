#!/bin/bash -
inDir=/d100/mc/reco/private_7TeV/yilmaz-MinBiasATLAS_RECO_0332_v1_extend
isMC=1

outDir=$inDir/pixeltree
log=$outDir/log
mkdir -p $outDir
mkdir -p $log

for i in `ls $inDir | grep bambu.*root` ; do
  cmd="./runMaker.sh $isMC $inDir/$i $outDir/PixelTree-$i >& $log/PixelTree-${i%.root}.txt"
  echo $cmd
  eval $cmd
done
