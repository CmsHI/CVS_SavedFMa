#!/bin/bash -
#sample=yilmaz-MinBiasPhojet_RECO_0404_v1
#sample=yilmaz-MinBiasPhojet_RECO_0413_v1
sample=yilmaz-MinBiasAtlas0Tesla_RECO_0416_v1
inDir=/castor/cern.ch/user/f/frankma/mc/ana/private_7TeV/$sample-Bambu
outDir=/d100/mc/reco/private_7TeV/$sample/pixeltree
#sample=133242-hfcuts
#inDir=/castor/cern.ch/user/f/frankma/data/MinimumBias-PromptReco/$sample
#outDir=/d100/data/MinimumBias-PromptReco/$sample/pixeltree
isMC=1

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
