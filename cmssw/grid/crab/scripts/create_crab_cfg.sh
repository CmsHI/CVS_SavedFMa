#!/bin/bash -
#
# - Env
#   * assumes crab env has been already set
# - For output to castor
#   * cf https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideCrabHowTo#4_Stage_out_in_your_own_director
#   * cf https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideCrabHowTo#5_LSF_Stage_out
#

if [ $# -eq 0 ]; then
   echo "Usage:"
   echo ". create_crab_cfg.sh <job_cfg.py> <totnevt>"
   return 1
fi

# ===== Job Setup =====
# input
dbspath=/Hydjet_B0_4TeV/Summer09-MC_31X_V3-GaussianVtx_312_ver1/GEN-SIM-RAW
pycfg=$1
# job specification
totnevt=$2
nperjob=250
# output
outfile=hydjetB0_325DigiToRawFixEcalTag.root
release=simCMSSW_3_2_5_DigiToRawFix_EcalTag
# - return local -
#outbase=$data/simCMSSW_3_2_5_DigiToRawFix_EcalTag_hyjetB0Job
outdir=
# - storage element -
sedir=/user/f/frankma/data/cmssw/$release

# ===== Execute =====

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
sed 's;^total_number_of_events.*;total_number_of_events='$totnevt';' | \
sed 's;^#events_per_job.*;events_per_job = '$nperjob';' | \
sed 's;^number_of_jobs;#number_of_jobs;' \
> tmpcrab.cfg

# Finally, output
if [ $outdir ]; then
   echo "Will output to local dir: $outdir"
   if [ ! -e $outbase ]; then mkdir $outbase; fi
   if [ ! -e $outdir ]; then mkdir $outdir; fi
   sed 's;^#outputdir=.*;outputdir= '$outdir';' tmpcrab.cfg > crab.cfg
else
   sed 's/^return_data = 1/return_data = 0/' tmpcrab.cfg | \
   sed 's/^copy_data = 0/copy_data = 1/' | \
   sed 's/^#storage_element=srm-cms.cern.ch/storage_element=srm-cms.cern.ch/' | \
   # Since we are working at cern, we should use the LSF mode
   sed 's;^#storage_path=/srm/managerv2?SFN=/castor/cern.ch/;/castor/cern.ch/;;' \
   > crab.cfg
   echo "user_remote_dir=$sedir" >> crab.cfg
   # check castor dir
   castordir=/castor/cern.ch$sedir
   rfdir $castordir
   if [ ! $? -eq 0 ]; then rfmkdir $castordir; fi
   rfchmod 775 $castordir
   rfdir $castordir
fi

# clean up
rm -f tmpcrab.cfg

