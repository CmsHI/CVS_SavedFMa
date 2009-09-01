# .bashrc

# User specific aliases and functions

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

#--------------------------Setup lxplus environment variables----------------
#---My top Paths---
export SavedFMa=$HOME/UserCode/SavedFMa
export scratch=/d01/frankma/scratch
#export results=$scratch/results
#export cmsswwork=$scratch/cmssw
export lxp=/afs/cern.ch/user/f/frankma/public
export work=$HOME/work
export genjet=$work/HI/jet/sw/CMSSW_2_2_5/src/MitHig/HeavyIonJetAnalysis
export jetsw=$work/HI/jet/sw
export jetrun=$scratch/HI/jet
export jet229src=$work/HI/jet/sw/CMSSW_2_2_9/src
export jet310pre10src=$work/HI/jet/sw/CMSSW_3_1_0_pre10/src
export jet229run=$scratch/HI/jet/cmssw229
export jet310pre10run=$scratch/HI/jet/cmssw310pre10

#---Printers---
export p32f5='32-4A201-HP'
export p32gd='32-SB02-HP'

#---My shortcuts---
alias lxbuild='ssh -Y lxbuild067'
alias cgate='ssh -Y cgate.mit.edu'
#-- my CMSSW local settings --
alias setcms=". $SavedFMa/bin/lxplus/basic/cmssw_set_basic.sh"
alias cvscms=". $SavedFMa/bin/cvsinit.sh"

#--------------------------------Universal------------------------------------
. $SavedFMa/dot_files/univ_dot_files/.bash_univ

