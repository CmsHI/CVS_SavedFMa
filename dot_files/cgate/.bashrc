# .bashrc

# User specific aliases and functions

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

#---setup environment variables---
#--mine--
export PATH=$PATH:$HOME/bin

#--------------------------------Universal------------------------------------
#---My paths---
export work=$HOME/work
export cmsswarea=$work/cmssw
export cur_pro=$cmsswarea/CMSSW_2_0_10/src

