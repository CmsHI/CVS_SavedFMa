cmd='lcg-cp -b --vo cms -D srmv2 -T srmv2 -v -n 10 srm://srm-cms.cern.ch:8443/srm/managerv2?SFN=/castor/cern.ch/user/f/frankma/forest/Hi2011ForestDiJet_v7_noDuplicate.root srm://se01.cmsaf.mit.edu:8443/srm/v2/server?SFN=/mnt/hadoop/cms/store/user/frankmalocal/forest/Hi2011ForestDiJet_v7_noDuplicate.root'
echo $cmd
eval $cmd
