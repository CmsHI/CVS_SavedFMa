#!/bin/bash -
if [ $# -eq 0 ]; then
   echo "Usage:"
   echo "  $0 <run_#> <tag>"
   echo "Eg:"
   echo "  ./runICone5_JetEt30_Mod.sh 1 121"
   exit 1
fi

. /net/hisrv0001/home/frankma/UserCode/SavedFMa/bin/cgate/set_hiroot.sh
#tar -xf macros.tar
runn=$1
tag=$2

base=/net/pstore01/d00/scratch/frankma/hiroot
job=prodPtHat50
gen0=pythia50
gen1=pyquen50

#=== run FF Jet Mod===
# ana var's
dRCut="0.35"
EtCut="30."
ana=iCone5_match35_jetEtCut30_Mod_${tag}

#--pythia--
for genSource in $gen0 $gen1; do
   # set top level inputs/outputs
   export HIROOT_BASEURL="$base/$job/$genSource/$ana"
   if [ ! -e $HIROOT_BASEURL ]; then mkdir $HIROOT_BASEURL; fi
   echo "HIROOT_BASEURL: $HIROOT_BASEURL"
   rfile=$(printf "hrt%06ds%03d.root\n" $runn 0)
   rfpath=$base/$job/$genSource/$rfile

   # define input/output indepdent var's
   logdir=$HIROOT_BASEURL/log
   if [ ! -e $logdir ]; then mkdir $logdir; fi
   logfile=${rfile%.root}.txt
   logpath=$logdir/$logfile
   echo log: $logpath

   # run
   if [ -e $rfpath ]; then
      echo root -b -q runJFPythia.C+\($runn,\"$rfpath\",$dRCut,$EtCut\)
      root -b -q runJFPythia.C+\($runn,\"$rfpath\",$dRCut,$EtCut\) >& $logpath
      echo
   else
      echo "bad gen file: $rfpath"
   fi
done


