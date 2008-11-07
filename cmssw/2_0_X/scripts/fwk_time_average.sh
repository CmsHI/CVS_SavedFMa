#!/bin/bash -
firstEvtCalc=$2

out="fwk_timing_summeries"
mkdir ./$out
grep '^%MSG-i FwkReport:' $1 | grep Event > $out/${1%.log}.FwkReportTime
n=$(more $out/${1%.log}.FwkReportTime | wc -l)
n=$((n-firstEvtCalc+1))
awk '{print $5}' $out/${1%.log}.FwkReportTime > $out/${1%.log}.TimeColumn
awk -F: '{print $1, $2, $3}' $out/${1%.log}.TimeColumn > $out/${1%.log}.TimeColumnSep
awk -F: '{print $1*3600 + $2*60 + $3}' $out/${1%.log}.TimeColumn > $out/${1%.log}.TimeColumnSec

first=$(head -n $firstEvtCalc $out/${1%.log}.TimeColumnSec | tail -n 1)
last=$(tail -n 1 $out/${1%.log}.TimeColumnSec)

echo first timing event time: $first, last timing event time: $last

dT=$((last-first))
echo interval time: $dT s, total number of events: $n  > $out/results.txt
echo rate: $((n*1000/dT)) mHz, $((dT*1000/n)) ms/event  >> $out/results.txt
cat $out/results.txt

