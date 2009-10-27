#/bin/bash -
if [ $# -lt 2 ]; then
  echo Usage:
  echo "  get_castor_from_list <flist> <n>"
  exit 1
fi

flist=$1
n=$2

for i in `cat $flist | head -n $n`; do
  if [ ! -e ${i##/*/} ]; then
    rfcp $i .
  fi
done

