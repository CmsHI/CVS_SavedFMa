#!/bin/bash -
if [ $# -lt 1 ]; then
  echo Usage:
  echo " run_jra.sh <input_jra_tree.root>"
  exit 1
fi

# set var's
cmsswbase=/home/frankma/work/HI/jet/sw/JES/ppjesCMSSW_3_3_1
config=config/hifine_binning.config
jrahistos=jra_pp.root
if [ $# -ge 1 ]; then
  input=$1
fi

#run
# 1. get histo's
jet_response_analyzer_x $cmsswbase/src/JetMETAnalysis/JetAnalyzers/$config -input $input \
-algs \
ic5calo ic5calol2l3 \
-output $jrahistos

# 2. get responses
jet_response_and_resolution_x -input $jrahistos \
-variables \
RelRsp:RefPt \
RelRsp:JetEta \
RelRsp:JetPhi \
"RelRsp:JetEta#1:RefPt" 


