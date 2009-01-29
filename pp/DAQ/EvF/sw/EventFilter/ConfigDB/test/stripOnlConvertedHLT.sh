#!/bin/bash
#
# cf
#   google: sed get line number. eg,
#   http://www.grymoire.com/Unix/Sed.html#uh-48
# also cf
#   man sed
#

if [ $# -eq 0 ]; then
   echo "usage: $0 <rawConverted> <output>"
   exit 1
fi

input=$1
output=$2

beg=$(sed -n '\:EP CONFIGURATION:=' $input)
end=$(sed -n '\:SM CONFIGURATION:=' $input)
length=$((end-1))
head -n $length $input | tail -n $((length-beg)) > $output

