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
if [[ ($pthat -ge 0) ]]; then
   inputFile="/net/hidsk0001/d00/scratch/jazzitup/macros/hiPhotonAna2011/mergedFiles/forest_loPhoton${pthat}-HYDJET-START44-V12-Aug29th.root"
   outputFile=trkcorr/$corrset"/"$corrset"_hy18gj${pthat}to${pthatMax}.root"
else
   inputFile="/net/hidsk0001/d00/scratch/jazzitup/macros/hiPhotonAna2011/mergedFiles/forest_minbias-HYDJET-START44-V12-Aug29th.root
"
   outputFile=trkcorr/$corrset"/"$corrset"_hy18mb.root"
fi
# final checks
echo "input:  "$inputFile
echo "output: "$outputFile
root -b -q analyzeTrackingCorrectionGJ.C+'("'$algo'","anaTrack","'$inputFile'","'$outputFile'",0,'$pthat,$pthatMax,$sampleWt','$trkPtMin,$vzMax',5000,'$leadingPtMin,$subleadingPtMin,$sigDPhi,$genJetMode')'
