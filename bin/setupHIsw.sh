#!/bin/bash -
if [ ! -e setup ]; then mkdir setup; fi

# check out scripts to setup/scripts

scripts=setup/scripts
#setupsct=setup31X.sh
setupsct=setup3XY.sh

cvs co -d $scripts UserCode/CmsHi/Utilities/scripts
# sed replace in place
sed -i '/CVSROOT/ d' $scripts/$setupsct

eval `scramv1 ru -sh`

echo ==================setup: ==================
cat $scripts/$setupsct
echo ===========================================
echo to execute:
echo $scripts/$setupsct
echo

