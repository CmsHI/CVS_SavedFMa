#!/bin/sh

#
# Script to setup root
# - main ideas
#   * setup $ROOTSYS
#   * setup $PATH and a few other related variables
# - cf
#   * http://root.cern.ch/drupal/content/installing-root-source
#

#--- Define variables ---
rootpath=/app/root/x86_64/5-18-00

#--- Setup Root env using script ---
#--Setup main root env variables--
source /opt/bin/sh/setrootpath.sh $rootpath
#--Setup system variables and include paths--
source $rootpath/bin/thisroot.sh


