#!/bin/bash -
. /home/frankma/UserCode/SavedFMa/bin/cvsinit.sh
if [ ! -e setup ]; then mkdir setup; fi

# check out scripts to setup/scripts
scripts=setup/scripts
cvs co -d $scripts UserCode/CmsHi/Utilities/scripts
sed -i '/CVSROOT/ d' $scripts/setup31X.sh

eval `scramv1 ru -sh`

echo ==================setup: ==================
cat $scripts/setup31X.sh
echo ===========================================
echo to execute:
echo $scripts/setup31X.sh
echo

