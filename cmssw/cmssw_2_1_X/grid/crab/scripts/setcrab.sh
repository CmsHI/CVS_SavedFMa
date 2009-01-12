#!/bin/bash
# 
# A test CRAB submission.
#   working in CMSSW_2_1_7
#   Following WorkBookRunningGrid (updated Nov 2008 for 2_1_9)
#

# 1. get access to a LCG User Interface (LCG UI). It will allow you to
#    access WLCG-affiliated resources.
source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.sh
voms-proxy-init -valid 720:00

# Now setup CMSSW
source $HOME/bin/basic/cmssw_set_basic.sh
cd $l1skim_area
cmsenv

# 2. Source the crab env variables
#source /afs/cern.ch/cms/ccs/wm/scripts/Crab/CRAB_2_4_0_Tutorial/crab.sh
source /afs/cern.ch/cms/ccs/wm/scripts/Crab/crab.sh

cd -

