#!/bin/bash -

if [ $# -eq 0 ]; then
   echo Usage
   echo "  $0 <macro.C> <input.root>"
   exit 1
fi

macro=$1
cmd="root -b -q $macro"'+\(\"'$2'\"\)'
echo $cmd

eval $cmd
