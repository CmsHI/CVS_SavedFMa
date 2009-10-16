#!/bin/bash -
if [ $# -eq 0 ]; then
   echo Usage
   echo "  $0 <dcache_path> <number_of_files>"
   exit 1
fi

path=$1
n=$2
exefile=./getData.sh

echo "# commandline options: $@" > $exefile
for i in `ls $1/*.root | sort -n | head -n $2`; do echo "dccp $i ." >> $exefile; done

chmod +x $exefile
