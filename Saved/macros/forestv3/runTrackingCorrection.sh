#!/bin/bash -
corrset=JetSelTrkCorrv12XSec
mkdir -p trkcorr/$corrset

# declare -a ptHatSamples=(80 100 120 170 200 250 300 9999 -1)
declare -a ptHatSamples=(30 50 80 100 300 9999)
#declare -a ptHatSamples=(100 9999)
N=${#ptHatSamples[@]}
N=$((N-1))

vzMax=15
leadingPtMin=$1 #100
subleadingPtMin=$2 #40
sigDPhi=$3 #2.74889
genJetMode=$4

for algo in akPu3PF akPu3Calo; do
   for (( s=0; s<$N; s++ )); do
      # setup
      date
      pthat=${ptHatSamples[$s]};
      pthatMax=${ptHatSamples[$((s+1))]};
      sampleWt=1;
      trkPtMin=0.5
      echo "sample pthat: $pthat, pthatMax = $pthatMax, trkPtMin = $trkPtMin"
      # samples
      inputFile="/mnt/hadoop/cms/store/user/vzhukova/tocern_1_v25/mergedFile${pthat}.root"
      if [[ ($pthat -eq 30) || ($pthat -eq 50) ]];
      then inputFile="/mnt/hadoop/cms/store/user/vzhukova/tocern_2_v25/mergedFile${pthat}.root"; fi
      if [[ ($pthat -eq 100) ]];
      then inputFile="/mnt/hadoop/cms/store/user/vzhukova/tocern_2_v25/mergedFile${pthat}v8.root"; fi
      outputFile=trkcorr/$corrset"/"$corrset"_hy18dj${pthat}.root"
      # minbias
#       if [ $pthat -eq -1 ]; then
#         #inputFile="/mnt/hadoop/cms/store/user/jazzitup/hiForest/mergedFiles/HydjetDrum03_HiForest_v05_test02_final_40k.root"
#         inputFile="/net/hisrv0001/home/yetkin/hidsk0001/prod/HydjetDrum03/forest05/HydjetDrum03_HiForest_v05_test02.root"
#         outputFile=trkcorr/$corrset"/"$corrset"_hy18mb.root"
#       fi      
      # final checks
      echo "input:  "$inputFile
      echo "output: "$outputFile
      root -b -q analyzeTrackingCorrection.C+'("'$algo'","mergedTrack","'$inputFile'","'$outputFile'",'$pthat,$pthatMax,$sampleWt','$trkPtMin,$vzMax',-1,'$leadingPtMin,$subleadingPtMin,$sigDPhi,$genJetMode')'
   done
done
