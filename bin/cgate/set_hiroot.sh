#!/bin/sh

#
# script to setup hiroot
# - basic ideas
#   * set the env variables for main paths
# - cf
#   * http://www.cmsaf.mit.edu/twiki/bin/view/Software/HIROOT
#   * http://viktor.lns.mit.edu/viewcvs/hiroot/trunk/README?view=markup
#


#--- Setup Root env using script ---
setroot

#--- Setup hiroot env ---
source /opt/bin/sh/sethirootpath.sh $HOME/work/hiroot

#export SCRAM_ARCH=slc3_ia32_gcc323

export HIROOT=$HOME/work/hiroot
export LD_LIBRARY_PATH=$HIROOT/lib:$LD_LIBRARY_PATH
