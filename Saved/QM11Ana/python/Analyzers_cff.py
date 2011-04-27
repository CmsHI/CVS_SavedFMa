import FWCore.ParameterSet.Config as cms

from MNguyen.InclusiveJetAnalyzer.inclusiveJetAnalyzer_cff import *
inclusiveJetAnalyzer.eventInfoTag = cms.InputTag("hiSignal")

akPu5PFJetAnalyzer = inclusiveJetAnalyzer.clone(
    jetTag = 'akPu5PFpatJets',
    genjetTag = 'ak5HiGenJets'
    )

akPu3PFJetAnalyzer = inclusiveJetAnalyzer.clone(
    jetTag = 'akPu3PFpatJets',
    genjetTag = 'ak3HiGenJets'
    )

# For backward xcheck
from Saved.DiJetAna.dijetAna_cfi import *
djicpu5 = djcalo.clone (
  jetsrc = "icPu5patJets",
  refjetsrc = "icPu5patJets",
  nearJetPtMin = 1000,
  trksrc = "hiGoodTracks",
  verbosity = 1
  )
djakpu3pf_pfcand = djicpu5.clone(
  jetsrc = "akPu3PFpatJets",
  refjetsrc = "akPu3PFpatJets",
  nearJetPtMin = 70,
  anaTrkType = cms.int32(4),
  trksrc = cms.InputTag("particleFlow",""),
  trkPtMin = 0.9
  )
djgenic5 = djgen.clone(
  refjetsrc = "icPu5patJets",
  nearJetPtMin = 1000
  )
djgenak3 = djgenic5.clone(
  jetsrc = "ak3HiGenJets",
  refjetsrc = "akPu3PFpatJets"
  )

#from MitHig.PixelTrackletAnalyzer
trkAnalyzer = cms.EDAnalyzer("TrackAnalyzer",
    trackSrc = cms.InputTag("hiGoodTracks"),
    vertexSrc = cms.vstring("hiSelectedVertex"),
    trackPtMin = cms.untracked.double(0.9),
    fiducialCut = cms.untracked.bool(True)
    )
hpttrkAnalyzer = trkAnalyzer.clone(
    trackSrc = "hiHighPtTracks"
    )
trkAnalyzerHighptNoTraj = trkAnalyzer.clone(
    trackSrc = "hiHighPtTracksNoTraj"
    )

genpAnalyzer = cms.EDAnalyzer('HiGenAnalyzer',
    useHepMCProduct = cms.untracked.bool(False),
    ptMin = cms.untracked.double(0.5),
    chargedOnly = cms.untracked.bool(True),
    src = cms.untracked.InputTag("hiSignal"),
    genpSrc = cms.untracked.InputTag("hiGenParticles"),
    genHiSrc = cms.untracked.InputTag("heavyIon"),
    )

# trk eff
from edwenger.HiTrkEffAnalyzer.hitrkEffAnalyzer_cff import *
hitrkEffAnalyzer_akpu3pf = hitrkEffAnalyzer.clone(
    # evt
    neededCentBins = cms.untracked.vint32(0, 1, 3, 11, 19, 35),
    # trk selection
    tracks = "hiGoodTracks",
    # trkPtMin = 0.9, # for nt
    fiducialCut = True,
    # setup
    fillNtuples = False,
    ptBinScheme = 3, # coarse binning for jet ana
    # jet-trk
    jets = 'akPu3PFpatJets',
    trkAcceptedJet = True, # jet |eta|<2
    useJetEtMode = 2, # mode2: jet1, jet1 or jet_pt=0
    jetTrkOnly = False # only trks in 0.8 cone or not
    )
hitrkEffAnalyzer_akpu3pf_j1 = hitrkEffAnalyzer_akpu3pf.clone(
    useJetEtMode = 1,
    jetTrkOnly = True
    )
hitrkEffAnalyzer_akpu3pf_j2 = hitrkEffAnalyzer_akpu3pf.clone(
    useJetEtMode = 1,
    jetTrkOnly = True,
    useSubLeadingJet = True
    )
hitrkEffAna_akpu3pf = cms.Sequence(cutsTPForFak*cutsTPForEff*hitrkEffAnalyzer_akpu3pf*hitrkEffAnalyzer_akpu3pf_j1*hitrkEffAnalyzer_akpu3pf_j2)

# hiHighPt
hiHighPtTrkEffAnalyzer_akpu3pf = hitrkEffAnalyzer_akpu3pf.clone(
    tracks = "hiHighPtTracks"
    )
hiHighPtTrkEffAnalyzer_akpu3pf_j1 = hiHighPtTrkEffAnalyzer_akpu3pf.clone(
    useJetEtMode = 1,
    jetTrkOnly = True
    )
