#/bin/bash -
if [ $# -lt 3 ]; then
  echo Usage:
  echo "  get_castor_from_list <tag> <flist> <n> [pref]"
  exit 1
fi

# set vars
tag=$1
flist=$2
n=$3
pref=$4

ncol=10

# run
for i in `seq 1 $ncol $n`; do
   end=$((i + ncol -1))
   dest=${tag}_$i.root
   curlist=`sed -n "$i,${end}p" $flist | sed "s;^;$pref;"`
   cmd="hadd $dest $curlist"
   echo $cmd
   eval $cmd
done

if [ ! -e final ]; then mkdir final; fi
finalcmd="hadd final/${tag}.root ${tag}_*.root"
echo $finalcmd
eval $finalcmd

rfcp final/${tag}.root /castor/cern.ch/user/f/frankma/data/HI/jet/pat/cmssw331/