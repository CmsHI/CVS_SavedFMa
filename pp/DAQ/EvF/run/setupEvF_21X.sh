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

# setup scramv1 and cvs
. ~/bin/cvsinit.sh

# Create project area
scramv1 project -n $cmsswName CMSSW CMSSW_2_1_11 > $log
echo project area: $cmsswName created.

# co and compile CMSSW code
cd $cmsswName/src
cmsenv
addpkg EventFilter/AutoBU
addpkg EventFilter/ResourceBroker
cvs co UserCode/SavedFMa/cmssw/cmssw_2_1_X/EventFilter
cp UserCode/SavedFMa/cmssw/cmssw_2_1_X/EventFilter/ResourceBroker/src/FUResourceBroker.cc EventFilter/ResourceBroker/src/
scramv1 b

# co and compile ConfigDB (java) code
evfSwPath="$HOME/work/pp/DAQ/EvF/sw"
$scriptPath/setupOnlConverter.sh $evfSwPath

# co private codes
cvs co "UserCode/SavedFMa/pp/DAQ/EvF/sw/EventFilter/ConfigDB/test"

confdbCfgPath="/user/frankma/timing/cmssw2111/v0.0/p5_cmsRun_online_timingHLT/V6"
rawConverted="onlineConverted.cfg"
# get converted hlt
$scriptPath/getOnlConvertedHLT.sh $confdbCfgPath $rawConverted "$evfSwPath/EventFilter/ConfigDB"

# run stripOnlConvertedHLT2.sh
playbackCfg="p5_playback_online_timingHLTV6.cfg"
UserCode/SavedFMa/pp/DAQ/EvF/sw/EventFilter/ConfigDB/test/stripOnlConvertedHLT2.sh $rawConverted $playbackCfg

mkdir "EventFilter/AutoBU/playback"
mkdir "EventFilter/AutoBU/playback/$projectName"
cp UserCode/SavedFMa/cmssw/cmssw_2_1_X/EventFilter/AutoBU/playback/setupFF/initialize_onlineTiming EventFilter/AutoBU/playback/$projectName 
mv $rawConverted EventFilter/AutoBU/playback/$projectName
mv $playbackCfg EventFilter/AutoBU/playback/$projectName
cd EventFilter/AutoBU/playback/$projectName

