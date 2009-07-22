#!/bin/bash -

#
# - Env needs:
#   * CMSSW
#

dbCfg=/user/frankma/HI/cmssw311/devHIon/V3

if [ $# -eq 0 ]; then
   echo usage:
   echo "  $0 <file:raw.root>"
   echo "versions:"
   edmConfigFromDB --configName $dbCfg --versions
   exit 1
fi

if echo $dbCfg | grep -q 'V[[:alnum:]]$'; then
   name=`echo $dbCfg | awk -F/ '{print $(NF-1) $(NF)}'`
else
   name=${dbCfg##/*/}
fi

echo === $dbCfg: ===
edmConfigFromDB --configName $dbCfg --versions
echo output: $name

# run
edmConfigFromDB --configName $dbCfg --input $1 > ${name}_cfg.py
