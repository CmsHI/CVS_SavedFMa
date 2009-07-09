#!/bin/bash -
data=$1
out=$2
filename=${data##*/}
log=$out/${filename%.root}.txt
#== do checks ==
if [ ! -e $data ]; then
   echo bad input: $data
   return 1
fi
if [ ! -e $out ]; then mkdir $out; fi 

echo data: $data
echo out: $out
echo log: $log

root -b -q analyze_pp.C+\(\"$data\"\) > $log
mv *.gif $out
mv plots.root $out

#===run 2ndary ana===
root -b -q plotMatch.C+\(\"$out\"\)

