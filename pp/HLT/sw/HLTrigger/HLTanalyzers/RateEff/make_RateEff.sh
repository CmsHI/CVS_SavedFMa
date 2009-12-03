#!/bin/bash -

#
# - cf
#   * http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/HLTrigger/HLTanalyzers/test/RateEff/INSTALL?view=markup
# 

cmsenv
if [ $? -ne 0 ]; then
   echo cmssw env not setup
   return 1
fi

echo $CMSSW_BASE
tar xvzf libconfig-1.3.tar.gz
cd libconfig-1.3/
./configure
make

cd ../
make

source setup.sh

#./OHltRateEff hltmenu_8E29_2009May11.cfg

