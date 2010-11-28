#!/bin/bash -
if [ $# -lt 2 ]; then
  echo Usage:
  echo "  $0 <input_list> <output_dir>"
  exit 1
fi

#inputTopDir=$1
inputList=$1
output_dir=$2

#prefix=dcache:
#prefix=file:
njobs=1000000

echo begin
for i in `cat $inputList | sort | head -n $njobs`
do
  ifile=${i##*/}
  inputTopDir=${i%/*.root}
  if [ -f $output_dir/dj_$ifile ];
  then echo file exits: $output_dir/dj_$ifile
  else cmd="./condor64Basic.sh runOne.sh $inputTopDir $ifile $output_dir -1"
  fi
  echo $cmd
  eval $cmd
done

