#!/bin/bash -
corrset=IterTrkCorrv12XSec
mkdir -p trkcorr/$corrset

# declare -a ptHatSamples=(80 100 120 170 200 250 300 9999 -1)
declare -a ptHatSamples=(80 100 120 170 200 250 300 9999)
N=${#ptHatSamples[@]}
N=$((N-1))

vzMax=15
leadingPtMin=$1 #100
subleadingPtMin=$2 #40
sigDPhi=$3 #2.74889
genJetMode=$4

for algo in akPu3PF; do
   for (( s=0; s<$N; s++ )); do
      # setup
      date
      pthat=${ptHatSamples[$s]};
      pthatMax=${ptHatSamples[$((s+1))]};
      sampleWt=1;
      trkPtMin=0.5
      echo "sample pthat: $pthat, pthatMax = $pthatMax, trkPtMin = $trkPtMin"
      # samples
      outputFile=trkcorr/$corrset"/"$corrset"_hy18dj${pthat}.root"
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
   done
done
