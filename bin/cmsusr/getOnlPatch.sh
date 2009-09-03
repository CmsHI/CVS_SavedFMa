#!/bin/bash -
if [ $# -eq 0 ]; then
   echo "Usage"
   echo "  $0 <dev,pro,stable>"
   exit 1
fi

onlsw=$1

cp -r /opt/cmssw/$onlsw/patch/src/* .

# log
echo $onlsw snapshot:
/nfshome0/frankma/UserCode/SavedFMa/bin/cmsusr/checkOnlineCmssw.sh $onlsw > setup.log
