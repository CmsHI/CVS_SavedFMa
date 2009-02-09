#!/bin/bash -

if [ $# -ge 1 ]; then
   evfSwPath=$1
else
   evfSwPath="."
fi
startpath=$(pwd)

cd $evfSwPath
echo checking out EventFilter/ConfigDB to $(pwd)
# co and compile ConfigDB (java) code
if [ -e EventFilter/ConfigDB ]; then
   cd EventFilter/ConfigDB
   cvs up
else
   cvs co -r V00-26-04 EventFilter/ConfigDB
   cd EventFilter/ConfigDB/
fi

# V00-26-04 is the head as of 2009.1.28
# cf https://twiki.cern.ch/twiki/bin/view/CMS/EvfHiltonWorkingTutorialOnlineConverter
#    for more information regarding the OnlineConverter
ant
cd $startpath

