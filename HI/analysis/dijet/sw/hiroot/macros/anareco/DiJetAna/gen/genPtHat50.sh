#!/bin/bash -

if [ $# -eq 0 ]; then
   echo "Usage:"
   echo "  genPtHat50.sh <nevt_per_job> <run_#>"
   exit 1
fi

. /net/hisrv0001/home/frankma/UserCode/SavedFMa/bin/cgate/set_hiroot.sh
tar -xf macros.tar
nevt=$1
runn=$2

base=/net/pstore01/d00/scratch/frankma/hiroot
job=prodPtHat50QRad
gen0=pythia50
gen1=pyquen50

# Make room for this job
if [ ! -e $base/$job ]; then mkdir $base/$job; fi

#=== Generate events===
# gen pythia
export HIROOT_BASEURL="$base/$job/$gen0"
echo "HIROOT_BASEURL: $HIROOT_BASEURL"
if [ ! -e $HIROOT_BASEURL ]; then mkdir $HIROOT_BASEURL; fi
# make room for log
logdir=$HIROOT_BASEURL/log
if [ ! -e $logdir ]; then mkdir $logdir; fi
logfile=$logdir/${gen0}_$runn.txt
root -b -q createPythiaDijet50.C+\($nevt,$runn\) >& $logfile

# gen pyquen
export HIROOT_BASEURL="$base/$job/$gen1"
echo "HIROOT_BASEURL: $HIROOT_BASEURL"
if [ ! -e $HIROOT_BASEURL ]; then mkdir $HIROOT_BASEURL; fi
logdir=$HIROOT_BASEURL/log
if [ ! -e $logdir ]; then mkdir $logdir; fi
# make room for log
logfile=$logdir/${gen1}_$runn.txt
root -b -q createPyquenDijet50.C+\($nevt,$runn\) >& $logfile

