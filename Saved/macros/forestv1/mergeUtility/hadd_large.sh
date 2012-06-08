#!/bin/bash -

if [ $# -lt 2 ]; then
  echo "$0 <indir> <outfile>"
  exit 1
fi

indir=$1
outfile=$2

N=`ls $indir/*.root | wc -l`

echo $N input files in $indir

npermerge=40
ngroups=$(($N/$npermerge+1))
echo npermerge: $npermerge, ngroups: $ngroups

listfinal=""
for k in `seq 1 $ngroups`; do
  submerge=merge_$k.root
  mergelist="hadd -T -f $submerge"
  beg=$(($k*$npermerge))
  end=$npermerge
  if [ $k -eq $ngroups ]; then 
    beg=$N
    end=$(($N%$npermerge));
  fi
  echo end at $end
  for ifile in `ls $indir/*.root | head -n $beg | tail -n $end`; do
    mergelist=$mergelist" $ifile"
  done
  echo $mergelist
  eval $mergelist > log
  listfinal=$listfinal" $submerge"
done

echo $listfinal
hadd $outfile $listfinal
if [ $? -eq 0 ]; then rm $listfinal; fi
