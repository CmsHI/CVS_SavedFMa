# $Id: setup.sh,v 1.13 2009/12/03 15:43:55 loizides Exp $

cvsdir=UserCode/SavedFMa/pp/HLT/sw/HLTrigger/HLTanalyzers
tag=HEAD

# check-out code
rm -rf HLTrigger/HLTanalyzers
#cvs co -r HEAD HLTrigger/HLTanalyzers
cvs co -r HEAD $cvsdir
mv UserCode/SavedFMa/pp/HLT/sw/HLTrigger .

touch HLTrigger/HLTanalyzers/BuildFile


