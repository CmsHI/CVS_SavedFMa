#!/bin/bash -
echo "$@"
"$@"
sleep 15
./configure
sleep 120
./enable

