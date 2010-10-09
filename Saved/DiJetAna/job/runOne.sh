#!/bin/bash -
if [ $# -eq 0 ]; then
   echo Usage
   echo "   $0 <input_dir> <dcache:input_file.root> <output_dir> <maxEvents>"
   exit 1
fi

# input parameters
cfg=custom_dijetAna_cfg.py
inputDir=$1
inputFile=$2
outputDir=$3
maxEvents=$4
swenv=/net/hibat0003/d00/scratch/frankma/HI/jet/Y1JAna_CMSSW_3_8_4/env.sh

# check
if [ ! -e $cfg ]; then
   echo cfg: $cfg does not exist!
   exit 1
fi

# setup job parameters
inputFileName=${inputFile##*/}
echo $inputFileName
outputFileName=dj_$inputFileName
echo $outputFileName
outputFile=$outputDir/$outputFileName
echo $outputFile
logDir=$outputDir/log
echo $logDir
logFile=$logDir/${outputFileName%.root}.txt


# setup env
. /net/hisrv0001/home/frankma/UserCode/SavedFMa/bin/cgate/basic/osg_cmssw_set_basic.sh
. $swenv

mkdir -p $logDir
cmd="cmsRun $cfg maxEvents=$maxEvents files=$inputDir/$inputFile output=$outputFile >& $logFile"

# review
which cmsRun
echo $cmd

# run!
eval $cmd
