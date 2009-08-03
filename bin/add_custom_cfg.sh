#!/bin/bash -
mainCfg=$1
cusCfg=$2

mainCfgName=${mainCfg##*/}
mainCfgName=${mainCfgName%.py}
cusCfgName=${cusCfg##*/}
cusCfgName=${cusCfgName%.py}
out=${mainCfgName}_${cusCfgName}.py

cmd="cat $mainCfg $cusCfg > $out"
echo $cmd
eval $cmd
