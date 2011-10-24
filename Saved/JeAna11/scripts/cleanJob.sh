#!/bin/bash -
indir=$1
mkdir -p $indir/bad
for i in `ls $indir | grep root`; do
  data=$indir/$i;
  log=$indir/log/${i%.root}.txt
  grep -q BadAlloc $log
  if [ $? -eq 0 ]; then
    echo "bad file: " $log
    mv $data $indir/bad
    mv $log $indir/bad
  fi
  #ls -l $data
  #ls -l $log
done
