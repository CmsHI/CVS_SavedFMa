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
cvs co -r V00-26-04 EventFilter/ConfigDB 
# V00-26-04 is the head as of 2009.1.28
# cf https://twiki.cern.ch/twiki/bin/view/CMS/EvfHiltonWorkingTutorialOnlineConverter
#    for more information regarding the OnlineConverter
cd $evfSwPath/EventFilter/ConfigDB/
ant
cd $startpath

