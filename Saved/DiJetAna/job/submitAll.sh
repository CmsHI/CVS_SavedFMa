#!/bin/bash -
./create_custom_python_cfg.sh

scratch=/net/hidsk0001/d00/scratch/frankma
job=s10-dj80to120
jobTag=dj1009_Y1JAna011

# ===================== JEx =====================
inputList=../../inputs/$job.txt
output_dir=$scratch/ana/${job}/${jobTag}
./submitJob.sh $inputList $output_dir
