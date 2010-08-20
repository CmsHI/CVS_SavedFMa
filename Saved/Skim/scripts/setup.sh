#!/bin/bash -
# HI sw
cvs co RecoHI/Configuration
cvs co RecoHI/HiCentralityAlgos
cvs co RecoHI/HiEvtPlaneAlgos
cvs co RecoHI/HiJetAlgos
cvs co RecoHI/HiMuonAlgos

cvs co GeneratorInterface/HiGenCommon

cvs co DataFormats/HeavyIonEvent
cvs co SimDataFormats/HiGenData

# jet specific
cvs co RecoJets/JetProducers
cvs co HeavyIonsAnalysis/Configuration

# pat
cvs co -r V08-00-24 PhysicsTools/PatAlgos

# dependencies
cvs co -r V00-04-02 RecoLocalCalo/Castor

# check dependencies
checkdeps
