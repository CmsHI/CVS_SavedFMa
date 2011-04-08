#!/bin/bash -
if [ $# -eq 0 ]; then
   echo Usage
   echo "   $0 <cfg.py> <out_dir> <outputfile> <maxEvents> <input_file1,input_file2,...>"
   exit 1
fi

# input parameters
cfg=$1
outputDir=$2
outputFile=$3
maxEvents=$4
inputFiles=$5
swenv=swenv=/net/hibat0003/d00/scratch/frankma/HI/jet/QM11_399p1_noMu/env.sh

# check
if [ ! -e $cfg ]; then
   echo cfg: $cfg does not exist!
   exit 1
fi

# setup job parameters
echo $outputFile
outputFileName=${outputFile##*/}
echo $outputFileName
logDir=$outputDir/log
logFile=$logDir/${outputFileName%.root}.txt
echo $logFile


# setup env
. /net/hisrv0001/home/frankma/UserCode/SavedFMa/bin/cgate/basic/osg_cmssw_set_basic.sh
. $swenv

mkdir -p $logDir
cmd="cmsRun $cfg maxEvents=$maxEvents files=$inputFiles output=$outputFile >& $logFile"

# review
which cmsRun
echo $cmd

# run!
eval $cmd
