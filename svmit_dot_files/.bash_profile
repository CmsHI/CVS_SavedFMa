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
#---setup private environment variables---
export PATH=$PATH:$HOME/bin

#---My paths---
export cfg=$cmsswwork/cfg
export projects=$mit/research/projects
export tier2=$projects/tier2
export NAGIOS=$tier2/t2_svn/nagios
export NAGIOS_PLUGINS=$tier2/t2_svn/trunk/packages/nagios/root/usr/lib64/nagios/plugins
export data=$scratch/data
export tsg=$cur_proj/hlt/tsg
export playback=$cur_proj/EventFilter/AutoBU/playback
# 21X working cvs
export cvsWorking=$SavedFMa/cmssw/cmssw_2_1_X

#---My shortcuts---
alias ema='emacs -nw'
alias xterm='xterm -fg green -bg black'

#---fixes---
#--backspace key--
stty erase ^?
