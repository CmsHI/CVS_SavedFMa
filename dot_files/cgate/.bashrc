# .bashrc

# User specific aliases and functions

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

#---setup environment variables---
#--mine--
export PATH=$PATH:$HOME/bin
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
#--editor--
export EDITOR=emacs
#---My top Paths---
export mit=~/mit

#--------------------------------Universal------------------------------------
#---My paths---
export projects=$mit/research/projects
export project=$projects/cmssw/CMSSW_2_0_0_pre6/src/first/MyAna
export tier2=$projects/tier2
export NAGIOS=$tier2/t2_svn/nagios
export NAGIOS_PLUGINS=$tier2/t2_svn/trunk/packages/nagios/root/usr/lib64/nagios/plugins
#export CATALOG=$project/cms/chain
#export REL_DIR=$project/cms/CMSSW_1_3_1_HLT1

#---My shortcuts---
#alias ana='cd $project/614; source INIT' // for 8.882 cdf
#alias ana='cd $project/cms; source INIT;cd ../root/hgg'
alias ema='emacs -nw'
alias xterm='xterm -fg green -bg black'
#alias ipmitool='sudo ipmitool'
#alias bmcutil='sudo bmcutil'
alias t9='ssh -Y t2srv0009'
alias t1='ssh -Y t2srv0001'
alias t8='ssh -Y t2srv0008'
alias daily='vi tier2/report/daily_operation/2008_daily_report.txt'

#---fixes---
#--backspace key--
stty erase ^?
