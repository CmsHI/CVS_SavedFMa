#!/bin/bash -

# usage:
#  setupOnlConverter.sh [$evfSwPath] 

if [ $# -ge 1 ]; then
   evfSwPath=$1
else
   evfSwPath=$DAQHIsw
fi
startpath=$(pwd)

# check path
if [ ! -e $evfSwPath ]; then
   echo "  $evfSwPath does not exist!"
   exit 1
fi

cd $evfSwPath
echo EventFilter/ConfigDB code at $evfSwPath
# co ConfigDB (java) code
# - cf https://twiki.cern.ch/twiki/bin/view/CMS/EvfHiltonWorkingTutorialOnlineConverter
# - history
#   * rHEAD of 2009.7.16:
#     OnlineConverter.java -r1.43 moved to python
#
if [ -e EventFilter/ConfigDB ]; then
   cd EventFilter/ConfigDB
   cvs up
else
   cvs co EventFilter/ConfigDB
   cd EventFilter/ConfigDB/
fi

# Compile
#    for more information regarding the OnlineConverter
# - Check the build.xml file
#   * I think all we are trying to do here is to build
#   * the .jar files ~ .so for c++?
# - Env needs
#   * [classic-]ant
#   * This is just java code, independent from CMSSW env!
ant
cd $startpath

