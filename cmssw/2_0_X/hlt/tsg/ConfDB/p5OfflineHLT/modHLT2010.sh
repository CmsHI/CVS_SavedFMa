#!/bin/bash -
if [ $# -eq 0 ]; then
   echo $0
   echo "   A script that carries out the modifications for p5 offline"
   echo "   20X menus (2_0_10+) to cmsRun in P5 2_0_10 + online tags"
   echo usage:
   echo "   $0 <inputHLTName> <outputHLTName> [maxEvents]"
elif [ $# -eq 2 ]; then
   sed 's/string fedColl = "rawDataCollector"/InputTag inputLabel = rawDataCollector/' $1 > $2
elif [ $# -eq 3 ]; then
   sed -e 's/string fedColl = "rawDataCollector"/InputTag inputLabel = rawDataCollector/' -e 's/untracked int32 input = -1/untracked int32 input = '$3'/' $1 > $2
fi

