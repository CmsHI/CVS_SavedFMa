#!/bin/bash -
if [ $# -eq 0 ]; then
   echo "Usage:"
   echo "  $0 <n_per_job> <njobs>"
   echo "Eg:"
   echo "  $0 10000 30"
   exit 1
fi

nPerJob=10
nJobs=2
if [ $1 ]; then nPerJob=$1; fi
if [ $2 ]; then nJobs=$2; fi
for i in $(seq 1 $nJobs)
do
   cmd="./condor64.sh genPtHat50.sh $nPerJob $i"
   echo $cmd
   eval $cmd
done

