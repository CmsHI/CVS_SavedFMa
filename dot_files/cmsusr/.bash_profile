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
export log=$work/log
export cmsswarea=$work/cmssw
export cur_pro=$cmsswarea/CMSSW_2_0_10/src

#---Shortcuts----
#alias pdg='firefox pdg.lbl.gov&'
alias ema='emacs -nw'
alias xterm='xterm -fg green -bg black'
alias newfuval1_17='ssh -Y fuval-C2A11-17'
