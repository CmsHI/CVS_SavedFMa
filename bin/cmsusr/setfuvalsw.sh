#!/bin/bash -
# make checks
hostname | grep '^fuval'
if [ ! $? -eq 0 ]; then
   echo "first go to a fuval machine!"
   return 1
fi
if [ -z $DAQHIsw ]; then
   echo "bad dir: $DAQHIsw"
   return 1
fi

cd $DAQHIsw
source EventFilter/Deployment/scripts/local-rpm-env.sh CMSSW
alias cmsenv='eval `scramv1 runtime -sh`'
export CMSSW_DATA_PATH=/opt/cmssw/slc4onl_ia32_gcc346/cms
cd -

