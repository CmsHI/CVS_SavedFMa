#!/bin/bash
awk -F 'L1' '{print $2}' $1 |
   sed 's/^[[:space:]]*/L1_/' |
   #awk -v 'OFS=_' '{ print $1,$2,$3 }' |
   awk -f ~/bin/awk/fill_space_with_underscore.awk |
   sort | uniq

