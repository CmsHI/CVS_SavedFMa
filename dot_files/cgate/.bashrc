# .bashrc

# User specific aliases and functions

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

#--------------------------------cmsusr bash env------------------------------------
#---My top paths---
export mit=$HOME/mit
export cmsswarea=$HOME/cmssw
#===Working projects===
# first practice with Yetkin
#export cur_pro=$cmsswarea/CMSSW_2_0_0_pre6/src
#--2_1_X--
# production for the quick gen level dN/dpt
#export cur_pro=$cmsswarea/CMSSW_2_1_4/src
# for genJet
export cur_pro=$cmsswarea/CMSSW_2_1_11/src
# for L1-skim
export l1skim_area=$cmsswarea/CMSSW_2_1_7/src
# for HLT validation
export hlt_area=$cmsswarea/hltCMSSW_2_1_11/src

#---Shortcuts---
#alias ana='cd $project/614; source INIT' // for 8.882 cdf
#alias ana='cd $project/cms; source INIT;cd ../root/hgg'
#alias ipmitool='sudo ipmitool'
#alias bmcutil='sudo bmcutil'
alias t9='ssh -Y t2srv0009'
alias t1='ssh -Y t2srv0001'
alias t8='ssh -Y t2srv0008'
alias hibat='ssh -Y hibat0003'
alias daily='vi tier2/report/daily_operation/2008_daily_report.txt'

#--------------------------------Universal------------------------------------
. $HOME/UserCode/SavedFMa/dot_files/univ_dot_files/.bash_univ

#---old---
#---setup environment variables---
#--root--
#source /opt/bin/sh/setroot.sh 5-14-00
#--hiroot--
#source /opt/bin/sh/sethirootpath.sh ~/hiroot
#. /opt/bin/sh/sethiroot.sh  
#--CMSSW--
#SL3
#slc3_ia32_gcc323, source /net/hisrv0001/app/cmsprod/cms/setup/bashrc
#SL4
#export SCRAM_ARCH=slc4_ia32_gcc345
#source /app/cms-sl4/cmsset_default.sh
#source /app/cms-soft/cmsset_default.sh

