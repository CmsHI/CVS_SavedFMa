#!/bin/sh -

export SCRAM_ARCH=slc4onl_ia32_gcc346
alias scramv1_local='/opt/cmssw/slc4onl_ia32_gcc346/lcg/SCRAMV1/V1_2_1/bin/scram'
eval `scramv1_local runtime -sh`
export CMSSW_DATA_PATH=/opt/cmssw/slc4onl_ia32_gcc346/cms
#project CMSSW
#cmscvsroot CMSSW
echo 'done'

