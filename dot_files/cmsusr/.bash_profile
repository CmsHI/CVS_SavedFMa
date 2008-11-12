# .bashrc

# User specific aliases and functions

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

#--------------------------------cmsusr bash env------------------------------------
#---My top paths---
export work=$HOME/work
export log=$work/log
export cmsswarea=$work/cmssw
export cur_pro=$cmsswarea/CMSSW_2_0_10/src

#---Shortcuts----
alias newfuval1_17='ssh -Y fuval-C2A11-17'

#--------------------------------Universal------------------------------------
. $HOME/UserCode/SavedFMa/univ_dot_files/.bash_univ

