#/bin/bash -
if [ $# -lt 3 ]; then
  echo Usage:
  echo "  get_castor_from_list <destination.root> <flist> <n> [pref]"
  exit 1
fi

dest=$1
flist=$2
n=$3
pref=$4

for i in `cat $flist | head -n $n`; do
   # todo: add a check of already merged ones from previous list
   cmd="hadd $dest ${pref}$i"
   echo $cmd
   eval $cmd
done

