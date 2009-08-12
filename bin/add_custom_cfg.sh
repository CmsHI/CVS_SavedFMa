#!/bin/bash -
if [ $# -eq 0 ]; then
   echo "Usage:"
   echo "  add_custom_cfg.sh <main_cfg.py> <cus_cfg.py>"
   exit 1
fi

mainCfg=$1
cusCfg=$2

mainCfgName=${mainCfg##*/}
mainCfgName=${mainCfgName%.py}
cusCfgName=${cusCfg##*/}
cusCfgName=${cusCfgName%.py}
out=${mainCfgName}_${cusCfgName}.py

cmd="cat $mainCfg $cusCfg >> $out"
echo $cmd

# run
echo "#cmdline: $cmd" > $out
eval $cmd
