#!/bin/bash -

#
# Sw
# - http://www.cmsaf.mit.edu/twiki/bin/view/CmsHi/JetAnalysisSWDocumentation
#   * r7
#

# head versions: 2009.03.09

cvs co GeneratorInterface/HydjetInterface
cvs co -r V01-00-03 HeavyIonsAnalysis/Configuration
# Ed: he moved the head version to 31X
#   * thus, V01-00-03 is for tracking cfg in 22X
#   * cf high/low pt tracking in HeavyIonsAnalysis/Configuration/python
#     + comment of tag V01-00-04
cvs co UserCode/MitHig/HeavyIonJetAnalysis
cvs co UserCode/yetkin/RecoHI
cvs co SimDataFormats/HiGenData
cvs co SimDataFormats/HepMCProduct
cvs co SimDataFormats/GeneratorProducts
mv UserCode/MitHig .
mv UserCode/yetkin/RecoHI .
#-- Now co private code --
cvs co -r cmssw_2_2_5_run1M_0 UserCode/SavedFMa/HI/jet/sw
# source codes
mv UserCode/SavedFMa/HI/jet/sw/SimDataFormats/HiGenData/BuildFile SimDataFormats/HiGenData/BuildFile
mv UserCode/SavedFMa/HI/jet/sw/RecoHI/HiJetAlgos/src/BaseHiGenJetProducer.cc RecoHI/HiJetAlgos/src/BaseHiGenJetProducer.cc
mv UserCode/SavedFMa/HI/jet/sw/MitHig/HeavyIonJetAnalysis/src/HeavyIonJetAnalyzer.cc MitHig/HeavyIonJetAnalysis/src/HeavyIonJetAnalyzer.cc
# configurations
mv UserCode/SavedFMa/HI/jet/sw/MitHig/HeavyIonJetAnalysis/test MitHig/HeavyIonJetAnalysis/
rm -r MitHig/HeavyIonJetAnalysis/prod
rm -r MitHig/HeavyIonJetAnalysis/analysis
mv UserCode/SavedFMa/HI/jet/sw/MitHig/HeavyIonJetAnalysis/prod MitHig/HeavyIonJetAnalysis/
mv UserCode/SavedFMa/HI/jet/sw/MitHig/HeavyIonJetAnalysis/analysis MitHig/HeavyIonJetAnalysis/
rm -r UserCode

#-- Finally build --
scramv1 b

