for i in \
/mnt/hadoop/cms/store/user/frankmalocal/forest/promptskim-hihighpt-hltjet80-pt90-v1_part.root \
/mnt/hadoop/cms/store/user/frankmalocal/forest/mergedPhoton40Skim.root
do
   src="srm://se01.cmsaf.mit.edu:8443/srm/v2/server?SFN="${i}
   out="srm://srm-cms.cern.ch:8443/srm/managerv2?SFN=/castor/cern.ch/user/f/frankma/forest2/"${i##*/}
   
   cmd='lcg-cp -b --vo cms -D srmv2 -T srmv2 -v -n 10 '${src}' '${out}
   echo $cmd
   eval $cmd
done
