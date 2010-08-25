#!/bin/bash -
./create_custom_python_cfg.sh

scratch=/net/hidsk0001/d00/scratch/frankma
job=JEx_Hard_GSR_ZS
jobTag=mc0824_hlt

# ===================== JEx =====================
inputList=../inputs/$job.txt
output_dir=$scratch/ana/${job}/${jobTag}
./submitJob.sh $inputList $output_dir
