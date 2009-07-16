#!/bin/bash -

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
echo checking out EventFilter/ConfigDB to $(pwd)
# co and compile ConfigDB (java) code
if [ -e EventFilter/ConfigDB ]; then
   cd EventFilter/ConfigDB
   cvs up
else
   cvs co EventFilter/ConfigDB
   cd EventFilter/ConfigDB/
fi

#
# rHEAD of 2009.7.16:
#   OnlineConverter.java -r1.43 moved to python
#

# V00-26-04 is the head as of 2009.1.28
# cf https://twiki.cern.ch/twiki/bin/view/CMS/EvfHiltonWorkingTutorialOnlineConverter
#    for more information regarding the OnlineConverter
ant
cd $startpath

