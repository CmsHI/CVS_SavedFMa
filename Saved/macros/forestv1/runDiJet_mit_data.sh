#!/bin/bash -

#
# mit
#
# data
# for mpt
#root -b -q analyzeDiJetMPT.C+'("icPu5","/mnt/hadoop/cms/store/user/velicanu/forest/HiForestDijet_v7.root","output-data-DiJet-v7_v1.root",1)'
#root -b -q analyzeDiJetMPT.C+'("icPu5","/mnt/hadoop/cms/store/user/frankmalocal/forest/Hi2011ForestDiJet_v7_noDuplicate.root","output-data-DiJet-v7-noDuplicate_v3.root",1)'
root -b -q analyzeDiJetMPT.C+'("icPu5","/net/hisrv0001/home/frankma/hadoopfranklocal/forest/HiForest_promptskim-hihighpt-jet115-v1_part.root","output-data-DiJetTrk_v1_v11hgtOnly_hgtcorr_mptOnly.root",1)'
