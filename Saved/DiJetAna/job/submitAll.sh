#!/bin/bash -
./create_custom_python_cfg.sh

scratch=/net/hidsk0001/d00/scratch/frankma
job=HCPR-J50U-151020to151076-PAT
jobTag="1116_trigana"

# ===================== JEx =====================
inputList=~/inputs_data/$job.txt
output_dir=$scratch/ana/${job}/${jobTag}
./submitJob.sh $inputList $output_dir
