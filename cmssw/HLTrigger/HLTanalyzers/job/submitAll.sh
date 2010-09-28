#!/bin/bash -
./create_custom_python_cfg.sh

scratch=/net/hidsk0001/d00/scratch/frankma
input=Hydjet_Quenched_MinBias_2760GeV_BSC_HF_L1Emul_v3_reco.txt
job=Hydj_BSC_HF_L1Emul
jobTag=oh0928_v2

# ===================== JEx =====================
inputList=../inputs/$input
output_dir=$scratch/ana/${job}/${jobTag}
./submitJob.sh $inputList $output_dir
