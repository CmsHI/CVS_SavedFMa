# Basic Openhlt
cvs co HLTrigger/HLTanalyzers
cvs co RecoHI/HiEgammaAlgos
cvs co -d Saved UserCode/SavedFMa/Saved
cvs co -d CmsHi/Analysis2010 UserCode/CmsHi/Analysis2010

# Enable wide bx for Openhlt
sed -i 's/_OR_BXes=false/_OR_BXes=true/' HLTrigger/HLTanalyzers/src/HLTInfo.cc

# Centrality
./CmsHi/Analysis2010/scripts/setup.sh

rm -rf RecoHI/HiCentralityAlgos
cvs co RecoHI/HiCentralityAlgos
