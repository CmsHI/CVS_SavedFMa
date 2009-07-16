# .bashrc

# User specific aliases and functions

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

#--------------------------------cmsusr bash env------------------------------------
#--- My top paths ---
export work=$HOME/work
export SavedFMa=$HOME/UserCode/SavedFMa
export log=$work/log
export cmsswarea=$work/cmssw
#--- Project specific paths ---
# for playback in 20X
#export cur_pro=$cmsswarea/CMSSW_2_0_10/src
# for hlt and test playback in 21X
export hlt_area=$cmsswarea/CMSSW_2_1_11/src
# for playback in privated modified 21X
export playback_area=$cmsswarea/modCMSSW_2_1_11/src

#---Shortcuts----
# --for compiling local cmssw--
alias nfuval1_30='ssh -Y fuval-C2A11-30'
alias nfuval2_30='ssh -Y fuval-C2F12-30'
# --for running local cmssw--
alias nfuval1_17='ssh -Y fuval-C2A11-17'
alias nfuval2_17='ssh -Y fuval-C2F12-17'
#-- my CMSSW local settings --
alias setcms=". $SavedFMa/bin/cmsusr/basic/cmssw_set_basic.sh"
alias cvscms=". $SavedFMa/bin/cmsusr/cvsinit.sh"
alias setfuval=". $SavedFMa/bin/cmsusr/setfuvalsw.sh"
alias checkOnlineCmssw=". $SavedFMa/bin/cmsusr/checkOnlineCmssw.sh"

#--------------------------------Universal------------------------------------
. $SavedFMa/dot_files/univ_dot_files/.bash_univ

