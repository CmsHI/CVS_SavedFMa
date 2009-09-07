cvs co -r Development_34x_HeavyIons PhysicsTools/HepMCCandAlgos # head of branch on 2009/9/7
cvs co -r Development_34x_HeavyIons DataFormats/HepMCCandidate # ..
# for the new RecoJets/JetProducers/plugins/SubEventGenJetProducer.cc
cvs co -r CMSSW_3_3_0_pre2 RecoJets/JetProducers

# get yetkin's un-checked in fixes for the new DataFormats/HepMCCandidate
scp /net/hisrv0001/home/yetkin/CMSSW_3_3_0_pre2/src/DataFormats/HepMCCandidate/interface/GenParticle.h DataFormats/HepMCCandidate/interface/
scp /net/hisrv0001/home/yetkin/CMSSW_3_3_0_pre2/src/RecoJets/JetProducers/plugins/SubEventGenJetProducer.* RecoJets/JetProducers/plugins/

scramv1 b
