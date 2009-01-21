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
# for production as well as playback setup for HLT online validation in 20X:
#export cur_proj=$cmsswwork/CMSSW_2_0_11/src
# for production for HLT online validation in 21X:
#export cur_proj=$cmsswwork/CMSSW_2_1_9/src
# for playback for HLT in 21X:
#export cur_proj=$cmsswwork/CMSSW_2_1_11/src
# for playback for HLT in privately modified 21X:
export cur_proj=$cmsswwork/modCMSSW_2_1_11/src
# for l1 skim in 21X:
export l1skim_area=$cmsswwork/l1skimCMSSW_2_1_7/src

#---Printers---
export p32f5='32-4A201-HP'
export p32gd='32-SB02-HP'

#---My shortcuts---
alias lxbuild='ssh -Y lxbuild067'
#--------------------------------Universal------------------------------------
. $HOME/UserCode/SavedFMa/univ_dot_files/.bash_univ

