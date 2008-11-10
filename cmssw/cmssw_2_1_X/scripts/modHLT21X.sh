#!/bin/bash -
if [ $# -eq 0 ]; then
   echo $0
   echo "   A script that carries out the modifications for "
   echo "   21X menus to cmsRun in 2_1_X "
   echo usage:
   echo "   $0 <inputHLTName> <outputHLTName> [maxEvents]"
elif [ $# -eq 3 ]; then
   sed -e 's/process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32( -1 ) )/process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32( '"$3"' ) )/' $1 > $2
fi

