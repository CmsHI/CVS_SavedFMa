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
outputFile=trkcorr/$corrset"/"$corrset"_hy18dj${pthat}to${pthatMax}.root"
if [[ ($pthat -eq 100) ]];
then inputFile="/mnt/hadoop/cms/store/user/yenjie/HiForest_v27/Dijet${pthat}_HydjetDrum_v27_mergedV1.root"; fi
if [[ ($pthat -eq 170) ]];
then inputFile="/net/hidsk0001/d00/scratch/yjlee/merge/v27/pthat${pthat}/Dijet170_HydjetDrum_v27_mergedV1.root"; fi
if [[ ($pthat -eq 200) ]];
then inputFile="/net/hidsk0001/d00/scratch/yjlee/merge/v27/pthat${pthat}/Dijet200_HydjetDrum_v28_mergedV1.root"; fi
# final checks
echo "input:  "$inputFile
echo "output: "$outputFile
root -b -q analyzeTrackingCorrection.C+'("'$algo'","anaTrack","'$inputFile'","'$outputFile'",'$pthat,$pthatMax,$sampleWt','$trkPtMin,$vzMax',-1,'$leadingPtMin,$subleadingPtMin,$sigDPhi,$genJetMode')'
