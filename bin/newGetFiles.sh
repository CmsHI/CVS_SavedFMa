#!/bin/bash -
if [ $# -eq 0 ]; then
   echo "Usage:"
   echo "  $0 <source_file> [destination_file]"
fi
if [ $# -eq 1 ]; then
   fileName=${1##/*/}
   destination="."
fi
if [ $# -gt 1 ]; then
   destination=$2
fi

echo "cp -v $1 $destination" > getfiles.sh
chmod +x ./getfiles.sh
