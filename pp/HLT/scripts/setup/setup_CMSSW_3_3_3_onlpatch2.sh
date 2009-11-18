# - following
#   * https://hypernews.cern.ch/HyperNews/CMS/get/relAnnounce/673.html
#   * but with the un-needed online tags taken out
scramv1 project CMSSW CMSSW_3_3_3
cd CMSSW_3_3_3/src
eval `scramv1 runtime -sh`
cvs co -r V01-03-00 Geometry/RPCGeometryBuilder
cvs co -r V01-05-06 HLTrigger/HLTfilters
cvs co -r V01-07-16 HLTrigger/special 
cvs co -r V02-00-35-05 RecoLocalCalo/EcalRecProducers
scramv1 b
