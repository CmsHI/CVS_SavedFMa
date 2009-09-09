#!/bin/bash -
# to be sourced

# setup project
scramv1 project -n subegenjetCMSSW_3_3_0_pre2 CMSSW CMSSW_3_3_0_pre2
cd subegenjetCMSSW_3_3_0_pre2/src

# cvs
cvscms
# Need the HiGenParticleProducer class
cvs co CmsHi/Utilities
# Need head version for the HiGenParticleProducer class to compile
cvs co V04-00-00 SimGeneral/TrackingAnalysis # HEAD as of 2009.9.9
# - No other code needed
#   * hi jet reco code is the new structure in 330pre2
#   * using the normal PhysicsTools/HepMCCandAlgos hepmc converter
#     with the normal DataFormats/HepMCCandidate for now
#     until it is fixed in the Development_3xy_HeavyIons branch

scramv1 b
