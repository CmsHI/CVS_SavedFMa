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

cd $confdbPath/test
./runOnlineConverter --configName $cfgPath > $rawOut
cd -
mv $confdbPath/test/$rawOut .

