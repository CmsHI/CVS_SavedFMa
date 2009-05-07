#!/bin/bash
if [ ! -e log ]; then
   mkdir log
fi

cmsRun $1 >& log/${1%%.py}.txt &

