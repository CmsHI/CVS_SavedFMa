#!/bin/bash
#setcms
#project CMSSW
kinit frankma@CERN.CH
# p5 has moved to Kerberos V
export CVS_RSH=ssh
#alias cvss='cvs -d ":ext:cmscvs.cern.ch:/cvs_server/repositories/CMSSW"'
export CVSROOT=":ext:cmscvs.cern.ch:/cvs_server/repositories/CMSSW"
# cf Eric Cano's email
# cf http://cvs.web.cern.ch/cvs/howto.php#accessing-clients

