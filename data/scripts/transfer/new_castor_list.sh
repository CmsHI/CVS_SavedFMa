#!/bin/bash -
if [ $# -lt 2 ]; then
   echo "Usage"
   echo "  new_castor_list.sh <castor_dir> <flistname> [n_files_to_get] [tag] [mode]"
   echo "    mode:"
   echo "    0 - create list don't copy"
   echo "    1 - create list and copy (default)"
   exit 1
fi

# set var's
castordir=$1
flist=$2
nfiles=1000000
tag=root
mode=1

if [ $# -ge 3 ]; then
  nfiles=$3
fi
if [ $# -ge 4 ]; then
  tag=$4
  echo tag: $tag
fi
if [ $# -ge 5 ]; then
  mode=$5
  echo mode: $mode
fi

# check
if [ -e $flist ]; then
  echo "$flist already exists. Will not create new one"
  exit 1
fi

# run
touch $flist
files=`rfdir $castordir | grep $tag | head -n $nfiles | awk '{print $NF}'`
for i in $files; do
   echo "$castordir/$i" >> $flist
done
echo $flist created

# check mode
if [ $mode -eq 0 ]; then exit; fi

# get
echo Will begin to copy
#get_castor_from_list.sh $flist >& ${flist%.*}.log &
