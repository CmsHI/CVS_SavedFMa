import FWCore.ParameterSet.Config as cms

cleanedTowers = cms.EDProducer("CaloTowersCleaner",
   regularTowerTag = cms.InputTag("towerMaker"),
   excludeEtaMin = cms.double(0),
   excludeEtaMax = cms.double(0),
   excludePhiMin = cms.double(0),
   excludePhiMax = cms.double(0)
   )
