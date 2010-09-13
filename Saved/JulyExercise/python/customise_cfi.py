# Turn of MC dependence in pat sequence
def removePatMCMatch(process):
  process.makeHeavyIonJets.remove(process.genPartons)
  process.makeHeavyIonJets.remove(process.heavyIonCleanedGenJets)
  process.makeHeavyIonJets.remove(process.hiPartons)
  process.makeHeavyIonJets.remove(process.patJetGenJetMatch)
  process.makeHeavyIonJets.remove(process.patJetPartonMatch)

  process.patJets.addGenPartonMatch   = False
  process.patJets.embedGenPartonMatch = False
  process.patJets.genPartonMatch      = ''
  process.patJets.addGenJetMatch      = False
  process.patJets.genJetMatch	      = ''
  process.patJets.getJetMCFlavour     = False
  process.patJets.JetPartonMapSource  = ''
  return process

# Top Config to turn off all mc dependence
def disableMC(process):
  process.heavyIon.doMC = False
  removePatMCMatch(process)
  return process
