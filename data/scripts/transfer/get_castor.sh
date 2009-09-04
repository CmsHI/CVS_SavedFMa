#!/bin/bash -
if [ $# -eq 0 ]; then
   echo "Usage"
   echo "  get_castor.sh <tag>"
   exit 1
fi

tag=$1
castordir=/castor/cern.ch/user/f/frankma/data/cmssw/simCMSSW_3_2_5_DigiToRawFix_EcalTag
files=`rfdir $castordir | grep $tag | awk '{print $NF}'`
for i in $files; do
   echo $i
   rfcp $castordir/$i raw/
done

