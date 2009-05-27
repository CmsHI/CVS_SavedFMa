#!/bin/bash -
. /net/hisrv0001/home/frankma/UserCode/SavedFMa/bin/cgate/set_hiroot.sh
tar -xf macros.tar
nevt=$1
runn=$2

base=/net/pstore01/d00/scratch/frankma/hiroot
job=prodPtHat50
gen0=pythia50
gen1=pyquen50

# Make room for this job
if [ ! -e $base/$job ]; then mkdir $base/$job; fi

#=== Generate events===
# gen pythia
export HIROOT_BASEURL="$base/$job/$gen0"
echo "HIROOT_BASEURL: $HIROOT_BASEURL"
if [ ! -e $HIROOT_BASEURL ]; then mkdir $HIROOT_BASEURL; fi
root -b -q createPythiaDijet50.C+\($nevt,$runn\)

# gen pyquen
export HIROOT_BASEURL="$base/$job/$gen1"
echo "HIROOT_BASEURL: $HIROOT_BASEURL"
if [ ! -e $HIROOT_BASEURL ]; then mkdir $HIROOT_BASEURL; fi
root -b -q createPyquenDijet50.C+\($nevt,$runn\)

#=== run FF Jet Mod===
# ana var's
dRCut="0.35"
EtCut="30."
ana=iCone5_match35_jetEtCut30_ntJetEt

#--pythia--
export HIROOT_BASEURL="$base/$job/$gen0/$ana"
echo "HIROOT_BASEURL: $HIROOT_BASEURL"
if [ ! -e $HIROOT_BASEURL ]; then mkdir $HIROOT_BASEURL; fi
rfile=$(printf "hrt%06ds%03d.root\n" $runn 0)
rfpath=$base/$job/$gen0/$rfile
if [ -e $rfpath ]; then
   echo root -b -q runJFPythia.C+\($runn,\"$rfpath\",$dRCut,$EtCut\)
   root -b -q runJFPythia.C+\($runn,\"$rfpath\",$dRCut,$EtCut\)
   echo
else
   echo "bad gen file: $rfpath"
fi

#--pyquen--
export HIROOT_BASEURL="$base/$job/$gen1/$ana"
echo "HIROOT_BASEURL: $HIROOT_BASEURL"
if [ ! -e $HIROOT_BASEURL ]; then mkdir $HIROOT_BASEURL; fi
rfile=$(printf "hrt%06ds%03d.root\n" $runn 0)
rfpath=$base/$job/$gen1/$rfile
echo root -b -q runJFPythia.C+\($runn,\"$rfpath\",$dRCut,$EtCut\)
root -b -q runJFPythia.C+\($runn,\"$rfpath\",$dRCut,$EtCut\)


