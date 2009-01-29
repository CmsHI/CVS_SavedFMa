#!/bin/bash
name="timeFF_2_1_11"
. ~/bin/cvsinit.sh
scramv1 project -n $name CMSSW CMSSW_2_1_11
echo project set
cd $name/src
cmsenv
addpkg EventFilter/AutoBU
addpkg EventFilter/ResourceBroker
cvs co UserCode/SavedFMa/cmssw/cmssw_2_1_X/EventFilter
cp UserCode/SavedFMa/cmssw/cmssw_2_1_X/EventFilter/ResourceBroker/src/FUResourceBroker.cc EventFilter/ResourceBroker/src/
scramv1 b

mkdir EventFilter/AutoBU/playback
mkdir EventFilter/AutoBU/playback/timeFF
cp UserCode/SavedFMa/cmssw/cmssw_2_1_X/EventFilter/AutoBU/playback/setupFF/initialize_onlineTiming EventFilter/AutoBU/playback/timeFF/ 
cd EventFilter/AutoBU/playback/timeFF/
