# Turn of MC dependence in pat sequence
def removePatMCMatch(patJets):
  patJets.addGenPartonMatch   = False
  patJets.embedGenPartonMatch = False
  patJets.genPartonMatch      = ''
  patJets.addGenJetMatch      = False
  patJets.genJetMatch	      = ''
  patJets.getJetMCFlavour     = False
  patJets.JetPartonMapSource  = ''

def usehiGoodMergedTracks(process):
  print "hiGoodMergedTracks is used (except PF re-reco)! --> re-reco of conformalPixelTrackReco!"
  process.hiextraTrackReco *= process.conformalPixelTrackReco
  process.hiextraTrackReco *= process.hiGoodMergedTracks
  print "change trk analyzers to include low pt trks"
  process.trkAnalyzer.trackPtMin = 0.5
  process.genpAnalyzer.ptMin = 0.5

def enableData(process):
  # jet
  removePatMCMatch(process.icPu5patJets)
  removePatMCMatch(process.akPu5PFpatJets)
  removePatMCMatch(process.akPu3PFpatJets)
  process.inclusiveJetAnalyzer.isMC = False
  process.akPu5PFJetAnalyzer.isMC = False
  process.akPu3PFJetAnalyzer.isMC = False
  process.djicpu5.isMC = False
  process.djakpu3pf_pfcand.isMC = False
