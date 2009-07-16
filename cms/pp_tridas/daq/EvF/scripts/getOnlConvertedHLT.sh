#!/bin/bash
if [ $# -eq 0 ]; then
   echo "usage:"
   echo "   $0 <cfgPath in ConfDB> [ConfigDB code Path]"
   echo
   echo "   use in the dir you want the playback cfg"
   exit 1
fi

# setup env
# This is where you checked out UserCode/SavedFMa/pp/DAQ/EvF
EvF=$SavedFMa/cms/pp_tridas/daq/EvF

if [ $# -ge 1 ]; then
   cfgPath=$1
   rawOut=playback_`echo $cfgPath | awk -F/ '{print $(NF-1) $(NF)}'`.rawOut
   playbackCfg=${rawOut%.*}_cfg.py
   echo playbackCfg: $playbackCfg
fi

if [ $# -ge 2 ]; then
   confdbPath=$2
else
   confdbPath=$DAQHIsw/EventFilter/ConfigDB
fi

# Run onlineConverter
# - Note
#   * don't forget to define streams!
$EvF/scripts/runOnlConverter.sh $1

# Run stripOnlConvertedHLT2.sh
$EvF/scripts/stripOnlConvertedHLT2.sh $rawOut $playbackCfg

# clean up
if [ ! -e backup ]; then
   mkdir backup
fi
mv $rawout old

