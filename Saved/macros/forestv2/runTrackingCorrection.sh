#!/bin/bash -
# for xsec see, https://twiki.cern.ch/twiki/bin/view/CMS/HiForest#X_HYDJET_1_8_Drum_For_analysis_a
corrset=TrkCorrv11XSec
#corrset=TrkCorrv9XSec
#corrset=TrkCorrv8JetAlgos
#corrset=trkcorr/Forest2_TrkCorrv7
mkdir -p trkcorr/$corrset

declare -a ptHatSamples=(80 100 120 170 200 250 300 9999)
N=${#ptHatSamples[@]}
N=$((N-1))

vzMax=-99

for algo in akPu3Calo akPu3PF icPu5; do
   for (( s=0; s<$N; s++ )); do
      date
      pthat=${ptHatSamples[$s]};
      pthatMax=${ptHatSamples[$((s+1))]};
      trkPtMin=0.5
      echo "sample pthat: $pthat, pthatMax = $pthatMax, trkPtMin = $trkPtMin"
      inputFile="/mnt/hadoop/cms/store/user/vzhukova/tocern_1_v25/mergedFile${pthat}.root"
      sampleWt=1;
      if [ $pthat -eq 30 ]; then inputFile="/mnt/hadoop/cms/store/user/frankmalocal/forest/Pythia${pthat}_HydjetDrum_mix01_HiForest2_v19.root"; fi
      if [[ ($pthat -eq 50) ]];
      then inputFile="/mnt/hadoop/cms/store/user/vzhukova/tocern_2_v25/mergedFile${pthat}.root"; fi
      if [[ ($pthat -eq 100) ]];
      then inputFile="/mnt/hadoop/cms/store/user/vzhukova/tocern_2_v25/mergedFile${pthat}v8.root"; fi
      outputFile=trkcorr/$corrset"/"$corrset"_hy18dj${pthat}.root"
#       ls $inputFile
      echo $outputFile
      if [ $pthat -eq 30 ]; then trkPtMin=1; fi
      root -b -q analyzeTrackingCorrection.C+'("'$algo'","'$inputFile'","'$outputFile'",'$pthat,$pthatMax,$sampleWt','$trkPtMin,$vzMax',-1)'
   done
done

# archive of samples
#    if [ $pthat -eq 50 ]; then inputFile="/mnt/hadoop/cms/store/user/frankmalocal/forest/Pythia${pthat}_HydjetDrum_mix01_HiForest2_v19.root"; fi
#    if [ $pthat -eq 80 ]; then inputFile="/net/hisrv0001/home/yenjie/scratch/hiForest/prod/production/CMSSW_4_4_2_patch6/test/forest21_simTrack/merged_pthat80_simTrack_v2.root"; fi
#    if [ $pthat -eq 100 ]; then inputFile="/mnt/hadoop/cms/store/user/vzhukova/tocern_1_v25/mergedFile${pthat}.root"; fi
#    if [ $pthat -eq 120 ]; then inputFile="/mnt/hadoop/cms/store/user/icali/Hydjet1.8/Z2/Dijet_merged/Dijet${pthat}_merged.root"; fi
#    if [ $pthat -eq 170 ]; then inputFile="/mnt/hadoop/cms/store/user/frankmalocal/forest/Pythia${pthat}_HydjetDrum_mix01_HiForest2_v19.root"; fi
#    if [ $pthat -eq 200 ]; then inputFile="/mnt/hadoop/cms/store/user/icali/Hydjet1.8/Z2/Dijet_merged/Dijet${pthat}_merged.root"; fi
#    if [ $pthat -eq 250 ]; then inputFile="/mnt/hadoop/cms/store/user/icali/Hydjet1.8/Z2/Dijet_merged/Dijet${pthat}_merged.root"; fi
#    if [ $pthat -eq 300 ]; then inputFile="/mnt/hadoop/cms/store/user/icali/Hydjet1.8/Z2/Dijet_merged/Dijet${pthat}_merged.root"; fi
