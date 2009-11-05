#!/bin/bash -

#
# Note: you need to have grid env setup first
# - cf Ed, Yetkin
#

if [ $# -eq 0 ]; then
  echo Usage:
  echo "  lcg_cp.sh <indir> <fname>"
  echo Eg:
  echo "  lcg_cp.sh /pnfs/cmsaf.mit.edu/hibat/cms/users/yetkin/pat/CMSSW_3_3_1_fix03/SignalOnly/Hydjet_MinBias_noColl_4TeV/pyquen_quendijet_nocoll_pt120to170_d20091024 002E300A-E59F-DE11-8DC7-001EC94B6929_numEvent16658.root"
  exit 1
fi

indir=$1
fname=$2

host=srm://se01.cmsaf.mit.edu:8443
lcg-cp ${host}${indir}/${fname} file:$fname
