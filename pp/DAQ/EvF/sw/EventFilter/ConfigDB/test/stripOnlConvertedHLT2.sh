#!/bin/bash
#
# cf
#    Classic Shell Scripting: ch 3.2.8 "sed Operation"
#    --> 3.2.9: Matching Specific Lines
#      * and it later also talks about using a different delimiter in patterns.
# also cf
#    man sed
#      * for the sed commands (s,p,d,..) 
#

if [ $# -eq 0 ]; then
   echo "usage: $0 <rawConverted> <output>"
   exit 1
fi

input=$1
output=$2

sed -n '\;EP CONFIGURATION;,/SM CONFIGURATION/p' $input | grep -v "EP CONFIGURATION" | grep -v "SM CONFIGURATION" > $output
# - here \; changes the pattern delimiter from '/' to ';'
# - p is the print command
# - -n suppresses automatic printing

