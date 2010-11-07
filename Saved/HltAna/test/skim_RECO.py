# Auto generated configuration file
# using: 
# Revision: 1.232.2.6 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: skim -s RECO --scenario HeavyIons --conditions GR10_P_V12::All --filein /store/hidata/HIRun2010/HIAllPhysics/RECO/PromptReco-v1/000/150/063/B497BEDB-8BE8-DF11-B09D-0030487A18F2.root --no_exec
import FWCore.ParameterSet.Config as cms
import os

process = cms.Process('TEST')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.MixingNoPileUp_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContentHeavyIons_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.232.2.6 $'),
    annotation = cms.untracked.string('skim nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      '/store/express/HIRun2010/HIExpressPhysics/FEVT/Express-v3/000/150/286/C627CB52-00EA-DF11-B833-001D09F24F1F.root',
    ),
)

# get a list of files from a specified directory
#mydir = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_GLOBAL/EventDisplay/Root',FileTempStorageArea"
#mylist = os.listdir(mydir)

# alternatively one can use all the files in the directory...
#for file in os.listdir(mydir):
#    if os.path.isfile(os.path.join(mydir,file):
#          process.source.fileNames.append('file:%s/%s' % (mydir,file))

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.Timing = cms.Service("Timing")

# Output definition

process.RECOoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.RECOEventContent.outputCommands,
    fileName = cms.untracked.string('skim_RECO.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    ),
    SelectEvents = cms.untracked.PSet(
    SelectEvents = cms.vstring('filter_step')
    ),
)

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'GR10_P_V12::All'

# filter on L1 bits
from HLTrigger.HLTfilters.hltLevel1GTSeed_cfi import hltLevel1GTSeed

process.bscSplash = hltLevel1GTSeed.clone(
    L1TechTriggerSeeding = cms.bool(True),
    L1SeedsLogicalExpression = cms.string('42 OR 43')
    )

process.bscHighMult = hltLevel1GTSeed.clone(
    L1TechTriggerSeeding = cms.bool(True),
    L1SeedsLogicalExpression = cms.string('35')
    )

process.bscOR = hltLevel1GTSeed.clone(
    L1TechTriggerSeeding = cms.bool(True),
    L1SeedsLogicalExpression = cms.string('(34) AND NOT (36 OR 37 OR 38 OR 39)')
    )

process.bscAND = hltLevel1GTSeed.clone(
    L1TechTriggerSeeding = cms.bool(True),
    L1SeedsLogicalExpression = cms.string('40')
    )

# filter on HLT bits
import HLTrigger.HLTfilters.hltHighLevel_cfi

# BSC high mult trigger
process.hltBSCHighMult = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltBSCHighMult.HLTPaths = ["HLT_HIL1Tech_BSC_HighMultiplicity"]
process.hltBSCHighMult.throw = False
process.hltBSCHighMult.andOr = True

# HF coincidence
process.towersAboveThreshold = cms.EDProducer("CaloTowerCandidateCreator",
    src = cms.InputTag("towerMaker"),
    verbose = cms.untracked.int32(0),
    minimumE = cms.double(3.0),
    minimumEt = cms.double(0.0),
)

process.hfPosTowers = cms.EDFilter("EtaPtMinCandSelector",
    src = cms.InputTag("towersAboveThreshold"),
    ptMin   = cms.double(0),
    etaMin = cms.double(3.0),
    etaMax = cms.double(6.0)
)

process.hfNegTowers = cms.EDFilter("EtaPtMinCandSelector",
    src = cms.InputTag("towersAboveThreshold"),
    ptMin   = cms.double(0),
    etaMin = cms.double(-6.0),
    etaMax = cms.double(-3.0)
)


process.hfPosFilter = cms.EDFilter("CandCountFilter",
    src = cms.InputTag("hfPosTowers"),
    minNumber = cms.uint32(3)
)

process.hfNegFilter = cms.EDFilter("CandCountFilter",
    src = cms.InputTag("hfNegTowers"),
    minNumber = cms.uint32(3)
)


process.hfCoincFilter = cms.Sequence(
    process.towersAboveThreshold *
    process.hfPosTowers *
    process.hfNegTowers *
    process.hfPosFilter *
    process.hfNegFilter)


process.filter_step = cms.Path(process.hfCoincFilter
                               #process.bscAND
                               #process.bscHighMult
                               #process.hltBSCHighMult
)

# Path and EndPath definitions

process.RECOoutput_step = cms.EndPath(process.RECOoutput)


# Schedule definition
process.schedule = cms.Schedule(process.filter_step,
                                process.RECOoutput_step)
