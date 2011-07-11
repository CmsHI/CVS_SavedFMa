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
  print "add pixtrk eff ana"
  process.trkcorr_seq *= process.hipixtrkEffAna_akpu3pf
  print "change trk analyzers to include low pt trks"
  process.trkAnalyzer.trackSrc = "hiGoodMergedTracks"
  process.trkAnalyzer.trackPtMin = 0.5
  process.genpAnalyzer.ptMin = 0.5

def usehiHighPtTracks(process):
  print "hiHighPtTracks is used for pf rereco"
  process.trackerDrivenElectronSeeds.TkColList =  ["hiHighPtTracks"]
  print "change trk-calo analyzer to use hiHighPtTracks"
  process.hitrkPfCandAnalyzer.Tracks = "hiHighPtTracks"

def enableDataPat(process):
  removePatMCMatch(process.icPu5patJets)
  removePatMCMatch(process.akPu5PFpatJets)
  removePatMCMatch(process.akPu3PFpatJets)

def enableDataAnalyzers(process):
  process.icPu5JetAnalyzer.isMC = False
  process.akPu5PFJetAnalyzer.isMC = False
  process.akPu3PFJetAnalyzer.isMC = False

def enableOpenHlt(process, seq):
	from HLTrigger.HLTanalyzers.HI_HLTAnalyser_cff import hltanalysis
	process.hltanalysis = hltanalysis
	process.hltanalysis.RunParameters.Debug = False
	process.hltanalysis.RunParameters.UseTFileService = True
	process.hltanalysis.RunParameters.Monte = (False)
	process.hltanalysis.RunParameters.DoMC = (False)
	process.hltanalysis.RunParameters.DoJets = True
	process.hltanalysis.RunParameters.DoPhotons = True
	process.hltanalysis.RunParameters.DoSuperClusters = True
	process.hltanalysis.recjets  = "iterativeConePu5CaloJets"
	process.hltanalysis.BarrelPhoton = "correctedIslandBarrelSuperClusters"
	process.hltanalysis.EndcapPhoton = "correctedIslandEndcapSuperClusters"
	process.hltanalysis.l1GtReadoutRecord = "gtDigis"
	# add to seq
	process.load("RecoHI.HiCentralityAlgos.CentralityBin_cfi")
	seq*=process.centralityBin
	seq*=process.hltanalysis