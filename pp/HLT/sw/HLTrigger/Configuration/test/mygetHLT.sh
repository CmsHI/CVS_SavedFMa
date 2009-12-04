#!/bin/bash -
if [ $# -eq 0 ]; then
  echo Usage
  echo "  $0 <source:data|mc> <use:cff|full>"
  exit 1
fi

# Note
# - For cff mode
#   * many ES are stripped, including globaltag
#   * PrescaleService is stripped
source=$1
use=$2

getsc=$CMSSW_BASE/src/HLTrigger/Configuration/test/getHLT.py
menu=orcoff:/cdaq/special/CirculatingBeam09/v5.1/HLT/V2
processName=Startup09_$source
if [ $source == "data" ]; then
  globaltag=GR09_H_V6OFF::All
elif [ $source == "mc" ]; then
  globaltag=STARTUP3X_V8D::All
fi
cmd="$getsc --globaltag $globaltag --$use --$source $menu $processName"
echo $cmd
eval $cmd

# Additional modifications
if [ $source == "data" -a $use == "cff" ]; then
  cat custom_data_cff.py >> HLT_${processName}_cff.py
fi
if [ $source == "mc" ]; then
  echo "Finally take away BPTX filter for the mc cff"
fi
