#!/bin/bash -
if [ $# -lt 2 ]; then
   echo "Usage"
   echo "  get_castor.sh <castor_dir> <flistname> [tag]"
   exit 1
fi

# set var's
castordir=$1
flist=$2
tag=root

if [ $# -gt 2 ]; then
  tag=$3
fi

# check
if [ -e $flist ]; then
  echo "$flist already exists. Will no create new one"
  exit 1
fi

# run
touch $flist
files=`rfdir $castordir | grep $tag | awk '{print $NF}'`
for i in $files; do
   echo "$castordir/$i" >> $flist
done
echo $flist created

