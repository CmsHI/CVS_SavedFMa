import FWCore.ParameterSet.Config as cms

hltbitanalysis = cms.EDAnalyzer("HLTBitAnalyzer",
    ### Trigger objects
    l1GctHFBitCounts                = cms.InputTag("hltGctDigis"),
    l1GctHFRingSums                 = cms.InputTag("hltGctDigis"),
    l1GtObjectMapRecord             = cms.InputTag("hltL1GtObjectMap::HLT"),
    l1GtReadoutRecord               = cms.InputTag("gtDigis"),

    l1extramc                       = cms.string('hltL1extraParticles'),
    l1extramu                       = cms.string('hltL1extraParticles'),
    hltresults                      = cms.InputTag("TriggerResults::HLT"),

    ### Run parameters
    RunParameters = cms.PSet(
      HistogramFile = cms.untracked.string('hltbitanalysis.root'),
      Monte = cms.bool(False),
      Debug = cms.bool(False)
    )
)

# run on reco
analyzeHLT_step = cms.Path( hltbitanalysis )

# run from raw
from HLTrigger.HLTanalyzers.HLT_Startup09_data_cff import *
DQM = cms.Service( "DQM",)
DQMStore = cms.Service( "DQMStore",)
analyzeHLT_fromRAW_step = cms.Path(HLTBeginSequence + hltbitanalysis )

# difference source cases
def update_cfg_mc(process,isMC):
  if (isMC):  # replace all instances of "source" with "rawDataCollector" in InputTags
      from FWCore.ParameterSet import Mixins
      for module in process.__dict__.itervalues():
          if isinstance(module, Mixins._Parameterizable):
              for parameter in module.__dict__.itervalues():
                  if isinstance(parameter, cms.InputTag):
                      if parameter.moduleLabel == 'source':
                          parameter.moduleLabel = 'rawDataCollector'
  return(process)

