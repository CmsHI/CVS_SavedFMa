#!/bin/bash -
if [ $# -lt 1 ]; then
  echo Usage:
  echo " run_jrahi.sh <input_jra_tree.root>"
  exit 1
fi

# set var's
cmsswbase=/home/frankma/work/HI/jet/sw/JES/ppjesCMSSW_3_3_1
config=config/hifine_binning.config
input=/d01/frankma/scratch/data/JES/cmssw331/pyquen_unquendijet_nocoll_pt80to120_d20091025/all_11.root
jrahistos=jra_hi.root
if [ $# -ge 1 ]; then
  input=$1
fi

#run
# 1. get histo's
jet_response_analyzer_x $cmsswbase/src/JetMETAnalysis/JetAnalyzers/$config -input $input \
-algs \
ic5caloNoPuSignalUncorrected ic5caloNoPuSignalCorrected \
ic5caloPuSignalUncorrected ic5caloPuSignalCorrected \
ic5caloMixedUncorrected ic5caloMixedCorrected \
-output $jrahistos

# 2. get responses
jet_response_and_resolution_x -input $jrahistos \
-variables \
RelRsp:RefPt \
RelRsp:JetEta \
RelRsp:JetPhi \
"RelRsp:JetEta#1:RefPt" 


