#!/bin/bash -
if [ $# -eq 0 ]; then
   echo Usage
   echo "  $0 <tag> <start> <end>"
   echo Eg:
   echo "  $0 pyquen_unquendijet_nocoll_pt120to170_d20091025"
   exit 1
fi

type=pat
version=CMSSW_3_3_1_fix03
tag=$1
files=~/work/transfer/$version/Hydjet_MinBias_noColl_4TeV/$tag/flist.txt

dir=/pnfs/cmsaf.mit.edu/hibat/cms/users/yetkin/$type/$version/Hydjet_MinBias_noColl_4TeV/$tag/merged
destination=/castor/cern.ch/user/f/frankma/data/$type/$version/Hydjet_MinBias_noColl_4TeV/$tag

counter=1

rfmkdir -p $destination
log=files.log
echo start > $log
for infile in `cat $files`
do
   if [ $counter -ge $2 ] && [ $counter -le $3 ]; then
      if [ -e $file ]; then continue; fi
      file=$infile
      echo $dir/$infile >> $log
      lcg-cp srm://se01.cmsaf.mit.edu:8443/$dir/$infile file:$file >> $log
      rfcp $file $destination/
      rm $file
   fi
   counter=$(($counter + 1))
done

echo "directory : $destination"
rfdir $destination

