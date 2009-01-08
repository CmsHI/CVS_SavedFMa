# .bashrc

# User specific aliases and functions

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

#---setup environment variables---
#--mine--
export PATH=$PATH:$HOME/bin
#---My top Paths---
export mit=~/mit
#---fixes---
#---x---
if [ -e /tmp/.X11-unix/* ] 
then
   X11_FOLDER=/tmp/.X11-unix
   currentUser=`(set \`whoami\`; echo $1)`
   bb=`ls -l $X11_FOLDER | grep $currentUser`
   bbb=${bb/*X/:}
   usedDISPLAY=$bbb.0
   export DISPLAY=$usedDISPLAY
fi
#--backspace key--
stty erase ^?

#---Preferences---
#--ls colors--
CLICOLOR=1
LSCOLORS=Dxfxcxdxbxegedabagacad
export CLICOLOR LSCOLORS
alias ls='ls -G'
alias la='ls -aG'

#--------------------------------Universal------------------------------------
#---My paths---
export projects=$mit/research/projects
export project=$projects/cmssw/CMSSW_2_0_0_pre6/src/first/MyAna
export tier2=$projects/tier2
export NAGIOS=$tier2/t2_svn/nagios
export NAGIOS_PLUGINS=$tier2/t2_svn/trunk/packages/nagios/root/usr/lib64/nagios/plugins

#---My shortcuts---
alias ema='emacs -nw'
alias xterm='xterm -fg green -bg black'

