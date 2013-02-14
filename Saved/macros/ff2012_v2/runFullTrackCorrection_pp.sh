#!/bin/bash -

corrset=$1
algo=$2
pthat=$3
pthatMax=$4
vzMax=$5
leadingPtMin=$6
subleadingPtMin=$7
sigDPhi=$8
genJetMode=$9

mkdir -p trkcorr/$corrset

# setup
date
sampleWt=1;
trkPtMin=0.5
echo "sample pthat: $pthat, pthatMax = $pthatMax, trkPtMin = $trkPtMin"
# samples
inputFile="/net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat${pthat}/mergedFile.root"
outputFile=trkcorr/$corrset"/"$corrset"_sigdj${pthat}to${pthatMax}.root"
# final checks
echo "input:  "$inputFile
echo "output: "$outputFile
root -b -q analyzeTrackingCorrection.C+'("'$algo'","anaTrack","'$inputFile'","'$outputFile'",'1,$pthat,$pthatMax,$sampleWt','$trkPtMin,$vzMax',-1,'$leadingPtMin,$subleadingPtMin,$sigDPhi,$genJetMode')'
