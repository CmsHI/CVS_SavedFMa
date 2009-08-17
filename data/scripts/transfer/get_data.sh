#!/bin/bash -
print_help () {
   echo usage:
   echo "  get_data.sh [-s source] [-d destination] [-r release] <-t data_tier> [-v]"
}

if [ $# -eq 0 ]; then
   print_help
   exit 1
fi

# address map
# for the use of pass by reference, cf:
#   * http://www.mcwalter.org/technology/shell/functions.html
getAdrs () {
   # $1=key
   # $2=value
case $1 in
   svmit)
   eval "$2=svmithi01.cern.ch:/home/frankma/scratch/data"
   ;;
   p5)
   eval "$2=cmsusr5:/tmp/frankma"
   ;;
   fuval)
   # to be used at fuval machine!
   eval "$2=/tmp/frankma"
   ;;
   mit)
   eval "$2=cgate.mit.edu:/net/pstore01/d00/scratch/frankma/data/cmssw"
esac
}

# initialze variables
# === set parameters ===
# -- defaults --
rel=cmssw322patch2_EcalDigi2RawFix
tier=raw
srcstoreKey=
localstoreKey=
verbose=0
check=false

# set variables
while [ $# -gt 0 ]; do
   case $1 in 
      -s)  srcstoreKey=$2
      shift
      ;;
      -d)  localstoreKey=$2
      shift
      ;;
      -r)  rel=$2
      shift
      ;;
      -t)  tier=$2
      shift
      ;;
      -v)  verbose=1
      ;;
      -c) check=true
      ;;
      --)  shift # By convention, --ends options
      break
      ;;
      -*)  echo $0: $1: unrecognized option >&2
      ;;
      *)  break # nonoption arguemtn, break while loop
      ;;
   esac
   shift # setup for next iteration
done
# show key values
if [ $verbose -ge 1 ]; then
   echo srcstoreKey: $srcstoreKey, localstoreKey, $localstoreKey, release: $release, tier: $tier, verbose: $verbose
fi

# now load parameter values
getAdrs $srcstoreKey srcstore
localstore=/tmp/frankma

# -relative data directory-
datadir=$rel/$tier
getcmd="scp $srcstore/$datadir/*.root $localstore/$datadir/"
if [ $verbose -ge 1 ]; then
   echo srcstore: $srcstore
   echo localstore: $localstore
   echo datadir: $datadir
   echo will get: $getcmd
fi

# if just checking exit here
if [ $check == true ]; then
   exit 0
fi

echo run
# prepare local area
if [ ! -e $localstore ]; then mkdir $localstore; fi
if [ ! -e $localstore/$rel ]; then mkdir $localstore/$rel; fi
if [ ! -e $localstore/$datadir ]; then mkdir $localstore/$datadir; fi

# === get ===
eval $getcmd

