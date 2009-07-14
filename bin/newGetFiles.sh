#!/bin/bash -
fileName=${1##/*/}
destination=`pwd`
echo "cp -v $1 $destination/$fileName" > getfiles.sh
chmod +x ./getfiles.sh
