#!/bin/bash -
echo "===netstat | grep 4000:"
netstat | grep 4000
echo
echo "===ps -elf | grep xdaq:"
ps -elf | grep xdaq
echo
echo "===ipcs:"
ipcs

