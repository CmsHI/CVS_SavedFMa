#!/bin/bash -
if [ $# -eq 0 ]; then
   echo "Usage"
   echo "  $0 <dev,pro,stable>"
   exit 1
fi

cp -r /opt/cmssw/$1/patch/src/* .
