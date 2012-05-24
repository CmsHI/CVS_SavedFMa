#!/bin/sh
beg=1
nSub=300
for i in {1..1000}; do
  date
  nJob=`condor_q frankma | grep frankma | wc -l`
  echo $nJob running
  if [ $nJob -lt 200 ]; then
    end=$(($beg+$nSub-1))
    echo will submit $beg to $end
    ./submit.sh $beg $end
    beg=$(($end+1))
    echo next will submit $beg to $(($beg+$nSub-1))
  fi
  sleep 999
done

