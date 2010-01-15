log=log/inspect.txt
echo Inspection Begin > $log
root -b -q compare.C+\(0,1\) >> $log 
root -b -q compare.C+\(0,4\) >> $log
root -b -q compare.C+\(0,10\) >> $log
root -b -q compare.C+\(4,4\) >> $log
root -b -q compare.C+\(3,10\) >> $log
