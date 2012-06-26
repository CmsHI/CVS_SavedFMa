#!/bin/bash -
# for xsec see, https://twiki.cern.ch/twiki/bin/view/CMS/HiForest#X_HYDJET_1_8_Drum_For_analysis_a

#
# mit
#
#for algo in akPu3PF; do
for algo in icPu5; do
  cBinRefFile="../ntout/output-data-DiJet-v7-noDuplicate_v2.root"
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
    echo sampleWt: $sampleWt
    #inputFile="/net/hisrv0001/home/yetkin/hidsk0001/prod/DrumMix/Z2/pythia80_mix01/forest19/Pythia${pthat}_HydjetDrum_mix01_HiForest2_v19_0.root"
    #inputFile="/net/hisrv0001/home/yetkin/hidsk0001/prod/DrumMix/Z2/pythia80_mix01/forest19/Pythia80_HydjetDrum_mix01_HiForest2_v19.root"
    #outputFile="output-hy18dj${pthat}_Forest2v19_v1_allTrks_xsec.root"
    inputFile="/net/hisrv0001/home/yenjie/scratch/hiForest/prod/production/CMSSW_4_4_2_patch6/test/forest21_simTrack/merged_pthat80_simTrack_v2.root"
    #outputFile="output-hy18dj${pthat}_Forest2v21_v1_allTrks_genp_xsec.root"
    outputFile="output-hy18dj${pthat}_Forest2v21_v3_allTrks_simtrk_jpt100_xsec.root"
    rewtFile="../ntout/output-hy18djallpthats_iterTrkv1_v9hgtOnly_hgtcorr_xsec.root"
    ls $inputFile
    echo $outputFile, $rewtFile
    # mc embedded
    #root -b -q analyzeDiJetMPT.C+'("'$algo'","'$inputFile'","'$rewtFile'",0,'$pthat,$sampleWt',0,"","",0)'
    root -b -q analyzeDiJetMPT.C+'("'$algo'","'$inputFile'","'$outputFile'",0,'$pthat,$sampleWt',1,"'$rewtFile'","'$cBinRefFile'",0)'
    #root -b -q analyzeDiJetMPT.C+'("'$algo'","'$inputFile'","'$outputFile'",0,'$pthat,$sampleWt',0,"","",0)'
  done
done
