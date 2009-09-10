#!/bin/bash -

#
# 325 for now
# - cf
#   * https://twiki.cern.ch/twiki/bin/view/CMS/WorkBookHLTTutorial#IllusTra
#     --> "When running on 31X-compatible data"
#

scramv1 project -n openhltCMSSW_3_2_5 CMSSW CMSSW_3_2_5
cd openhltCMSSW_3_2_5/src
cvs co  HLTrigger/HLTanalyzers
eval `scramv1 runtime -sh`
scramv1 b

#
# make OpenHLT executable
# - cf
#   * http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/HLTrigger/HLTanalyzers/test/RateEff/INSTALL?view=markup
#

cd HLTrigger/HLTanalyzers/test/RateEff/
tar xvzf libconfig-1.3.tar.gz
cd libconfig-1.3
./configure
make
cd ..
make

#
# now run with
# source setup.sh
# ./OHltRateEff hltmenu_8E29_2009May11.cfg
# - cf
#   * http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/HLTrigger/HLTanalyzers/test/RateEff/README?view=markup
#

# finished
cd $CMSSW_BASE/src
