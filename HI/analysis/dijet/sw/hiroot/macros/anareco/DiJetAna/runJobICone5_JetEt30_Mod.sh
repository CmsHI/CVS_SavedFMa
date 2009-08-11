#!/bin/bash -
nJobs=2
if [ $1 ]; then nJobs=$1; fi
for i in $(seq 1 $nJobs)
do
   echo ./condor64.sh runICone5_JetEt30_Mod.sh $i 123SelPJob
   ./condor64.sh runICone5_JetEt30_Mod.sh $i 123SelPJob
done

