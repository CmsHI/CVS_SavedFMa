# Auto generated configuration file
# using: 
# Revision: 1.341 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: hiReco -s RAW2DIGI,RECO --scenario HeavyIons --conditions auto:hltonline --datatier GEN-SIM-FEVTDEBUGHLT --eventcontent FEVTDEBUGHLT --geometry DB --filein=file:/net/hisrv0001/home/davidlw/scratch1/HLTStudies/CMSSW_4_4_0_NewZS/src/RECO_highptInNewNotOld_NewZS_noBS.root --data --processName=RECOZSHEAD --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('RECOZSHEAD')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContentHeavyIons_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('file:/net/hisrv0001/home/davidlw/scratch1/HLTStudies/CMSSW_4_4_0_NewZS/src/RECO_highptInNewNotOld_NewZS_noBS.root')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.4 $'),
    annotation = cms.untracked.string('hiReco nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    #fileName = cms.untracked.string('hiReco_RAW2DIGI_RECO_ZSHEAD10_07_1148am_ALLVR_GTV4_hi2010Pedestal.root'),
    fileName = cms.untracked.string('hiReco_RAW2DIGI_RECO_ZSHEAD10_07_1148am_ALLVR_DistrotionThresh0MeanCM158GTV4_hi2010Pedestal.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-FEVTDEBUGHLT')
    )
)
process.FEVTDEBUGHLToutput.outputCommands.append("keep *_siStripZeroSuppression_*_*")
process.FEVTDEBUGHLToutput.outputCommands.append("keep *_siStripDigis_*_*")

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'GR_R_44_V4::All'

# ZS Settings
process.siStripZeroSuppression.produceRawDigis = cms.bool(True)
process.siStripZeroSuppression.produceCalculatedBaseline = cms.bool(True)
process.siStripZeroSuppression.produceBaselinePoints = cms.bool(True)
process.siStripZeroSuppression.storeInZScollBadAPV = cms.bool(True)
process.siStripZeroSuppression.Algorithms.DeltaCMThreshold = cms.uint32(0)       # for BaselineFollower inspect
process.siStripZeroSuppression.Algorithms.distortionThreshold = cms.uint32(0)
process.siStripZeroSuppression.Algorithms.MeanCM = cms.int32(158)

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstructionHeavyIons)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.FEVTDEBUGHLToutput_step)

def customiseSiStripConditions(process):
    process.stripConditions = cms.ESSource("PoolDBESSource",
      process.CondDBSetup,
      timetype = cms.untracked.string('runnumber'),
      connect = cms.string('frontier://FrontierProd/CMS_COND_31X_STRIP'),
      toGet = cms.VPSet(
        cms.PSet(
          record = cms.string('SiStripNoisesRcd'),
          tag = cms.string('SiStripNoise_GR10_v1_hlt')
          ),
          cms.PSet(
          record = cms.string('SiStripPedestalsRcd'),
          tag = cms.string('SiStripPedestals_GR10_v1_hlt')
          ),
          cms.PSet(
          record = cms.string('SiStripFedCablingRcd'),
          tag = cms.string('SiStripFedCabling_GR10_v1_hlt')
          ),
          cms.PSet(
          record = cms.string('SiStripBadChannelRcd'),
          tag = cms.string('SiStripBadChannel_FromOnline_GR10_v1_hlt')
          ),
          cms.PSet(
          record = cms.string('SiStripLatencyRcd'),
          tag = cms.string('SiStripLatency_GR10_v2_hlt')
          ),
          cms.PSet(
          record = cms.string('SiStripThresholdRcd'),
          tag = cms.string('SiStripThreshold_GR10_v1_hlt')
          ),
          cms.PSet(
          record = cms.string('SiStripBadFiberRcd'),
          tag = cms.string('SiStripBadChannel_FromOfflineCalibration_GR10_v2_offline')
          ),
          cms.PSet(
          record = cms.string('SiStripBadModuleRcd'),
          tag = cms.string('SiStripBadChannel_FromEfficiencyAnalysis_GR10_v1_offline')
          ),
          cms.PSet(
          record = cms.string('SiStripConfObjectRcd'),
          tag = cms.string('SiStripShiftAndCrosstalk_GR10_v1_offline')
          ),
          cms.PSet(
          record = cms.string('SiStripLorentzAngleRcd'),
          tag = cms.string('SiStripLorentzAngle_GR10_v1_offline')
          ),
          cms.PSet(
          record = cms.string('SiStripApvGain2Rcd'),
          tag = cms.string('SiStripApvGain_FromParticles_GR10_v2_offline')
          ),
          cms.PSet(
          record = cms.string('SiStripApvGainRcd'),
          tag = cms.string('SiStripApvGain_GR10_v1_hlt')
          )
        )
     )    
    process.es_prefer_strips = cms.ESPrefer("PoolDBESSource","stripConditions")

customiseSiStripConditions(process)