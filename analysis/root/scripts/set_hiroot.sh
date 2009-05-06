#!/bin/sh


source /opt/bin/sh/setrootpath.sh /app/root/x86_64/5-14-00
source /opt/bin/sh/sethirootpath.sh $HOME/HiRoot/hiroot_dev

export SCRAM_ARCH=slc3_ia32_gcc323

export HIROOT=$HOME/HiRoot/hiroot_dev

export LD_LIBRARY_PATH=$HIROOT/lib:$LD_LIBRARY_PATH
