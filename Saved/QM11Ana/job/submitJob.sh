#!/bin/bash -
if [ $# -lt 2 ]; then
  echo Usage:
  echo "  $0 <cfg> <input_list> <output_dir>"
  exit 1
fi

#inputTopDir=$1
cfg=$1
inputList=$2
output_dir=$3

#prefix=dcache:
#prefix=file:
njobs=1000000

ct=0
echo begin
for i in `cat $inputList | sort | head -n $njobs`
do
  ifile=${i##*/}
  inputTopDir=${i%/*.root}
  if [ -f $output_dir/tr_$ifile ];
  then echo file exits: $output_dir/dj_$ifile
  else cmd="./condor64Basic.sh runOne.sh $cfg $inputTopDir $ifile $output_dir -1"
  fi
  echo $cmd
  eval $cmd
  if [ $((ct%1)) -eq 0 ]; then echo "job $ct sleep"; sleep 2; fi
  ct=$((ct+1))
done

