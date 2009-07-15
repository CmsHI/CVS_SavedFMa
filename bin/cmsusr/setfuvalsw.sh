#!/bin/bash -
if [ -z $DAQHIsw ]; then
   echo "bad dir: $DAQHIsw"
   return 1
fi

cd $DAQHIsw
source EventFilter/Deployment/scripts/local-rpm-env.sh CMSSW
alias cmsenv='eval `scramv1 runtime -sh`'
cd -

