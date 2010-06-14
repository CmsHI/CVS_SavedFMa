#!/bin/bash -
if [ $# -eq 0 ]; then
   echo Usage
   echo "  $0 <dcache_path> <tag> <number_of_files>"
   exit 1
fi

path=$1
tag=$2
nfiles=$3

exefile=./getDcache.sh
echo "# commandline options: $@" > $exefile
for i in `ls $path/*.root | grep $tag | sort -n | head -n $nfiles`; do echo "dccp $i ." >> $exefile; done

chmod +x $exefile
