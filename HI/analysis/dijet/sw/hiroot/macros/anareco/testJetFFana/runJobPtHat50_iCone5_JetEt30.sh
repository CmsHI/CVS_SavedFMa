#!/bin/bash -
nPerJob=10
nJobs=2
if [ $1 ]; then nPerJob=$1; fi
if [ $2 ]; then nJobs=$2; fi
for i in $(seq 1 $nJobs)
do
   echo ./condor64.sh runPtHat50_iCone5_JetEt30.sh $nPerJob $i
   ./condor64.sh runPtHat50_iCone5_JetEt30.sh $nPerJob $i
done

