## hiGoodTracks and the track analyzer, pixel tracks commented out
cvs co -d       edwenger/HiVertexAnalyzer UserCode/edwenger/HiVertexAnalyzer
cvs co -d       edwenger/HiTrkEffAnalyzer UserCode/edwenger/HiTrkEffAnalyzer
#cvs co -d      Appeltel/PixelTrackAnalysis  UserCode/Appeltel/PixelTrackAnalysis
#cvs co -d      Appeltel/PixelTracksRun2010  UserCode/Appeltel/PixelTracksRun2010
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
cp /net/hidsk0001/d00/scratch/mnguyen/CMSSW_3_9_9_patch1/src/modifiedFiles/hiGenJets_cff.py RecoHI/HiJetAlgos/python/

# needed to get PFTowers to work
addpkg DataFormats/ParticleFlowCandidate

# private modes related to PFTowers
cp /net/hidsk0001/d00/scratch/mnguyen/CMSSW_3_9_9_patch1/src/modifiedFiles/VirtualJetProducer.cc RecoJets/JetProducers/plugins/
cp /net/hidsk0001/d00/scratch/mnguyen/CMSSW_3_9_9_patch1/src/modifiedFiles/PileUpSubtractor.cc RecoJets/JetProducers/src/
cp /net/hidsk0001/d00/scratch/mnguyen/CMSSW_3_9_9_patch1/src/modifiedFiles/PileUpSubtractor.h RecoJets/JetProducers/interface/
cp /net/hidsk0001/d00/scratch/mnguyen/CMSSW_3_9_9_patch1/src/modifiedFiles/PFCandidate.cc DataFormats/ParticleFlowCandidate/src/


#Particle Flow stuff needed for HI muon reco, omit if you don't care about muons!

addpkg RecoParticleFlow/PFProducer
addpkg RecoParticleFlow/PFRootEvent

# private mods related to muon reco
cp /net/hidsk0001/d00/scratch/mnguyen/CMSSW_3_9_9_patch1/src/modifiedFiles/PFRootEventManager.cc RecoParticleFlow/PFRootEvent/src/ 
cp /net/hidsk0001/d00/scratch/mnguyen/CMSSW_3_9_9_patch1/src/modifiedFiles/PFBlockAlgo.h RecoParticleFlow/PFProducer/interface/
cp /net/hidsk0001/d00/scratch/mnguyen/CMSSW_3_9_9_patch1/src/modifiedFiles/PFBlockProducer.cc RecoParticleFlow/PFProducer/plugins/ 
cp /net/hidsk0001/d00/scratch/mnguyen/CMSSW_3_9_9_patch1/src/modifiedFiles/HiHackedAnalyticalTrackSelector.cc  edwenger/HiTrkEffAnalyzer/src/

# analyzers
cvs co -d     Saved/QM11Ana UserCode/SavedFMa/Saved/QM11Ana
cvs co -d     MitHig/PixelTrackletAnalyzer UserCode/MitHig/PixelTrackletAnalyzer
rm -rf MitHig/PixelTrackletAnalyzer/src/PixelHitAnalyzer.cc

scram b -j4
