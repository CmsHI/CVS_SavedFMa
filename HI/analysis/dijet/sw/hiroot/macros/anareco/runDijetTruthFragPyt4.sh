#!/bin/sh

#
# Setup a temporary directory
#

platform=`uname -i`
echo $platform

source /opt/bin/sh/setrootpath.sh /app/root/$platform/5-14-00
source /opt/bin/sh/sethirootpath.sh ~/HiRoot/hiroot_dev
export HIROOT=$HOME/HiRoot/hiroot_dev
export LD_LIBRARY_PATH=$HIROOT/lib:$LD_LIBRARY_PATH

me=`id -u -n`
tempdir=/osg/tmp/$me.$$
#tempdir=/tmp/cer/$me.$$

function cleanup() {

   if [ -n "$tempdir" -a -d $tempdir ]; then
      /bin/rm -fr "$tempdir"
   else
      echo "$0: tempdir ($tempdir) does not exists" >&2
   fi

}

function process() {
  outfile="$outpath"truth_"$runno".root
  echo $outfile
  if [ -a $outfile ]; then
     echo "$outfile exits. done"
  else
    infile=dcap:///net/hisrv0001/home/cer/hirootdata/pythiadijet4/hrt"$runno"s000.root
  root.exe -b -q "/net/hisrv0001/home/cer/HiProd/Process/runDijetTruthAnaFrag.C+($runno,\"$infile\",kFALSE)"
fi
}

if [ -e $tempdir ]; then
   echo "$0: tempdir ($tempdir) exists. exiting" >&2
   exit 1;

else
   trap cleanup EXIT
   mkdir $tempdir
fi

#
# End setup temporary directory
#

run=$1
run=`expr $run + $2`
i=0;
max=1
runno=0

while [ $i != $max ]; do

 runno=`expr $run + $runno`
 echo $runno
 i=`expr 1 + $i`
done

runno=`expr $runno + 900000`

echo processing run $runno

echo tempdir is $tempdir

cd /net/hisrv0001/home/cer/data/WORK/NuGammaJet/Prod/Process

i=0;
max=100

while [ $i != $max ]; do

 cbin=$i
 export HIROOT_BASEURL=/net/pstore02/d00/scratch/rolandc/DiJet/Frag/pythia4/
 outpath=$HIROOT_BASEURL/
 process
 runno=`expr 1 + $runno`
 i=`expr 1 + $i`
done

echo done

