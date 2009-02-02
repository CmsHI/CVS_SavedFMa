#!/bin/bash -
if [ $# -eq 0 ]; then
   echo "usage:"
   echo "   setupEvF_21X.sh <project_name>"
   # $0 is bash - here!
   echo "defaults:"
   echo "   project_name = timeFF"
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

startPath=$(pwd)
#log=$projectName'_setup.log'

# Create project area
scramv1 project -n $cmsswName CMSSW CMSSW_2_1_11
echo project area: $cmsswName created.

# co and compile CMSSW code
cd $cmsswName/src
cmsenv
echo addpkg EventFilter/AutoBU
addpkg EventFilter/AutoBU
echo addpkg EventFilter/ResourceBroker
addpkg EventFilter/ResourceBroker
echo check out user code
cvs co -r cmssw_2_1_11 UserCode/SavedFMa/cmssw/cmssw_2_1_X/EventFilter
cp UserCode/SavedFMa/cmssw/cmssw_2_1_X/EventFilter/ResourceBroker/src/FUResourceBroker.cc EventFilter/ResourceBroker/src/
scramv1 b
echo project area built

mkdir "EventFilter/AutoBU/playback"
mkdir "EventFilter/AutoBU/playback/$projectName"

echo Get scripts
cp UserCode/SavedFMa/cmssw/cmssw_2_1_X/EventFilter/AutoBU/playback/setupFF/initialize_onlineTiming EventFilter/AutoBU/playback/$projectName 

#mv $log $cmsswName/logs/
echo EvF setup done.
echo   CMSSW project area created: $cmsswName
echo   initialize script at: EventFilter/AutoBU/playback/$projectName
#echo   setup log at $cmsswName/logs/

cd $startPath

