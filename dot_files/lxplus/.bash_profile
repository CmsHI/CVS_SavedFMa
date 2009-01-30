# .bashrc

# User specific aliases and functions

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

#--------------------------------Universal------------------------------------
. $HOME/UserCode/SavedFMa/univ_dot_files/.bash_univ

#--------------------------Setup lxplus environment variables----------------
#---My top Paths---
export scratch=/build/frankma
export results=$scratch/results
export cmsswwork=$scratch/cmssw
export lxp=/afs/cern.ch/user/f/frankma/public
export mit=~/mit

#---Printers---
export p32f5='32-4A201-HP'
export p32gd='32-SB02-HP'

#---My shortcuts---
alias lxbuild='ssh -Y lxbuild067'
#-- my CMSSW local settings --
alias setcms=". $SavedFMa/bin/lxplus/basic/cmssw_set_basic.sh"
alias cvscms=". $SavedFMa/bin/lxplus/cvsinit.sh"