hiHighPtTrkEffAnalyzer_akpu3pf_j2 = hiHighPtTrkEffAnalyzer_akpu3pf.clone(
    useJetEtMode = 1,
    jetTrkOnly = True,
    useSubLeadingJet = True
    )
hiHighPtTrkEffAna_akpu3pf = cms.Sequence(cutsTPForFak*cutsTPForEff*hiHighPtTrkEffAnalyzer_akpu3pf*hiHighPtTrkEffAnalyzer_akpu3pf_j1*hiHighPtTrkEffAnalyzer_akpu3pf_j2)
# merged tracks
hipixtrkEffAnalyzer_akpu3pf = hitrkEffAnalyzer_akpu3pf.clone(
    tracks = "hiGoodMergedTracks",
    label_tp_effic = "cutsTPForEffPxl",
    label_tp_fake = "cutsTPForFakPxl"
    )
hipixtrkEffAnalyzer_akpu3pf_j1 = hipixtrkEffAnalyzer_akpu3pf.clone(
    useJetEtMode = 1,
    jetTrkOnly = True
    )
hipixtrkEffAnalyzer_akpu3pf_j2 = hipixtrkEffAnalyzer_akpu3pf.clone(
    useJetEtMode = 1,
    jetTrkOnly = True,
    useSubLeadingJet = True
    )
hipixtrkEffAna_akpu3pf = cms.Sequence(cutsTPForFakPxl * cutsTPForEffPxl* hipixtrkEffAnalyzer_akpu3pf * hipixtrkEffAnalyzer_akpu3pf_j1 * hipixtrkEffAnalyzer_akpu3pf_j2)

# calo-trk matching
from edwenger.HiTrkEffAnalyzer.hipfCandAnalyzer_cfi import *
hitrkPfCandAnalyzer = pfCandidateAnalyzer.clone(
    Tracks = "hiGoodTracks",
    ptMin = 2,
    hasSimInfo = True,
    isData = False
    )

# trk val
from PbPbTrackingTools.HiTrackValidator.hitrackvalidator_cfi import *
from edwenger.HiTrkEffAnalyzer.HiTPCuts_cff import *
hihighTrkVal = hitrkvalidator.clone(trklabel=cms.untracked.InputTag("hiHighPtTracks"))
hihighTrkVal_fake = hihighTrkVal.clone(simtrklabel = cms.untracked.InputTag("cutsTPForFak"),
                                     hasSimInfo=cms.untracked.bool(True),
                                     selectFake=cms.untracked.bool(True))
higoodTrkVal = hitrkvalidator.clone(trklabel=cms.untracked.InputTag("hiGoodTracks"))
higoodTrkVal_fake = higoodTrkVal.clone(simtrklabel = cms.untracked.InputTag("cutsTPForFak"),
                                       hasSimInfo=cms.untracked.bool(True),
                                       selectFake=cms.untracked.bool(True))

hihighTrkVal_fakeOnly = cms.Sequence(cutsTPForFak*
                                     hihighTrkVal_fake)
higoodTrkVal_fakeOnly = cms.Sequence(cutsTPForFak*
                                     higoodTrkVal_fake)


# centrality
makeCentralityTableTFile = cms.EDAnalyzer('CentralityTableProducer',
    isMC = cms.untracked.bool(True),
    makeDBFromTFile = cms.untracked.bool(False),
    makeTFileFromDB = cms.untracked.bool(True)
    )


# final sequences
trkcorr_seq = cms.Sequence( (hitrkEffAna_akpu3pf+hiHighPtTrkEffAna_akpu3pf) )
trkval_seq = cms.Sequence( hihighTrkVal + higoodTrkVal + hitrkPfCandAnalyzer)
#trkana_seq = cms.Sequence( trkAnalyzer * hpttrkAnalyzer * genpAnalyzer * djakpu3pf_pfcand)
trkana_seq = cms.Sequence( trkAnalyzer * hpttrkAnalyzer * genpAnalyzer)
trkana_seq_data = cms.Sequence( trkAnalyzer )
#jetana_seq = cms.Sequence( inclusiveJetAnalyzer * akPu5PFJetAnalyzer * akPu3PFJetAnalyzer * djgenic5 * djgenak3)
#jetana_seq_data = cms.Sequence( inclusiveJetAnalyzer * akPu5PFJetAnalyzer * akPu3PFJetAnalyzer * djakpu3pf_pfcand )
jetana_seq = cms.Sequence( inclusiveJetAnalyzer * akPu5PFJetAnalyzer * akPu3PFJetAnalyzer)
jetana_seq_data = cms.Sequence( inclusiveJetAnalyzer * akPu5PFJetAnalyzer * akPu3PFJetAnalyzer)

