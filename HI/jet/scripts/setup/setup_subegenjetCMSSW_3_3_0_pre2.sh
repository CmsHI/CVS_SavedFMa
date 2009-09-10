#!/bin/bash -

#
# - setup set genjet code with tag 330pre2 using subevent map
# - to be sourced
#

# setup project
rel=CMSSW_3_2_6
pname=semgenjet$rel
scramv1 project -n $pname CMSSW $rel
cd $pname/src

# cvs
cvscms
# First co the new jet code taged in 330pre2
cvs co -r CMSSW_3_3_0_pre2 RecoJets/JetProducers
cvs co -r CMSSW_3_3_0_pre2 RecoJets/JetAlgorithms
cvs co -r CMSSW_3_3_0_pre2 RecoJets/Configuration
# HI jet reco sequences
cvs co -r CMSSW_3_3_0_pre2 RecoHI/HiJetAlgos

# Need the HiGenParticleProducer class
cvs co -r V00-01-02 UserCode/CmsHi/Utilities
# Need head version for the HiGenParticleProducer class to compile
cvs co -r V04-00-00 SimGeneral/TrackingAnalysis # HEAD as of 2009.9.9
# other needed classes for the package to compile
cvs co -r cvs co -r V00-01-02 DataFormats/HeavyIonEvent

# - No other code needed
#   * hi jet reco code is the new structure in 330pre2
#   * using the normal PhysicsTools/HepMCCandAlgos hepmc converter
#     with the normal DataFormats/HepMCCandidate for now
#     until it is fixed in the Development_3xy_HeavyIons branch

# just for quick check
cvs co UserCode/CmsHi/JetAnalysis

mv UserCode/CmsHi .
rm -rf UserCode

scramv1 b
