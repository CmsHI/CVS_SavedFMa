#!/bin/bash
if [ $# -eq 0 ]; then
   echo "usage:"
   echo "   $0 <cfgPath in ConfDB> [ConfigDB code Path]"
   exit 1
fi

if [ $# -ge 1 ]; then
   cfgPath=$1
   rawOut=playback_`echo $cfgPath | awk -F/ '{print $(NF-1) $(NF)}'`.rawOut
fi

if [ $# -ge 2 ]; then
   confdbPath=$2
else
   confdbPath=$DAQHIsw/EventFilter/ConfigDB
fi

# Run ConfigDB/test/runOnlineConverter
#  * a perl script
#  * env needs
#    + cmssw-evf-confdb-converter.jar defined
#  * parameter needs
#    + the proper arguments for the java converter to access the database
#  
#  Note the generated cmd in runOnlineConverter near the end:
#    " -cp ../ext/ojdbc14.jar:../lib/cmssw-evf-confdb-converter.jar"
#    * So the script is mean to be ran in the test dir!
#      if you run in any other directory, it won't find
#      cmssw-evf-confdb-converter.jar
#
cd $confdbPath/test

# Note option: --hltdev
#   starting from ~ 22X, Philipp adds the option to choose your
#   Favorite database!
./runOnlineConverter --configName $cfgPath --hltdev > $rawOut
cd -
mv $confdbPath/test/$rawOut .

