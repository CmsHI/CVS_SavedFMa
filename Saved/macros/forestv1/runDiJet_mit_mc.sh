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
    #inputFile="/mnt/hadoop/cms/store/user/mnguyen/MC_Production/Pythia${pthat}_HydjetDrum_mix01/merged_HydjetDrum.root"
    inputFile="/net/hisrv0001/home/icali/hadoop/Pythia/Z2/ppDijet_merged/pp276Dijet${pthat}_merged.root"
    outputFile="output-dj${pthat}_v12hgtOnly_hgtcorr_xsec.root"
    #inputFile="/net/hisrv0001/home/frankma/hadoopfranklocal/forest/Hydjet18/DiJet80_IterTrkPixTrk_v1_allGenp.root"
    #outputFile="output-hy18dj${pthat}_IterPixTrkv1_v9hgtOnly_hgtcorr_xsec.root"
    #inputFile="/net/hidsk0001/d00/scratch/frankma/tmp/DiJet80_MergeTrkAllGenpAllCh_v1.root"
    #outputFile="output-hy18dj${pthat}_IterPixTrkAllGenpAllChv1_v9_hgt_xsec.root"
    #inputFile="~/hadoopfranklocal/forest/Hydjet18/DiJet80_MergeTrk_v1_SimTrack_part2.root"
    #outputFile="output-hy18dj${pthat}_IterPixTrkv1_SimTrack_hgt_xsec.root"
    #inputFile="~/hadoopfranklocal/forest/Hydjet18/DiJet${pthat}_iterTrk_v1.root"
    #outputFile="output-hy18dj${pthat}_iterTrkv1_v11hgtOnly_hgtcorr_saveGenpSig_xsec.root"
    #inputFile="/net/hisrv0001/home/icali/hadoop/Pythia/Z2/ppDijet_merged/pp276Dijet50_merged.root"
    #outputFile="output-dj${pthat}_highptv0_xsec.root"
    rewtFile="../ntout/output-hy18djallpthats_iterTrkv1_v9hgtOnly_hgtcorr_xsec.root"
    ls $inputFile
    echo $outputFile, $rewtFile
    # mc embedded
    #root -b -q analyzeDiJetMPT.C+'("'$algo'","'$inputFile'","'$rewtFile'",0,'$pthat,$sampleWt',0,"","",0)'
    #root -b -q analyzeDiJetMPT.C+'("'$algo'","'$inputFile'","'$outputFile'",0,'$pthat,$sampleWt',1,"'$rewtFile'","'$cBinRefFile'",0)'
    root -b -q analyzeDiJetMPT.C+'("'$algo'","'$inputFile'","'$outputFile'",0,'$pthat,$sampleWt',0,"","",0)'
  done
done
