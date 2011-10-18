#!/bin/bash
# 
# Certificate:
# - https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideCrabHowTo#Get_a_Grid_certificate
#
# Setup CRAB env.
# - Following WorkBookRunningGrid
#
# Note:
# - Make you you source these env scripts in the correct order, as
#   according to the twiki instructions
#

# 1. get access to a LCG User Interface (LCG UI). It will allow you to
#    access WLCG-affiliated resources.
#source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.sh
source /osg/app/glite/etc/profile.d/grid_env.sh
grid-proxy-init -debug -verify
voms-proxy-init -voms cms

# Now setup CMSSW
curr_dir=`pwd`
cd $CMSSW_BASE/src
cmsenv
cd $curr_dir

# 2. Source the crab env variables
# The lastest version:
#source /afs/cern.ch/cms/ccs/wm/scripts/Crab/crab.sh
source /osg/app/crab/crab.sh


