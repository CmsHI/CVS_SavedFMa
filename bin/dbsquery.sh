#!/bin/bash -

#
# env:
# - after sourcing setup.sh in
#   DBS/Clients/Python package
#   * cf http://indico.cern.ch/conferenceDisplay.py?confId=65275
#        --> " CRAFT09 ..." talk --> slide 5
#

dbs search --query=$1
