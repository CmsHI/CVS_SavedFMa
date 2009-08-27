#!/bin/bash -
if [ $# -eq 0 ]; then
   echo "Usage: "
   echo "  runJobICone5_JetEt30_Mod <njobs>"
   echo "Eg:"
   echo "  runJobICone5_JetEt30_Mod 15"
   exit 1
fi

nJobs=2
if [ $1 ]; then nJobs=$1; fi

for i in $(seq 1 $nJobs)
do
   cmd="./condor64.sh runICone5_JetEt30_Mod.sh $i 126NoSelP_Job15"
   echo $cmd
   eval $cmd
done

