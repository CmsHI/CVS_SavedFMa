#!/bin/bash -
# - Env
#   * assumes crab env has been already set
if [ $# -eq 0 ]; then
   echo "Usage:"
   echo ". create_crab_cfg.sh <job_cfg.py> <totnevt>"
   return 1
fi

# input
dbspath=/Hydjet_MinBias_4TeV/Summer09-MC_31X_V3-GaussianVtx_312_ver1/GEN-SIM-RAW
pycfg=$1
# job specification
totnevt=$2
nperjob=500
# output
outfile=hydjetMinBias325DigiToRawFixEcalTag.root
outbase=$data/simCMSSW_3_2_5_DigiToRawFix_EcalTag_hyjetMinBiasJob
outdir=$outbase/raw

if [ ! -e $outbase ]; then mkdir $outbase; fi
if [ ! -e $outdir ]; then mkdir $outdir; fi

# edit python
jobpycfg=job_${pycfg##*/}
cp $pycfg $jobpycfg
echo '# now customization for crab' >> $jobpycfg
echo "process.output.fileName = '$outfile'" >> $jobpycfg
echo "print 'crab job output file:', process.output.fileName" >> $jobpycfg


cat $CRABPATH/full_crab.cfg | \
sed 's;^datasetpath=.*;datasetpath='$dbspath';' | \
sed 's;^pset=.*;pset='$jobpycfg';' | \
sed 's;^output_file =.*;output_file = '$outfile';' | \
sed 's;^#outputdir=.*;outputdir= '$outdir';' | \
sed 's;^total_number_of_events.*;total_number_of_events='$totnevt';' | \
sed 's;^#events_per_job.*;events_per_job = '$nperjob';' | \
sed 's;^number_of_jobs;#number_of_jobs;' \
> crab.cfg
