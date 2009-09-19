#!/bin/bash -
#
# - Env
#   * assumes crab env has been already set
# - For output to castor
#   * cf https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideCrabHowTo#4_Stage_out_in_your_own_director
#
# - Warning
#   * be careful with command line parsing!
#     + don't use command line parsing for cmssw when using crab, b/c both are using python
#       and cmssw will get the command line of crab :(
#

if [ $# -eq 0 -o $# -lt 4 ]; then
   echo "Usage:"
   echo ". ./create_crab_cfg.sh <job_cfg.py> <totnevt> <output_storage> <storage_path_base/subdir>"
   echo "Output_storage choices:"
   echo "  castor,mit,local"
   echo "Eg:"
   echo '  . ./create_crab_cfg.sh custom_genjets_cfg.py 10 local $data/semgenjetCMSSW_3_2_6/tree'
   echo '  . ./create_crab_cfg.sh custom_DIGI_RAW_MC_31X_V3_driver.py castor simCMSSW_3_2_5_DigiToRawFix_EcalTag/raw'
   echo "Warning:"
   echo " if output to local, size limit: 50MB"
   return 1
fi

# ===== Job Setup =====
# input
dbspath=/Hydjet_MinBias_4TeV/Summer09-MC_31X_V3-GaussianVtx_312_ver1/GEN-SIM-RAW
# job specification
nperjob=100
# output
outfile=output.root

# ===== Execute =====
# automated setup from command line
pycfg=$1
totnevt=$2
storage=$3
sbasepath=${4%/*}
spath=$4

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
case $storage in
   local)
   echo "Will output to local dir: $spath"
   if [ ! -e $sbasepath ]; then mkdir $sbasepath; fi
   if [ ! -e $spath ]; then mkdir $spath; fi
   sed 's;^#outputdir=.*;outputdir= '$spath';' tmpcrab.cfg > crab.cfg
   ;;

   castor)
   suserpath=/user/f/frankma/data/cmssw/$spath
   sed 's/^return_data = 1/return_data = 0/' tmpcrab.cfg | \
   sed 's/^copy_data = 0/copy_data = 1/' | \
   sed 's/^#storage_element=srm-cms.cern.ch/storage_element=srm-cms.cern.ch/' | \
   # The order of parameter is important!
   sed 's;^#storage_path=/srm/managerv2?SFN=/castor/cern.ch/;storage_path=/srm/managerv2?SFN=/castor/cern.ch/\nuser_remote_dir='$suserpath';' \
   > crab.cfg
   # check castor dir
   for i in $sbasepath $spath; do
      cdir=/castor/cern.ch/user/f/frankma/data/cmssw/$i
      rfdir $cdir | tail
      if [ ! $? -eq 0 ]; then rfmkdir $cdir; fi
      rfchmod 775 $cdir
   done
   ;;
esac

# clean up
rm -f tmpcrab.cfg

