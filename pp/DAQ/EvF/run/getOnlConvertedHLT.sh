#!/bin/bash
if [ $# -eq 0 ]; then
   echo "usage:"
   echo "   $0 <cfgPath in ConfDB> <raw_outname> [ConfigDB code Path]"
   exit 1
fi

if [ $# -ge 2 ]; then
   cfgPath=$1
   rawOut=$2
fi

if [ $# -ge 3 ]; then
   confdbPath=$3
else
   confdbPath="."
fi

# Run onlineConverter
cd $confdbPath/test
./runOnlineConverter --configName $cfgPath > $rawOut
cd -
mv $confdbPath/test/$rawOut .

# Run stripOnlConvertedHLT2.sh
cvs co -r cmssw_2_1_11 UserCode/SavedFMa/pp/DAQ/EvF/sw/EventFilter/ConfigDB/test/
playbackCfg=$(echo "$cfgPath" | awk -F/ '{print $(NF-1)$(NF)}'| sed 's/cmsRun/playback/')
playbackCfg=$playbackCfg.cfg
echo playbackCfg: $playbackCfg
UserCode/SavedFMa/pp/DAQ/EvF/sw/EventFilter/ConfigDB/test/stripOnlConvertedHLT2.sh $rawOut $playbackCfg

