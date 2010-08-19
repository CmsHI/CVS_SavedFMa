# .bashrc

# User specific aliases and functions

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

#--------------------------------cmsusr bash env------------------------------------
#===My top paths===
export mit=$HOME/mit
#export scratch=/net/pstore01/d00/scratch/frankma
export scratch=/net/hibat0003/d00/scratch/frankma
export work=$HOME/work
export cmsswarea=$work/cmssw
export hiroot=$work/hiroot
export SavedFMa=$HOME/UserCode/SavedFMa

#===Working projects===
# first practice with Yetkin
#export cur_pro=$cmsswarea/CMSSW_2_0_0_pre6/src
#
#--2_1_X--
# production for the quick gen level dN/dpt
#export cur_pro=$cmsswarea/CMSSW_2_1_4/src
# for 1st genJet test with producer copied from Yektin's dir:
# first jet area, directly copied from ~yetkin/cmsJet/src
#export jet_area=$cmsswarea/CMSSW_2_1_11/src
# jet area setting up from cvs
#export jet_area=$cmsswarea/jetCMSSW_2_1_11/src
# for L1-skim
export l1skim_area=$cmsswarea/CMSSW_2_1_7/src
# for HLT validation
export hlt_area=$cmsswarea/hltCMSSW_2_1_11/src
#
#---2_2_X---
#export jet_sw=$work/HI/jet/sw/CMSSW_2_2_5/src
#---jet gen level studies---
export dijetSens=$work/hiroot/test/dijet
#---Jet sw---
export jet31Xsw=$work/HI/jet/sw/CMSSW_3_1_0_pre10/src
export jet31Xrun=$scratch/HI/jet/cmssw310pre10

#---Shortcuts---
#-- 8.882 class --
#alias ana='cd $project/614; source INIT' // for 8.882 cdf
#alias ana='cd $project/cms; source INIT;cd ../root/hgg'
#-- IPMI project --
#alias ipmitool='sudo ipmitool'
#alias bmcutil='sudo bmcutil'
#-- T2_MIT machines--
alias t9='ssh -Y t2srv0009'
alias t1='ssh -Y t2srv0001'
alias t8='ssh -Y t2srv0008'
alias hibat1='ssh -Y hibat0001'
alias hibat2='ssh -Y hibat0002'
alias hibat3='ssh -Y hibat0003'
alias hissh1='ssh -Y hissh0001'
alias hissh2='ssh -Y hissh0002'
alias hidsk1='ssh -Y hidsk0001'

#-- T2 report--
alias daily='vi tier2/report/daily_operation/2008_daily_report.txt'
#-- my CMSSW local settings --
alias setcms=". $SavedFMa/bin/cgate/basic/cmssw_set_basic.sh"
alias setosg=". $SavedFMa/bin/cgate/basic/osg_cmssw_set_basic.sh"
alias cvscms=". $SavedFMa/bin/cvsinit.sh"
#-- Root stup --
alias setroot=". /net/hisrv0001/home/frankma/UserCode/SavedFMa/bin/cgate/set_root.sh"
alias sethiroot=". /net/hisrv0001/home/frankma/UserCode/SavedFMa/bin/cgate/set_hiroot.sh"

#--------------------------------Universal------------------------------------
. $HOME/UserCode/SavedFMa/dot_files/univ_dot_files/.bash_univ

