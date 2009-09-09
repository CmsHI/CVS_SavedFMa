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

