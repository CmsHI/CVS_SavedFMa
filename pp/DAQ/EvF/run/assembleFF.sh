#!/bin/bash -
if [ $# -eq 0 ]; then
   echo "usage:"
   echo "   setupEvF_21X.sh <project_name> [run_script_path]"
   # $0 is bash - here!
   echo "defaults:"
   echo "   project_name = timeFF"
   echo "   run_script_path  = ."
   #exit 1
   return 1
   # since we are sourcing this file, exit will cause the shell to exit!
fi

# Set parameters
if [ $# -ge 1 ]; then
   projectName=$1
else
   projectName="timeFF"
fi
cmsswName="$projectName"'_2_1_11'

if [ $# -ge 2 ]; then
   scriptPath=$2
else
   scriptPath=$(pwd)
fi
startPath=$(pwd)
log=$projectName'_setup.log'
echo scripts used:
ls $scriptPath

# co EventFilter, modification code and compile
. $scriptPath/setupEvF_21X.sh $projectName

# co and compile ConfigDB (java) code
evfSwPath="$HOME/work/pp/DAQ/EvF/sw"
$scriptPath/setupOnlConverter.sh $evfSwPath

# get converted hlt
confdbCfgPath="/user/frankma/timing/cmssw2111/v0.0/p5_cmsRun_online_timingHLT/V6"
rawConverted="onlineConverted.cfg"
$scriptPath/getOnlConvertedHLT.sh $confdbCfgPath $rawConverted "$evfSwPath/EventFilter/ConfigDB"
mv $rawConverted $cmsswName/src/EventFilter/AutoBU/playback/$projectName
mv $(ls -tr | tail -n 1) $cmsswName/src/EventFilter/AutoBU/playback/$projectName

