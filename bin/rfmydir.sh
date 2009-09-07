#!/bin/bash -
datadir=/castor/cern.ch/user/f/frankma/data

if [ $# -eq 0 ]; then
   echo Usage
   echo "  rfmydir [path after usr dir at castor]"
   echo Current user dir at castor:
   echo "  $datadir"
   exit 1
fi

localpath=$1

fullpath=$datadir/$localpath
cmd="rfdir $fullpath"
echo $cmd

# === run ===
eval $cmd
