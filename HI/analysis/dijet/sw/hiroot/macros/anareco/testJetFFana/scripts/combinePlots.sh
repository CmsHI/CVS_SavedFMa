#!/bin/bash -
input=$dijetSens/jetana/plots
output=$input/combine
if [ ! -e $output ]; then mkdir $output; fi

for i in $(find $input | grep $1 | grep $2 | grep -v __ | grep -v combine); do
   cut=$(echo $i | awk -F/ '{print $(NF-2)}')
   data=$(echo $i | awk -F/ '{print $(NF)}')
   data=${data%.*}
   #echo cut: $cut
   #echo data: $data
   #echo $i | awk -F/ '{print "cp", $0,"'$output'/"'$data'"_"'$cut'}'
   echo cp $i $output/${data}_${cut}.gif
done
