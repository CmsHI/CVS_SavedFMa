
import FWCore.ParameterSet.Config as cms

process = cms.Process("DumpDTRaw")

# the source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    #'file:/data/c/cerminar/data/GlobalRun/run61642_BeamSplash/Run61642_EventNumberSkim_RAW.root'
    
    'file:/export/04a/frankma/data/217/raw_skim/minbiasSummer08_IDEAL_V9_v1_Raw1k.root'
    #'file:/export/04a/frankma/data/2011/official/officialMB_PureRaw_8k.root'
    ),
                            skipEvents = cms.untracked.uint32(0) )


# process.source = cms.Source("NewEventStreamFileReader",
#                             fileNames = cms.untracked.vstring(
#     'file:/directory/pippo.dat'
#     ))

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

process.MessageLogger = cms.Service("MessageLogger",
                                    destinations = cms.untracked.vstring('cout'),
                                    cout = cms.untracked.PSet(threshold = cms.untracked.string('INFO'))
                                    )

process.dump = cms.EDAnalyzer("DumpFEDRawDataProduct",
	                      #feds = cms.untracked.vint32(770, 771, 772, 773, 774, 775),
                              feds = cms.untracked.vint32(812,813),
                              dumpPayload = cms.untracked.bool(True)
                              )

# Emilio added analyzer to see event # readout of Gt FED's
process.ana = cms.EDAnalyzer("GlobalNumbersAnalysis"
			     )

process.dtRawDump = cms.Path(process.ana)
