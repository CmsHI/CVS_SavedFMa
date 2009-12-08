#/bin/bash -
if [ $# -lt 1 ]; then
  echo Usage:
  echo "  get_castor_from_list <flist> [n]"
  exit 1
fi

flist=$1
n=100000
if [ $2 ]; then n=$2; fi

for i in `cat $flist | head -n $n`; do
  if [ ! -e ${i##/*/} ]; then
    cmd="rfcp $i ."
    echo $cmd
    eval $cmd
  fi
done

