#!/bin/bash -
result= `env | grep "$1=" | awk -F= '{print $2}'`

if [ $result ]; then
   echo $result
else
   echo "no env variable: '$1' found"
   exit 1
fi
