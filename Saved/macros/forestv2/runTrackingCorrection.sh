#!/bin/bash -
# for xsec see, https://twiki.cern.ch/twiki/bin/view/CMS/HiForest#X_HYDJET_1_8_Drum_For_analysis_a
corrset=test
#corrset=trkcorr/Forest2_TrkCorrv7
mkdir -p trkcorr/$corrset

for algo in icPu5; do
  #for pthat in 30 50 80 120 170 200 250 300; do
  for pthat in 80; do
    sampleWt=1;
    if [ $pthat -eq 30 ]; then inputFile="/mnt/hadoop/cms/store/user/frankmalocal/forest/Pythia${pthat}_HydjetDrum_mix01_HiForest2_v19.root"; fi
    if [ $pthat -eq 50 ]; then inputFile="/mnt/hadoop/cms/store/user/frankmalocal/forest/Pythia${pthat}_HydjetDrum_mix01_HiForest2_v19.root"; fi
    if [ $pthat -eq 80 ]; then inputFile="/net/hisrv0001/home/yenjie/scratch/hiForest/prod/production/CMSSW_4_4_2_patch6/test/forest21_simTrack/merged_pthat80_simTrack_v2.root"; fi
    if [ $pthat -eq 120 ]; then inputFile="/mnt/hadoop/cms/store/user/icali/Hydjet1.8/Z2/Dijet_merged/Dijet${pthat}_merged.root"; fi
    if [ $pthat -eq 170 ]; then inputFile="/mnt/hadoop/cms/store/user/icali/Hydjet1.8/Z2/Dijet_merged/Dijet${pthat}_merged.root"; fi
    #if [ $pthat -eq 170 ]; then inputFile="/mnt/hadoop/cms/store/user/frankmalocal/forest/Pythia${pthat}_HydjetDrum_mix01_HiForest2_v19.root"; fi
    if [ $pthat -eq 200 ]; then inputFile="/mnt/hadoop/cms/store/user/icali/Hydjet1.8/Z2/Dijet_merged/Dijet${pthat}_merged.root"; fi
    if [ $pthat -eq 250 ]; then inputFile="/mnt/hadoop/cms/store/user/icali/Hydjet1.8/Z2/Dijet_merged/Dijet${pthat}_merged.root"; fi
    if [ $pthat -eq 300 ]; then inputFile="/mnt/hadoop/cms/store/user/icali/Hydjet1.8/Z2/Dijet_merged/Dijet${pthat}_merged.root"; fi
    outputFile=trkcorr/$corrset"/"$corrset"_hy18dj${pthat}.root"
    ls $inputFile
    echo $outputFile
    root -b -q analyzeTrackingCorrection.C+'("'$algo'","'$inputFile'","'$outputFile'",'$pthat,$sampleWt',10000)'
  done
done
