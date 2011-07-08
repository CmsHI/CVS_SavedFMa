import FWCore.ParameterSet.Config as cms

from edwenger.HiTrkEffAnalyzer.hitrkEffAnalyzer_cff import *

hitrkEffAnalyzer_akpu3pf = hitrkEffAnalyzer.clone(
  # evt
  neededCentBins = cms.untracked.vint32(0, 1, 3, 11, 19, 35),
  # trk selection
  tracks = "hiGoodTightTracks",
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

hitrkEffAna_akpu3pf = cms.Sequence(cutsTPForFak*cutsTPForEff*hitrkEffAnalyzer_akpu3pf)