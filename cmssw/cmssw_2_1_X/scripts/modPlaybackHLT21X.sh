#!/bin/bash -
if [ $# -eq 0 ]; then
    echo $0
    echo "   A script that carries out the modifications for "
    echo "   21X menus to cmsRun in 2_1_X "
    echo usage:
    echo "   $0 <inputHLTName> <outputHLTName> [maxEvents]"
else
    echo "# $0 $*" > $2
    if [ $# -eq 3 ]; then
        sed -e 's/untracked int32 input = -1/untracked int32 input = '$3'/' $1 >> $2
    fi
fi

