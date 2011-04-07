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

