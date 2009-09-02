#!/bin/bash -
# - Env
#   * assumes crab env has been already set
if [ $# -eq 0 ]; then
   echo "Usage:"
   echo ". create_crab_cfg.sh <job_cfg.py>"
   return 1
fi

jobcfg=$1
totnevt=5000
nperjob=500

cat $CRABPATH/full_crab.cfg | \
sed 's;^datasetpath=.*;datasetpath=/Hydjet_MinBias_4TeV/Summer09-MC_31X_V3-GaussianVtx_312_ver1/GEN-SIM-RAW;' | \
sed 's;^pset=.*;pset='$jobcfg';' | \
sed 's;^output_file =.*;output_file = hydjet325DigiToRawFixEcalTag.root;' | \
sed 's;^#outputdir=.*;outputdir= /d01/frankma/scratch/data/simCMSSW_3_2_5_DigiToRawFix_EcalTag/raw;' | \
sed 's;^total_number_of_events.*;total_number_of_events='$totnevt';' | \
sed 's;^#events_per_job.*;events_per_job = '$number_of_jobs';' | \
sed 's;^number_of_jobs;#number_of_jobs;' \
> crab.cfg
