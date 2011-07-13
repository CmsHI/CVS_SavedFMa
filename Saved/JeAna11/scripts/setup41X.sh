## hiGoodMergedTracks and the track analyzer
cvs co -d       edwenger/HiVertexAnalyzer UserCode/edwenger/HiVertexAnalyzer
cvs co -d       edwenger/HiTrkEffAnalyzer UserCode/edwenger/HiTrkEffAnalyzer
cvs co -d       edwenger/HiTrkEffAnalyzer/python/ UserCode/edwenger/HiTrkEffAnalyzer/python/

# pat macros
cvs co -d      MNguyen/patMacrosForMC  UserCode/MNguyen/patMacrosForMC
cvs co -d      MNguyen/patMacrosForDataSkims  UserCode/MNguyen/patMacrosForDataSkims
cvs co -d      MNguyen/Configuration  UserCode/MNguyen/Configuration
cvs co         HeavyIonsAnalysis/Configuration

# Jet and HI Software
cvs co -d       MNguyen/InclusiveJetAnalyzer UserCode/MNguyen/InclusiveJetAnalyzer

cvs co -r       V05-01-09 RecoJets/JetProducers
cvs co          RecoHI/HiJetAlgos
addpkg           RecoHI/Configuration

#Centrality
cvs co          CondFormats/HIObjects
cvs co          DataFormats/HeavyIonEvent
cvs co          RecoHI/HiCentralityAlgos
cvs co -d       CmsHi/Analysis2010 UserCode/CmsHi/Analysis2010

# needed to get PFTowers to work
cvs co -d Saved/JeAna11 UserCode/SavedFMa/Saved/JeAna11
cp Saved/JeAna11/modifiedFiles/VirtualJetProducer.cc RecoJets/JetProducers/plugins/
cp Saved/JeAna11/modifiedFiles/PileUpSubtractor.cc RecoJets/JetProducers/src/
cp Saved/JeAna11/modifiedFiles/PileUpSubtractor.h RecoJets/JetProducers/interface/

#ecal and hcal cleaning
#hcal
addpkg RecoLocalCalo/HcalRecAlgos
cvs co -r V00-07-21 RecoLocalCalo/HcalRecAlgos/src/HBHETimingShapedFlag.cc
cvs co -r V00-07-21 RecoLocalCalo/HcalRecAlgos/interface/HBHETimingShapedFlag.h
cvs co -r V00-00-18 JetMETAnalysis/HcalReflagging
cvs co -d HcalFilter UserCode/MTonjes/HcalFilter

#ecal requires some code from the photon analyzer, 'rm' commands are to truncate the endless chain of packages obviously, this should be cleaned up
cvs co -d CmsHi/PhotonAnalysis UserCode/CmsHi/PhotonAnalysis
rm CmsHi/PhotonAnalysis/plugins/MultiPhotonAnalyzer.*
rm CmsHi/PhotonAnalysis/plugins/SinglePhotonAnalyzer.*
cvs co -d UserCode/HafHistogram UserCode/CmsHi/HafHistogram

# trigger analyzers
cvs co -r CMSSW_4_1_2_patch1 HLTrigger/HLTanalyzers
# some output cleanup
sed -i 's|std::cout << " Beginning HLTAnalyzer Analysis|//std::cout << " Beginning HLTAnalyzer Analysis|' HLTrigger/HLTanalyzers/src/HLTAnalyzer.cc
sed -i 's|errMax(){return 100;}|errMax(){return 0;}|' HLTrigger/HLTanalyzers/interface/HLTAnalyzer.h

scram build -c
scram b -j4
