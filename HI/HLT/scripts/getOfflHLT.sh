#!/bin/bash -

#
# - Env needs:
#   * CMSSW
#

if [ $# -eq 0 ]; then
   echo usage:
   echo "  $0 <file:raw.root>"
   exit 1
fi
dbCfg=/user/frankma/HI/cmssw311/devHIon
name=${dbCfg##/*/}
echo === $dbCfg: ===
edmConfigFromDB --configName $dbCfg --versions
edmConfigFromDB --configName $dbCfg --input $1 > ${name}_cfg.py
