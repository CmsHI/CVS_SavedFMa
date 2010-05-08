import FWCore.ParameterSet.Config as cms

analysisSkimContent = cms.PSet(
    outputCommands = cms.untracked.vstring('drop *',
      # event
      'keep *_hiSelectedVertex_*_*',
      'keep *_pixelVertices_*_*',
      'keep *_offlineBeamSpot_*_*',
      'keep *_TriggerResults_*_HLT',
      'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*RECO',

      # hi event
      'keep *_heavyIon_*_*',
      'keep *_hiCentrality_*_*',

      # mc gen info
      'keep recoGenJets_*_*_HISIGNAL',
      'keep reco*_hiGenParticles_*_*',
      'keep GenEventInfoProduct_*_*_*',
      'keep *_partons_*_*',

      # jet
      'keep *_towerMaker_*_*',
      'keep patJets_*_*_*',

      # tracks
      'keep recoRecoChargedCandidates_allTracks_*_*',                                   
      'keep recoTracks_hiSelectedTracks_*_*'                                     
      )
    )
