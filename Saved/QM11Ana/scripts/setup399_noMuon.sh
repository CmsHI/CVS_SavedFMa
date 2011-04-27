## hiGoodTracks and the track analyzer, pixel tracks commented out
cvs co -d       edwenger/HiVertexAnalyzer UserCode/edwenger/HiVertexAnalyzer
cvs co -d       edwenger/HiTrkEffAnalyzer UserCode/edwenger/HiTrkEffAnalyzer
cvs co -d      Appeltel/PixelTrackAnalysis  UserCode/Appeltel/PixelTrackAnalysis
cvs co -d      Appeltel/PixelTracksRun2010  UserCode/Appeltel/PixelTracksRun2010
#
## pat macros
cvs co -d      MNguyen/patMacrosForMC  UserCode/MNguyen/patMacrosForMC
cvs co -d      MNguyen/patMacrosForDataSkims  UserCode/MNguyen/patMacrosForDataSkims
cvs co -d      MNguyen/Configuration  UserCode/MNguyen/Configuration
cvs co         HeavyIonsAnalysis/Configuration
#

# Jet and HI Software
cvs co -d       MNguyen/InclusiveJetAnalyzer UserCode/MNguyen/InclusiveJetAnalyzer
cvs co -r       V05-01-09 RecoJets/JetProducers
cvs co          RecoHI/HiJetAlgos
addpkg          RecoHI/Configuration

#Centrality
cvs co          CondFormats/HIObjects
cvs co          DataFormats/HeavyIonEvent
cvs co          RecoHI/HiCentralityAlgos
cvs co -d       CmsHi/Analysis2010 UserCode/CmsHi/Analysis2010

# Jet plus tracks
cvs co -r       V00-02-00 RecoJets/JetPlusTracks  

#ecal and hcal cleaning
#hcal
addpkg RecoLocalCalo/HcalRecAlgos
cvs co -r V00-07-21 RecoLocalCalo/HcalRecAlgos/src/HBHETimingShapedFlag.cc
cvs co -r V00-07-21 RecoLocalCalo/HcalRecAlgos/interface/HBHETimingShapedFlag.h
cvs co -r V00-00-18 JetMETAnalysis/HcalReflagging
cvs co -d HcalFilter UserCode/MTonjes/HcalFilter

#ecal requires some code from the photon analyzer, 'rm' commands are to truncate the endless chain of packages
# obviously, this should be cleaned up
cvs co -d CmsHi/PhotonAnalysis UserCode/CmsHi/PhotonAnalysis
rm CmsHi/PhotonAnalysis/plugins/MultiPhotonAnalyzer.*
rm CmsHi/PhotonAnalysis/plugins/SinglePhotonAnalyzer.*
cvs co -d UserCode/HafHistogram UserCode/CmsHi/HafHistogram

# private mod to run the right genJet collections
cp /net/hisrv0001/home/frankma/work/HI/jet/sw/QM11_399p1/src/RecoHI/HiJetAlgos/python/HiGenJets_cff.py RecoHI/HiJetAlgos/python/HiGenJets_cff.py

# needed to get PFTowers to work
addpkg DataFormats/ParticleFlowCandidate

# private modes related to PFTowers
cp /net/hidsk0001/d00/scratch/mnguyen/CMSSW_3_9_9_patch1/src/modifiedFiles/VirtualJetProducer.cc RecoJets/JetProducers/plugins/
cp /net/hidsk0001/d00/scratch/mnguyen/CMSSW_3_9_9_patch1/src/modifiedFiles/PileUpSubtractor.cc RecoJets/JetProducers/src/
cp /net/hidsk0001/d00/scratch/mnguyen/CMSSW_3_9_9_patch1/src/modifiedFiles/PileUpSubtractor.h RecoJets/JetProducers/interface/
cp /net/hidsk0001/d00/scratch/mnguyen/CMSSW_3_9_9_patch1/src/modifiedFiles/PFCandidate.cc DataFormats/ParticleFlowCandidate/src/

# tracking tools
cvs co -d PbPbTrackingTools/VertexConstraintProducer UserCode/ASYoon/PbPbTrackingTools/VertexConstraintProducer
cvs co -d PbPbTrackingTools/CaloCompatibleTrackSelector UserCode/ASYoon/PbPbTrackingTools/CaloCompatibleTrackSelector
cvs co -d PbPbTrackingTools/HiTrackValidator UserCode/ASYoon/PbPbTrackingTools/HiTrackValidator

# analyzers
cvs co -d     Saved/QM11Ana UserCode/SavedFMa/Saved/QM11Ana
cvs co -d     MitHig/PixelTrackletAnalyzer UserCode/MitHig/PixelTrackletAnalyzer
rm -rf MitHig/PixelTrackletAnalyzer/src/PixelHitAnalyzer.cc

scram b -j4
