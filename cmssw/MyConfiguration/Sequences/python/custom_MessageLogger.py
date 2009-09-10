#
# custom message logger
#
# - Top Tips
#   * https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMessageLogger
#     + http://www.uscms.org/LPC/lpc_offl/MessageLogger/parameters.html
# - cf
#   * http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/FWCore/MessageLogger/examples/timingFilteredMLexample_cfg.py?view=log
#   * http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/HLTrigger/Configuration/python/customL1THLT_Options.py?view=markup
#   * https://hypernews.cern.ch/HyperNews/CMS/get/swDevelopment/1836/1.html
#   * https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMessageLogger
# 

def custom_messagelogger(process):
   #process.load("FWCore.MessageService.MessageLogger_cfi")
   process.MessageLogger.categories.append('GenParticleProducer')
   process.MessageLogger.categories.append('InputGenJetsParticleSelector')
   process.MessageLogger.categories.append('SubEventGenJetProducer')

   process.MessageLogger.debugModules = ['iterativeCone5HiGenJets']

   process.MessageLogger.cout = cms.untracked.PSet(
      threshold = cms.untracked.string('DEBUG'),
      default = cms.untracked.PSet(
	 limit = cms.untracked.int32(0)
	 ),
      GenParticleProducer = cms.untracked.PSet(
	 limit = cms.untracked.int32(-1)
	 ),
      InputGenJetsParticleSelector = cms.untracked.PSet(
	 limit = cms.untracked.int32(-1)
	 ),
      SubEventGenJetProducer = cms.untracked.PSet(
	 limit = cms.untracked.int32(-1)
	 )
      )

   process.MessageLogger.cerr.threshold = 'DEBUG'
   return(process)

custom_messagelogger(process)
