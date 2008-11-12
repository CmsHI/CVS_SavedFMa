# .bashrc

# User specific aliases and functions

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

#---setup environment variables---
#--root--
#source /opt/bin/sh/setroot.sh 5-14-00
#--hiroot--
#source /opt/bin/sh/sethirootpath.sh ~/hiroot
#. /opt/bin/sh/sethiroot.sh  
#--editor--
#export EDITOR=emacs
#---My top Paths---
export scratch=/export/04a/frankma
export results=$scratch/results
export cmsswwork=$scratch/cmssw
export lxp=/afs/cern.ch/user/f/frankma/public
export mit=~/mit
# for production for HLT online validation in 20X:
#export cur_proj=$scratch/cmssw/CMSSW_2_0_11/src
# for production for HLT online validation in 21X:
#export cur_proj=$scratch/cmssw/CMSSW_2_1_9/src
# for playback for HLT in 21X:
export cur_proj=$scratch/cmssw/CMSSW_2_1_11/src
export SavedFMa=$HOME/UserCode/SavedFMa

#--------------------------------Universal------------------------------------
. $HOME/UserCode/SavedFMa/univ_dot_files/.bash_univ

