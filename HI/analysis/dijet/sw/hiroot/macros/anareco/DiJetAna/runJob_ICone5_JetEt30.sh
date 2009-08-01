#!/bin/bash -
nJobs=6
if [ $1 ]; then nJobs=$1; fi
for i in $(seq 2 $nJobs)
do
   #echo ./condor64.sh runPtHat50_iCone5_JetEt30.sh $nPerJob $i
   #./condor64.sh runPtHat50_iCone5_JetEt30.sh $nPerJob $i
   ./runICone5_JetEt30_Mod_r115.sh $i
done

