#!/bin/bash -
# for xsec see, https://twiki.cern.ch/twiki/bin/view/CMS/HiForest#X_HYDJET_1_8_Drum_For_analysis_a

#
# mit
#
for algo in icPu5; do
  #for pthat in 50 80 120 170 200; do
  #for pthat in 50 120 170 200; do
  #for pthat in 50 120 170; do
  for pthat in 80; do
    sampleWt=1;
    if [ $pthat -eq 50 ]; then sampleWt=102.1; fi
    if [ $pthat -eq 80 ]; then sampleWt=9.913; fi
    if [ $pthat -eq 120 ]; then sampleWt=1.128; fi
    if [ $pthat -eq 170 ]; then sampleWt=0.147; fi
    if [ $pthat -eq 200 ]; then sampleWt=0.0531; fi
    #inputFile="/net/hisrv0001/home/yetkin/hidsk0001/prod/DrumMix/Z2/pythia80_mix01/forest19/Pythia${pthat}_HydjetDrum_mix01_HiForest2_v19_0.root"
    #inputFile="/net/hisrv0001/home/yetkin/hidsk0001/prod/DrumMix/Z2/pythia80_mix01/forest19/Pythia80_HydjetDrum_mix01_HiForest2_v19.root"
    #outputFile="output-hy18dj${pthat}_Forest2v19_v1_allTrks_xsec.root"
    #inputFile="/mnt/hadoop/cms/store/user/icali/Hydjet1.8/Z2/Dijet_merged/Dijet${pthat}_merged.root"
    #inputFile="/mnt/hadoop/cms/store/user/frankmalocal/forest/Pythia${pthat}_HydjetDrum_mix01_HiForest2_v19.root"
    inputFile="/net/hisrv0001/home/yenjie/scratch/hiForest/prod/production/CMSSW_4_4_2_patch6/test/forest21_simTrack/merged_pthat80_simTrack_v2.root"
    #outputFile="output-hy18dj${pthat}_Forest2v21_v1_allTrks_genp_xsec.root"
    #inputFile="/net/hisrv0001/home/yetkin/hidsk0001/prod/DrumMix/Z2/validate/merge/test.root"
    outputFile="trkcorr/test/Forest2TrkCorrv7_hy18dj${pthat}.root"
    ls $inputFile
    echo $outputFile
    root -b -q analyzeTrackingCorrection.C+'("'$algo'","'$inputFile'","'$outputFile'",'$pthat,$sampleWt',10000)'
  done
done
