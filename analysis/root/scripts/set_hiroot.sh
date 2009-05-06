#!/bin/sh

#--- Setup Root env using script ---
. $SavedFMa/analysis/root/scripts/set_root.sh

#--- Setup hiroot env ---
source /opt/bin/sh/sethirootpath.sh $HOME/work/hiroot

#export SCRAM_ARCH=slc3_ia32_gcc323

export HIROOT=$HOME/work/hiroot
export LD_LIBRARY_PATH=$HIROOT/lib:$LD_LIBRARY_PATH
