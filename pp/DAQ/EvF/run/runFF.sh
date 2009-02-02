#!/bin/bash -
echo "$@"
"$@"
sleep 20
./configure
sleep 150
./enable

