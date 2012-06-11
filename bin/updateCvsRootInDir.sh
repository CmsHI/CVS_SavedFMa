for i in `find . | grep 'CVS/Root'`; do sed -i 's;:ext:cmscvs.cern.ch:/cvs_server/repositories/CMSSW;:ext:cmscvs.cern.ch:/local/reps/CMSSW;' $i ; done
