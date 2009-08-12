#!/bin/bash -
if [ $# -eq 0 ]; then
   echo "Usage:"
   echo "  add_custom_cfg.sh <main_cfg.py> <cus_cfg.py>"
   exit 1
fi

# set var's
mainCfg=$1
cusCfg=$2

# save copy
cp $mainCfg tmp.py

mainCfgName=${mainCfg##*/}
mainCfgName=${mainCfgName%.py}
cusCfgName=${cusCfg##*/}
cusCfgName=${cusCfgName%.py}
#out=${mainCfgName}_${cusCfgName}.py
pref="custom"
echo $mainCfgName | grep $pref
if [ $? -ne 0 ]; then
   out=${pref}_${mainCfgName}.py
   cmd="cat $mainCfg $cusCfg >> $out"
else
   out=$mainCfg
   cmd="cat tmp.py $cusCfg >> $out"
fi

echo $cmd

# run
echo "#cmdline: $cmd" > $out
eval $cmd

# clean up
rm -f tmp.py

# add output of cfg summary
#if [ $SavedFMa/cmssw/MyConfiguration/Sequences/python/output_cfg_summary.py ]; then
#   cat $SavedFMa/cmssw/MyConfiguration/Sequences/python/output_cfg_summary.py >> $out
#fi
