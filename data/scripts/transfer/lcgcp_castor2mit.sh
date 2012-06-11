for i in \
/castor/cern.ch/user/y/yjlee/HiForest/merged_pp2760_AllPhysics_Part_Prod03.root
do
   src="srm://srm-cms.cern.ch:8443/srm/managerv2?SFN="${i}
   out="srm://se01.cmsaf.mit.edu:8443/srm/v2/server?SFN=/mnt/hadoop/cms/store/user/frankmalocal/"${i##*/}
   
   cmd='lcg-cp -b --vo cms -D srmv2 -T srmv2 -v -n 10 '${src}' '${out}
   echo $cmd
   eval $cmd
done
