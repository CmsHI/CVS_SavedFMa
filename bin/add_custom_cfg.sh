#!/bin/bash -
mainCfg=$1
cusCfg=$2

mainCfgName=${mainCfg%.py}
cusCfgName=${cusCfg%.py}
out=${mainCfgName}_${cusCfgName}.py

cmd="cat $mainCfg $cusCfg >& $out"
echo $cmd
eval $cmd
