import FWCore.ParameterSet.Config as cms

cleanedTowers = cms.EDProducer("CaloTowersCleaner",
   regularTowerTag = cms.InputTag("towerMaker"),
   excludeEtaMin = cms.double(-999),
   excludeEtaMax = cms.double(999),
   excludePhiMin = cms.double(-999),
   excludePhiMax = cms.double(999)
   )
