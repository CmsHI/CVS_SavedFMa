#!/bin/bash -
sample=yilmaz-MinBiasPhojet_RECO_0404_v1
inDir=/castor/cern.ch/user/f/frankma/mc/ana/private_7TeV/$sample-Bambu
isMC=1

outDir=/d100/mc/reco/private_7TeV/$sample/pixeltree
log=$outDir/log
mkdir -p $outDir
mkdir -p $log
echo "output to: " $outDir
echo

for i in `rfdir $inDir | grep -o bambu.*root` ; do
  cmd="./runMaker.sh $isMC rfio:$inDir/$i $outDir/PixelTree-$i >& $log/PixelTree-${i%.root}.txt"
  echo $cmd
  eval $cmd
done
