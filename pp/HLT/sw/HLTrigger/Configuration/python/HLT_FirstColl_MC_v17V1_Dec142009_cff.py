# /cdaq/physics/firstCollisions09/v1.7/HLT/V1 (CMSSW_3_3_3_HLT8)

import FWCore.ParameterSet.Config as cms


HLTConfigVersion = cms.PSet(
  tableName = cms.string('/cdaq/physics/firstCollisions09/v1.7/HLT/V1')
)


BTagRecord = cms.ESSource( "EmptyESSource",
  recordName = cms.string( "JetTagComputerRecord" ),
  iovIsRunNotTime = cms.bool( True ),
  appendToDataLabel = cms.string( "" ),
  firstValid = cms.vuint32( 1 )
)
L2RelativeCorrectionService = cms.ESSource( "L2RelativeCorrectionService",
  appendToDataLabel = cms.string( "" ),
  tagName = cms.string( "Summer09_7TeV_L2Relative_IC5Calo" ),
  label = cms.string( "L2RelativeJetCorrector" )
)
L3AbsoluteCorrectionService = cms.ESSource( "L3AbsoluteCorrectionService",
  appendToDataLabel = cms.string( "" ),
  tagName = cms.string( "Summer09_7TeV_L3Absolute_IC5Calo" ),
  label = cms.string( "L3AbsoluteJetCorrector" )
)
MCJetCorrectorIcone5 = cms.ESSource( "JetCorrectionServiceChain",
  label = cms.string( "MCJetCorrectorIcone5" ),
  appendToDataLabel = cms.string( "" ),
  correctors = cms.vstring( 'L2RelativeJetCorrector',
    'L3AbsoluteJetCorrector' )
)
MCJetCorrectorIcone5HF07 = cms.ESSource( "L2RelativeCorrectionService",
  appendToDataLabel = cms.string( "" ),
  tagName = cms.string( "HLT_L2Relative" ),
  label = cms.string( "MCJetCorrectorIcone5HF07" )
)
MCJetCorrectorIcone5Unit = cms.ESSource( "L2RelativeCorrectionService",
  appendToDataLabel = cms.string( "" ),
  tagName = cms.string( "HLT_L2RelativeFlat" ),
  label = cms.string( "MCJetCorrectorIcone5Unit" )
)
essourceSev = cms.ESSource( "EmptyESSource",
  recordName = cms.string( "HcalSeverityLevelComputerRcd" ),
  iovIsRunNotTime = cms.bool( True ),
  appendToDataLabel = cms.string( "" ),
  firstValid = cms.vuint32( 1 )
)

AnalyticalPropagator = cms.ESProducer( "AnalyticalPropagatorESProducer",
  ComponentName = cms.string( "AnalyticalPropagator" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  MaxDPhi = cms.double( 1.6 ),
  appendToDataLabel = cms.string( "" )
)
AnyDirectionAnalyticalPropagator = cms.ESProducer( "AnalyticalPropagatorESProducer",
  ComponentName = cms.string( "AnyDirectionAnalyticalPropagator" ),
  PropagationDirection = cms.string( "anyDirection" ),
  MaxDPhi = cms.double( 1.6 ),
  appendToDataLabel = cms.string( "" )
)
CaloTopologyBuilder = cms.ESProducer( "CaloTopologyBuilder",
  appendToDataLabel = cms.string( "" )
)
CaloTowerConstituentsMapBuilder = cms.ESProducer( "CaloTowerConstituentsMapBuilder",
  MapFile = cms.untracked.string( "Geometry/CaloTopology/data/CaloTowerEEGeometric.map.gz" ),
  appendToDataLabel = cms.string( "" )
)
Chi2EstimatorForRefit = cms.ESProducer( "Chi2MeasurementEstimatorESProducer",
  ComponentName = cms.string( "Chi2EstimatorForRefit" ),
  MaxChi2 = cms.double( 100000.0 ),
  nSigma = cms.double( 3.0 ),
  appendToDataLabel = cms.string( "" )
)
Chi2MeasurementEstimator = cms.ESProducer( "Chi2MeasurementEstimatorESProducer",
  ComponentName = cms.string( "Chi2" ),
  MaxChi2 = cms.double( 30.0 ),
  nSigma = cms.double( 3.0 ),
  appendToDataLabel = cms.string( "" )
)
CkfTrajectoryBuilder = cms.ESProducer( "CkfTrajectoryBuilderESProducer",
  ComponentName = cms.string( "CkfTrajectoryBuilder" ),
  updator = cms.string( "KFUpdator" ),
  propagatorAlong = cms.string( "PropagatorWithMaterial" ),
  propagatorOpposite = cms.string( "PropagatorWithMaterialOpposite" ),
  estimator = cms.string( "Chi2" ),
  TTRHBuilder = cms.string( "WithTrackAngle" ),
  MeasurementTrackerName = cms.string( "" ),
  trajectoryFilterName = cms.string( "ckfBaseTrajectoryFilter" ),
  maxCand = cms.int32( 5 ),
  lostHitPenalty = cms.double( 30.0 ),
  intermediateCleaning = cms.bool( True ),
  alwaysUseInvalidHits = cms.bool( True ),
  appendToDataLabel = cms.string( "" )
)
DummyDetLayerGeometry = cms.ESProducer( "DetLayerGeometryESProducer",
  ComponentName = cms.string( "DummyDetLayerGeometry" ),
  appendToDataLabel = cms.string( "" )
)
ESUnpackerWorkerESProducer = cms.ESProducer( "ESUnpackerWorkerESProducer",
  ComponentName = cms.string( "esRawToRecHit" ),
  appendToDataLabel = cms.string( "" ),
  DCCDataUnpacker = cms.PSet(  LookupTable = cms.FileInPath( "EventFilter/ESDigiToRaw/data/ES_lookup_table.dat" ) ),
  RHAlgo = cms.PSet( 
    Type = cms.string( "ESRecHitWorker" ),
    ESGain = cms.int32( 1 ),
    ESMIPkeV = cms.double( 81.08 ),
    ESMIPADC = cms.double( 9.0 ),
    ESBaseline = cms.int32( 1000 )
  )
)
EcalRegionCablingESProducer = cms.ESProducer( "EcalRegionCablingESProducer",
  appendToDataLabel = cms.string( "" ),
  esMapping = cms.PSet(  LookupTable = cms.FileInPath( "EventFilter/ESDigiToRaw/data/ES_lookup_table.dat" ) )
)
EcalTrigTowerConstituentsMapBuilder = cms.ESProducer( "EcalTrigTowerConstituentsMapBuilder",
  MapFile = cms.untracked.string( "Geometry/EcalMapping/data/EndCap_TTMap.txt" ),
  appendToDataLabel = cms.string( "" )
)
EcalUnpackerWorkerESProducer = cms.ESProducer( "EcalUnpackerWorkerESProducer",
  ComponentName = cms.string( "" ),
  appendToDataLabel = cms.string( "" ),
  DCCDataUnpacker = cms.PSet( 
    orderedDCCIdList = cms.vint32( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54 ),
    tccUnpacking = cms.bool( True ),
    srpUnpacking = cms.bool( False ),
    syncCheck = cms.bool( False ),
    feIdCheck = cms.bool( True ),
    headerUnpacking = cms.bool( False ),
    orderedFedList = cms.vint32( 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611, 612, 613, 614, 615, 616, 617, 618, 619, 620, 621, 622, 623, 624, 625, 626, 627, 628, 629, 630, 631, 632, 633, 634, 635, 636, 637, 638, 639, 640, 641, 642, 643, 644, 645, 646, 647, 648, 649, 650, 651, 652, 653, 654 ),
    feUnpacking = cms.bool( True ),
    forceKeepFRData = cms.bool( False ),
    memUnpacking = cms.bool( False )
  ),
  ElectronicsMapper = cms.PSet( 
    numbXtalTSamples = cms.uint32( 10 ),
    numbTriggerTSamples = cms.uint32( 1 )
  ),
  UncalibRHAlgo = cms.PSet(  Type = cms.string( "EcalUncalibRecHitWorkerWeights" ) ),
  CalibRHAlgo = cms.PSet( 
    flagsMapDBReco = cms.vint32( 0, 0, 0, 0, 4, -1, -1, -1, 4, 4, 6, 6, 6, 7, 8 ),
    Type = cms.string( "EcalRecHitWorkerSimple" ),
    killDeadChannels = cms.bool( True ),
    ChannelStatusToBeExcluded = cms.vint32( 10, 11, 12, 13, 14, 78, 142 ),
    laserCorrection = cms.bool( False )
  )
)
FastSteppingHelixPropagatorAny = cms.ESProducer( "SteppingHelixPropagatorESProducer",
  ComponentName = cms.string( "FastSteppingHelixPropagatorAny" ),
  PropagationDirection = cms.string( "anyDirection" ),
  useInTeslaFromMagField = cms.bool( False ),
  SetVBFPointer = cms.bool( False ),
  useMagVolumes = cms.bool( True ),
  VBFName = cms.string( "VolumeBasedMagneticField" ),
  ApplyRadX0Correction = cms.bool( True ),
  AssumeNoMaterial = cms.bool( False ),
  NoErrorPropagation = cms.bool( False ),
  debug = cms.bool( False ),
  useMatVolumes = cms.bool( True ),
  useIsYokeFlag = cms.bool( True ),
  returnTangentPlane = cms.bool( True ),
  sendLogWarning = cms.bool( False ),
  useTuningForL2Speed = cms.bool( True ),
  useEndcapShiftsInZ = cms.bool( False ),
  endcapShiftInZPos = cms.double( 0.0 ),
  endcapShiftInZNeg = cms.double( 0.0 ),
  appendToDataLabel = cms.string( "" )
)
FastSteppingHelixPropagatorOpposite = cms.ESProducer( "SteppingHelixPropagatorESProducer",
  ComponentName = cms.string( "FastSteppingHelixPropagatorOpposite" ),
  PropagationDirection = cms.string( "oppositeToMomentum" ),
  useInTeslaFromMagField = cms.bool( False ),
  SetVBFPointer = cms.bool( False ),
  useMagVolumes = cms.bool( True ),
  VBFName = cms.string( "VolumeBasedMagneticField" ),
  ApplyRadX0Correction = cms.bool( True ),
  AssumeNoMaterial = cms.bool( False ),
  NoErrorPropagation = cms.bool( False ),
  debug = cms.bool( False ),
  useMatVolumes = cms.bool( True ),
  useIsYokeFlag = cms.bool( True ),
  returnTangentPlane = cms.bool( True ),
  sendLogWarning = cms.bool( False ),
  useTuningForL2Speed = cms.bool( True ),
  useEndcapShiftsInZ = cms.bool( False ),
  endcapShiftInZPos = cms.double( 0.0 ),
  endcapShiftInZNeg = cms.double( 0.0 ),
  appendToDataLabel = cms.string( "" )
)
FitterRK = cms.ESProducer( "KFTrajectoryFitterESProducer",
  ComponentName = cms.string( "FitterRK" ),
  Propagator = cms.string( "RungeKuttaTrackerPropagator" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2" ),
  RecoGeometry = cms.string( "DummyDetLayerGeometry" ),
  minHits = cms.int32( 3 ),
  appendToDataLabel = cms.string( "" )
)
FittingSmootherRK = cms.ESProducer( "KFFittingSmootherESProducer",
  ComponentName = cms.string( "FittingSmootherRK" ),
  Fitter = cms.string( "FitterRK" ),
  Smoother = cms.string( "SmootherRK" ),
  EstimateCut = cms.double( -1.0 ),
  MinNumberOfHits = cms.int32( 5 ),
  RejectTracks = cms.bool( True ),
  BreakTrajWith2ConsecutiveMissing = cms.bool( False ),
  NoInvalidHitsBeginEnd = cms.bool( False ),
  appendToDataLabel = cms.string( "" )
)
GlobalTrackingGeometryESProducer = cms.ESProducer( "GlobalTrackingGeometryESProducer",
  appendToDataLabel = cms.string( "" )
)
HITTRHBuilderWithoutRefit = cms.ESProducer( "TkTransientTrackingRecHitBuilderESProducer",
  ComponentName = cms.string( "HITTRHBuilderWithoutRefit" ),
  StripCPE = cms.string( "Fake" ),
  PixelCPE = cms.string( "Fake" ),
  Matcher = cms.string( "Fake" ),
  ComputeCoarseLocalPositionFromDisk = cms.bool( False ),
  appendToDataLabel = cms.string( "" )
)
KFFitterForRefitInsideOut = cms.ESProducer( "KFTrajectoryFitterESProducer",
  ComponentName = cms.string( "KFFitterForRefitInsideOut" ),
  Propagator = cms.string( "SmartPropagatorAny" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2EstimatorForRefit" ),
  RecoGeometry = cms.string( "DummyDetLayerGeometry" ),
  minHits = cms.int32( 3 ),
  appendToDataLabel = cms.string( "" )
)
KFFitterSmootherForL2Muon = cms.ESProducer( "KFFittingSmootherESProducer",
  ComponentName = cms.string( "KFFitterSmootherForL2Muon" ),
  Fitter = cms.string( "KFTrajectoryFitterForL2Muon" ),
  Smoother = cms.string( "KFTrajectorySmootherForL2Muon" ),
  EstimateCut = cms.double( -1.0 ),
  MinNumberOfHits = cms.int32( 5 ),
  RejectTracks = cms.bool( True ),
  BreakTrajWith2ConsecutiveMissing = cms.bool( False ),
  NoInvalidHitsBeginEnd = cms.bool( False ),
  appendToDataLabel = cms.string( "" )
)
KFSmootherForMuonTrackLoader = cms.ESProducer( "KFTrajectorySmootherESProducer",
  ComponentName = cms.string( "KFSmootherForMuonTrackLoader" ),
  Propagator = cms.string( "SmartPropagatorAnyOpposite" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2" ),
  RecoGeometry = cms.string( "DummyDetLayerGeometry" ),
  errorRescaling = cms.double( 10.0 ),
  minHits = cms.int32( 3 ),
  appendToDataLabel = cms.string( "" )
)
KFSmootherForRefitInsideOut = cms.ESProducer( "KFTrajectorySmootherESProducer",
  ComponentName = cms.string( "KFSmootherForRefitInsideOut" ),
  Propagator = cms.string( "SmartPropagatorAnyOpposite" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2EstimatorForRefit" ),
  RecoGeometry = cms.string( "DummyDetLayerGeometry" ),
  errorRescaling = cms.double( 100.0 ),
  minHits = cms.int32( 3 ),
  appendToDataLabel = cms.string( "" )
)
KFTrajectoryFitterForL2Muon = cms.ESProducer( "KFTrajectoryFitterESProducer",
  ComponentName = cms.string( "KFTrajectoryFitterForL2Muon" ),
  Propagator = cms.string( "FastSteppingHelixPropagatorAny" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2" ),
  RecoGeometry = cms.string( "DummyDetLayerGeometry" ),
  minHits = cms.int32( 3 ),
  appendToDataLabel = cms.string( "" )
)
KFTrajectorySmootherForL2Muon = cms.ESProducer( "KFTrajectorySmootherESProducer",
  ComponentName = cms.string( "KFTrajectorySmootherForL2Muon" ),
  Propagator = cms.string( "FastSteppingHelixPropagatorOpposite" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2" ),
  RecoGeometry = cms.string( "DummyDetLayerGeometry" ),
  errorRescaling = cms.double( 100.0 ),
  minHits = cms.int32( 3 ),
  appendToDataLabel = cms.string( "" )
)
KFUpdatorESProducer = cms.ESProducer( "KFUpdatorESProducer",
  ComponentName = cms.string( "KFUpdator" ),
  appendToDataLabel = cms.string( "" )
)
L3MuKFFitter = cms.ESProducer( "KFTrajectoryFitterESProducer",
  ComponentName = cms.string( "L3MuKFFitter" ),
  Propagator = cms.string( "SmartPropagatorAny" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2" ),
  RecoGeometry = cms.string( "DummyDetLayerGeometry" ),
  minHits = cms.int32( 3 ),
  appendToDataLabel = cms.string( "" )
)
MaterialPropagator = cms.ESProducer( "PropagatorWithMaterialESProducer",
  ComponentName = cms.string( "PropagatorWithMaterial" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  Mass = cms.double( 0.105 ),
  MaxDPhi = cms.double( 1.6 ),
  useRungeKutta = cms.bool( False ),
  ptMin = cms.double( -1.0 ),
  appendToDataLabel = cms.string( "" )
)
MeasurementTracker = cms.ESProducer( "MeasurementTrackerESProducer",
  ComponentName = cms.string( "" ),
  PixelCPE = cms.string( "PixelCPEGeneric" ),
  StripCPE = cms.string( "StripCPEfromTrackAngle" ),
  HitMatcher = cms.string( "StandardMatcher" ),
  Regional = cms.bool( True ),
  OnDemand = cms.bool( True ),
  UsePixelModuleQualityDB = cms.bool( True ),
  DebugPixelModuleQualityDB = cms.untracked.bool( False ),
  UsePixelROCQualityDB = cms.bool( True ),
  DebugPixelROCQualityDB = cms.untracked.bool( False ),
  UseStripModuleQualityDB = cms.bool( True ),
  DebugStripModuleQualityDB = cms.untracked.bool( False ),
  UseStripAPVFiberQualityDB = cms.bool( True ),
  DebugStripAPVFiberQualityDB = cms.untracked.bool( False ),
  MaskBadAPVFibers = cms.bool( True ),
  UseStripStripQualityDB = cms.bool( True ),
  DebugStripStripQualityDB = cms.untracked.bool( False ),
  switchOffPixelsIfEmpty = cms.bool( True ),
  pixelClusterProducer = cms.string( "hltSiPixelClusters" ),
  stripClusterProducer = cms.string( "hltSiStripClusters" ),
  stripLazyGetterProducer = cms.string( "hltSiStripRawToClustersFacility" ),
  appendToDataLabel = cms.string( "" ),
  inactivePixelDetectorLabels = cms.VInputTag(  ),
  inactiveStripDetectorLabels = cms.VInputTag(  )
)
MuonCkfTrajectoryBuilder = cms.ESProducer( "MuonCkfTrajectoryBuilderESProducer",
  ComponentName = cms.string( "muonCkfTrajectoryBuilder" ),
  updator = cms.string( "KFUpdator" ),
  propagatorAlong = cms.string( "PropagatorWithMaterial" ),
  propagatorOpposite = cms.string( "PropagatorWithMaterialOpposite" ),
  propagatorProximity = cms.string( "SteppingHelixPropagatorAny" ),
  estimator = cms.string( "Chi2" ),
  TTRHBuilder = cms.string( "WithTrackAngle" ),
  MeasurementTrackerName = cms.string( "" ),
  trajectoryFilterName = cms.string( "muonCkfTrajectoryFilter" ),
  useSeedLayer = cms.bool( False ),
  rescaleErrorIfFail = cms.double( 1.0 ),
  deltaEta = cms.double( 0.1 ),
  deltaPhi = cms.double( 0.1 ),
  appendToDataLabel = cms.string( "" ),
  maxCand = cms.int32( 5 ),
  lostHitPenalty = cms.double( 30.0 ),
  intermediateCleaning = cms.bool( False ),
  alwaysUseInvalidHits = cms.bool( True )
)
MuonDetLayerGeometryESProducer = cms.ESProducer( "MuonDetLayerGeometryESProducer",
  appendToDataLabel = cms.string( "" )
)
MuonTransientTrackingRecHitBuilderESProducer = cms.ESProducer( "MuonTransientTrackingRecHitBuilderESProducer",
  ComponentName = cms.string( "MuonRecHitBuilder" ),
  appendToDataLabel = cms.string( "" )
)
OppositeMaterialPropagator = cms.ESProducer( "PropagatorWithMaterialESProducer",
  ComponentName = cms.string( "PropagatorWithMaterialOpposite" ),
  PropagationDirection = cms.string( "oppositeToMomentum" ),
  Mass = cms.double( 0.105 ),
  MaxDPhi = cms.double( 1.6 ),
  useRungeKutta = cms.bool( False ),
  ptMin = cms.double( -1.0 ),
  appendToDataLabel = cms.string( "" )
)
PixelCPEGenericESProducer = cms.ESProducer( "PixelCPEGenericESProducer",
  ComponentName = cms.string( "PixelCPEGeneric" ),
  eff_charge_cut_lowX = cms.double( 0.0 ),
  eff_charge_cut_lowY = cms.double( 0.0 ),
  eff_charge_cut_highX = cms.double( 1.0 ),
  eff_charge_cut_highY = cms.double( 1.0 ),
  size_cutX = cms.double( 3.0 ),
  size_cutY = cms.double( 3.0 ),
  EdgeClusterErrorX = cms.double( 50.0 ),
  EdgeClusterErrorY = cms.double( 85.0 ),
  inflate_errors = cms.bool( False ),
  inflate_all_errors_no_trk_angle = cms.bool( False ),
  UseErrorsFromTemplates = cms.bool( True ),
  TruncatePixelCharge = cms.bool( True ),
  IrradiationBiasCorrection = cms.bool( False ),
  DoCosmics = cms.bool( False ),
  LoadTemplatesFromDB = cms.bool( True ),
  appendToDataLabel = cms.string( "" ),
  TanLorentzAnglePerTesla = cms.double( 0.106 ),
  PixelErrorParametrization = cms.string( "NOTcmsim" ),
  Alpha2Order = cms.bool( True ),
  ClusterProbComputationFlag = cms.int32( 0 )
)
RungeKuttaTrackerPropagator = cms.ESProducer( "PropagatorWithMaterialESProducer",
  ComponentName = cms.string( "RungeKuttaTrackerPropagator" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  Mass = cms.double( 0.105 ),
  MaxDPhi = cms.double( 1.6 ),
  useRungeKutta = cms.bool( True ),
  ptMin = cms.double( -1.0 ),
  appendToDataLabel = cms.string( "" )
)
SiStripRegionConnectivity = cms.ESProducer( "SiStripRegionConnectivity",
  EtaDivisions = cms.untracked.uint32( 20 ),
  PhiDivisions = cms.untracked.uint32( 20 ),
  EtaMax = cms.untracked.double( 2.5 )
)
SmartPropagator = cms.ESProducer( "SmartPropagatorESProducer",
  ComponentName = cms.string( "SmartPropagator" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  Epsilon = cms.double( 5.0 ),
  TrackerPropagator = cms.string( "PropagatorWithMaterial" ),
  MuonPropagator = cms.string( "SteppingHelixPropagatorAlong" ),
  appendToDataLabel = cms.string( "" )
)
SmartPropagatorAny = cms.ESProducer( "SmartPropagatorESProducer",
  ComponentName = cms.string( "SmartPropagatorAny" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  Epsilon = cms.double( 5.0 ),
  TrackerPropagator = cms.string( "PropagatorWithMaterial" ),
  MuonPropagator = cms.string( "SteppingHelixPropagatorAny" ),
  appendToDataLabel = cms.string( "" )
)
SmartPropagatorAnyOpposite = cms.ESProducer( "SmartPropagatorESProducer",
  ComponentName = cms.string( "SmartPropagatorAnyOpposite" ),
  PropagationDirection = cms.string( "oppositeToMomentum" ),
  Epsilon = cms.double( 5.0 ),
  TrackerPropagator = cms.string( "PropagatorWithMaterialOpposite" ),
  MuonPropagator = cms.string( "SteppingHelixPropagatorAny" ),
  appendToDataLabel = cms.string( "" )
)
SmartPropagatorOpposite = cms.ESProducer( "SmartPropagatorESProducer",
  ComponentName = cms.string( "SmartPropagatorOpposite" ),
  PropagationDirection = cms.string( "oppositeToMomentum" ),
  Epsilon = cms.double( 5.0 ),
  TrackerPropagator = cms.string( "PropagatorWithMaterialOpposite" ),
  MuonPropagator = cms.string( "SteppingHelixPropagatorOpposite" ),
  appendToDataLabel = cms.string( "" )
)
SmootherRK = cms.ESProducer( "KFTrajectorySmootherESProducer",
  ComponentName = cms.string( "SmootherRK" ),
  Propagator = cms.string( "RungeKuttaTrackerPropagator" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2" ),
  RecoGeometry = cms.string( "DummyDetLayerGeometry" ),
  errorRescaling = cms.double( 100.0 ),
  minHits = cms.int32( 3 ),
  appendToDataLabel = cms.string( "" )
)
SteppingHelixPropagatorAlong = cms.ESProducer( "SteppingHelixPropagatorESProducer",
  ComponentName = cms.string( "SteppingHelixPropagatorAlong" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  useInTeslaFromMagField = cms.bool( False ),
  SetVBFPointer = cms.bool( False ),
  useMagVolumes = cms.bool( True ),
  VBFName = cms.string( "VolumeBasedMagneticField" ),
  ApplyRadX0Correction = cms.bool( True ),
  AssumeNoMaterial = cms.bool( False ),
  NoErrorPropagation = cms.bool( False ),
  debug = cms.bool( False ),
  useMatVolumes = cms.bool( True ),
  useIsYokeFlag = cms.bool( True ),
  returnTangentPlane = cms.bool( True ),
  sendLogWarning = cms.bool( False ),
  useTuningForL2Speed = cms.bool( False ),
  useEndcapShiftsInZ = cms.bool( False ),
  endcapShiftInZPos = cms.double( 0.0 ),
  endcapShiftInZNeg = cms.double( 0.0 ),
  appendToDataLabel = cms.string( "" )
)
SteppingHelixPropagatorAny = cms.ESProducer( "SteppingHelixPropagatorESProducer",
  ComponentName = cms.string( "SteppingHelixPropagatorAny" ),
  PropagationDirection = cms.string( "anyDirection" ),
  useInTeslaFromMagField = cms.bool( False ),
  SetVBFPointer = cms.bool( False ),
  useMagVolumes = cms.bool( True ),
  VBFName = cms.string( "VolumeBasedMagneticField" ),
  ApplyRadX0Correction = cms.bool( True ),
  AssumeNoMaterial = cms.bool( False ),
  NoErrorPropagation = cms.bool( False ),
  debug = cms.bool( False ),
  useMatVolumes = cms.bool( True ),
  useIsYokeFlag = cms.bool( True ),
  returnTangentPlane = cms.bool( True ),
  sendLogWarning = cms.bool( False ),
  useTuningForL2Speed = cms.bool( False ),
  useEndcapShiftsInZ = cms.bool( False ),
  endcapShiftInZPos = cms.double( 0.0 ),
  endcapShiftInZNeg = cms.double( 0.0 ),
  appendToDataLabel = cms.string( "" )
)
SteppingHelixPropagatorOpposite = cms.ESProducer( "SteppingHelixPropagatorESProducer",
  ComponentName = cms.string( "SteppingHelixPropagatorOpposite" ),
  PropagationDirection = cms.string( "oppositeToMomentum" ),
  useInTeslaFromMagField = cms.bool( False ),
  SetVBFPointer = cms.bool( False ),
  useMagVolumes = cms.bool( True ),
  VBFName = cms.string( "VolumeBasedMagneticField" ),
  ApplyRadX0Correction = cms.bool( True ),
  AssumeNoMaterial = cms.bool( False ),
  NoErrorPropagation = cms.bool( False ),
  debug = cms.bool( False ),
  useMatVolumes = cms.bool( True ),
  useIsYokeFlag = cms.bool( True ),
  returnTangentPlane = cms.bool( True ),
  sendLogWarning = cms.bool( False ),
  useTuningForL2Speed = cms.bool( False ),
  useEndcapShiftsInZ = cms.bool( False ),
  endcapShiftInZPos = cms.double( 0.0 ),
  endcapShiftInZNeg = cms.double( 0.0 ),
  appendToDataLabel = cms.string( "" )
)
StraightLinePropagator = cms.ESProducer( "StraightLinePropagatorESProducer",
  ComponentName = cms.string( "StraightLinePropagator" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  appendToDataLabel = cms.string( "" )
)
TTRHBuilderPixelOnly = cms.ESProducer( "TkTransientTrackingRecHitBuilderESProducer",
  ComponentName = cms.string( "TTRHBuilderPixelOnly" ),
  StripCPE = cms.string( "Fake" ),
  PixelCPE = cms.string( "PixelCPEGeneric" ),
  Matcher = cms.string( "StandardMatcher" ),
  ComputeCoarseLocalPositionFromDisk = cms.bool( False ),
  appendToDataLabel = cms.string( "" )
)
TrackerRecoGeometryESProducer = cms.ESProducer( "TrackerRecoGeometryESProducer",
  appendToDataLabel = cms.string( "" )
)
TransientTrackBuilderESProducer = cms.ESProducer( "TransientTrackBuilderESProducer",
  ComponentName = cms.string( "TransientTrackBuilder" ),
  appendToDataLabel = cms.string( "" )
)
WithTrackAngle = cms.ESProducer( "TkTransientTrackingRecHitBuilderESProducer",
  ComponentName = cms.string( "WithTrackAngle" ),
  StripCPE = cms.string( "StripCPEfromTrackAngle" ),
  PixelCPE = cms.string( "PixelCPEGeneric" ),
  Matcher = cms.string( "StandardMatcher" ),
  ComputeCoarseLocalPositionFromDisk = cms.bool( False ),
  appendToDataLabel = cms.string( "" )
)
bJetRegionalTrajectoryBuilder = cms.ESProducer( "CkfTrajectoryBuilderESProducer",
  ComponentName = cms.string( "bJetRegionalTrajectoryBuilder" ),
  updator = cms.string( "KFUpdator" ),
  propagatorAlong = cms.string( "PropagatorWithMaterial" ),
  propagatorOpposite = cms.string( "PropagatorWithMaterialOpposite" ),
  estimator = cms.string( "Chi2" ),
  TTRHBuilder = cms.string( "WithTrackAngle" ),
  MeasurementTrackerName = cms.string( "" ),
  trajectoryFilterName = cms.string( "bJetRegionalTrajectoryFilter" ),
  maxCand = cms.int32( 1 ),
  lostHitPenalty = cms.double( 30.0 ),
  intermediateCleaning = cms.bool( True ),
  alwaysUseInvalidHits = cms.bool( False ),
  appendToDataLabel = cms.string( "" )
)
bJetRegionalTrajectoryFilter = cms.ESProducer( "TrajectoryFilterESProducer",
  ComponentName = cms.string( "bJetRegionalTrajectoryFilter" ),
  appendToDataLabel = cms.string( "" ),
  filterPset = cms.PSet( 
    minimumNumberOfHits = cms.int32( 5 ),
    minHitsMinPt = cms.int32( 3 ),
    ComponentType = cms.string( "CkfBaseTrajectoryFilter" ),
    maxLostHits = cms.int32( 1 ),
    maxNumberOfHits = cms.int32( 8 ),
    maxConsecLostHits = cms.int32( 1 ),
    chargeSignificance = cms.double( -1.0 ),
    nSigmaMinPt = cms.double( 5.0 ),
    minPt = cms.double( 1.0 )
  )
)
ckfBaseTrajectoryFilter = cms.ESProducer( "TrajectoryFilterESProducer",
  ComponentName = cms.string( "ckfBaseTrajectoryFilter" ),
  appendToDataLabel = cms.string( "" ),
  filterPset = cms.PSet( 
    minimumNumberOfHits = cms.int32( 5 ),
    minHitsMinPt = cms.int32( 3 ),
    ComponentType = cms.string( "CkfBaseTrajectoryFilter" ),
    maxLostHits = cms.int32( 1 ),
    maxNumberOfHits = cms.int32( -1 ),
    maxConsecLostHits = cms.int32( 1 ),
    chargeSignificance = cms.double( -1.0 ),
    nSigmaMinPt = cms.double( 5.0 ),
    minPt = cms.double( 0.9 )
  )
)
hcalRecAlgos = cms.ESProducer( "HcalRecAlgoESProducer",
  SeverityLevels = cms.VPSet( 
    cms.PSet(  RecHitFlags = cms.vstring(  ),
      ChannelStatus = cms.vstring(  ),
      Level = cms.int32( 0 )
    )
  ),
  RecoveredRecHitBits = cms.vstring(  ),
  appendToDataLabel = cms.string( "" ),
  DropChannelStatusBits = cms.vstring(  )
)
hltCkfTrajectoryBuilderMumu = cms.ESProducer( "CkfTrajectoryBuilderESProducer",
  ComponentName = cms.string( "hltCkfTrajectoryBuilderMumu" ),
  updator = cms.string( "KFUpdator" ),
  propagatorAlong = cms.string( "PropagatorWithMaterial" ),
  propagatorOpposite = cms.string( "PropagatorWithMaterialOpposite" ),
  estimator = cms.string( "Chi2" ),
  TTRHBuilder = cms.string( "WithTrackAngle" ),
  MeasurementTrackerName = cms.string( "" ),
  trajectoryFilterName = cms.string( "hltCkfTrajectoryFilterMumu" ),
  maxCand = cms.int32( 3 ),
  lostHitPenalty = cms.double( 30.0 ),
  intermediateCleaning = cms.bool( True ),
  alwaysUseInvalidHits = cms.bool( False ),
  appendToDataLabel = cms.string( "" )
)
hltCkfTrajectoryFilterMumu = cms.ESProducer( "TrajectoryFilterESProducer",
  ComponentName = cms.string( "hltCkfTrajectoryFilterMumu" ),
  appendToDataLabel = cms.string( "" ),
  filterPset = cms.PSet( 
    minimumNumberOfHits = cms.int32( 5 ),
    minHitsMinPt = cms.int32( 3 ),
    ComponentType = cms.string( "CkfBaseTrajectoryFilter" ),
    maxLostHits = cms.int32( 1 ),
    maxNumberOfHits = cms.int32( 5 ),
    maxConsecLostHits = cms.int32( 1 ),
    chargeSignificance = cms.double( -1.0 ),
    nSigmaMinPt = cms.double( 5.0 ),
    minPt = cms.double( 3.0 )
  )
)
hltKFFitter = cms.ESProducer( "KFTrajectoryFitterESProducer",
  ComponentName = cms.string( "hltKFFitter" ),
  Propagator = cms.string( "PropagatorWithMaterial" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2" ),
  RecoGeometry = cms.string( "DummyDetLayerGeometry" ),
  minHits = cms.int32( 3 ),
  appendToDataLabel = cms.string( "" )
)
hltKFFittingSmoother = cms.ESProducer( "KFFittingSmootherESProducer",
  ComponentName = cms.string( "hltKFFittingSmoother" ),
  Fitter = cms.string( "hltKFFitter" ),
  Smoother = cms.string( "hltKFSmoother" ),
  EstimateCut = cms.double( -1.0 ),
  MinNumberOfHits = cms.int32( 5 ),
  RejectTracks = cms.bool( True ),
  BreakTrajWith2ConsecutiveMissing = cms.bool( False ),
  NoInvalidHitsBeginEnd = cms.bool( False ),
  appendToDataLabel = cms.string( "" )
)
hltKFSmoother = cms.ESProducer( "KFTrajectorySmootherESProducer",
  ComponentName = cms.string( "hltKFSmoother" ),
  Propagator = cms.string( "PropagatorWithMaterial" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2" ),
  RecoGeometry = cms.string( "DummyDetLayerGeometry" ),
  errorRescaling = cms.double( 100.0 ),
  minHits = cms.int32( 3 ),
  appendToDataLabel = cms.string( "" )
)
mixedlayerpairs = cms.ESProducer( "SeedingLayersESProducer",
  appendToDataLabel = cms.string( "" ),
  ComponentName = cms.string( "MixedLayerPairs" ),
  layerList = cms.vstring( 'BPix1+BPix2',
    'BPix1+BPix3',
    'BPix2+BPix3',
    'BPix1+FPix1_pos',
    'BPix1+FPix1_neg',
    'BPix1+FPix2_pos',
    'BPix1+FPix2_neg',
    'BPix2+FPix1_pos',
    'BPix2+FPix1_neg',
    'BPix2+FPix2_pos',
    'BPix2+FPix2_neg',
    'FPix1_pos+FPix2_pos',
    'FPix1_neg+FPix2_neg',
    'FPix2_pos+TEC1_pos',
    'FPix2_pos+TEC2_pos',
    'TEC1_pos+TEC2_pos',
    'TEC2_pos+TEC3_pos',
    'FPix2_neg+TEC1_neg',
    'FPix2_neg+TEC2_neg',
    'TEC1_neg+TEC2_neg',
    'TEC2_neg+TEC3_neg' ),
  BPix = cms.PSet( 
    useErrorsFromParam = cms.bool( True ),
    hitErrorRPhi = cms.double( 0.0027 ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    hitErrorRZ = cms.double( 0.0060 )
  ),
  FPix = cms.PSet( 
    useErrorsFromParam = cms.bool( True ),
    hitErrorRPhi = cms.double( 0.0051 ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    hitErrorRZ = cms.double( 0.0036 )
  ),
  TEC = cms.PSet( 
    useRingSlector = cms.bool( True ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    minRing = cms.int32( 1 ),
    maxRing = cms.int32( 1 )
  )
)
muonCkfTrajectoryFilter = cms.ESProducer( "TrajectoryFilterESProducer",
  ComponentName = cms.string( "muonCkfTrajectoryFilter" ),
  appendToDataLabel = cms.string( "" ),
  filterPset = cms.PSet( 
    chargeSignificance = cms.double( -1.0 ),
    minHitsMinPt = cms.int32( 3 ),
    ComponentType = cms.string( "CkfBaseTrajectoryFilter" ),
    maxLostHits = cms.int32( 1 ),
    maxNumberOfHits = cms.int32( -1 ),
    maxConsecLostHits = cms.int32( 1 ),
    minimumNumberOfHits = cms.int32( 5 ),
    nSigmaMinPt = cms.double( 5.0 ),
    minPt = cms.double( 0.9 )
  )
)
navigationSchoolESProducer = cms.ESProducer( "NavigationSchoolESProducer",
  ComponentName = cms.string( "SimpleNavigationSchool" ),
  appendToDataLabel = cms.string( "" )
)
pixellayerpairs = cms.ESProducer( "SeedingLayersESProducer",
  appendToDataLabel = cms.string( "" ),
  ComponentName = cms.string( "PixelLayerPairs" ),
  layerList = cms.vstring( 'BPix1+BPix2',
    'BPix1+BPix3',
    'BPix2+BPix3',
    'BPix1+FPix1_pos',
    'BPix1+FPix1_neg',
    'BPix1+FPix2_pos',
    'BPix1+FPix2_neg',
    'BPix2+FPix1_pos',
    'BPix2+FPix1_neg',
    'BPix2+FPix2_pos',
    'BPix2+FPix2_neg',
    'FPix1_pos+FPix2_pos',
    'FPix1_neg+FPix2_neg' ),
  BPix = cms.PSet( 
    useErrorsFromParam = cms.bool( True ),
    hitErrorRPhi = cms.double( 0.0027 ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    hitErrorRZ = cms.double( 0.0060 )
  ),
  FPix = cms.PSet( 
    useErrorsFromParam = cms.bool( True ),
    hitErrorRPhi = cms.double( 0.0051 ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    hitErrorRZ = cms.double( 0.0036 )
  ),
  TEC = cms.PSet(  )
)
pixellayertriplets = cms.ESProducer( "SeedingLayersESProducer",
  appendToDataLabel = cms.string( "" ),
  ComponentName = cms.string( "PixelLayerTriplets" ),
  layerList = cms.vstring( 'BPix1+BPix2+BPix3',
    'BPix1+BPix2+FPix1_pos',
    'BPix1+BPix2+FPix1_neg',
    'BPix1+FPix1_pos+FPix2_pos',
    'BPix1+FPix1_neg+FPix2_neg' ),
  BPix = cms.PSet( 
    useErrorsFromParam = cms.bool( True ),
    hitErrorRPhi = cms.double( 0.0027 ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    hitErrorRZ = cms.double( 0.0060 )
  ),
  FPix = cms.PSet( 
    useErrorsFromParam = cms.bool( True ),
    hitErrorRPhi = cms.double( 0.0051 ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    hitErrorRZ = cms.double( 0.0036 )
  ),
  TEC = cms.PSet(  )
)
pixellayertripletsHITHB = cms.ESProducer( "SeedingLayersESProducer",
  appendToDataLabel = cms.string( "" ),
  ComponentName = cms.string( "PixelLayerTripletsHITHB" ),
  layerList = cms.vstring( 'BPix1+BPix2+BPix3' ),
  BPix = cms.PSet( 
    useErrorsFromParam = cms.bool( True ),
    hitErrorRPhi = cms.double( 0.0027 ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    hitErrorRZ = cms.double( 0.0060 )
  ),
  FPix = cms.PSet( 
    useErrorsFromParam = cms.bool( True ),
    hitErrorRPhi = cms.double( 0.0051 ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    hitErrorRZ = cms.double( 0.0036 )
  ),
  TEC = cms.PSet(  )
)
pixellayertripletsHITHE = cms.ESProducer( "SeedingLayersESProducer",
  appendToDataLabel = cms.string( "" ),
  ComponentName = cms.string( "PixelLayerTripletsHITHE" ),
  layerList = cms.vstring( 'BPix1+BPix2+FPix1_pos',
    'BPix1+BPix2+FPix1_neg',
    'BPix1+FPix1_pos+FPix2_pos',
    'BPix1+FPix1_neg+FPix2_neg' ),
  BPix = cms.PSet( 
    useErrorsFromParam = cms.bool( True ),
    hitErrorRPhi = cms.double( 0.0027 ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    hitErrorRZ = cms.double( 0.0060 )
  ),
  FPix = cms.PSet( 
    useErrorsFromParam = cms.bool( True ),
    hitErrorRPhi = cms.double( 0.0051 ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    hitErrorRZ = cms.double( 0.0036 )
  ),
  TEC = cms.PSet(  )
)
softLeptonByDistance = cms.ESProducer( "LeptonTaggerByDistanceESProducer",
  appendToDataLabel = cms.string( "" ),
  distance = cms.double( 0.5 )
)
softLeptonByPt = cms.ESProducer( "LeptonTaggerByPtESProducer",
  appendToDataLabel = cms.string( "" ),
  ipSign = cms.string( "any" )
)
trackCounting3D2nd = cms.ESProducer( "TrackCountingESProducer",
  appendToDataLabel = cms.string( "" ),
  nthTrack = cms.int32( 2 ),
  impactParameterType = cms.int32( 0 ),
  deltaR = cms.double( -1.0 ),
  maximumDecayLength = cms.double( 5.0 ),
  maximumDistanceToJetAxis = cms.double( 0.07 ),
  trackQualityClass = cms.string( "any" )
)
trajBuilderL3 = cms.ESProducer( "CkfTrajectoryBuilderESProducer",
  ComponentName = cms.string( "trajBuilderL3" ),
  updator = cms.string( "KFUpdator" ),
  propagatorAlong = cms.string( "PropagatorWithMaterial" ),
  propagatorOpposite = cms.string( "PropagatorWithMaterialOpposite" ),
  estimator = cms.string( "Chi2" ),
  TTRHBuilder = cms.string( "WithTrackAngle" ),
  MeasurementTrackerName = cms.string( "" ),
  trajectoryFilterName = cms.string( "trajFilterL3" ),
  maxCand = cms.int32( 5 ),
  lostHitPenalty = cms.double( 30.0 ),
  intermediateCleaning = cms.bool( True ),
  alwaysUseInvalidHits = cms.bool( False ),
  appendToDataLabel = cms.string( "" )
)
trajFilterL3 = cms.ESProducer( "TrajectoryFilterESProducer",
  ComponentName = cms.string( "trajFilterL3" ),
  appendToDataLabel = cms.string( "" ),
  filterPset = cms.PSet( 
    minimumNumberOfHits = cms.int32( 5 ),
    minHitsMinPt = cms.int32( 3 ),
    ComponentType = cms.string( "CkfBaseTrajectoryFilter" ),
    maxLostHits = cms.int32( 1 ),
    maxNumberOfHits = cms.int32( 7 ),
    maxConsecLostHits = cms.int32( 1 ),
    chargeSignificance = cms.double( -1.0 ),
    nSigmaMinPt = cms.double( 5.0 ),
    minPt = cms.double( 0.9 )
  )
)
trajectoryCleanerBySharedHits = cms.ESProducer( "TrajectoryCleanerESProducer",
  ComponentName = cms.string( "TrajectoryCleanerBySharedHits" ),
  appendToDataLabel = cms.string( "" ),
  fractionShared = cms.double( 0.5 )
)

DTDataIntegrityTask = cms.Service( "DTDataIntegrityTask",
  getSCInfo = cms.untracked.bool( True ),
  hltMode = cms.untracked.bool( True )
)
UpdaterService = cms.Service( "UpdaterService",
)

hltTriggerType = cms.EDFilter( "HLTTriggerTypeFilter",
    SelectedTriggerType = cms.int32( 1 )
)
hltL1EventNumber = cms.EDFilter( "HLTL1NumberFilter",
    rawInput = cms.InputTag( "rawDataCollector" ),
    period = cms.uint32( 4096 ),
    invert = cms.bool( True )
)
hltGtDigis = cms.EDProducer( "L1GlobalTriggerRawToDigi",
    DaqGtInputTag = cms.InputTag( "rawDataCollector" ),
    DaqGtFedId = cms.untracked.int32( 813 ),
    ActiveBoardsMask = cms.uint32( 0xffff ),
    UnpackBxInEvent = cms.int32( 5 ),
    Verbosity = cms.untracked.int32( 0 )
)
hltGctDigis = cms.EDProducer( "GctRawToDigi",
    inputLabel = cms.InputTag( "rawDataCollector" ),
    gctFedId = cms.int32( 745 ),
    hltMode = cms.bool( True ),
    unpackSharedRegions = cms.bool( False ),
    unpackerVersion = cms.uint32( 0 )
)
hltL1GtObjectMap = cms.EDProducer( "L1GlobalTrigger",
    GmtInputTag = cms.InputTag( "hltGtDigis" ),
    GctInputTag = cms.InputTag( "hltGctDigis" ),
    CastorInputTag = cms.InputTag( "castorL1Digis" ),
    ProduceL1GtDaqRecord = cms.bool( False ),
    ProduceL1GtEvmRecord = cms.bool( False ),
    ProduceL1GtObjectMapRecord = cms.bool( True ),
    WritePsbL1GtDaqRecord = cms.bool( False ),
    ReadTechnicalTriggerRecords = cms.bool( True ),
    EmulateBxInEvent = cms.int32( 1 ),
    AlternativeNrBxBoardDaq = cms.uint32( 0 ),
    AlternativeNrBxBoardEvm = cms.uint32( 0 ),
    BstLengthBytes = cms.int32( -1 ),
    TechnicalTriggersInputTags = cms.VInputTag( 'simBscDigis' ),
    RecordLength = cms.vint32( 3, 0 )
)
hltL1extraParticles = cms.EDProducer( "L1ExtraParticlesProd",
    produceMuonParticles = cms.bool( True ),
    muonSource = cms.InputTag( "hltGtDigis" ),
    produceCaloParticles = cms.bool( True ),
    isolatedEmSource = cms.InputTag( 'hltGctDigis','isoEm' ),
    nonIsolatedEmSource = cms.InputTag( 'hltGctDigis','nonIsoEm' ),
    centralJetSource = cms.InputTag( 'hltGctDigis','cenJets' ),
    forwardJetSource = cms.InputTag( 'hltGctDigis','forJets' ),
    tauJetSource = cms.InputTag( 'hltGctDigis','tauJets' ),
    etTotalSource = cms.InputTag( "hltGctDigis" ),
    etHadSource = cms.InputTag( "hltGctDigis" ),
    etMissSource = cms.InputTag( "hltGctDigis" ),
    htMissSource = cms.InputTag( "hltGctDigis" ),
    hfRingEtSumsSource = cms.InputTag( "hltGctDigis" ),
    hfRingBitCountsSource = cms.InputTag( "hltGctDigis" ),
    centralBxOnly = cms.bool( True ),
    ignoreHtMiss = cms.bool( False )
)
hltBPTXCoincidence = cms.EDFilter( "HLTLevel1Activity",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    ignoreL1Mask = cms.bool( False ),
    physicsLoBits = cms.uint64( 0x1 ),
    physicsHiBits = cms.uint64( 0x40000 ),
    technicalBits = cms.uint64( 0x0 ),
    bunchCrossings = cms.vint32( 0, 1, -1, 2, -2 )
)
hltOfflineBeamSpot = cms.EDProducer( "BeamSpotProducer" )
hltLevel1Activity = cms.EDFilter( "HLTLevel1Activity",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    ignoreL1Mask = cms.bool( False ),
    physicsLoBits = cms.uint64( 0x7fdfff03c03fbffc ),
    physicsHiBits = cms.uint64( 0x3f18fdbb01800bf6 ),
    technicalBits = cms.uint64( 0x70000fffff001f00 ),
    bunchCrossings = cms.vint32( 0, 1, -1, 2, -2 )
)
hltPreActivityL1A = cms.EDFilter( "HLTPrescaler" )
hltBoolEnd = cms.EDFilter( "HLTBool",
    result = cms.bool( True )
)
hltPreActivityPixelClusters = cms.EDFilter( "HLTPrescaler" )
hltSiPixelDigis = cms.EDProducer( "SiPixelRawToDigi",
    IncludeErrors = cms.bool( False ),
    CheckPixelOrder = cms.bool( False ),
    InputLabel = cms.InputTag( "rawDataCollector" )
)
hltSiPixelClusters = cms.EDProducer( "SiPixelClusterProducer",
    src = cms.InputTag( "hltSiPixelDigis" ),
    payloadType = cms.string( "HLT" ),
    ChannelThreshold = cms.int32( 1000 ),
    SeedThreshold = cms.int32( 1000 ),
    ClusterThreshold = cms.double( 4000.0 ),
    VCaltoElectronGain = cms.int32( 65 ),
    VCaltoElectronOffset = cms.int32( -414 ),
    MissCalibrate = cms.untracked.bool( True ),
    SplitClusters = cms.bool( False )
)
hltSiPixelRecHits = cms.EDProducer( "SiPixelRecHitConverter",
    src = cms.InputTag( "hltSiPixelClusters" ),
    CPE = cms.string( "PixelCPEGeneric" )
)
hltPixelActivityFilter = cms.EDFilter( "HLTPixelActivityFilter",
    inputTag = cms.InputTag( "hltSiPixelClusters" ),
    minClusters = cms.uint32( 3 )
)
hltPreActivityDT = cms.EDFilter( "HLTPrescaler" )
hltMuonDTDigis = cms.EDProducer( "DTUnpackingModule",
    inputLabel = cms.untracked.InputTag( "rawDataCollector" ),
    dataType = cms.string( "DDU" ),
    fedbyType = cms.untracked.bool( False ),
    readOutParameters = cms.PSet( 
      debug = cms.untracked.bool( False ),
      rosParameters = cms.PSet( 
        writeSC = cms.untracked.bool( True ),
        readingDDU = cms.untracked.bool( True ),
        performDataIntegrityMonitor = cms.untracked.bool( False ),
        readDDUIDfromDDU = cms.untracked.bool( True ),
        debug = cms.untracked.bool( False ),
        localDAQ = cms.untracked.bool( False )
      ),
      performDataIntegrityMonitor = cms.untracked.bool( False ),
      localDAQ = cms.untracked.bool( False )
    )
)
hltDTTFUnpacker = cms.EDProducer( "DTTFFEDReader",
    DTTF_FED_Source = cms.InputTag( "rawDataCollector" )
)
hltDTActivityFilter = cms.EDFilter( "HLTDTActivityFilter",
    inputDCC = cms.InputTag( "hltDTTFUnpacker" ),
    inputDDU = cms.InputTag( "hltMuonDTDigis" ),
    inputDigis = cms.InputTag( "hltMuonDTDigis" ),
    processDCC = cms.bool( True ),
    processDDU = cms.bool( True ),
    processDigis = cms.bool( True ),
    processingMode = cms.int32( 2 ),
    minQual = cms.int32( 2 ),
    maxStation = cms.int32( 3 ),
    minChamberLayers = cms.int32( 6 ),
    minDDUBX = cms.int32( 9 ),
    maxDDUBX = cms.int32( 14 ),
    minActiveChambs = cms.int32( 1 )
)
hltVetoL1SingleEG2 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "NOT L1_SingleEG2" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreActivityCaloTower = cms.EDFilter( "HLTPrescaler" )
hltEcalRawToRecHitFacility = cms.EDProducer( "EcalRawToRecHitFacility",
    sourceTag = cms.InputTag( "rawDataCollector" ),
    workerName = cms.string( "" )
)
hltEcalRegionalRestFEDs = cms.EDProducer( "EcalRawToRecHitRoI",
    sourceTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    type = cms.string( "all" ),
    doES = cms.bool( False ),
    sourceTag_es = cms.InputTag( "NotNeededoESfalse" ),
    MuJobPSet = cms.PSet(  ),
    JetJobPSet = cms.VPSet( 
    ),
    EmJobPSet = cms.VPSet( 
    ),
    CandJobPSet = cms.VPSet( 
    )
)
hltEcalRecHitAll = cms.EDProducer( "EcalRawToRecHitProducer",
    lazyGetterTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    sourceTag = cms.InputTag( "hltEcalRegionalRestFEDs" ),
    splitOutput = cms.bool( True ),
    EBrechitCollection = cms.string( "EcalRecHitsEB" ),
    EErechitCollection = cms.string( "EcalRecHitsEE" ),
    rechitCollection = cms.string( "NotNeededsplitOutputTrue" )
)
hltHcalDigis = cms.EDProducer( "HcalRawToDigi",
    InputLabel = cms.InputTag( "rawDataCollector" ),
    UnpackCalib = cms.untracked.bool( True ),
    UnpackZDC = cms.untracked.bool( True ),
    firstSample = cms.int32( 0 ),
    lastSample = cms.int32( 9 ),
    FilterDataQuality = cms.bool( True )
)
hltHbhereco = cms.EDProducer( "HcalSimpleReconstructor",
    digiLabel = cms.InputTag( "hltHcalDigis" ),
    dropZSmarkedPassed = cms.bool( True ),
    Subdetector = cms.string( "HBHE" ),
    firstSample = cms.int32( 4 ),
    samplesToAdd = cms.int32( 4 ),
    correctForTimeslew = cms.bool( True ),
    correctForPhaseContainment = cms.bool( True ),
    correctionPhaseNS = cms.double( 13.0 )
)
hltHfreco = cms.EDProducer( "HcalSimpleReconstructor",
    digiLabel = cms.InputTag( "hltHcalDigis" ),
    dropZSmarkedPassed = cms.bool( True ),
    Subdetector = cms.string( "HF" ),
    firstSample = cms.int32( 3 ),
    samplesToAdd = cms.int32( 4 ),
    correctForTimeslew = cms.bool( False ),
    correctForPhaseContainment = cms.bool( False ),
    correctionPhaseNS = cms.double( 0.0 )
)
hltHoreco = cms.EDProducer( "HcalSimpleReconstructor",
    digiLabel = cms.InputTag( "hltHcalDigis" ),
    dropZSmarkedPassed = cms.bool( True ),
    Subdetector = cms.string( "HO" ),
    firstSample = cms.int32( 4 ),
    samplesToAdd = cms.int32( 4 ),
    correctForTimeslew = cms.bool( True ),
    correctForPhaseContainment = cms.bool( True ),
    correctionPhaseNS = cms.double( 13.0 )
)
hltTowerMakerForAll = cms.EDProducer( "CaloTowersCreator",
    EBThreshold = cms.double( 0.09 ),
    EEThreshold = cms.double( 0.45 ),
    UseEtEBTreshold = cms.bool( False ),
    UseEtEETreshold = cms.bool( False ),
    UseSymEBTreshold = cms.bool( False ),
    UseSymEETreshold = cms.bool( False ),
    HcalThreshold = cms.double( -1000.0 ),
    HBThreshold = cms.double( 0.9 ),
    HESThreshold = cms.double( 1.4 ),
    HEDThreshold = cms.double( 1.4 ),
    HOThreshold0 = cms.double( 1.1 ),
    HOThresholdPlus1 = cms.double( 1.1 ),
    HOThresholdMinus1 = cms.double( 1.1 ),
    HOThresholdPlus2 = cms.double( 1.1 ),
    HOThresholdMinus2 = cms.double( 1.1 ),
    HF1Threshold = cms.double( 1.2 ),
    HF2Threshold = cms.double( 1.8 ),
    EBWeight = cms.double( 1.0 ),
    EEWeight = cms.double( 1.0 ),
    HBWeight = cms.double( 1.0 ),
    HESWeight = cms.double( 1.0 ),
    HEDWeight = cms.double( 1.0 ),
    HOWeight = cms.double( 1.0E-99 ),
    HF1Weight = cms.double( 1.0 ),
    HF2Weight = cms.double( 1.0 ),
    EcutTower = cms.double( -1000.0 ),
    EBSumThreshold = cms.double( 0.2 ),
    EESumThreshold = cms.double( 0.45 ),
    UseHO = cms.bool( False ),
    MomConstrMethod = cms.int32( 1 ),
    MomHBDepth = cms.double( 0.2 ),
    MomHEDepth = cms.double( 0.4 ),
    MomEBDepth = cms.double( 0.3 ),
    MomEEDepth = cms.double( 0.0 ),
    hbheInput = cms.InputTag( "hltHbhereco" ),
    hoInput = cms.InputTag( "hltHoreco" ),
    hfInput = cms.InputTag( "hltHfreco" ),
    AllowMissingInputs = cms.bool( False ),
    HcalAcceptSeverityLevel = cms.uint32( 999 ),
    EcalAcceptSeverityLevel = cms.uint32( 1 ),
    UseHcalRecoveredHits = cms.bool( True ),
    UseEcalRecoveredHits = cms.bool( True ),
    EBGrid = cms.vdouble(  ),
    EBWeights = cms.vdouble(  ),
    EEGrid = cms.vdouble(  ),
    EEWeights = cms.vdouble(  ),
    HBGrid = cms.vdouble(  ),
    HBWeights = cms.vdouble(  ),
    HESGrid = cms.vdouble(  ),
    HESWeights = cms.vdouble(  ),
    HEDGrid = cms.vdouble(  ),
    HEDWeights = cms.vdouble(  ),
    HOGrid = cms.vdouble(  ),
    HOWeights = cms.vdouble(  ),
    HF1Grid = cms.vdouble(  ),
    HF1Weights = cms.vdouble(  ),
    HF2Grid = cms.vdouble(  ),
    HF2Weights = cms.vdouble(  ),
    ecalInputs = cms.VInputTag( 'hltEcalRecHitAll:EcalRecHitsEB','hltEcalRecHitAll:EcalRecHitsEE' )
)
hltTowerMakerForEcal = cms.EDProducer( "CaloTowersCreator",
    EBThreshold = cms.double( 0.09 ),
    EEThreshold = cms.double( 0.45 ),
    UseEtEBTreshold = cms.bool( False ),
    UseEtEETreshold = cms.bool( False ),
    UseSymEBTreshold = cms.bool( False ),
    UseSymEETreshold = cms.bool( False ),
    HcalThreshold = cms.double( -1000.0 ),
    HBThreshold = cms.double( 0.9 ),
    HESThreshold = cms.double( 1.4 ),
    HEDThreshold = cms.double( 1.4 ),
    HOThreshold0 = cms.double( 1.1 ),
    HOThresholdPlus1 = cms.double( 1.1 ),
    HOThresholdMinus1 = cms.double( 1.1 ),
    HOThresholdPlus2 = cms.double( 1.1 ),
    HOThresholdMinus2 = cms.double( 1.1 ),
    HF1Threshold = cms.double( 1.2 ),
    HF2Threshold = cms.double( 1.8 ),
    EBWeight = cms.double( 1.0 ),
    EEWeight = cms.double( 1.0 ),
    HBWeight = cms.double( 1.0E-99 ),
    HESWeight = cms.double( 1.0E-99 ),
    HEDWeight = cms.double( 1.0E-99 ),
    HOWeight = cms.double( 1.0E-99 ),
    HF1Weight = cms.double( 1.0E-99 ),
    HF2Weight = cms.double( 1.0E-99 ),
    EcutTower = cms.double( -1000.0 ),
    EBSumThreshold = cms.double( 0.2 ),
    EESumThreshold = cms.double( 0.45 ),
    UseHO = cms.bool( False ),
    MomConstrMethod = cms.int32( 1 ),
    MomHBDepth = cms.double( 0.2 ),
    MomHEDepth = cms.double( 0.4 ),
    MomEBDepth = cms.double( 0.3 ),
    MomEEDepth = cms.double( 0.0 ),
    hbheInput = cms.InputTag( "" ),
    hoInput = cms.InputTag( "" ),
    hfInput = cms.InputTag( "" ),
    AllowMissingInputs = cms.bool( True ),
    HcalAcceptSeverityLevel = cms.uint32( 999 ),
    EcalAcceptSeverityLevel = cms.uint32( 1 ),
    UseHcalRecoveredHits = cms.bool( True ),
    UseEcalRecoveredHits = cms.bool( True ),
    EBGrid = cms.vdouble(  ),
    EBWeights = cms.vdouble(  ),
    EEGrid = cms.vdouble(  ),
    EEWeights = cms.vdouble(  ),
    HBGrid = cms.vdouble(  ),
    HBWeights = cms.vdouble(  ),
    HESGrid = cms.vdouble(  ),
    HESWeights = cms.vdouble(  ),
    HEDGrid = cms.vdouble(  ),
    HEDWeights = cms.vdouble(  ),
    HOGrid = cms.vdouble(  ),
    HOWeights = cms.vdouble(  ),
    HF1Grid = cms.vdouble(  ),
    HF1Weights = cms.vdouble(  ),
    HF2Grid = cms.vdouble(  ),
    HF2Weights = cms.vdouble(  ),
    ecalInputs = cms.VInputTag( 'hltEcalRecHitAll:EcalRecHitsEB','hltEcalRecHitAll:EcalRecHitsEE' )
)
hltCaloTowerFilter = cms.EDFilter( "HLTCaloTowerFilter",
    inputTag = cms.InputTag( "hltTowerMakerForEcal" ),
    MinPt = cms.double( 2.0 ),
    MaxEta = cms.double( 3.0 ),
    MinN = cms.uint32( 1 )
)
hltPreActivityEcal = cms.EDFilter( "HLTPrescaler" )
hltEcalDigis = cms.EDProducer( "EcalRawToDigi",
    numbXtalTSamples = cms.int32( 10 ),
    numbTriggerTSamples = cms.int32( 1 ),
    headerUnpacking = cms.bool( True ),
    srpUnpacking = cms.bool( False ),
    tccUnpacking = cms.bool( False ),
    feUnpacking = cms.bool( True ),
    memUnpacking = cms.bool( False ),
    syncCheck = cms.bool( False ),
    feIdCheck = cms.bool( True ),
    forceToKeepFRData = cms.bool( False ),
    eventPut = cms.bool( True ),
    InputLabel = cms.string( "rawDataCollector" ),
    DoRegional = cms.bool( False ),
    FedLabel = cms.InputTag( "listfeds" ),
    silentMode = cms.untracked.bool( True ),
    FEDs = cms.vint32( 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611, 612, 613, 614, 615, 616, 617, 618, 619, 620, 621, 622, 623, 624, 625, 626, 627, 628, 629, 630, 631, 632, 633, 634, 635, 636, 637, 638, 639, 640, 641, 642, 643, 644, 645, 646, 647, 648, 649, 650, 651, 652, 653, 654 ),
    orderedFedList = cms.vint32( 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611, 612, 613, 614, 615, 616, 617, 618, 619, 620, 621, 622, 623, 624, 625, 626, 627, 628, 629, 630, 631, 632, 633, 634, 635, 636, 637, 638, 639, 640, 641, 642, 643, 644, 645, 646, 647, 648, 649, 650, 651, 652, 653, 654 ),
    orderedDCCIdList = cms.vint32( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54 )
)
hltEcalFixedAlphaBetaFitUncalibRecHit = cms.EDProducer( "EcalUncalibRecHitProducer",
    EBdigiCollection = cms.InputTag( 'hltEcalDigis','ebDigis' ),
    EEdigiCollection = cms.InputTag( 'hltEcalDigis','eeDigis' ),
    EBhitCollection = cms.string( "EcalUncalibRecHitsEB" ),
    EEhitCollection = cms.string( "EcalUncalibRecHitsEE" ),
    algo = cms.string( "EcalUncalibRecHitWorkerFixedAlphaBetaFit" ),
    betaEB = cms.double( 1.7 ),
    betaEE = cms.double( 1.37 ),
    AlphaBetaFilename = cms.untracked.string( "NOFILE" ),
    MinAmplEndcap = cms.double( 14.0 ),
    MinAmplBarrel = cms.double( 8.0 ),
    UseDynamicPedestal = cms.bool( True ),
    alphaEB = cms.double( 1.2 ),
    alphaEE = cms.double( 1.63 )
)
hltEcalRecHitAB = cms.EDProducer( "EcalRecHitProducer",
    EBuncalibRecHitCollection = cms.InputTag( 'hltEcalFixedAlphaBetaFitUncalibRecHit','EcalUncalibRecHitsEB' ),
    EEuncalibRecHitCollection = cms.InputTag( 'hltEcalFixedAlphaBetaFitUncalibRecHit','EcalUncalibRecHitsEE' ),
    EBrechitCollection = cms.string( "EcalRecHitsEB" ),
    EErechitCollection = cms.string( "EcalRecHitsEE" ),
    recoverEBIsolatedChannels = cms.bool( False ),
    recoverEEIsolatedChannels = cms.bool( False ),
    recoverEBVFE = cms.bool( False ),
    recoverEEVFE = cms.bool( False ),
    recoverEBFE = cms.bool( False ),
    recoverEEFE = cms.bool( False ),
    killDeadChannels = cms.bool( True ),
    ebDetIdToBeRecovered = cms.InputTag( "" ),
    eeDetIdToBeRecovered = cms.InputTag( "" ),
    ebFEToBeRecovered = cms.InputTag( "" ),
    eeFEToBeRecovered = cms.InputTag( "" ),
    algo = cms.string( "EcalRecHitWorkerSimple" ),
    algoRecover = cms.string( "EcalRecHitWorkerRecover" ),
    ChannelStatusToBeExcluded = cms.vint32( 1, 2, 3, 4, 8, 9, 10, 11, 12, 13, 14, 78, 142 ),
    singleChannelRecoveryMethod = cms.string( "NeuralNetworks" ),
    singleChannelRecoveryThreshold = cms.double( 8.0 ),
    triggerPrimitiveDigiCollection = cms.InputTag( 'hltEcalDigis','EcalTriggerPrimitives' ),
    flagsMapDBReco = cms.vint32( 0, 0, 0, 0, 4, -1, -1, -1, 4, 4, 7, 7, 7, 8, 9 )
)
hltTowerMakerForEcalABFit = cms.EDProducer( "CaloTowersCreator",
    EBThreshold = cms.double( 0.09 ),
    EEThreshold = cms.double( 0.45 ),
    UseEtEBTreshold = cms.bool( False ),
    UseEtEETreshold = cms.bool( False ),
    UseSymEBTreshold = cms.bool( False ),
    UseSymEETreshold = cms.bool( False ),
    HcalThreshold = cms.double( -1000.0 ),
    HBThreshold = cms.double( 0.9 ),
    HESThreshold = cms.double( 1.4 ),
    HEDThreshold = cms.double( 1.4 ),
    HOThreshold0 = cms.double( 1.1 ),
    HOThresholdPlus1 = cms.double( 1.1 ),
    HOThresholdMinus1 = cms.double( 1.1 ),
    HOThresholdPlus2 = cms.double( 1.1 ),
    HOThresholdMinus2 = cms.double( 1.1 ),
    HF1Threshold = cms.double( 1.2 ),
    HF2Threshold = cms.double( 1.8 ),
    EBWeight = cms.double( 1.0 ),
    EEWeight = cms.double( 1.0 ),
    HBWeight = cms.double( 1.0 ),
    HESWeight = cms.double( 1.0 ),
    HEDWeight = cms.double( 1.0 ),
    HOWeight = cms.double( 1.0E-99 ),
    HF1Weight = cms.double( 1.0 ),
    HF2Weight = cms.double( 1.0 ),
    EcutTower = cms.double( -1000.0 ),
    EBSumThreshold = cms.double( 0.2 ),
    EESumThreshold = cms.double( 0.45 ),
    UseHO = cms.bool( False ),
    MomConstrMethod = cms.int32( 1 ),
    MomHBDepth = cms.double( 0.2 ),
    MomHEDepth = cms.double( 0.4 ),
    MomEBDepth = cms.double( 0.3 ),
    MomEEDepth = cms.double( 0.0 ),
    hbheInput = cms.InputTag( "" ),
    hoInput = cms.InputTag( "" ),
    hfInput = cms.InputTag( "" ),
    AllowMissingInputs = cms.bool( True ),
    HcalAcceptSeverityLevel = cms.uint32( 999 ),
    EcalAcceptSeverityLevel = cms.uint32( 1 ),
    UseHcalRecoveredHits = cms.bool( True ),
    UseEcalRecoveredHits = cms.bool( True ),
    EBGrid = cms.vdouble(  ),
    EBWeights = cms.vdouble(  ),
    EEGrid = cms.vdouble(  ),
    EEWeights = cms.vdouble(  ),
    HBGrid = cms.vdouble(  ),
    HBWeights = cms.vdouble(  ),
    HESGrid = cms.vdouble(  ),
    HESWeights = cms.vdouble(  ),
    HEDGrid = cms.vdouble(  ),
    HEDWeights = cms.vdouble(  ),
    HOGrid = cms.vdouble(  ),
    HOWeights = cms.vdouble(  ),
    HF1Grid = cms.vdouble(  ),
    HF1Weights = cms.vdouble(  ),
    HF2Grid = cms.vdouble(  ),
    HF2Weights = cms.vdouble(  ),
    ecalInputs = cms.VInputTag( 'hltEcalRecHitAB:EcalRecHitsEB','hltEcalRecHitAB:EcalRecHitsEE' )
)
hltEcalTowerFilter = cms.EDFilter( "HLTEcalTowerFilter",
    inputTag = cms.InputTag( "hltTowerMakerForEcalABFit" ),
    MinE = cms.double( 2.0 ),
    MaxEta = cms.double( 2.75 ),
    MinN = cms.int32( 1 )
)
hltL1sSplashEcalSumET = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleEG5 OR L1_SingleMuBeamHalo" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreSplashEcalSumET = cms.EDFilter( "HLTPrescaler" )
hltMet = cms.EDProducer( "METProducer",
    src = cms.InputTag( "hltTowerMakerForAll" ),
    InputType = cms.string( "CandidateCollection" ),
    METType = cms.string( "CaloMET" ),
    alias = cms.string( "RawCaloMET" ),
    globalThreshold = cms.double( 0.3 ),
    noHF = cms.bool( False ),
    HO_EtResPar = cms.vdouble( 0.0, 1.3, 0.0050 ),
    HF_EtResPar = cms.vdouble( 0.0, 1.82, 0.09 ),
    HB_PhiResPar = cms.vdouble( 0.02511 ),
    HE_PhiResPar = cms.vdouble( 0.02511 ),
    EE_EtResPar = cms.vdouble( 0.2, 0.03, 0.0050 ),
    EB_PhiResPar = cms.vdouble( 0.00502 ),
    EE_PhiResPar = cms.vdouble( 0.02511 ),
    HB_EtResPar = cms.vdouble( 0.0, 1.22, 0.05 ),
    EB_EtResPar = cms.vdouble( 0.2, 0.03, 0.0050 ),
    HF_PhiResPar = cms.vdouble( 0.05022 ),
    HE_EtResPar = cms.vdouble( 0.0, 1.3, 0.05 ),
    HO_PhiResPar = cms.vdouble( 0.02511 )
)
hltSplashEcalEtSum = cms.EDFilter( "HLTGlobalSumsCaloMET",
    inputTag = cms.InputTag( "hltEcalMet" ),
    saveTag = cms.untracked.bool( True ),
    observable = cms.string( "sumEt" ),
    Min = cms.double( 30.0 ),
    Max = cms.double( -1.0 ),
    MinN = cms.int32( 1 )
)
hltL1sL1Jet6U = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet6U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1Jet6U = cms.EDFilter( "HLTPrescaler" )
hltL1sJet15U = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet6U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreJet15U = cms.EDFilter( "HLTPrescaler" )
hltIterativeCone5CaloJets = cms.EDProducer( "FastjetJetProducer",
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.5 ),
    src = cms.InputTag( "hltTowerMakerForAll" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.5 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    doPUFastjet = cms.bool( False ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 )
)
hltMCJetCorJetIcone5HF07 = cms.EDProducer( "CaloJetCorrectionProducer",
    src = cms.InputTag( "hltIterativeCone5CaloJets" ),
    verbose = cms.untracked.bool( False ),
    alias = cms.untracked.string( "MCJetCorJetIcone5" ),
    correctors = cms.vstring( 'MCJetCorrectorIcone5HF07' )
)
hlt1jet15U = cms.EDFilter( "HLT1CaloJet",
    inputTag = cms.InputTag( "hltMCJetCorJetIcone5HF07" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 15.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 1 )
)
hltL1sJet30U = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet20U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreJet30U = cms.EDFilter( "HLTPrescaler" )
hlt1jet30U = cms.EDFilter( "HLT1CaloJet",
    inputTag = cms.InputTag( "hltMCJetCorJetIcone5HF07" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 30.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 1 )
)
hltL1sJet50U = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet30U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreJet50U = cms.EDFilter( "HLTPrescaler" )
hlt1jet50U = cms.EDFilter( "HLT1CaloJet",
    inputTag = cms.InputTag( "hltMCJetCorJetIcone5HF07" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 50.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 1 )
)
hltL1sL1SingleForJet = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleForJet2U OR L1_SingleForJet4U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1SingleForJet = cms.EDFilter( "HLTPrescaler" )
hltL1sFwdJet20U = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_IsoEG10_Jet6U_ForJet6U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreFwdJet20U = cms.EDFilter( "HLTPrescaler" )
hltRapGap20U = cms.EDFilter( "HLTRapGapFilter",
    inputTag = cms.InputTag( "hltMCJetCorJetIcone5HF07" ),
    saveTag = cms.untracked.bool( True ),
    minEta = cms.double( 3.0 ),
    maxEta = cms.double( 5.0 ),
    caloThresh = cms.double( 20.0 )
)
hltL1sDiJetAve15U8E29 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet6U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreDiJetAve15U8E29 = cms.EDFilter( "HLTPrescaler" )
hltDiJetAve15U8E29 = cms.EDFilter( "HLTDiJetAveFilter",
    inputJetTag = cms.InputTag( "hltIterativeCone5CaloJets" ),
    saveTag = cms.untracked.bool( True ),
    minPtAve = cms.double( 15.0 ),
    minPtJet3 = cms.double( 99999.0 ),
    minDphi = cms.double( -1.0 )
)
hltL1sDiJetAve30U8E29 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet20U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreDiJetAve30U8E29 = cms.EDFilter( "HLTPrescaler" )
hltDiJetAve30U8E29 = cms.EDFilter( "HLTDiJetAveFilter",
    inputJetTag = cms.InputTag( "hltIterativeCone5CaloJets" ),
    saveTag = cms.untracked.bool( True ),
    minPtAve = cms.double( 30.0 ),
    minPtJet3 = cms.double( 99999.0 ),
    minDphi = cms.double( -1.0 )
)
hltL1sQuadJet15U = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_QuadJet6U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreQuadJet15U = cms.EDFilter( "HLTPrescaler" )
hlt4jet15U = cms.EDFilter( "HLT1CaloJet",
    inputTag = cms.InputTag( "hltMCJetCorJetIcone5HF07" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 15.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 4 )
)
hltL1sL1MET20 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_ETM20" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1MET20 = cms.EDFilter( "HLTPrescaler" )
hltL1sMET45 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_ETM30" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreMET45 = cms.EDFilter( "HLTPrescaler" )
hlt1MET45 = cms.EDFilter( "HLT1CaloMET",
    inputTag = cms.InputTag( "hltMet" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 45.0 ),
    MaxEta = cms.double( -1.0 ),
    MinN = cms.int32( 1 )
)
hltL1sMET100 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_ETM70" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreMET100 = cms.EDFilter( "HLTPrescaler" )
hlt1MET100 = cms.EDFilter( "HLT1CaloMET",
    inputTag = cms.InputTag( "hltMet" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 100.0 ),
    MaxEta = cms.double( -1.0 ),
    MinN = cms.int32( 1 )
)
hltL1sHT100 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_HTT50" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreHT100 = cms.EDFilter( "HLTPrescaler" )
hltJet15UHt = cms.EDProducer( "METProducer",
    src = cms.InputTag( "hltMCJetCorJetIcone5HF07" ),
    InputType = cms.string( "CaloJetCollection" ),
    METType = cms.string( "MET" ),
    alias = cms.string( "HTMET" ),
    globalThreshold = cms.double( 30.0 ),
    noHF = cms.bool( False ),
    HO_EtResPar = cms.vdouble( 0.0, 1.3, 0.0050 ),
    HF_EtResPar = cms.vdouble( 0.0, 1.82, 0.09 ),
    HB_PhiResPar = cms.vdouble( 0.02511 ),
    HE_PhiResPar = cms.vdouble( 0.02511 ),
    EE_EtResPar = cms.vdouble( 0.2, 0.03, 0.0050 ),
    EB_PhiResPar = cms.vdouble( 0.00502 ),
    EE_PhiResPar = cms.vdouble( 0.02511 ),
    HB_EtResPar = cms.vdouble( 0.0, 1.22, 0.05 ),
    EB_EtResPar = cms.vdouble( 0.2, 0.03, 0.0050 ),
    HF_PhiResPar = cms.vdouble( 0.05022 ),
    HE_EtResPar = cms.vdouble( 0.0, 1.3, 0.05 ),
    HO_PhiResPar = cms.vdouble( 0.02511 )
)
hltHT100U = cms.EDFilter( "HLTGlobalSumsMET",
    inputTag = cms.InputTag( "hltJet15UHt" ),
    saveTag = cms.untracked.bool( True ),
    observable = cms.string( "sumEt" ),
    Min = cms.double( 100.0 ),
    Max = cms.double( -1.0 ),
    MinN = cms.int32( 1 )
)
hltL1sL1SingleMuOpenL1SingleMu0 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMuOpen OR L1_SingleMu0" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1MuOpenNoBptx = cms.EDFilter( "HLTPrescaler" )
hltL1MuOpenL1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1SingleMuOpenL1SingleMu0" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    SaveTag = cms.untracked.bool( True ),
    SelectQualities = cms.vint32(  )
)
hltPreL1MuOpen = cms.EDFilter( "HLTPrescaler" )
hltL1sL1Mu = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMu7 OR L1_DoubleMu3" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1Mu = cms.EDFilter( "HLTPrescaler" )
hltL1MuL1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1Mu" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    SaveTag = cms.untracked.bool( True ),
    SelectQualities = cms.vint32(  )
)
hltL1sL1SingleMu20 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMu20" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1Mu20 = cms.EDFilter( "HLTPrescaler" )
hltL1Mu20L1Filtered20 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1SingleMu20" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 20.0 ),
    MinN = cms.int32( 1 ),
    SaveTag = cms.untracked.bool( True ),
    SelectQualities = cms.vint32(  )
)
hltL1sL1SingleMu7 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMu7" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL2Mu9 = cms.EDFilter( "HLTPrescaler" )
hltL1SingleMu7L1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1SingleMu7" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    SelectQualities = cms.vint32(  )
)
hltDt1DRecHits = cms.EDProducer( "DTRecHitProducer",
    debug = cms.untracked.bool( False ),
    dtDigiLabel = cms.InputTag( "hltMuonDTDigis" ),
    recAlgo = cms.string( "DTLinearDriftFromDBAlgo" ),
    recAlgoConfig = cms.PSet( 
      debug = cms.untracked.bool( False ),
      minTime = cms.double( -3.0 ),
      maxTime = cms.double( 420.0 ),
      tTrigModeConfig = cms.PSet( 
        vPropWire = cms.double( 24.4 ),
        doTOFCorrection = cms.bool( True ),
        tofCorrType = cms.int32( 1 ),
        wirePropCorrType = cms.int32( 1 ),
        doWirePropCorrection = cms.bool( True ),
        doT0Correction = cms.bool( True ),
        debug = cms.untracked.bool( False )
      ),
      tTrigMode = cms.string( "DTTTrigSyncFromDB" )
    )
)
hltDt4DSegments = cms.EDProducer( "DTRecSegment4DProducer",
    debug = cms.untracked.bool( False ),
    recHits1DLabel = cms.InputTag( "hltDt1DRecHits" ),
    recHits2DLabel = cms.InputTag( "dt2DSegments" ),
    Reco4DAlgoName = cms.string( "DTCombinatorialPatternReco4D" ),
    Reco4DAlgoConfig = cms.PSet( 
      segmCleanerMode = cms.int32( 1 ),
      Reco2DAlgoName = cms.string( "DTCombinatorialPatternReco" ),
      recAlgo = cms.string( "DTLinearDriftFromDBAlgo" ),
      nSharedHitsMax = cms.int32( 2 ),
      hit_afterT0_resolution = cms.double( 0.03 ),
      Reco2DAlgoConfig = cms.PSet( 
        segmCleanerMode = cms.int32( 1 ),
        recAlgo = cms.string( "DTLinearDriftFromDBAlgo" ),
        nSharedHitsMax = cms.int32( 2 ),
        AlphaMaxPhi = cms.double( 1.0 ),
        hit_afterT0_resolution = cms.double( 0.03 ),
        MaxAllowedHits = cms.uint32( 50 ),
        performT0_vdriftSegCorrection = cms.bool( False ),
        AlphaMaxTheta = cms.double( 0.1 ),
        debug = cms.untracked.bool( False ),
        recAlgoConfig = cms.PSet( 
          debug = cms.untracked.bool( False ),
          minTime = cms.double( -3.0 ),
          maxTime = cms.double( 420.0 ),
          tTrigModeConfig = cms.PSet( 
            vPropWire = cms.double( 24.4 ),
            doTOFCorrection = cms.bool( True ),
            tofCorrType = cms.int32( 1 ),
            wirePropCorrType = cms.int32( 1 ),
            doWirePropCorrection = cms.bool( True ),
            doT0Correction = cms.bool( True ),
            debug = cms.untracked.bool( False )
          ),
          tTrigMode = cms.string( "DTTTrigSyncFromDB" )
        ),
        nUnSharedHitsMin = cms.int32( 2 ),
        performT0SegCorrection = cms.bool( False )
      ),
      performT0_vdriftSegCorrection = cms.bool( False ),
      debug = cms.untracked.bool( False ),
      recAlgoConfig = cms.PSet( 
        debug = cms.untracked.bool( False ),
        minTime = cms.double( -3.0 ),
        maxTime = cms.double( 420.0 ),
        tTrigModeConfig = cms.PSet( 
          vPropWire = cms.double( 24.4 ),
          doTOFCorrection = cms.bool( True ),
          tofCorrType = cms.int32( 1 ),
          wirePropCorrType = cms.int32( 1 ),
          doWirePropCorrection = cms.bool( True ),
          doT0Correction = cms.bool( True ),
          debug = cms.untracked.bool( False )
        ),
        tTrigMode = cms.string( "DTTTrigSyncFromDB" )
      ),
      nUnSharedHitsMin = cms.int32( 2 ),
      AllDTRecHits = cms.bool( True ),
      performT0SegCorrection = cms.bool( False )
    )
)
hltMuonCSCDigis = cms.EDProducer( "CSCDCCUnpacker",
    InputObjects = cms.InputTag( "rawDataCollector" ),
    UseExaminer = cms.bool( True ),
    ExaminerMask = cms.uint32( 0x1febf3f6 ),
    UseSelectiveUnpacking = cms.bool( True ),
    ErrorMask = cms.uint32( 0x0 ),
    UnpackStatusDigis = cms.bool( False ),
    UseFormatStatus = cms.bool( True ),
    PrintEventNumber = cms.untracked.bool( False )
)
hltCsc2DRecHits = cms.EDProducer( "CSCRecHitDProducer",
    CSCUseCalibrations = cms.bool( True ),
    stripDigiTag = cms.InputTag( 'hltMuonCSCDigis','MuonCSCStripDigi' ),
    wireDigiTag = cms.InputTag( 'hltMuonCSCDigis','MuonCSCWireDigi' ),
    CSCstripWireDeltaTime = cms.int32( 8 ),
    CSCUseStaticPedestals = cms.bool( False ),
    CSCNoOfTimeBinsForDynamicPedestal = cms.int32( 2 ),
    CSCStripPeakThreshold = cms.double( 10.0 ),
    CSCStripClusterChargeCut = cms.double( 25.0 ),
    CSCWireClusterDeltaT = cms.int32( 1 ),
    CSCStripxtalksOffset = cms.double( 0.03 ),
    NoiseLevel_ME1a = cms.double( 7.0 ),
    XTasymmetry_ME1a = cms.double( 0.0 ),
    ConstSyst_ME1a = cms.double( 0.022 ),
    NoiseLevel_ME1b = cms.double( 8.0 ),
    XTasymmetry_ME1b = cms.double( 0.0 ),
    ConstSyst_ME1b = cms.double( 0.0070 ),
    NoiseLevel_ME12 = cms.double( 9.0 ),
    XTasymmetry_ME12 = cms.double( 0.0 ),
    ConstSyst_ME12 = cms.double( 0.0 ),
    NoiseLevel_ME13 = cms.double( 8.0 ),
    XTasymmetry_ME13 = cms.double( 0.0 ),
    ConstSyst_ME13 = cms.double( 0.0 ),
    NoiseLevel_ME21 = cms.double( 9.0 ),
    XTasymmetry_ME21 = cms.double( 0.0 ),
    ConstSyst_ME21 = cms.double( 0.0 ),
    NoiseLevel_ME22 = cms.double( 9.0 ),
    XTasymmetry_ME22 = cms.double( 0.0 ),
    ConstSyst_ME22 = cms.double( 0.0 ),
    NoiseLevel_ME31 = cms.double( 9.0 ),
    XTasymmetry_ME31 = cms.double( 0.0 ),
    ConstSyst_ME31 = cms.double( 0.0 ),
    NoiseLevel_ME32 = cms.double( 9.0 ),
    XTasymmetry_ME32 = cms.double( 0.0 ),
    ConstSyst_ME32 = cms.double( 0.0 ),
    NoiseLevel_ME41 = cms.double( 9.0 ),
    XTasymmetry_ME41 = cms.double( 0.0 ),
    ConstSyst_ME41 = cms.double( 0.0 ),
    readBadChannels = cms.bool( True ),
    readBadChambers = cms.bool( True ),
    UseAverageTime = cms.bool( False ),
    UseParabolaFit = cms.bool( False ),
    UseFourPoleFit = cms.bool( True ),
    UseFivePoleFit = cms.bool( True )
)
hltCscSegments = cms.EDProducer( "CSCSegmentProducer",
    inputObjects = cms.InputTag( "hltCsc2DRecHits" ),
    algo_type = cms.int32( 1 ),
    algo_psets = cms.VPSet( 
      cms.PSet(  chamber_types = cms.vstring( 'ME1/a',
  'ME1/b',
  'ME1/2',
  'ME1/3',
  'ME2/1',
  'ME2/2',
  'ME3/1',
  'ME3/2',
  'ME4/1',
  'ME4/2' ),
        algo_name = cms.string( "CSCSegAlgoST" ),
        algo_psets = cms.VPSet( 
          cms.PSet(  maxRatioResidualPrune = cms.double( 3.0 ),
            yweightPenalty = cms.double( 1.5 ),
            maxRecHitsInCluster = cms.int32( 20 ),
            hitDropLimit6Hits = cms.double( 0.3333 ),
            BPMinImprovement = cms.double( 10000.0 ),
            tanPhiMax = cms.double( 0.5 ),
            onlyBestSegment = cms.bool( False ),
            dRPhiFineMax = cms.double( 8.0 ),
            curvePenalty = cms.double( 2.0 ),
            dXclusBoxMax = cms.double( 4.0 ),
            BrutePruning = cms.bool( True ),
            tanThetaMax = cms.double( 1.2 ),
            hitDropLimit4Hits = cms.double( 0.6 ),
            useShowering = cms.bool( False ),
            CSCDebug = cms.untracked.bool( False ),
            curvePenaltyThreshold = cms.double( 0.85 ),
            minHitsPerSegment = cms.int32( 3 ),
            dPhiFineMax = cms.double( 0.025 ),
            yweightPenaltyThreshold = cms.double( 1.0 ),
            hitDropLimit5Hits = cms.double( 0.8 ),
            preClustering = cms.bool( True ),
            maxDPhi = cms.double( 999.0 ),
            maxDTheta = cms.double( 999.0 ),
            Pruning = cms.bool( True ),
            dYclusBoxMax = cms.double( 8.0 )
          ),
          cms.PSet(  maxRatioResidualPrune = cms.double( 3.0 ),
            yweightPenalty = cms.double( 1.5 ),
            maxRecHitsInCluster = cms.int32( 24 ),
            hitDropLimit6Hits = cms.double( 0.3333 ),
            BPMinImprovement = cms.double( 10000.0 ),
            tanPhiMax = cms.double( 0.5 ),
            onlyBestSegment = cms.bool( False ),
            dRPhiFineMax = cms.double( 8.0 ),
            curvePenalty = cms.double( 2.0 ),
            dXclusBoxMax = cms.double( 4.0 ),
            BrutePruning = cms.bool( True ),
            tanThetaMax = cms.double( 1.2 ),
            hitDropLimit4Hits = cms.double( 0.6 ),
            useShowering = cms.bool( False ),
            CSCDebug = cms.untracked.bool( False ),
            curvePenaltyThreshold = cms.double( 0.85 ),
            minHitsPerSegment = cms.int32( 3 ),
            dPhiFineMax = cms.double( 0.025 ),
            yweightPenaltyThreshold = cms.double( 1.0 ),
            hitDropLimit5Hits = cms.double( 0.8 ),
            preClustering = cms.bool( True ),
            maxDPhi = cms.double( 999.0 ),
            maxDTheta = cms.double( 999.0 ),
            Pruning = cms.bool( True ),
            dYclusBoxMax = cms.double( 8.0 )
          )
        ),
        parameters_per_chamber_type = cms.vint32( 2, 1, 1, 1, 1, 1, 1, 1, 1, 1 )
      )
    )
)
hltMuonRPCDigis = cms.EDProducer( "RPCUnpackingModule",
    InputLabel = cms.InputTag( "rawDataCollector" ),
    doSynchro = cms.bool( False )
)
hltRpcRecHits = cms.EDProducer( "RPCRecHitProducer",
    rpcDigiLabel = cms.InputTag( "hltMuonRPCDigis" ),
    recAlgo = cms.string( "RPCRecHitStandardAlgo" ),
    maskSource = cms.string( "File" ),
    maskvecfile = cms.FileInPath( "RecoLocalMuon/RPCRecHit/data/RPCMaskVec.dat" ),
    deadSource = cms.string( "File" ),
    deadvecfile = cms.FileInPath( "RecoLocalMuon/RPCRecHit/data/RPCDeadVec.dat" ),
    recAlgoConfig = cms.PSet(  )
)
hltL2MuonSeeds = cms.EDProducer( "L2MuonSeedGenerator",
    InputObjects = cms.InputTag( "hltL1extraParticles" ),
    GMTReadoutCollection = cms.InputTag( "hltGtDigis" ),
    Propagator = cms.string( "SteppingHelixPropagatorAny" ),
    L1MinPt = cms.double( 0.0 ),
    L1MaxEta = cms.double( 2.5 ),
    L1MinQuality = cms.uint32( 1 ),
    ServiceParameters = cms.PSet( 
      Propagators = cms.untracked.vstring( 'SteppingHelixPropagatorAny' ),
      RPCLayers = cms.bool( True ),
      UseMuonNavigation = cms.untracked.bool( True )
    )
)
hltL2Muons = cms.EDProducer( "L2MuonProducer",
    InputObjects = cms.InputTag( "hltL2MuonSeeds" ),
    L2TrajBuilderParameters = cms.PSet( 
      DoRefit = cms.bool( False ),
      SeedPropagator = cms.string( "FastSteppingHelixPropagatorAny" ),
      FilterParameters = cms.PSet( 
        NumberOfSigma = cms.double( 3.0 ),
        FitDirection = cms.string( "insideOut" ),
        DTRecSegmentLabel = cms.InputTag( "hltDt4DSegments" ),
        MaxChi2 = cms.double( 1000.0 ),
        MuonTrajectoryUpdatorParameters = cms.PSet( 
          MaxChi2 = cms.double( 25.0 ),
          Granularity = cms.int32( 0 ),
          RescaleErrorFactor = cms.double( 100.0 ),
          UseInvalidHits = cms.bool( True ),
          RescaleError = cms.bool( False )
        ),
        EnableRPCMeasurement = cms.bool( True ),
        CSCRecSegmentLabel = cms.InputTag( "hltCscSegments" ),
        EnableDTMeasurement = cms.bool( True ),
        RPCRecSegmentLabel = cms.InputTag( "hltRpcRecHits" ),
        Propagator = cms.string( "FastSteppingHelixPropagatorAny" ),
        EnableCSCMeasurement = cms.bool( True )
      ),
      NavigationType = cms.string( "Standard" ),
      SeedTransformerParameters = cms.PSet( 
        Fitter = cms.string( "KFFitterSmootherForL2Muon" ),
        MuonRecHitBuilder = cms.string( "MuonRecHitBuilder" ),
        Propagator = cms.string( "FastSteppingHelixPropagatorAny" ),
        UseSubRecHits = cms.bool( False ),
        NMinRecHits = cms.uint32( 2 ),
        RescaleError = cms.double( 100.0 )
      ),
      DoBackwardFilter = cms.bool( True ),
      SeedPosition = cms.string( "in" ),
      BWFilterParameters = cms.PSet( 
        NumberOfSigma = cms.double( 3.0 ),
        CSCRecSegmentLabel = cms.InputTag( "hltCscSegments" ),
        FitDirection = cms.string( "outsideIn" ),
        DTRecSegmentLabel = cms.InputTag( "hltDt4DSegments" ),
        MaxChi2 = cms.double( 100.0 ),
        MuonTrajectoryUpdatorParameters = cms.PSet( 
          MaxChi2 = cms.double( 25.0 ),
          Granularity = cms.int32( 2 ),
          RescaleErrorFactor = cms.double( 100.0 ),
          UseInvalidHits = cms.bool( True ),
          RescaleError = cms.bool( False )
        ),
        EnableRPCMeasurement = cms.bool( True ),
        BWSeedType = cms.string( "fromGenerator" ),
        EnableDTMeasurement = cms.bool( True ),
        RPCRecSegmentLabel = cms.InputTag( "hltRpcRecHits" ),
        Propagator = cms.string( "FastSteppingHelixPropagatorAny" ),
        EnableCSCMeasurement = cms.bool( True )
      ),
      DoSeedRefit = cms.bool( False )
    ),
    ServiceParameters = cms.PSet( 
      Propagators = cms.untracked.vstring( 'FastSteppingHelixPropagatorAny',
        'FastSteppingHelixPropagatorOpposite' ),
      RPCLayers = cms.bool( True ),
      UseMuonNavigation = cms.untracked.bool( True )
    ),
    TrackLoaderParameters = cms.PSet( 
      Smoother = cms.string( "KFSmootherForMuonTrackLoader" ),
      DoSmoothing = cms.bool( False ),
      MuonUpdatorAtVertexParameters = cms.PSet( 
        MaxChi2 = cms.double( 1000000.0 ),
        BeamSpotPosition = cms.vdouble( 0.0, 0.0, 0.0 ),
        Propagator = cms.string( "FastSteppingHelixPropagatorOpposite" ),
        BeamSpotPositionErrors = cms.vdouble( 0.1, 0.1, 5.3 )
      ),
      VertexConstraint = cms.bool( True )
    )
)
hltL2MuonCandidates = cms.EDProducer( "L2MuonCandidateProducer",
    InputObjects = cms.InputTag( 'hltL2Muons','UpdatedAtVtx' )
)
hltL2Mu9L2Filtered9 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltL1SingleMu7L1Filtered0" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 9.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
hltPreL2Mu11 = cms.EDFilter( "HLTPrescaler" )
hltL2Mu11L2Filtered11 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltL1SingleMu7L1Filtered0" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 11.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
hltL1sL1SingleMu3 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMu3" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreIsoMu3 = cms.EDFilter( "HLTPrescaler" )
hltSingleMuIsoL1Filtered3 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1SingleMu3" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    SelectQualities = cms.vint32(  )
)
hltSingleMuIsoL2PreFiltered3 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltSingleMuIsoL1Filtered3" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 3.0 ),
    NSigmaPt = cms.double( 0.0 )
)
hltEcalRegionalMuonsFEDs = cms.EDProducer( "EcalRawToRecHitRoI",
    sourceTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    type = cms.string( "candidate" ),
    doES = cms.bool( False ),
    sourceTag_es = cms.InputTag( "NotNeededoESfalse" ),
    MuJobPSet = cms.PSet(  ),
    JetJobPSet = cms.VPSet( 
    ),
    EmJobPSet = cms.VPSet( 
    ),
    CandJobPSet = cms.VPSet( 
      cms.PSet(  bePrecise = cms.bool( False ),
        propagatorNameToBePrecise = cms.string( "" ),
        epsilon = cms.double( 0.01 ),
        regionPhiMargin = cms.double( 0.3 ),
        cType = cms.string( "chargedcandidate" ),
        Source = cms.InputTag( "hltL2MuonCandidates" ),
        Ptmin = cms.double( 0.0 ),
        regionEtaMargin = cms.double( 0.3 )
      )
    )
)
hltEcalRegionalMuonsRecHit = cms.EDProducer( "EcalRawToRecHitProducer",
    lazyGetterTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    sourceTag = cms.InputTag( "hltEcalRegionalMuonsFEDs" ),
    splitOutput = cms.bool( True ),
    EBrechitCollection = cms.string( "EcalRecHitsEB" ),
    EErechitCollection = cms.string( "EcalRecHitsEE" ),
    rechitCollection = cms.string( "NotNeededsplitOutputTrue" )
)
hltTowerMakerForMuons = cms.EDProducer( "CaloTowersCreator",
    EBThreshold = cms.double( 0.09 ),
    EEThreshold = cms.double( 0.45 ),
    UseEtEBTreshold = cms.bool( False ),
    UseEtEETreshold = cms.bool( False ),
    UseSymEBTreshold = cms.bool( False ),
    UseSymEETreshold = cms.bool( False ),
    HcalThreshold = cms.double( -1000.0 ),
    HBThreshold = cms.double( 0.9 ),
    HESThreshold = cms.double( 1.4 ),
    HEDThreshold = cms.double( 1.4 ),
    HOThreshold0 = cms.double( 1.1 ),
    HOThresholdPlus1 = cms.double( 1.1 ),
    HOThresholdMinus1 = cms.double( 1.1 ),
    HOThresholdPlus2 = cms.double( 1.1 ),
    HOThresholdMinus2 = cms.double( 1.1 ),
    HF1Threshold = cms.double( 1.2 ),
    HF2Threshold = cms.double( 1.8 ),
    EBWeight = cms.double( 1.0 ),
    EEWeight = cms.double( 1.0 ),
    HBWeight = cms.double( 1.0 ),
    HESWeight = cms.double( 1.0 ),
    HEDWeight = cms.double( 1.0 ),
    HOWeight = cms.double( 1.0E-99 ),
    HF1Weight = cms.double( 1.0 ),
    HF2Weight = cms.double( 1.0 ),
    EcutTower = cms.double( -1000.0 ),
    EBSumThreshold = cms.double( 0.2 ),
    EESumThreshold = cms.double( 0.45 ),
    UseHO = cms.bool( False ),
    MomConstrMethod = cms.int32( 1 ),
    MomHBDepth = cms.double( 0.2 ),
    MomHEDepth = cms.double( 0.4 ),
    MomEBDepth = cms.double( 0.3 ),
    MomEEDepth = cms.double( 0.0 ),
    hbheInput = cms.InputTag( "hltHbhereco" ),
    hoInput = cms.InputTag( "hltHoreco" ),
    hfInput = cms.InputTag( "hltHfreco" ),
    AllowMissingInputs = cms.bool( False ),
    HcalAcceptSeverityLevel = cms.uint32( 999 ),
    EcalAcceptSeverityLevel = cms.uint32( 1 ),
    UseHcalRecoveredHits = cms.bool( True ),
    UseEcalRecoveredHits = cms.bool( True ),
    EBGrid = cms.vdouble(  ),
    EBWeights = cms.vdouble(  ),
    EEGrid = cms.vdouble(  ),
    EEWeights = cms.vdouble(  ),
    HBGrid = cms.vdouble(  ),
    HBWeights = cms.vdouble(  ),
    HESGrid = cms.vdouble(  ),
    HESWeights = cms.vdouble(  ),
    HEDGrid = cms.vdouble(  ),
    HEDWeights = cms.vdouble(  ),
    HOGrid = cms.vdouble(  ),
    HOWeights = cms.vdouble(  ),
    HF1Grid = cms.vdouble(  ),
    HF1Weights = cms.vdouble(  ),
    HF2Grid = cms.vdouble(  ),
    HF2Weights = cms.vdouble(  ),
    ecalInputs = cms.VInputTag( 'hltEcalRegionalMuonsRecHit:EcalRecHitsEB','hltEcalRegionalMuonsRecHit:EcalRecHitsEE' )
)
hltL2MuonIsolations = cms.EDProducer( "L2MuonIsolationProducer",
    StandAloneCollectionLabel = cms.InputTag( 'hltL2Muons','UpdatedAtVtx' ),
    ExtractorPSet = cms.PSet( 
      DR_Veto_H = cms.double( 0.1 ),
      Vertex_Constraint_Z = cms.bool( False ),
      Threshold_H = cms.double( 0.5 ),
      ComponentName = cms.string( "CaloExtractor" ),
      Threshold_E = cms.double( 0.2 ),
      DR_Max = cms.double( 0.24 ),
      DR_Veto_E = cms.double( 0.07 ),
      Weight_E = cms.double( 1.5 ),
      Vertex_Constraint_XY = cms.bool( False ),
      DepositLabel = cms.untracked.string( "EcalPlusHcal" ),
      CaloTowerCollectionLabel = cms.InputTag( "hltTowerMakerForMuons" ),
      Weight_H = cms.double( 1.0 )
    ),
    IsolatorPSet = cms.PSet( 
      ConeSizes = cms.vdouble( 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24 ),
      ComponentName = cms.string( "SimpleCutsIsolator" ),
      Thresholds = cms.vdouble( 4.0, 3.7, 4.0, 3.5, 3.4, 3.4, 3.2, 3.4, 3.1, 2.9, 2.9, 2.7, 3.1, 3.0, 2.4, 2.1, 2.0, 2.3, 2.2, 2.4, 2.5, 2.5, 2.6, 2.9, 3.1, 2.9 ),
      EtaBounds = cms.vdouble( 0.0435, 0.1305, 0.2175, 0.3045, 0.3915, 0.4785, 0.5655, 0.6525, 0.7395, 0.8265, 0.9135, 1.0005, 1.0875, 1.1745, 1.2615, 1.3485, 1.4355, 1.5225, 1.6095, 1.6965, 1.785, 1.88, 1.9865, 2.1075, 2.247, 2.411 )
    )
)
hltSingleMuIsoL2IsoFiltered3 = cms.EDFilter( "HLTMuonIsoFilter",
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltSingleMuIsoL2PreFiltered3" ),
    MinN = cms.int32( 1 ),
    DepTag = cms.VInputTag( 'hltL2MuonIsolations' ),
    IsolatorPSet = cms.PSet(  )
)
hltSiStripRawToClustersFacility = cms.EDProducer( "SiStripRawToClusters",
    ProductLabel = cms.InputTag( "rawDataCollector" ),
    Clusterizer = cms.PSet( 
      ChannelThreshold = cms.double( 2.0 ),
      MaxSequentialBad = cms.uint32( 1 ),
      Algorithm = cms.string( "ThreeThresholdAlgorithm" ),
      MaxSequentialHoles = cms.uint32( 0 ),
      MaxAdjacentBad = cms.uint32( 0 ),
      QualityLabel = cms.string( "" ),
      SeedThreshold = cms.double( 3.0 ),
      ClusterThreshold = cms.double( 5.0 )
    ),
    Algorithms = cms.PSet( 
      SiStripFedZeroSuppressionMode = cms.uint32( 4 ),
      CommonModeNoiseSubtractionMode = cms.string( "Median" )
    )
)
hltSiStripClusters = cms.EDProducer( "MeasurementTrackerSiStripRefGetterProducer",
    InputModuleLabel = cms.InputTag( "hltSiStripRawToClustersFacility" ),
    measurementTrackerName = cms.string( "" )
)
hltL3TrajectorySeed = cms.EDProducer( "TSGFromL2Muon",
    PtCut = cms.double( 1.0 ),
    PCut = cms.double( 2.5 ),
    MuonCollectionLabel = cms.InputTag( 'hltL2Muons','UpdatedAtVtx' ),
    tkSeedGenerator = cms.string( "TSGForRoadSearchOI" ),
    ServiceParameters = cms.PSet( 
      Propagators = cms.untracked.vstring( 'SteppingHelixPropagatorOpposite',
        'SteppingHelixPropagatorAlong' ),
      RPCLayers = cms.bool( True ),
      UseMuonNavigation = cms.untracked.bool( True )
    ),
    MuonTrackingRegionBuilder = cms.PSet(  ),
    TrackerSeedCleaner = cms.PSet(  ),
    TSGFromMixedPairs = cms.PSet(  ),
    TSGFromPixelTriplets = cms.PSet(  ),
    TSGFromPixelPairs = cms.PSet(  ),
    TSGForRoadSearchOI = cms.PSet( 
      propagatorCompatibleName = cms.string( "SteppingHelixPropagatorOpposite" ),
      option = cms.uint32( 3 ),
      maxChi2 = cms.double( 40.0 ),
      errorMatrixPset = cms.PSet( 
        atIP = cms.bool( True ),
        action = cms.string( "use" ),
        errorMatrixValuesPSet = cms.PSet( 
          pf3_V12 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V13 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V11 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 3.0, 3.0, 3.0, 5.0, 4.0, 5.0, 10.0, 7.0, 10.0, 10.0, 10.0, 10.0 )
          ),
          pf3_V14 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V15 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V34 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          yAxis = cms.vdouble( 0.0, 1.0, 1.4, 10.0 ),
          pf3_V33 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 3.0, 3.0, 3.0, 5.0, 4.0, 5.0, 10.0, 7.0, 10.0, 10.0, 10.0, 10.0 )
          ),
          pf3_V45 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V44 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 3.0, 3.0, 3.0, 5.0, 4.0, 5.0, 10.0, 7.0, 10.0, 10.0, 10.0, 10.0 )
          ),
          xAxis = cms.vdouble( 0.0, 13.0, 30.0, 70.0, 1000.0 ),
          pf3_V23 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V22 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 3.0, 3.0, 3.0, 5.0, 4.0, 5.0, 10.0, 7.0, 10.0, 10.0, 10.0, 10.0 )
          ),
          pf3_V55 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 3.0, 3.0, 3.0, 5.0, 4.0, 5.0, 10.0, 7.0, 10.0, 10.0, 10.0, 10.0 )
          ),
          zAxis = cms.vdouble( -3.14159, 3.14159 ),
          pf3_V35 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V25 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V24 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          )
        )
      ),
      propagatorName = cms.string( "SteppingHelixPropagatorAlong" ),
      manySeeds = cms.bool( False ),
      copyMuonRecHit = cms.bool( False ),
      ComponentName = cms.string( "TSGForRoadSearch" )
    ),
    TSGForRoadSearchIOpxl = cms.PSet(  ),
    TSGFromPropagation = cms.PSet(  ),
    TSGFromCombinedHits = cms.PSet(  )
)
hltL3TrackCandidateFromL2 = cms.EDProducer( "CkfTrajectoryMaker",
    trackCandidateAlso = cms.bool( True ),
    src = cms.InputTag( "hltL3TrajectorySeed" ),
    TrajectoryBuilder = cms.string( "muonCkfTrajectoryBuilder" ),
    TrajectoryCleaner = cms.string( "TrajectoryCleanerBySharedHits" ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    useHitsSplitting = cms.bool( False ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    doSeedingRegionRebuilding = cms.bool( False ),
    cleanTrajectoryAfterInOut = cms.bool( False )
)
hltL3TkTracksFromL2 = cms.EDProducer( "TrackProducer",
    TrajectoryInEvent = cms.bool( True ),
    useHitsSplitting = cms.bool( False ),
    clusterRemovalInfo = cms.InputTag( "" ),
    alias = cms.untracked.string( "" ),
    Fitter = cms.string( "hltKFFittingSmoother" ),
    Propagator = cms.string( "PropagatorWithMaterial" ),
    src = cms.InputTag( "hltL3TrackCandidateFromL2" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    NavigationSchool = cms.string( "" )
)
hltL3Muons = cms.EDProducer( "L3MuonProducer",
    MuonCollectionLabel = cms.InputTag( 'hltL2Muons','UpdatedAtVtx' ),
    L3TrajBuilderParameters = cms.PSet( 
      MatcherOutPropagator = cms.string( "SmartPropagator" ),
      Direction = cms.int32( 0 ),
      GlbRefitterParameters = cms.PSet( 
        TrackerSkipSection = cms.int32( -1 ),
        DoPredictionsOnly = cms.bool( False ),
        PropDirForCosmics = cms.bool( False ),
        HitThreshold = cms.int32( 1 ),
        MuonHitsOption = cms.int32( 1 ),
        Chi2CutRPC = cms.double( 1.0 ),
        Fitter = cms.string( "L3MuKFFitter" ),
        RPCRecSegmentLabel = cms.InputTag( "hltRpcRecHits" ),
        TrackerRecHitBuilder = cms.string( "WithTrackAngle" ),
        Smoother = cms.string( "KFSmootherForRefitInsideOut" ),
        MuonRecHitBuilder = cms.string( "MuonRecHitBuilder" ),
        RefitDirection = cms.string( "insideOut" ),
        CSCRecSegmentLabel = cms.InputTag( "hltCscSegments" ),
        Chi2CutCSC = cms.double( 150.0 ),
        Chi2CutDT = cms.double( 10.0 ),
        RefitRPCHits = cms.bool( True ),
        SkipStation = cms.int32( -1 ),
        Propagator = cms.string( "SmartPropagatorAny" ),
        DTRecSegmentLabel = cms.InputTag( "hltDt4DSegments" ),
        TrackerSkipSystem = cms.int32( -1 )
      ),
      ScaleTECyFactor = cms.double( -1.0 ),
      StateOnTrackerBoundOutPropagator = cms.string( "SmartPropagatorAny" ),
      ScaleTECxFactor = cms.double( -1.0 ),
      TrackerRecHitBuilder = cms.string( "WithTrackAngle" ),
      TkTrackBuilder = cms.string( "muonCkfTrajectoryBuilder" ),
      MuonRecHitBuilder = cms.string( "MuonRecHitBuilder" ),
      tkTrajLabel = cms.InputTag( "hltL3TkTracksFromL2" ),
      MuonTrackingRegionBuilder = cms.PSet( 
        EtaR_UpperLimit_Par1 = cms.double( 0.25 ),
        Eta_fixed = cms.double( 0.2 ),
        OnDemand = cms.double( -1.0 ),
        Rescale_Dz = cms.double( 3.0 ),
        Eta_min = cms.double( 0.05 ),
        Rescale_phi = cms.double( 3.0 ),
        EtaR_UpperLimit_Par2 = cms.double( 0.15 ),
        DeltaZ_Region = cms.double( 15.9 ),
        Rescale_eta = cms.double( 3.0 ),
        PhiR_UpperLimit_Par2 = cms.double( 0.2 ),
        vertexCollection = cms.InputTag( "pixelVertices" ),
        Phi_fixed = cms.double( 0.2 ),
        DeltaR = cms.double( 0.2 ),
        EscapePt = cms.double( 1.5 ),
        UseFixedRegion = cms.bool( False ),
        PhiR_UpperLimit_Par1 = cms.double( 0.6 ),
        Phi_min = cms.double( 0.05 ),
        UseVertex = cms.bool( False ),
        beamSpot = cms.InputTag( "hltOfflineBeamSpot" )
      ),
      RefitRPCHits = cms.bool( True ),
      TrackTransformer = cms.PSet( 
        DoPredictionsOnly = cms.bool( False ),
        Fitter = cms.string( "L3MuKFFitter" ),
        TrackerRecHitBuilder = cms.string( "WithTrackAngle" ),
        Smoother = cms.string( "KFSmootherForMuonTrackLoader" ),
        MuonRecHitBuilder = cms.string( "MuonRecHitBuilder" ),
        RefitDirection = cms.string( "insideOut" ),
        RefitRPCHits = cms.bool( True )
      ),
      GlobalMuonTrackMatcher = cms.PSet( 
        Pt_threshold1 = cms.double( 0.0 ),
        DeltaDCut_3 = cms.double( 15.0 ),
        MinP = cms.double( 2.5 ),
        MinPt = cms.double( 1.0 ),
        Chi2Cut_1 = cms.double( 50.0 ),
        Pt_threshold2 = cms.double( 9.99999999E8 ),
        LocChi2Cut = cms.double( 0.0010 ),
        Eta_threshold = cms.double( 1.2 ),
        Quality_3 = cms.double( 7.0 ),
        Quality_2 = cms.double( 15.0 ),
        Chi2Cut_2 = cms.double( 50.0 ),
        Chi2Cut_3 = cms.double( 200.0 ),
        DeltaDCut_1 = cms.double( 40.0 ),
        DeltaRCut_2 = cms.double( 0.2 ),
        DeltaRCut_3 = cms.double( 1.0 ),
        DeltaDCut_2 = cms.double( 10.0 ),
        DeltaRCut_1 = cms.double( 0.1 ),
        Quality_1 = cms.double( 20.0 )
      ),
      TransformerOutPropagator = cms.string( "SmartPropagatorAny" ),
      PtCut = cms.double( 1.0 ),
      TrackerPropagator = cms.string( "SteppingHelixPropagatorAny" ),
      KFFitter = cms.string( "L3MuKFFitter" )
    ),
    ServiceParameters = cms.PSet( 
      Propagators = cms.untracked.vstring( 'SmartPropagatorAny',
        'SteppingHelixPropagatorAny',
        'SmartPropagator',
        'SteppingHelixPropagatorOpposite' ),
      RPCLayers = cms.bool( True ),
      UseMuonNavigation = cms.untracked.bool( True )
    ),
    TrackLoaderParameters = cms.PSet( 
      PutTkTrackIntoEvent = cms.untracked.bool( True ),
      VertexConstraint = cms.bool( False ),
      MuonSeededTracksInstance = cms.untracked.string( "L2Seeded" ),
      Smoother = cms.string( "KFSmootherForMuonTrackLoader" ),
      MuonUpdatorAtVertexParameters = cms.PSet( 
        MaxChi2 = cms.double( 1000000.0 ),
        BeamSpotPosition = cms.vdouble( 0.0, 0.0, 0.0 ),
        Propagator = cms.string( "SteppingHelixPropagatorOpposite" ),
        BeamSpotPositionErrors = cms.vdouble( 0.1, 0.1, 5.3 )
      ),
      SmoothTkTrack = cms.untracked.bool( False ),
      DoSmoothing = cms.bool( True )
    )
)
hltL3MuonCandidates = cms.EDProducer( "L3MuonCandidateProducer",
    InputObjects = cms.InputTag( "hltL3Muons" )
)
hltSingleMuIsoL3PreFiltered3 = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltSingleMuIsoL2IsoFiltered3" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 3.0 ),
    NSigmaPt = cms.double( 0.0 )
)
hltPixelTracks = cms.EDProducer( "PixelTrackProducer",
    useFilterWithES = cms.bool( False ),
    RegionFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "GlobalRegionProducer" ),
      RegionPSet = cms.PSet( 
        precise = cms.bool( True ),
        originHalfLength = cms.double( 15.9 ),
        originZPos = cms.double( 0.0 ),
        originYPos = cms.double( 0.0 ),
        ptMin = cms.double( 0.9 ),
        originXPos = cms.double( 0.0 ),
        originRadius = cms.double( 0.2 )
      )
    ),
    OrderedHitsFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "StandardHitTripletGenerator" ),
      GeneratorPSet = cms.PSet( 
        useBending = cms.bool( True ),
        useFixedPreFiltering = cms.bool( False ),
        phiPreFiltering = cms.double( 0.3 ),
        extraHitRPhitolerance = cms.double( 0.06 ),
        useMultScattering = cms.bool( True ),
        ComponentName = cms.string( "PixelTripletHLTGenerator" ),
        extraHitRZtolerance = cms.double( 0.06 )
      ),
      SeedingLayers = cms.string( "PixelLayerTriplets" )
    ),
    FitterPSet = cms.PSet( 
      ComponentName = cms.string( "PixelFitterByHelixProjections" ),
      TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" )
    ),
    FilterPSet = cms.PSet( 
      chi2 = cms.double( 1000.0 ),
      nSigmaTipMaxTolerance = cms.double( 0.0 ),
      ComponentName = cms.string( "PixelTrackFilterByKinematics" ),
      nSigmaInvPtTolerance = cms.double( 0.0 ),
      ptMin = cms.double( 0.1 ),
      tipMax = cms.double( 1.0 )
    ),
    CleanerPSet = cms.PSet(  ComponentName = cms.string( "PixelTrackCleanerBySharedHits" ) )
)
hltL3MuonIsolations = cms.EDProducer( "L3MuonIsolationProducer",
    inputMuonCollection = cms.InputTag( "hltL3Muons" ),
    OutputMuIsoDeposits = cms.bool( True ),
    TrackPt_Min = cms.double( -1.0 ),
    CutsPSet = cms.PSet( 
      ConeSizes = cms.vdouble( 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24 ),
      ComponentName = cms.string( "SimpleCuts" ),
      Thresholds = cms.vdouble( 1.1, 1.1, 1.1, 1.1, 1.2, 1.1, 1.2, 1.1, 1.2, 1.0, 1.1, 1.0, 1.0, 1.1, 1.0, 1.0, 1.1, 0.9, 1.1, 0.9, 1.1, 1.0, 1.0, 0.9, 0.8, 0.1 ),
      maxNTracks = cms.int32( -1 ),
      EtaBounds = cms.vdouble( 0.0435, 0.1305, 0.2175, 0.3045, 0.3915, 0.4785, 0.5655, 0.6525, 0.7395, 0.8265, 0.9135, 1.0005, 1.0875, 1.1745, 1.2615, 1.3485, 1.4355, 1.5225, 1.6095, 1.6965, 1.785, 1.88, 1.9865, 2.1075, 2.247, 2.411 ),
      applyCutsORmaxNTracks = cms.bool( False )
    ),
    ExtractorPSet = cms.PSet( 
      Chi2Prob_Min = cms.double( -1.0 ),
      Diff_z = cms.double( 0.2 ),
      inputTrackCollection = cms.InputTag( "hltPixelTracks" ),
      ReferenceRadius = cms.double( 6.0 ),
      BeamSpotLabel = cms.InputTag( "hltOfflineBeamSpot" ),
      ComponentName = cms.string( "PixelTrackExtractor" ),
      DR_Max = cms.double( 0.24 ),
      Diff_r = cms.double( 0.1 ),
      PropagateTracksToRadius = cms.bool( True ),
      DR_VetoPt = cms.double( 0.025 ),
      DR_Veto = cms.double( 0.01 ),
      NHits_Min = cms.uint32( 0 ),
      Chi2Ndof_Max = cms.double( 1.0E64 ),
      Pt_Min = cms.double( -1.0 ),
      DepositLabel = cms.untracked.string( "PXLS" ),
      BeamlineOption = cms.string( "BeamSpotFromEvent" ),
      VetoLeadingTrack = cms.bool( True ),
      PtVeto_Min = cms.double( 2.0 )
    )
)
hltSingleMuIsoL3IsoFiltered3 = cms.EDFilter( "HLTMuonIsoFilter",
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltSingleMuIsoL3PreFiltered3" ),
    MinN = cms.int32( 1 ),
    SaveTag = cms.untracked.bool( True ),
    DepTag = cms.VInputTag( 'hltL3MuonIsolations' ),
    IsolatorPSet = cms.PSet(  )
)
hltL1sL1SingleMuOpenL1SingleMu0L1SingleMu3 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMuOpen OR L1_SingleMu0 OR L1_SingleMu3" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreMu3 = cms.EDFilter( "HLTPrescaler" )
hltL1SingleMu0L1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1SingleMuOpenL1SingleMu0L1SingleMu3" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    SelectQualities = cms.vint32(  )
)
hltSingleMu3L2Filtered3 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltL1SingleMu0L1Filtered0" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 3.0 ),
    NSigmaPt = cms.double( 0.0 )
)
hltSingleMu3L3Filtered3 = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltSingleMu3L2Filtered3" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 3.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
hltPreMu5 = cms.EDFilter( "HLTPrescaler" )
hltL1SingleMu3L1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1SingleMu3" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    SelectQualities = cms.vint32(  )
)
hltSingleMu5L2Filtered4 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltL1SingleMu3L1Filtered0" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 4.0 ),
    NSigmaPt = cms.double( 0.0 )
)
hltSingleMu5L3Filtered5 = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltSingleMu5L2Filtered4" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 5.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
hltPreMu9 = cms.EDFilter( "HLTPrescaler" )
hltSingleMu9L2Filtered7 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltL1SingleMu7L1Filtered0" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 7.0 ),
    NSigmaPt = cms.double( 0.0 )
)
hltSingleMu9L3Filtered9 = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltSingleMu9L2Filtered7" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 9.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
hltL1sL1DoubleMuOpen = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_DoubleMuOpen" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1DoubleMuOpen = cms.EDFilter( "HLTPrescaler" )
hltDoubleMuLevel1PathL1OpenFiltered = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1DoubleMuOpen" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 2 ),
    SaveTag = cms.untracked.bool( True ),
    SelectQualities = cms.vint32(  )
)
hltPreDoubleMu0 = cms.EDFilter( "HLTPrescaler" )
hltDiMuonL1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1DoubleMuOpen" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 2 ),
    SelectQualities = cms.vint32(  )
)
hltDiMuonL2PreFiltered0 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltDiMuonL1Filtered0" ),
    MinN = cms.int32( 2 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 0.0 ),
    NSigmaPt = cms.double( 0.0 )
)
hltDiMuonL3PreFiltered0 = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltDiMuonL2PreFiltered0" ),
    MinN = cms.int32( 2 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 0.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
hltL1sL1DoubleMu3 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_DoubleMu3" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreDoubleMu3 = cms.EDFilter( "HLTPrescaler" )
hltDiMuonL1Filtered = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1DoubleMu3" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 2 ),
    SelectQualities = cms.vint32(  )
)
hltDiMuonL2PreFiltered = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltDiMuonL1Filtered" ),
    MinN = cms.int32( 2 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 3.0 ),
    NSigmaPt = cms.double( 0.0 )
)
hltDiMuonL3PreFiltered = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltDiMuonL2PreFiltered" ),
    MinN = cms.int32( 2 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 3.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
hltL1sL1SingleEG1 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleEG1" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1SingleEG1 = cms.EDFilter( "HLTPrescaler" )
hltL1sL1SingleEG2 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleEG2" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1SingleEG2 = cms.EDFilter( "HLTPrescaler" )
hltL1sL1SingleEG5 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleEG5" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1SingleEG5 = cms.EDFilter( "HLTPrescaler" )
hltPreL1SingleEG5_NoBPTX = cms.EDFilter( "HLTPrescaler" )
hltL1sL1SingleEG8 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleEG8" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1SingleEG8 = cms.EDFilter( "HLTPrescaler" )
hltL1sL1SingleEG20 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleEG20" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1SingleEG20 = cms.EDFilter( "HLTPrescaler" )
hltPreEle10LWL1R = cms.EDFilter( "HLTPrescaler" )
hltESRawToRecHitFacility = cms.EDProducer( "EcalRawToRecHitFacility",
    sourceTag = cms.InputTag( "rawDataCollector" ),
    workerName = cms.string( "esRawToRecHit" )
)
hltEcalRegionalEgammaFEDs = cms.EDProducer( "EcalRawToRecHitRoI",
    sourceTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    type = cms.string( "egamma" ),
    doES = cms.bool( True ),
    sourceTag_es = cms.InputTag( "hltESRawToRecHitFacility" ),
    MuJobPSet = cms.PSet(  ),
    JetJobPSet = cms.VPSet( 
    ),
    EmJobPSet = cms.VPSet( 
      cms.PSet(  Source = cms.InputTag( 'hltL1extraParticles','Isolated' ),
        regionPhiMargin = cms.double( 0.4 ),
        Ptmin = cms.double( 5.0 ),
        regionEtaMargin = cms.double( 0.25 )
      ),
      cms.PSet(  Source = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
        regionPhiMargin = cms.double( 0.4 ),
        Ptmin = cms.double( 5.0 ),
        regionEtaMargin = cms.double( 0.25 )
      )
    ),
    CandJobPSet = cms.VPSet( 
    )
)
hltEcalRegionalEgammaRecHit = cms.EDProducer( "EcalRawToRecHitProducer",
    lazyGetterTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    sourceTag = cms.InputTag( "hltEcalRegionalEgammaFEDs" ),
    splitOutput = cms.bool( True ),
    EBrechitCollection = cms.string( "EcalRecHitsEB" ),
    EErechitCollection = cms.string( "EcalRecHitsEE" ),
    rechitCollection = cms.string( "NotNeededsplitOutputTrue" )
)
hltESRegionalEgammaRecHit = cms.EDProducer( "EcalRawToRecHitProducer",
    lazyGetterTag = cms.InputTag( "hltESRawToRecHitFacility" ),
    sourceTag = cms.InputTag( 'hltEcalRegionalEgammaFEDs','es' ),
    splitOutput = cms.bool( False ),
    EBrechitCollection = cms.string( "" ),
    EErechitCollection = cms.string( "" ),
    rechitCollection = cms.string( "EcalRecHitsES" )
)
hltHybridSuperClustersL1Isolated = cms.EDProducer( "EgammaHLTHybridClusterProducer",
    debugLevel = cms.string( "INFO" ),
    basicclusterCollection = cms.string( "" ),
    superclusterCollection = cms.string( "" ),
    ecalhitproducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    ecalhitcollection = cms.string( "EcalRecHitsEB" ),
    l1TagIsolated = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    l1TagNonIsolated = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    doIsolated = cms.bool( True ),
    l1LowerThr = cms.double( 5.0 ),
    l1UpperThr = cms.double( 999.0 ),
    l1LowerThrIgnoreIsolation = cms.double( 999.0 ),
    regionEtaMargin = cms.double( 0.14 ),
    regionPhiMargin = cms.double( 0.4 ),
    posCalc_logweight = cms.bool( True ),
    posCalc_t0 = cms.double( 7.4 ),
    posCalc_w0 = cms.double( 4.2 ),
    posCalc_x0 = cms.double( 0.89 ),
    HybridBarrelSeedThr = cms.double( 1.5 ),
    step = cms.int32( 10 ),
    ethresh = cms.double( 0.1 ),
    eseed = cms.double( 0.35 ),
    ewing = cms.double( 1.0 )
)
hltCorrectedHybridSuperClustersL1Isolated = cms.EDProducer( "EgammaSCCorrectionMaker",
    VerbosityLevel = cms.string( "ERROR" ),
    recHitProducer = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEB' ),
    rawSuperClusterProducer = cms.InputTag( "hltHybridSuperClustersL1Isolated" ),
    superClusterAlgo = cms.string( "Hybrid" ),
    applyEnergyCorrection = cms.bool( True ),
    sigmaElectronicNoise = cms.double( 0.03 ),
    etThresh = cms.double( 5.0 ),
    corectedSuperClusterCollection = cms.string( "" ),
    hyb_fCorrPset = cms.PSet( 
      brLinearLowThr = cms.double( 1.1 ),
      fEtEtaVec = cms.vdouble( 1.0012, -0.5714, 0.0, 0.0, 0.0, 0.5549, 12.74, 1.0448, 0.0, 0.0, 0.0, 0.0, 8.0, 1.023, -0.00181, 0.0, 0.0 ),
      brLinearHighThr = cms.double( 8.0 ),
      fBremVec = cms.vdouble( -0.05208, 0.1331, 0.9196, -5.735E-4, 1.343 )
    ),
    isl_fCorrPset = cms.PSet(  ),
    dyn_fCorrPset = cms.PSet(  ),
    fix_fCorrPset = cms.PSet(  )
)
hltMulti5x5BasicClustersL1Isolated = cms.EDProducer( "EgammaHLTMulti5x5ClusterProducer",
    VerbosityLevel = cms.string( "ERROR" ),
    doBarrel = cms.bool( False ),
    doEndcaps = cms.bool( True ),
    doIsolated = cms.bool( True ),
    barrelHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    endcapHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    barrelHitCollection = cms.string( "EcalRecHitsEB" ),
    endcapHitCollection = cms.string( "EcalRecHitsEE" ),
    barrelClusterCollection = cms.string( "notused" ),
    endcapClusterCollection = cms.string( "multi5x5EndcapBasicClusters" ),
    Multi5x5BarrelSeedThr = cms.double( 0.5 ),
    Multi5x5EndcapSeedThr = cms.double( 0.18 ),
    l1TagIsolated = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    l1TagNonIsolated = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    l1LowerThr = cms.double( 5.0 ),
    l1UpperThr = cms.double( 999.0 ),
    l1LowerThrIgnoreIsolation = cms.double( 999.0 ),
    regionEtaMargin = cms.double( 0.3 ),
    regionPhiMargin = cms.double( 0.4 ),
    posCalc_logweight = cms.bool( True ),
    posCalc_t0_barl = cms.double( 7.4 ),
    posCalc_t0_endc = cms.double( 3.1 ),
    posCalc_t0_endcPresh = cms.double( 1.2 ),
    posCalc_w0 = cms.double( 4.2 ),
    posCalc_x0 = cms.double( 0.89 )
)
hltMulti5x5SuperClustersL1Isolated = cms.EDProducer( "Multi5x5SuperClusterProducer",
    VerbosityLevel = cms.string( "ERROR" ),
    endcapClusterProducer = cms.string( "hltMulti5x5BasicClustersL1Isolated" ),
    barrelClusterProducer = cms.string( "notused" ),
    endcapClusterCollection = cms.string( "multi5x5EndcapBasicClusters" ),
    barrelClusterCollection = cms.string( "multi5x5BarrelBasicClusters" ),
    endcapSuperclusterCollection = cms.string( "multi5x5EndcapSuperClusters" ),
    barrelSuperclusterCollection = cms.string( "multi5x5BarrelSuperClusters" ),
    doBarrel = cms.bool( False ),
    doEndcaps = cms.bool( True ),
    barrelEtaSearchRoad = cms.double( 0.06 ),
    barrelPhiSearchRoad = cms.double( 0.8 ),
    endcapEtaSearchRoad = cms.double( 0.14 ),
    endcapPhiSearchRoad = cms.double( 0.6 ),
    seedTransverseEnergyThreshold = cms.double( 1.0 ),
    dynamicPhiRoad = cms.bool( False ),
    bremRecoveryPset = cms.PSet( 
      barrel = cms.PSet(  ),
      endcap = cms.PSet( 
        a = cms.double( 47.85 ),
        c = cms.double( 0.1201 ),
        b = cms.double( 108.8 )
      ),
      doEndcaps = cms.bool( True ),
      doBarrel = cms.bool( False )
    )
)
hltMulti5x5EndcapSuperClustersWithPreshowerL1Isolated = cms.EDProducer( "PreshowerClusterProducer",
    preshRecHitProducer = cms.InputTag( 'hltESRegionalEgammaRecHit','EcalRecHitsES' ),
    endcapSClusterProducer = cms.InputTag( 'hltMulti5x5SuperClustersL1Isolated','multi5x5EndcapSuperClusters' ),
    preshClusterCollectionX = cms.string( "preshowerXClusters" ),
    preshClusterCollectionY = cms.string( "preshowerYClusters" ),
    preshNclust = cms.int32( 4 ),
    etThresh = cms.double( 5.0 ),
    preshCalibPlaneX = cms.double( 1.0 ),
    preshCalibPlaneY = cms.double( 0.7 ),
    preshCalibGamma = cms.double( 0.024 ),
    preshCalibMIP = cms.double( 9.0E-5 ),
    assocSClusterCollection = cms.string( "" ),
    preshStripEnergyCut = cms.double( 0.0 ),
    preshSeededNstrip = cms.int32( 15 ),
    preshClusterEnergyCut = cms.double( 0.0 ),
    debugLevel = cms.string( "" )
)
hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated = cms.EDProducer( "EgammaSCCorrectionMaker",
    VerbosityLevel = cms.string( "ERROR" ),
    recHitProducer = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEE' ),
    rawSuperClusterProducer = cms.InputTag( "hltMulti5x5EndcapSuperClustersWithPreshowerL1Isolated" ),
    superClusterAlgo = cms.string( "Multi5x5" ),
    applyEnergyCorrection = cms.bool( True ),
    sigmaElectronicNoise = cms.double( 0.15 ),
    etThresh = cms.double( 5.0 ),
    corectedSuperClusterCollection = cms.string( "" ),
    hyb_fCorrPset = cms.PSet(  ),
    isl_fCorrPset = cms.PSet(  ),
    dyn_fCorrPset = cms.PSet(  ),
    fix_fCorrPset = cms.PSet( 
      brLinearLowThr = cms.double( 0.6 ),
      fEtEtaVec = cms.vdouble( 0.9746, -6.512, 0.0, 0.0, 0.02771, 4.983, 0.0, 0.0, -0.007288, -0.9446, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0 ),
      brLinearHighThr = cms.double( 6.0 ),
      fBremVec = cms.vdouble( -0.04163, 0.08552, 0.95048, -0.002308, 1.077 )
    )
)
hltHybridSuperClustersL1NonIsolated = cms.EDProducer( "EgammaHLTHybridClusterProducer",
    debugLevel = cms.string( "INFO" ),
    basicclusterCollection = cms.string( "" ),
    superclusterCollection = cms.string( "" ),
    ecalhitproducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    ecalhitcollection = cms.string( "EcalRecHitsEB" ),
    l1TagIsolated = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    l1TagNonIsolated = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    doIsolated = cms.bool( False ),
    l1LowerThr = cms.double( 5.0 ),
    l1UpperThr = cms.double( 999.0 ),
    l1LowerThrIgnoreIsolation = cms.double( 999.0 ),
    regionEtaMargin = cms.double( 0.14 ),
    regionPhiMargin = cms.double( 0.4 ),
    posCalc_logweight = cms.bool( True ),
    posCalc_t0 = cms.double( 7.4 ),
    posCalc_w0 = cms.double( 4.2 ),
    posCalc_x0 = cms.double( 0.89 ),
    HybridBarrelSeedThr = cms.double( 1.5 ),
    step = cms.int32( 10 ),
    ethresh = cms.double( 0.1 ),
    eseed = cms.double( 0.35 ),
    ewing = cms.double( 1.0 )
)
hltCorrectedHybridSuperClustersL1NonIsolatedTemp = cms.EDProducer( "EgammaSCCorrectionMaker",
    VerbosityLevel = cms.string( "ERROR" ),
    recHitProducer = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEB' ),
    rawSuperClusterProducer = cms.InputTag( "hltHybridSuperClustersL1NonIsolated" ),
    superClusterAlgo = cms.string( "Hybrid" ),
    applyEnergyCorrection = cms.bool( True ),
    sigmaElectronicNoise = cms.double( 0.03 ),
    etThresh = cms.double( 5.0 ),
    corectedSuperClusterCollection = cms.string( "" ),
    hyb_fCorrPset = cms.PSet( 
      brLinearLowThr = cms.double( 1.1 ),
      fEtEtaVec = cms.vdouble( 1.0012, -0.5714, 0.0, 0.0, 0.0, 0.5549, 12.74, 1.0448, 0.0, 0.0, 0.0, 0.0, 8.0, 1.023, -0.00181, 0.0, 0.0 ),
      brLinearHighThr = cms.double( 8.0 ),
      fBremVec = cms.vdouble( -0.05208, 0.1331, 0.9196, -5.735E-4, 1.343 )
    ),
    isl_fCorrPset = cms.PSet(  ),
    dyn_fCorrPset = cms.PSet(  ),
    fix_fCorrPset = cms.PSet(  )
)
hltCorrectedHybridSuperClustersL1NonIsolated = cms.EDProducer( "EgammaHLTRemoveDuplicatedSC",
    L1NonIsoUskimmedSC = cms.InputTag( "hltCorrectedHybridSuperClustersL1NonIsolatedTemp" ),
    L1IsoSC = cms.InputTag( "hltCorrectedHybridSuperClustersL1Isolated" ),
    L1NonIsoSkimmedCollection = cms.string( "" )
)
hltMulti5x5BasicClustersL1NonIsolated = cms.EDProducer( "EgammaHLTMulti5x5ClusterProducer",
    VerbosityLevel = cms.string( "ERROR" ),
    doBarrel = cms.bool( False ),
    doEndcaps = cms.bool( True ),
    doIsolated = cms.bool( False ),
    barrelHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    endcapHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    barrelHitCollection = cms.string( "EcalRecHitsEB" ),
    endcapHitCollection = cms.string( "EcalRecHitsEE" ),
    barrelClusterCollection = cms.string( "notused" ),
    endcapClusterCollection = cms.string( "multi5x5EndcapBasicClusters" ),
    Multi5x5BarrelSeedThr = cms.double( 0.5 ),
    Multi5x5EndcapSeedThr = cms.double( 0.5 ),
    l1TagIsolated = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    l1TagNonIsolated = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    l1LowerThr = cms.double( 5.0 ),
    l1UpperThr = cms.double( 999.0 ),
    l1LowerThrIgnoreIsolation = cms.double( 999.0 ),
    regionEtaMargin = cms.double( 0.3 ),
    regionPhiMargin = cms.double( 0.4 ),
    posCalc_logweight = cms.bool( True ),
    posCalc_t0_barl = cms.double( 7.4 ),
    posCalc_t0_endc = cms.double( 3.1 ),
    posCalc_t0_endcPresh = cms.double( 1.2 ),
    posCalc_w0 = cms.double( 4.2 ),
    posCalc_x0 = cms.double( 0.89 )
)
hltMulti5x5SuperClustersL1NonIsolated = cms.EDProducer( "Multi5x5SuperClusterProducer",
    VerbosityLevel = cms.string( "ERROR" ),
    endcapClusterProducer = cms.string( "hltMulti5x5BasicClustersL1NonIsolated" ),
    barrelClusterProducer = cms.string( "notused" ),
    endcapClusterCollection = cms.string( "multi5x5EndcapBasicClusters" ),
    barrelClusterCollection = cms.string( "multi5x5BarrelBasicClusters" ),
    endcapSuperclusterCollection = cms.string( "multi5x5EndcapSuperClusters" ),
    barrelSuperclusterCollection = cms.string( "multi5x5BarrelSuperClusters" ),
    doBarrel = cms.bool( False ),
    doEndcaps = cms.bool( True ),
    barrelEtaSearchRoad = cms.double( 0.06 ),
    barrelPhiSearchRoad = cms.double( 0.8 ),
    endcapEtaSearchRoad = cms.double( 0.14 ),
    endcapPhiSearchRoad = cms.double( 0.6 ),
    seedTransverseEnergyThreshold = cms.double( 1.0 ),
    dynamicPhiRoad = cms.bool( False ),
    bremRecoveryPset = cms.PSet( 
      barrel = cms.PSet(  ),
      endcap = cms.PSet( 
        a = cms.double( 47.85 ),
        c = cms.double( 0.1201 ),
        b = cms.double( 108.8 )
      ),
      doEndcaps = cms.bool( True ),
      doBarrel = cms.bool( False )
    )
)
hltMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated = cms.EDProducer( "PreshowerClusterProducer",
    preshRecHitProducer = cms.InputTag( 'hltESRegionalEgammaRecHit','EcalRecHitsES' ),
    endcapSClusterProducer = cms.InputTag( 'hltMulti5x5SuperClustersL1NonIsolated','multi5x5EndcapSuperClusters' ),
    preshClusterCollectionX = cms.string( "preshowerXClusters" ),
    preshClusterCollectionY = cms.string( "preshowerYClusters" ),
    preshNclust = cms.int32( 4 ),
    etThresh = cms.double( 5.0 ),
    preshCalibPlaneX = cms.double( 1.0 ),
    preshCalibPlaneY = cms.double( 0.7 ),
    preshCalibGamma = cms.double( 0.024 ),
    preshCalibMIP = cms.double( 9.0E-5 ),
    assocSClusterCollection = cms.string( "" ),
    preshStripEnergyCut = cms.double( 0.0 ),
    preshSeededNstrip = cms.int32( 15 ),
    preshClusterEnergyCut = cms.double( 0.0 ),
    debugLevel = cms.string( "" )
)
hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolatedTemp = cms.EDProducer( "EgammaSCCorrectionMaker",
    VerbosityLevel = cms.string( "ERROR" ),
    recHitProducer = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEE' ),
    rawSuperClusterProducer = cms.InputTag( "hltMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated" ),
    superClusterAlgo = cms.string( "Multi5x5" ),
    applyEnergyCorrection = cms.bool( True ),
    sigmaElectronicNoise = cms.double( 0.15 ),
    etThresh = cms.double( 5.0 ),
    corectedSuperClusterCollection = cms.string( "" ),
    hyb_fCorrPset = cms.PSet(  ),
    isl_fCorrPset = cms.PSet(  ),
    dyn_fCorrPset = cms.PSet(  ),
    fix_fCorrPset = cms.PSet( 
      brLinearLowThr = cms.double( 0.6 ),
      fEtEtaVec = cms.vdouble( 0.9746, -6.512, 0.0, 0.0, 0.02771, 4.983, 0.0, 0.0, -0.007288, -0.9446, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0 ),
      brLinearHighThr = cms.double( 6.0 ),
      fBremVec = cms.vdouble( -0.04163, 0.08552, 0.95048, -0.002308, 1.077 )
    )
)
hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated = cms.EDProducer( "EgammaHLTRemoveDuplicatedSC",
    L1NonIsoUskimmedSC = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolatedTemp" ),
    L1IsoSC = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated" ),
    L1NonIsoSkimmedCollection = cms.string( "" )
)
hltL1IsoRecoEcalCandidate = cms.EDProducer( "EgammaHLTRecoEcalCandidateProducers",
    scHybridBarrelProducer = cms.InputTag( "hltCorrectedHybridSuperClustersL1Isolated" ),
    scIslandEndcapProducer = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated" ),
    recoEcalCandidateCollection = cms.string( "" )
)
hltL1NonIsoRecoEcalCandidate = cms.EDProducer( "EgammaHLTRecoEcalCandidateProducers",
    scHybridBarrelProducer = cms.InputTag( "hltCorrectedHybridSuperClustersL1NonIsolated" ),
    scIslandEndcapProducer = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated" ),
    recoEcalCandidateCollection = cms.string( "" )
)
hltL1NonIsoHLTNonIsoSingleElectronLWEt10L1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG5" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
hltL1NonIsoHLTNonIsoSingleElectronLWEt10EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10L1MatchFilterRegional" ),
    etcutEB = cms.double( 10.0 ),
    etcutEE = cms.double( 10.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1IsolatedElectronHcalIsol = cms.EDProducer( "EgammaHLTHcalIsolationProducersRegional",
    recoEcalCandidateProducer = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    hbRecHitProducer = cms.InputTag( "hltHbhereco" ),
    hfRecHitProducer = cms.InputTag( "hltHfreco" ),
    egHcalIsoPtMin = cms.double( 0.0 ),
    egHcalIsoConeSize = cms.double( 0.15 )
)
hltL1NonIsolatedElectronHcalIsol = cms.EDProducer( "EgammaHLTHcalIsolationProducersRegional",
    recoEcalCandidateProducer = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    hbRecHitProducer = cms.InputTag( "hltHbhereco" ),
    hfRecHitProducer = cms.InputTag( "hltHfreco" ),
    egHcalIsoPtMin = cms.double( 0.0 ),
    egHcalIsoConeSize = cms.double( 0.15 )
)
hltL1NonIsoHLTNonIsoSingleElectronLWEt10HcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10EtFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1IsoLargeWindowElectronPixelSeeds = cms.EDProducer( "ElectronSeedProducer",
    barrelSuperClusters = cms.InputTag( "hltCorrectedHybridSuperClustersL1Isolated" ),
    endcapSuperClusters = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated" ),
    SeedConfiguration = cms.PSet( 
      searchInTIDTEC = cms.bool( True ),
      HighPtThreshold = cms.double( 35.0 ),
      r2MinF = cms.double( -0.3 ),
      OrderedHitsFactoryPSet = cms.PSet( 
        ComponentName = cms.string( "StandardHitPairGenerator" ),
        SeedingLayers = cms.string( "MixedLayerPairs" ),
        useOnDemandTracker = cms.untracked.int32( 0 )
      ),
      DeltaPhi1Low = cms.double( 0.23 ),
      DeltaPhi1High = cms.double( 0.08 ),
      ePhiMin1 = cms.double( -0.045 ),
      PhiMin2 = cms.double( -0.01 ),
      LowPtThreshold = cms.double( 3.0 ),
      RegionPSet = cms.PSet( 
        deltaPhiRegion = cms.double( 0.4 ),
        originHalfLength = cms.double( 15.0 ),
        useZInVertex = cms.bool( True ),
        deltaEtaRegion = cms.double( 0.1 ),
        ptMin = cms.double( 1.5 ),
        originRadius = cms.double( 0.2 ),
        VertexProducer = cms.InputTag( "dummyVertices" )
      ),
      maxHOverE = cms.double( 999999.0 ),
      dynamicPhiRoad = cms.bool( False ),
      ePhiMax1 = cms.double( 0.03 ),
      DeltaPhi2 = cms.double( 0.0040 ),
      SizeWindowENeg = cms.double( 0.675 ),
      nSigmasDeltaZ1 = cms.double( 5.0 ),
      rMaxI = cms.double( 0.2 ),
      rMinI = cms.double( -0.2 ),
      preFilteredSeeds = cms.bool( True ),
      r2MaxF = cms.double( 0.3 ),
      pPhiMin1 = cms.double( -0.03 ),
      initialSeeds = cms.InputTag( "noSeedsHere" ),
      pPhiMax1 = cms.double( 0.045 ),
      hbheModule = cms.string( "hbhereco" ),
      SCEtCut = cms.double( 3.0 ),
      z2MaxB = cms.double( 0.2 ),
      fromTrackerSeeds = cms.bool( True ),
      hcalRecHits = cms.InputTag( "hltHbhereco" ),
      z2MinB = cms.double( -0.2 ),
      hbheInstance = cms.string( "" ),
      PhiMax2 = cms.double( 0.01 ),
      hOverEConeSize = cms.double( 0.0 ),
      hOverEHBMinE = cms.double( 999999.0 ),
      applyHOverECut = cms.bool( False ),
      hOverEHFMinE = cms.double( 999999.0 )
    )
)
hltL1NonIsoLargeWindowElectronPixelSeeds = cms.EDProducer( "ElectronSeedProducer",
    barrelSuperClusters = cms.InputTag( "hltCorrectedHybridSuperClustersL1NonIsolated" ),
    endcapSuperClusters = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated" ),
    SeedConfiguration = cms.PSet( 
      searchInTIDTEC = cms.bool( True ),
      HighPtThreshold = cms.double( 35.0 ),
      r2MinF = cms.double( -0.3 ),
      OrderedHitsFactoryPSet = cms.PSet( 
        ComponentName = cms.string( "StandardHitPairGenerator" ),
        SeedingLayers = cms.string( "MixedLayerPairs" ),
        useOnDemandTracker = cms.untracked.int32( 0 )
      ),
      DeltaPhi1Low = cms.double( 0.23 ),
      DeltaPhi1High = cms.double( 0.08 ),
      ePhiMin1 = cms.double( -0.045 ),
      PhiMin2 = cms.double( -0.01 ),
      LowPtThreshold = cms.double( 3.0 ),
      RegionPSet = cms.PSet( 
        deltaPhiRegion = cms.double( 0.4 ),
        originHalfLength = cms.double( 15.0 ),
        useZInVertex = cms.bool( True ),
        deltaEtaRegion = cms.double( 0.1 ),
        ptMin = cms.double( 1.5 ),
        originRadius = cms.double( 0.2 ),
        VertexProducer = cms.InputTag( "dummyVertices" )
      ),
      maxHOverE = cms.double( 999999.0 ),
      dynamicPhiRoad = cms.bool( False ),
      ePhiMax1 = cms.double( 0.03 ),
      DeltaPhi2 = cms.double( 0.0040 ),
      SizeWindowENeg = cms.double( 0.675 ),
      nSigmasDeltaZ1 = cms.double( 5.0 ),
      rMaxI = cms.double( 0.2 ),
      rMinI = cms.double( -0.2 ),
      preFilteredSeeds = cms.bool( True ),
      r2MaxF = cms.double( 0.3 ),
      pPhiMin1 = cms.double( -0.03 ),
      initialSeeds = cms.InputTag( "noSeedsHere" ),
      pPhiMax1 = cms.double( 0.045 ),
      hbheModule = cms.string( "hbhereco" ),
      SCEtCut = cms.double( 3.0 ),
      z2MaxB = cms.double( 0.2 ),
      fromTrackerSeeds = cms.bool( True ),
      hcalRecHits = cms.InputTag( "hltHbhereco" ),
      z2MinB = cms.double( -0.2 ),
      hbheInstance = cms.string( "" ),
      PhiMax2 = cms.double( 0.01 ),
      hOverEConeSize = cms.double( 0.0 ),
      hOverEHBMinE = cms.double( 999999.0 ),
      applyHOverECut = cms.bool( False ),
      hOverEHFMinE = cms.double( 999999.0 )
    )
)
hltL1NonIsoHLTNonIsoSingleElectronLWEt10PixelMatchFilter = cms.EDFilter( "HLTElectronPixelMatchFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10HcalIsolFilter" ),
    L1IsoPixelSeedsTag = cms.InputTag( "hltL1IsoLargeWindowElectronPixelSeeds" ),
    L1NonIsoPixelSeedsTag = cms.InputTag( "hltL1NonIsoLargeWindowElectronPixelSeeds" ),
    npixelmatchcut = cms.double( 1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltPreEle10LWEleIdL1R = cms.EDFilter( "HLTPrescaler" )
hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdL1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG5" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdEtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdL1MatchFilterRegional" ),
    etcutEB = cms.double( 10.0 ),
    etcutEE = cms.double( 10.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1IsoHLTClusterShape = cms.EDProducer( "EgammaHLTClusterShapeProducer",
    recoEcalCandidateProducer = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    ecalRechitEB = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEB' ),
    ecalRechitEE = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEE' ),
    isIeta = cms.bool( False )
)
hltL1NonIsoHLTClusterShape = cms.EDProducer( "EgammaHLTClusterShapeProducer",
    recoEcalCandidateProducer = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    ecalRechitEB = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEB' ),
    ecalRechitEE = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEE' ),
    isIeta = cms.bool( False )
)
hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdClusterShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdEtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoHLTClusterShape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoHLTClusterShape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 0.015 ),
    thrRegularEE = cms.double( 0.04 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdHcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdClusterShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdPixelMatchFilter = cms.EDFilter( "HLTElectronPixelMatchFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdHcalIsolFilter" ),
    L1IsoPixelSeedsTag = cms.InputTag( "hltL1IsoLargeWindowElectronPixelSeeds" ),
    L1NonIsoPixelSeedsTag = cms.InputTag( "hltL1NonIsoLargeWindowElectronPixelSeeds" ),
    npixelmatchcut = cms.double( 1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltCkfL1IsoLargeWindowTrackCandidates = cms.EDProducer( "CkfTrackCandidateMaker",
    src = cms.InputTag( "hltL1IsoLargeWindowElectronPixelSeeds" ),
    TrajectoryBuilder = cms.string( "CkfTrajectoryBuilder" ),
    TrajectoryCleaner = cms.string( "TrajectoryCleanerBySharedHits" ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    useHitsSplitting = cms.bool( False ),
    doSeedingRegionRebuilding = cms.bool( False ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    cleanTrajectoryAfterInOut = cms.bool( False )
)
hltCtfL1IsoLargeWindowWithMaterialTracks = cms.EDProducer( "TrackProducer",
    TrajectoryInEvent = cms.bool( True ),
    useHitsSplitting = cms.bool( False ),
    clusterRemovalInfo = cms.InputTag( "" ),
    alias = cms.untracked.string( "ctfWithMaterialTracks" ),
    Fitter = cms.string( "hltKFFittingSmoother" ),
    Propagator = cms.string( "PropagatorWithMaterial" ),
    src = cms.InputTag( "hltCkfL1IsoLargeWindowTrackCandidates" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    NavigationSchool = cms.string( "" )
)
hltPixelMatchLargeWindowElectronsL1Iso = cms.EDProducer( "EgammaHLTPixelMatchElectronProducers",
    TrackProducer = cms.InputTag( "hltCtfL1IsoLargeWindowWithMaterialTracks" ),
    BSProducer = cms.InputTag( "hltOfflineBeamSpot" )
)
hltCkfL1NonIsoLargeWindowTrackCandidates = cms.EDProducer( "CkfTrackCandidateMaker",
    src = cms.InputTag( "hltL1NonIsoLargeWindowElectronPixelSeeds" ),
    TrajectoryBuilder = cms.string( "CkfTrajectoryBuilder" ),
    TrajectoryCleaner = cms.string( "TrajectoryCleanerBySharedHits" ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    useHitsSplitting = cms.bool( False ),
    doSeedingRegionRebuilding = cms.bool( False ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    cleanTrajectoryAfterInOut = cms.bool( False )
)
hltCtfL1NonIsoLargeWindowWithMaterialTracks = cms.EDProducer( "TrackProducer",
    TrajectoryInEvent = cms.bool( False ),
    useHitsSplitting = cms.bool( False ),
    clusterRemovalInfo = cms.InputTag( "" ),
    alias = cms.untracked.string( "ctfWithMaterialTracks" ),
    Fitter = cms.string( "hltKFFittingSmoother" ),
    Propagator = cms.string( "PropagatorWithMaterial" ),
    src = cms.InputTag( "hltCkfL1NonIsoLargeWindowTrackCandidates" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    NavigationSchool = cms.string( "" )
)
hltPixelMatchLargeWindowElectronsL1NonIso = cms.EDProducer( "EgammaHLTPixelMatchElectronProducers",
    TrackProducer = cms.InputTag( "hltCtfL1NonIsoLargeWindowWithMaterialTracks" ),
    BSProducer = cms.InputTag( "hltOfflineBeamSpot" )
)
hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdOneOEMinusOneOPFilter = cms.EDFilter( "HLTElectronOneOEMinusOneOPFilterRegional",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdPixelMatchFilter" ),
    electronIsolatedProducer = cms.InputTag( "hltPixelMatchLargeWindowElectronsL1Iso" ),
    electronNonIsolatedProducer = cms.InputTag( "hltPixelMatchLargeWindowElectronsL1NonIso" ),
    barrelcut = cms.double( 999.9 ),
    endcapcut = cms.double( 999.9 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False )
)
hltElectronL1IsoLargeWindowDetaDphi = cms.EDProducer( "EgammaHLTElectronDetaDphiProducer",
    electronProducer = cms.InputTag( "hltPixelMatchLargeWindowElectronsL1Iso" ),
    BSProducer = cms.InputTag( "hltOfflineBeamSpot" )
)
hltElectronL1NonIsoLargeWindowDetaDphi = cms.EDProducer( "EgammaHLTElectronDetaDphiProducer",
    electronProducer = cms.InputTag( "hltPixelMatchLargeWindowElectronsL1NonIso" ),
    BSProducer = cms.InputTag( "hltOfflineBeamSpot" )
)
hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDetaFilter = cms.EDFilter( "HLTElectronGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdOneOEMinusOneOPFilter" ),
    isoTag = cms.InputTag( 'hltElectronL1IsoLargeWindowDetaDphi','Deta' ),
    nonIsoTag = cms.InputTag( 'hltElectronL1NonIsoLargeWindowDetaDphi','Deta' ),
    lessThan = cms.bool( True ),
    thrRegularEB = cms.double( 0.0080 ),
    thrRegularEE = cms.double( 0.011 ),
    thrOverPtEB = cms.double( -1.0 ),
    thrOverPtEE = cms.double( -1.0 ),
    thrTimesPtEB = cms.double( -1.0 ),
    thrTimesPtEE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDphiFilter = cms.EDFilter( "HLTElectronGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDetaFilter" ),
    isoTag = cms.InputTag( 'hltElectronL1IsoLargeWindowDetaDphi','Dphi' ),
    nonIsoTag = cms.InputTag( 'hltElectronL1NonIsoLargeWindowDetaDphi','Dphi' ),
    lessThan = cms.bool( True ),
    thrRegularEB = cms.double( 0.1 ),
    thrRegularEE = cms.double( 0.1 ),
    thrOverPtEB = cms.double( -1.0 ),
    thrOverPtEE = cms.double( -1.0 ),
    thrTimesPtEB = cms.double( -1.0 ),
    thrTimesPtEE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltPixelMatchLargeWindowElectronsL1Iso" ),
    L1NonIsoCand = cms.InputTag( "hltPixelMatchLargeWindowElectronsL1NonIso" )
)
hltPreEle15LWL1R = cms.EDFilter( "HLTPrescaler" )
hltL1NonIsoHLTNonIsoSingleElectronLWEt15L1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG8" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
hltL1NonIsoHLTNonIsoSingleElectronLWEt15EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt15L1MatchFilterRegional" ),
    etcutEB = cms.double( 15.0 ),
    etcutEE = cms.double( 15.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoHLTNonIsoSingleElectronLWEt15HcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt15EtFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoHLTNonIsoSingleElectronLWEt15PixelMatchFilter = cms.EDFilter( "HLTElectronPixelMatchFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt15HcalIsolFilter" ),
    L1IsoPixelSeedsTag = cms.InputTag( "hltL1IsoLargeWindowElectronPixelSeeds" ),
    L1NonIsoPixelSeedsTag = cms.InputTag( "hltL1NonIsoLargeWindowElectronPixelSeeds" ),
    npixelmatchcut = cms.double( 1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltPreEle15SC10LWL1R = cms.EDFilter( "HLTPrescaler" )
hltL1NonIsoHLTNonIsoSingleElectronLWEt15ESscWrapper = cms.EDFilter( "HLTEgammaTriggerFilterObjectWrapper",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    doIsolated = cms.bool( False )
)
hltL1NonIsoHLTNonIsoSingleElectronLWEt15ESDoubleSC10 = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt15ESscWrapper" ),
    etcutEB = cms.double( 10.0 ),
    etcutEE = cms.double( 10.0 ),
    ncandcut = cms.int32( 2 ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltPreEle15SiStripL1R = cms.EDFilter( "HLTPrescaler" )
hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15L1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG8" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15L1MatchFilterRegional" ),
    etcutEB = cms.double( 15.0 ),
    etcutEE = cms.double( 15.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15HcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15EtFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1IsoSiStripElectronPixelSeeds = cms.EDProducer( "SiStripElectronSeedProducer",
    barrelSuperClusters = cms.InputTag( "hltCorrectedHybridSuperClustersL1Isolated" ),
    endcapSuperClusters = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated" )
)
hltL1NonIsoSiStripElectronPixelSeeds = cms.EDProducer( "SiStripElectronSeedProducer",
    barrelSuperClusters = cms.InputTag( "hltCorrectedHybridSuperClustersL1NonIsolated" ),
    endcapSuperClusters = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated" )
)
hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15PixelMatchFilter = cms.EDFilter( "HLTElectronPixelMatchFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15HcalIsolFilter" ),
    L1IsoPixelSeedsTag = cms.InputTag( "hltL1IsoSiStripElectronPixelSeeds" ),
    L1NonIsoPixelSeedsTag = cms.InputTag( "hltL1NonIsoSiStripElectronPixelSeeds" ),
    npixelmatchcut = cms.double( 1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltPreEle20LWL1R = cms.EDFilter( "HLTPrescaler" )
hltL1NonIsoHLTNonIsoSingleElectronLWEt15EtFilterESet20 = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt15PixelMatchFilter" ),
    etcutEB = cms.double( 20.0 ),
    etcutEE = cms.double( 20.0 ),
    ncandcut = cms.int32( 1 ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1sL1DoubleEG5 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_DoubleEG5" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1DoubleEG5 = cms.EDFilter( "HLTPrescaler" )
hltPreDoubleEle5SWL1R = cms.EDFilter( "HLTPrescaler" )
hltL1NonIsoHLTNonIsoDoubleElectronEt5L1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1DoubleEG5" ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
hltL1NonIsoHLTNonIsoDoubleElectronEt5EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoDoubleElectronEt5L1MatchFilterRegional" ),
    etcutEB = cms.double( 5.0 ),
    etcutEE = cms.double( 5.0 ),
    ncandcut = cms.int32( 2 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoHLTNonIsoDoubleElectronEt5HcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoDoubleElectronEt5EtFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1IsoStartUpElectronPixelSeeds = cms.EDProducer( "ElectronSeedProducer",
    barrelSuperClusters = cms.InputTag( "hltCorrectedHybridSuperClustersL1Isolated" ),
    endcapSuperClusters = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated" ),
    SeedConfiguration = cms.PSet( 
      searchInTIDTEC = cms.bool( True ),
      HighPtThreshold = cms.double( 35.0 ),
      r2MinF = cms.double( -0.08 ),
      OrderedHitsFactoryPSet = cms.PSet( 
        ComponentName = cms.string( "StandardHitPairGenerator" ),
        SeedingLayers = cms.string( "MixedLayerPairs" ),
        useOnDemandTracker = cms.untracked.int32( 0 )
      ),
      DeltaPhi1Low = cms.double( 0.23 ),
      DeltaPhi1High = cms.double( 0.08 ),
      ePhiMin1 = cms.double( -0.035 ),
      PhiMin2 = cms.double( -0.0050 ),
      LowPtThreshold = cms.double( 3.0 ),
      RegionPSet = cms.PSet( 
        deltaPhiRegion = cms.double( 0.4 ),
        originHalfLength = cms.double( 15.0 ),
        useZInVertex = cms.bool( True ),
        deltaEtaRegion = cms.double( 0.1 ),
        ptMin = cms.double( 1.5 ),
        originRadius = cms.double( 0.2 ),
        VertexProducer = cms.InputTag( "dummyVertices" )
      ),
      maxHOverE = cms.double( 999999.0 ),
      dynamicPhiRoad = cms.bool( False ),
      ePhiMax1 = cms.double( 0.025 ),
      DeltaPhi2 = cms.double( 0.0040 ),
      SizeWindowENeg = cms.double( 0.675 ),
      nSigmasDeltaZ1 = cms.double( 5.0 ),
      rMaxI = cms.double( 0.11 ),
      rMinI = cms.double( -0.11 ),
      preFilteredSeeds = cms.bool( True ),
      r2MaxF = cms.double( 0.08 ),
      pPhiMin1 = cms.double( -0.025 ),
      initialSeeds = cms.InputTag( "noSeedsHere" ),
      pPhiMax1 = cms.double( 0.035 ),
      hbheModule = cms.string( "hbhereco" ),
      SCEtCut = cms.double( 3.0 ),
      z2MaxB = cms.double( 0.05 ),
      fromTrackerSeeds = cms.bool( True ),
      hcalRecHits = cms.InputTag( "hltHbhereco" ),
      z2MinB = cms.double( -0.05 ),
      hbheInstance = cms.string( "" ),
      PhiMax2 = cms.double( 0.0050 ),
      hOverEConeSize = cms.double( 0.0 ),
      hOverEHBMinE = cms.double( 999999.0 ),
      applyHOverECut = cms.bool( False ),
      hOverEHFMinE = cms.double( 999999.0 )
    )
)
hltL1NonIsoStartUpElectronPixelSeeds = cms.EDProducer( "ElectronSeedProducer",
    barrelSuperClusters = cms.InputTag( "hltCorrectedHybridSuperClustersL1NonIsolated" ),
    endcapSuperClusters = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated" ),
    SeedConfiguration = cms.PSet( 
      searchInTIDTEC = cms.bool( True ),
      HighPtThreshold = cms.double( 35.0 ),
      r2MinF = cms.double( -0.08 ),
      OrderedHitsFactoryPSet = cms.PSet( 
        ComponentName = cms.string( "StandardHitPairGenerator" ),
        SeedingLayers = cms.string( "MixedLayerPairs" ),
        useOnDemandTracker = cms.untracked.int32( 0 )
      ),
      DeltaPhi1Low = cms.double( 0.23 ),
      DeltaPhi1High = cms.double( 0.08 ),
      ePhiMin1 = cms.double( -0.035 ),
      PhiMin2 = cms.double( -0.0050 ),
      LowPtThreshold = cms.double( 3.0 ),
      RegionPSet = cms.PSet( 
        deltaPhiRegion = cms.double( 0.4 ),
        originHalfLength = cms.double( 15.0 ),
        useZInVertex = cms.bool( True ),
        deltaEtaRegion = cms.double( 0.1 ),
        ptMin = cms.double( 1.5 ),
        originRadius = cms.double( 0.2 ),
        VertexProducer = cms.InputTag( "dummyVertices" )
      ),
      maxHOverE = cms.double( 999999.0 ),
      dynamicPhiRoad = cms.bool( False ),
      ePhiMax1 = cms.double( 0.025 ),
      DeltaPhi2 = cms.double( 0.0040 ),
      SizeWindowENeg = cms.double( 0.675 ),
      nSigmasDeltaZ1 = cms.double( 5.0 ),
      rMaxI = cms.double( 0.11 ),
      rMinI = cms.double( -0.11 ),
      preFilteredSeeds = cms.bool( True ),
      r2MaxF = cms.double( 0.08 ),
      pPhiMin1 = cms.double( -0.025 ),
      initialSeeds = cms.InputTag( "noSeedsHere" ),
      pPhiMax1 = cms.double( 0.035 ),
      hbheModule = cms.string( "hbhereco" ),
      SCEtCut = cms.double( 3.0 ),
      z2MaxB = cms.double( 0.05 ),
      fromTrackerSeeds = cms.bool( True ),
      hcalRecHits = cms.InputTag( "hltHbhereco" ),
      z2MinB = cms.double( -0.05 ),
      hbheInstance = cms.string( "" ),
      PhiMax2 = cms.double( 0.0050 ),
      hOverEConeSize = cms.double( 0.0 ),
      hOverEHBMinE = cms.double( 999999.0 ),
      applyHOverECut = cms.bool( False ),
      hOverEHFMinE = cms.double( 999999.0 )
    )
)
hltL1NonIsoHLTNonIsoDoubleElectronEt5PixelMatchFilter = cms.EDFilter( "HLTElectronPixelMatchFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoDoubleElectronEt5HcalIsolFilter" ),
    L1IsoPixelSeedsTag = cms.InputTag( "hltL1IsoStartUpElectronPixelSeeds" ),
    L1NonIsoPixelSeedsTag = cms.InputTag( "hltL1NonIsoStartUpElectronPixelSeeds" ),
    npixelmatchcut = cms.double( 1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1sL1SingleEG8orL1DoubleEG5 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleEG8 OR L1_DoubleEG5" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreDoublePhoton5eeResL1R = cms.EDFilter( "HLTPrescaler" )
hltL1NonIsoDoublePhotonEt5eeResL1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG8orL1DoubleEG5" ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
hltL1NonIsoDoublePhotonEt5eeResEtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5eeResL1MatchFilterRegional" ),
    etcutEB = cms.double( 4.0 ),
    etcutEE = cms.double( 4.0 ),
    ncandcut = cms.int32( 2 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoDoublePhotonEt5eeResClusterShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5eeResEtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoHLTClusterShape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoHLTClusterShape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 0.016 ),
    thrRegularEE = cms.double( 0.042 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1IsolatedPhotonEcalIsol = cms.EDProducer( "EgammaHLTEcalRecIsolationProducer",
    recoEcalCandidateProducer = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    ecalBarrelRecHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    ecalBarrelRecHitCollection = cms.InputTag( "EcalRecHitsEB" ),
    ecalEndcapRecHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    ecalEndcapRecHitCollection = cms.InputTag( "EcalRecHitsEE" ),
    etMinBarrel = cms.double( -9999.0 ),
    eMinBarrel = cms.double( 0.08 ),
    etMinEndcap = cms.double( -9999.0 ),
    eMinEndcap = cms.double( 0.3 ),
    intRadiusBarrel = cms.double( 0.045 ),
    intRadiusEndcap = cms.double( 0.07 ),
    extRadius = cms.double( 0.4 ),
    jurassicWidth = cms.double( 0.02 ),
    useIsolEt = cms.bool( True ),
    tryBoth = cms.bool( True ),
    subtract = cms.bool( False )
)
hltL1NonIsolatedPhotonEcalIsol = cms.EDProducer( "EgammaHLTEcalRecIsolationProducer",
    recoEcalCandidateProducer = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    ecalBarrelRecHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    ecalBarrelRecHitCollection = cms.InputTag( "EcalRecHitsEB" ),
    ecalEndcapRecHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    ecalEndcapRecHitCollection = cms.InputTag( "EcalRecHitsEE" ),
    etMinBarrel = cms.double( -9999.0 ),
    eMinBarrel = cms.double( 0.08 ),
    etMinEndcap = cms.double( -9999.0 ),
    eMinEndcap = cms.double( 0.3 ),
    intRadiusBarrel = cms.double( 0.045 ),
    intRadiusEndcap = cms.double( 0.07 ),
    extRadius = cms.double( 0.4 ),
    jurassicWidth = cms.double( 0.02 ),
    useIsolEt = cms.bool( True ),
    tryBoth = cms.bool( True ),
    subtract = cms.bool( False )
)
hltL1NonIsoDoublePhotonEt5eeResEcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5eeResClusterShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonEcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonEcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoDoublePhotonEt5eeResHcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5eeResEcalIsolFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoDoublePhotonEt5eeResPMMassFilter = cms.EDFilter( "HLTPMMassFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5eeResHcalIsolFilter" ),
    lowerMassCut = cms.double( 2.0 ),
    upperMassCut = cms.double( 999999.9 ),
    nZcandcut = cms.int32( 1 ),
    reqOppCharge = cms.untracked.bool( True ),
    isElectron1 = cms.untracked.bool( False ),
    isElectron2 = cms.untracked.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltPreDoublePhoton5JpsiL1R = cms.EDFilter( "HLTPrescaler" )
hltL1NonIsoDoublePhotonEt5JpsiL1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG8orL1DoubleEG5" ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
hltL1NonIsoDoublePhotonEt5JpsiEtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5JpsiL1MatchFilterRegional" ),
    etcutEB = cms.double( 4.0 ),
    etcutEE = cms.double( 4.0 ),
    ncandcut = cms.int32( 2 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoDoublePhotonEt5JpsiClusterShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5JpsiEtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoHLTClusterShape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoHLTClusterShape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 0.016 ),
    thrRegularEE = cms.double( 0.042 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoDoublePhotonEt5JpsiEcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5JpsiClusterShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonEcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonEcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoDoublePhotonEt5JpsiHcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5JpsiEcalIsolFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoDoublePhotonEt5JpsiPMMassFilter = cms.EDFilter( "HLTPMMassFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5JpsiHcalIsolFilter" ),
    lowerMassCut = cms.double( 2.0 ),
    upperMassCut = cms.double( 4.6 ),
    nZcandcut = cms.int32( 1 ),
    reqOppCharge = cms.untracked.bool( True ),
    isElectron1 = cms.untracked.bool( False ),
    isElectron2 = cms.untracked.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltPreDoublePhoton5UpsL1R = cms.EDFilter( "HLTPrescaler" )
hltL1NonIsoDoublePhotonEt5UpsL1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG8orL1DoubleEG5" ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
hltL1NonIsoDoublePhotonEt5UpsEtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5UpsL1MatchFilterRegional" ),
    etcutEB = cms.double( 4.0 ),
    etcutEE = cms.double( 4.0 ),
    ncandcut = cms.int32( 2 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoDoublePhotonEt5UpsClusterShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5UpsEtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoHLTClusterShape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoHLTClusterShape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 0.016 ),
    thrRegularEE = cms.double( 0.042 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoDoublePhotonEt5UpsEcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5UpsClusterShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonEcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonEcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoDoublePhotonEt5UpsHcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5UpsEcalIsolFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoDoublePhotonEt5UpsPMMassFilter = cms.EDFilter( "HLTPMMassFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5UpsHcalIsolFilter" ),
    lowerMassCut = cms.double( 8.0 ),
    upperMassCut = cms.double( 11.0 ),
    nZcandcut = cms.int32( 1 ),
    reqOppCharge = cms.untracked.bool( True ),
    isElectron1 = cms.untracked.bool( False ),
    isElectron2 = cms.untracked.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltPrePhoton10L1R = cms.EDFilter( "HLTPrescaler" )
hltL1NonIsoHLTNonIsoSinglePhotonEt10L1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG5" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
hltL1NonIsoHLTNonIsoSinglePhotonEt10EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSinglePhotonEt10L1MatchFilterRegional" ),
    etcutEB = cms.double( 10.0 ),
    etcutEE = cms.double( 10.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1IsolatedPhotonHcalIsol = cms.EDProducer( "EgammaHLTHcalIsolationProducersRegional",
    recoEcalCandidateProducer = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    hbRecHitProducer = cms.InputTag( "hltHbhereco" ),
    hfRecHitProducer = cms.InputTag( "hltHfreco" ),
    egHcalIsoPtMin = cms.double( 0.0 ),
    egHcalIsoConeSize = cms.double( 0.3 )
)
hltL1NonIsolatedPhotonHcalIsol = cms.EDProducer( "EgammaHLTHcalIsolationProducersRegional",
    recoEcalCandidateProducer = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    hbRecHitProducer = cms.InputTag( "hltHbhereco" ),
    hfRecHitProducer = cms.InputTag( "hltHfreco" ),
    egHcalIsoPtMin = cms.double( 0.0 ),
    egHcalIsoConeSize = cms.double( 0.3 )
)
hltL1NonIsoHLTNonIsoSinglePhotonEt10HcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSinglePhotonEt10EtFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltPrePhoton15L1R = cms.EDFilter( "HLTPrescaler" )
hltL1NonIsoHLTNonIsoSinglePhotonEt15L1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG8" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
hltL1NonIsoHLTNonIsoSinglePhotonEt15EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSinglePhotonEt15L1MatchFilterRegional" ),
    etcutEB = cms.double( 15.0 ),
    etcutEE = cms.double( 15.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoHLTNonIsoSinglePhotonEt15HcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSinglePhotonEt15EtFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltPrePhoton15HTIL1R = cms.EDFilter( "HLTPrescaler" )
hltL1NonIsoSinglePhotonEt15HTIL1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG8" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
hltL1NonIsoSinglePhotonEt15HTIEtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoSinglePhotonEt15HTIL1MatchFilterRegional" ),
    etcutEB = cms.double( 15.0 ),
    etcutEE = cms.double( 15.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoSinglePhotonEt15HTIHcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoSinglePhotonEt15HTIEtFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1IsoEgammaRegionalPixelSeedGenerator = cms.EDProducer( "EgammaHLTRegionalPixelSeedGeneratorProducers",
    ptMin = cms.double( 1.5 ),
    vertexZ = cms.double( 0.0 ),
    originRadius = cms.double( 0.02 ),
    originHalfLength = cms.double( 15.0 ),
    deltaEtaRegion = cms.double( 0.3 ),
    deltaPhiRegion = cms.double( 0.3 ),
    candTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    candTagEle = cms.InputTag( "pixelMatchElectrons" ),
    UseZInVertex = cms.bool( False ),
    BSProducer = cms.InputTag( "hltOfflineBeamSpot" ),
    OrderedHitsFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "StandardHitPairGenerator" ),
      SeedingLayers = cms.string( "PixelLayerPairs" )
    )
)
hltL1IsoEgammaRegionalCkfTrackCandidates = cms.EDProducer( "CkfTrackCandidateMaker",
    src = cms.InputTag( "hltL1IsoEgammaRegionalPixelSeedGenerator" ),
    TrajectoryBuilder = cms.string( "CkfTrajectoryBuilder" ),
    TrajectoryCleaner = cms.string( "TrajectoryCleanerBySharedHits" ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    useHitsSplitting = cms.bool( False ),
    doSeedingRegionRebuilding = cms.bool( False ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    cleanTrajectoryAfterInOut = cms.bool( False )
)
hltL1IsoEgammaRegionalCTFFinalFitWithMaterial = cms.EDProducer( "TrackProducer",
    TrajectoryInEvent = cms.bool( False ),
    useHitsSplitting = cms.bool( False ),
    clusterRemovalInfo = cms.InputTag( "" ),
    alias = cms.untracked.string( "hltEgammaRegionalCTFFinalFitWithMaterial" ),
    Fitter = cms.string( "hltKFFittingSmoother" ),
    Propagator = cms.string( "PropagatorWithMaterial" ),
    src = cms.InputTag( "hltL1IsoEgammaRegionalCkfTrackCandidates" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    NavigationSchool = cms.string( "" )
)
hltL1NonIsoEgammaRegionalPixelSeedGenerator = cms.EDProducer( "EgammaHLTRegionalPixelSeedGeneratorProducers",
    ptMin = cms.double( 1.5 ),
    vertexZ = cms.double( 0.0 ),
    originRadius = cms.double( 0.02 ),
    originHalfLength = cms.double( 15.0 ),
    deltaEtaRegion = cms.double( 0.3 ),
    deltaPhiRegion = cms.double( 0.3 ),
    candTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    candTagEle = cms.InputTag( "pixelMatchElectrons" ),
    UseZInVertex = cms.bool( False ),
    BSProducer = cms.InputTag( "hltOfflineBeamSpot" ),
    OrderedHitsFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "StandardHitPairGenerator" ),
      SeedingLayers = cms.string( "PixelLayerPairs" )
    )
)
hltL1NonIsoEgammaRegionalCkfTrackCandidates = cms.EDProducer( "CkfTrackCandidateMaker",
    src = cms.InputTag( "hltL1NonIsoEgammaRegionalPixelSeedGenerator" ),
    TrajectoryBuilder = cms.string( "CkfTrajectoryBuilder" ),
    TrajectoryCleaner = cms.string( "TrajectoryCleanerBySharedHits" ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    useHitsSplitting = cms.bool( False ),
    doSeedingRegionRebuilding = cms.bool( False ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    cleanTrajectoryAfterInOut = cms.bool( False )
)
hltL1NonIsoEgammaRegionalCTFFinalFitWithMaterial = cms.EDProducer( "TrackProducer",
    TrajectoryInEvent = cms.bool( False ),
    useHitsSplitting = cms.bool( False ),
    clusterRemovalInfo = cms.InputTag( "" ),
    alias = cms.untracked.string( "hltEgammaRegionalCTFFinalFitWithMaterial" ),
    Fitter = cms.string( "hltKFFittingSmoother" ),
    Propagator = cms.string( "PropagatorWithMaterial" ),
    src = cms.InputTag( "hltL1NonIsoEgammaRegionalCkfTrackCandidates" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    NavigationSchool = cms.string( "" )
)
hltL1IsoPhotonHollowTrackIsol = cms.EDProducer( "EgammaHLTPhotonTrackIsolationProducersRegional",
    recoEcalCandidateProducer = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    trackProducer = cms.InputTag( "hltL1IsoEgammaRegionalCTFFinalFitWithMaterial" ),
    countTracks = cms.bool( False ),
    egTrkIsoPtMin = cms.double( 1.5 ),
    egTrkIsoConeSize = cms.double( 0.3 ),
    egTrkIsoZSpan = cms.double( 999999.0 ),
    egTrkIsoRSpan = cms.double( 999999.0 ),
    egTrkIsoVetoConeSize = cms.double( 0.06 )
)
hltL1NonIsoPhotonHollowTrackIsol = cms.EDProducer( "EgammaHLTPhotonTrackIsolationProducersRegional",
    recoEcalCandidateProducer = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    trackProducer = cms.InputTag( "hltL1NonIsoEgammaRegionalCTFFinalFitWithMaterial" ),
    countTracks = cms.bool( False ),
    egTrkIsoPtMin = cms.double( 1.5 ),
    egTrkIsoConeSize = cms.double( 0.3 ),
    egTrkIsoZSpan = cms.double( 999999.0 ),
    egTrkIsoRSpan = cms.double( 999999.0 ),
    egTrkIsoVetoConeSize = cms.double( 0.06 )
)
hltL1NonIsoSinglePhotonEt15HTITrackIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoSinglePhotonEt15HTIHcalIsolFilter" ),
    isoTag = cms.InputTag( "hltL1IsoPhotonHollowTrackIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoPhotonHollowTrackIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( True ),
    thrRegularEB = cms.double( 4.0 ),
    thrRegularEE = cms.double( 4.0 ),
    thrOverEEB = cms.double( 0.05 ),
    thrOverEEE = cms.double( 0.05 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltPrePhoton15LEIL1R = cms.EDFilter( "HLTPrescaler" )
hltL1NonIsoSinglePhotonEt15LEIL1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG8" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
hltL1NonIsoSinglePhotonEt15LEIEtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoSinglePhotonEt15LEIL1MatchFilterRegional" ),
    etcutEB = cms.double( 15.0 ),
    etcutEE = cms.double( 15.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoSinglePhotonEt15LEIEcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoSinglePhotonEt15LEIEtFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonEcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonEcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 3.0 ),
    thrRegularEE = cms.double( 3.0 ),
    thrOverEEB = cms.double( 0.1 ),
    thrOverEEE = cms.double( 0.1 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoSinglePhotonEt15LEIHcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoSinglePhotonEt15LEIEcalIsolFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltPrePhoton20L1R = cms.EDFilter( "HLTPrescaler" )
hltL1NonIsoHLTNonIsoSinglePhotonEt20L1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG8" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
hltL1NonIsoHLTNonIsoSinglePhotonEt20EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSinglePhotonEt20L1MatchFilterRegional" ),
    etcutEB = cms.double( 20.0 ),
    etcutEE = cms.double( 20.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoHLTNonIsoSinglePhotonEt20HcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSinglePhotonEt20EtFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltPrePhoton30L1R8E29 = cms.EDFilter( "HLTPrescaler" )
hltL1NonIsoHLTNonIsoSinglePhotonEt15EtFilterESet308E29 = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSinglePhotonEt15EtFilter" ),
    etcutEB = cms.double( 30.0 ),
    etcutEE = cms.double( 30.0 ),
    ncandcut = cms.int32( 1 ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltPreDoublePhoton10L1R = cms.EDFilter( "HLTPrescaler" )
hltL1NonIsoHLTNonIsoDoublePhotonEt10L1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1DoubleEG5" ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
hltL1NonIsoHLTNonIsoDoublePhotonEt10EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoDoublePhotonEt10L1MatchFilterRegional" ),
    etcutEB = cms.double( 10.0 ),
    etcutEE = cms.double( 10.0 ),
    ncandcut = cms.int32( 2 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1NonIsoHLTNonIsoDoublePhotonEt10HcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoDoublePhotonEt10EtFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltL1sSingleLooseIsoTau20 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleTauJet20U OR L1_SingleJet30U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreSingleLooseIsoTau20 = cms.EDFilter( "HLTPrescaler" )
hltEcalRegionalJetsFEDs = cms.EDProducer( "EcalRawToRecHitRoI",
    sourceTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    type = cms.string( "jet" ),
    doES = cms.bool( False ),
    sourceTag_es = cms.InputTag( "NotNeededoESfalse" ),
    MuJobPSet = cms.PSet(  ),
    JetJobPSet = cms.VPSet( 
      cms.PSet(  Source = cms.InputTag( 'hltL1extraParticles','Central' ),
        regionPhiMargin = cms.double( 1.0 ),
        Ptmin = cms.double( 14.0 ),
        regionEtaMargin = cms.double( 1.0 )
      ),
      cms.PSet(  Source = cms.InputTag( 'hltL1extraParticles','Forward' ),
        regionPhiMargin = cms.double( 1.0 ),
        Ptmin = cms.double( 20.0 ),
        regionEtaMargin = cms.double( 1.0 )
      ),
      cms.PSet(  Source = cms.InputTag( 'hltL1extraParticles','Tau' ),
        regionPhiMargin = cms.double( 1.0 ),
        Ptmin = cms.double( 14.0 ),
        regionEtaMargin = cms.double( 1.0 )
      )
    ),
    EmJobPSet = cms.VPSet( 
    ),
    CandJobPSet = cms.VPSet( 
    )
)
hltEcalRegionalJetsRecHit = cms.EDProducer( "EcalRawToRecHitProducer",
    lazyGetterTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    sourceTag = cms.InputTag( "hltEcalRegionalJetsFEDs" ),
    splitOutput = cms.bool( True ),
    EBrechitCollection = cms.string( "EcalRecHitsEB" ),
    EErechitCollection = cms.string( "EcalRecHitsEE" ),
    rechitCollection = cms.string( "NotNeededsplitOutputTrue" )
)
hltTowerMakerForJets = cms.EDProducer( "CaloTowersCreator",
    EBThreshold = cms.double( 0.09 ),
    EEThreshold = cms.double( 0.45 ),
    UseEtEBTreshold = cms.bool( False ),
    UseEtEETreshold = cms.bool( False ),
    UseSymEBTreshold = cms.bool( False ),
    UseSymEETreshold = cms.bool( False ),
    HcalThreshold = cms.double( -1000.0 ),
    HBThreshold = cms.double( 0.9 ),
    HESThreshold = cms.double( 1.4 ),
    HEDThreshold = cms.double( 1.4 ),
    HOThreshold0 = cms.double( 1.1 ),
    HOThresholdPlus1 = cms.double( 1.1 ),
    HOThresholdMinus1 = cms.double( 1.1 ),
    HOThresholdPlus2 = cms.double( 1.1 ),
    HOThresholdMinus2 = cms.double( 1.1 ),
    HF1Threshold = cms.double( 1.2 ),
    HF2Threshold = cms.double( 1.8 ),
    EBWeight = cms.double( 1.0 ),
    EEWeight = cms.double( 1.0 ),
    HBWeight = cms.double( 1.0 ),
    HESWeight = cms.double( 1.0 ),
    HEDWeight = cms.double( 1.0 ),
    HOWeight = cms.double( 1.0E-99 ),
    HF1Weight = cms.double( 1.0 ),
    HF2Weight = cms.double( 1.0 ),
    EcutTower = cms.double( -1000.0 ),
    EBSumThreshold = cms.double( 0.2 ),
    EESumThreshold = cms.double( 0.45 ),
    UseHO = cms.bool( False ),
    MomConstrMethod = cms.int32( 1 ),
    MomHBDepth = cms.double( 0.2 ),
    MomHEDepth = cms.double( 0.4 ),
    MomEBDepth = cms.double( 0.3 ),
    MomEEDepth = cms.double( 0.0 ),
    hbheInput = cms.InputTag( "hltHbhereco" ),
    hoInput = cms.InputTag( "hltHoreco" ),
    hfInput = cms.InputTag( "hltHfreco" ),
    AllowMissingInputs = cms.bool( False ),
    HcalAcceptSeverityLevel = cms.uint32( 999 ),
    EcalAcceptSeverityLevel = cms.uint32( 1 ),
    UseHcalRecoveredHits = cms.bool( True ),
    UseEcalRecoveredHits = cms.bool( True ),
    EBGrid = cms.vdouble(  ),
    EBWeights = cms.vdouble(  ),
    EEGrid = cms.vdouble(  ),
    EEWeights = cms.vdouble(  ),
    HBGrid = cms.vdouble(  ),
    HBWeights = cms.vdouble(  ),
    HESGrid = cms.vdouble(  ),
    HESWeights = cms.vdouble(  ),
    HEDGrid = cms.vdouble(  ),
    HEDWeights = cms.vdouble(  ),
    HOGrid = cms.vdouble(  ),
    HOWeights = cms.vdouble(  ),
    HF1Grid = cms.vdouble(  ),
    HF1Weights = cms.vdouble(  ),
    HF2Grid = cms.vdouble(  ),
    HF2Weights = cms.vdouble(  ),
    ecalInputs = cms.VInputTag( 'hltEcalRegionalJetsRecHit:EcalRecHitsEB','hltEcalRegionalJetsRecHit:EcalRecHitsEE' )
)
hltCaloTowersTau1Regional = cms.EDProducer( "CaloTowerCreatorForTauHLT",
    towers = cms.InputTag( "hltTowerMakerForJets" ),
    UseTowersInCone = cms.double( 0.8 ),
    TauTrigger = cms.InputTag( 'hltL1extraParticles','Tau' ),
    minimumEt = cms.double( 0.5 ),
    minimumE = cms.double( 0.8 ),
    TauId = cms.int32( 0 )
)
hltIconeTau1Regional = cms.EDProducer( "FastjetJetProducer",
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.2 ),
    src = cms.InputTag( "hltCaloTowersTau1Regional" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.5 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    doPUFastjet = cms.bool( False ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 )
)
hltCaloTowersTau2Regional = cms.EDProducer( "CaloTowerCreatorForTauHLT",
    towers = cms.InputTag( "hltTowerMakerForJets" ),
    UseTowersInCone = cms.double( 0.8 ),
    TauTrigger = cms.InputTag( 'hltL1extraParticles','Tau' ),
    minimumEt = cms.double( 0.5 ),
    minimumE = cms.double( 0.8 ),
    TauId = cms.int32( 1 )
)
hltIconeTau2Regional = cms.EDProducer( "FastjetJetProducer",
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.2 ),
    src = cms.InputTag( "hltCaloTowersTau2Regional" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.5 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    doPUFastjet = cms.bool( False ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 )
)
hltCaloTowersTau3Regional = cms.EDProducer( "CaloTowerCreatorForTauHLT",
    towers = cms.InputTag( "hltTowerMakerForJets" ),
    UseTowersInCone = cms.double( 0.8 ),
    TauTrigger = cms.InputTag( 'hltL1extraParticles','Tau' ),
    minimumEt = cms.double( 0.5 ),
    minimumE = cms.double( 0.8 ),
    TauId = cms.int32( 2 )
)
hltIconeTau3Regional = cms.EDProducer( "FastjetJetProducer",
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.2 ),
    src = cms.InputTag( "hltCaloTowersTau3Regional" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.5 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    doPUFastjet = cms.bool( False ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 )
)
hltCaloTowersTau4Regional = cms.EDProducer( "CaloTowerCreatorForTauHLT",
    towers = cms.InputTag( "hltTowerMakerForJets" ),
    UseTowersInCone = cms.double( 0.8 ),
    TauTrigger = cms.InputTag( 'hltL1extraParticles','Tau' ),
    minimumEt = cms.double( 0.5 ),
    minimumE = cms.double( 0.8 ),
    TauId = cms.int32( 3 )
)
hltIconeTau4Regional = cms.EDProducer( "FastjetJetProducer",
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.2 ),
    src = cms.InputTag( "hltCaloTowersTau4Regional" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.5 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    doPUFastjet = cms.bool( False ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 )
)
hltCaloTowersCentral1Regional = cms.EDProducer( "CaloTowerCreatorForTauHLT",
    towers = cms.InputTag( "hltTowerMakerForJets" ),
    UseTowersInCone = cms.double( 0.8 ),
    TauTrigger = cms.InputTag( 'hltL1extraParticles','Central' ),
    minimumEt = cms.double( 0.5 ),
    minimumE = cms.double( 0.8 ),
    TauId = cms.int32( 0 )
)
hltIconeCentral1Regional = cms.EDProducer( "FastjetJetProducer",
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.2 ),
    src = cms.InputTag( "hltCaloTowersCentral1Regional" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.5 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    doPUFastjet = cms.bool( False ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 )
)
hltCaloTowersCentral2Regional = cms.EDProducer( "CaloTowerCreatorForTauHLT",
    towers = cms.InputTag( "hltTowerMakerForJets" ),
    UseTowersInCone = cms.double( 0.8 ),
    TauTrigger = cms.InputTag( 'hltL1extraParticles','Central' ),
    minimumEt = cms.double( 0.5 ),
    minimumE = cms.double( 0.8 ),
    TauId = cms.int32( 1 )
)
hltIconeCentral2Regional = cms.EDProducer( "FastjetJetProducer",
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.2 ),
    src = cms.InputTag( "hltCaloTowersCentral2Regional" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.5 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    doPUFastjet = cms.bool( False ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 )
)
hltCaloTowersCentral3Regional = cms.EDProducer( "CaloTowerCreatorForTauHLT",
    towers = cms.InputTag( "hltTowerMakerForJets" ),
    UseTowersInCone = cms.double( 0.8 ),
    TauTrigger = cms.InputTag( 'hltL1extraParticles','Central' ),
    minimumEt = cms.double( 0.5 ),
    minimumE = cms.double( 0.8 ),
    TauId = cms.int32( 2 )
)
hltIconeCentral3Regional = cms.EDProducer( "FastjetJetProducer",
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.2 ),
    src = cms.InputTag( "hltCaloTowersCentral3Regional" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.5 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    doPUFastjet = cms.bool( False ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 )
)
hltCaloTowersCentral4Regional = cms.EDProducer( "CaloTowerCreatorForTauHLT",
    towers = cms.InputTag( "hltTowerMakerForJets" ),
    UseTowersInCone = cms.double( 0.8 ),
    TauTrigger = cms.InputTag( 'hltL1extraParticles','Central' ),
    minimumEt = cms.double( 0.5 ),
    minimumE = cms.double( 0.8 ),
    TauId = cms.int32( 3 )
)
hltIconeCentral4Regional = cms.EDProducer( "FastjetJetProducer",
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.2 ),
    src = cms.InputTag( "hltCaloTowersCentral4Regional" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.5 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    doPUFastjet = cms.bool( False ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 )
)
hltL2TauJets = cms.EDProducer( "L2TauJetsMerger",
    EtMin = cms.double( 15.0 ),
    JetSrc = cms.VInputTag( 'hltIconeTau1Regional','hltIconeTau2Regional','hltIconeTau3Regional','hltIconeTau4Regional','hltIconeCentral1Regional','hltIconeCentral2Regional','hltIconeCentral3Regional','hltIconeCentral4Regional' )
)
hltFilterL2EtCutSingleLooseIsoTau20 = cms.EDFilter( "HLT1Tau",
    inputTag = cms.InputTag( "hltL2TauJets" ),
    MinPt = cms.double( 20.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 1 )
)
hltL2TauNarrowConeIsolationProducer = cms.EDProducer( "L2TauNarrowConeIsolationProducer",
    L2TauJetCollection = cms.InputTag( "hltL2TauJets" ),
    EBRecHits = cms.InputTag( 'hltEcalRegionalJetsRecHit','EcalRecHitsEB' ),
    EERecHits = cms.InputTag( 'hltEcalRegionalJetsRecHit','EcalRecHitsEE' ),
    CaloTowers = cms.InputTag( "hltTowerMakerForJets" ),
    associationRadius = cms.double( 0.5 ),
    crystalThresholdEE = cms.double( 0.45 ),
    crystalThresholdEB = cms.double( 0.15 ),
    towerThreshold = cms.double( 1.0 ),
    ECALIsolation = cms.PSet( 
      innerCone = cms.double( 0.15 ),
      runAlgorithm = cms.bool( True ),
      outerCone = cms.double( 0.5 )
    ),
    ECALClustering = cms.PSet( 
      runAlgorithm = cms.bool( True ),
      clusterRadius = cms.double( 0.08 )
    ),
    TowerIsolation = cms.PSet( 
      innerCone = cms.double( 0.2 ),
      runAlgorithm = cms.bool( True ),
      outerCone = cms.double( 0.5 )
    )
)
hltL2TauRelaxingIsolationSelector = cms.EDProducer( "L2TauRelaxingIsolationSelector",
    L2InfoAssociation = cms.InputTag( "hltL2TauNarrowConeIsolationProducer" ),
    MinJetEt = cms.double( 15.0 ),
    SeedTowerEt = cms.double( -10.0 ),
    EcalIsolationEt = cms.vdouble( 5.0, 0.025, 7.5E-4 ),
    TowerIsolationEt = cms.vdouble( 1000.0, 0.0, 0.0 ),
    NumberOfClusters = cms.vdouble( 1000.0, 0.0, 0.0 ),
    ClusterPhiRMS = cms.vdouble( 1000.0, 0.0, 0.0 ),
    ClusterEtaRMS = cms.vdouble( 1000.0, 0.0, 0.0 ),
    ClusterDRRMS = cms.vdouble( 1000.0, 0.0, 0.0 )
)
hltL1HLTSingleLooseIsoTau20JetsMatch = cms.EDProducer( "L1HLTJetsMatching",
    JetSrc = cms.InputTag( 'hltL2TauRelaxingIsolationSelector','Isolated' ),
    L1TauTrigger = cms.InputTag( "hltL1sSingleLooseIsoTau20" ),
    EtMin = cms.double( 20.0 )
)
hltFilterL2EcalIsolationSingleLooseIsoTau20 = cms.EDFilter( "HLT1Tau",
    inputTag = cms.InputTag( "hltL1HLTSingleLooseIsoTau20JetsMatch" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 20.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 1 )
)
hltL1sDoubleLooseIsoTau15 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_DoubleTauJet14U OR L1_DoubleJet30U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreDoubleLooseIsoTau15 = cms.EDFilter( "HLTPrescaler" )
hltFilterL2EtCutDoubleLooseIsoTau15 = cms.EDFilter( "HLT1Tau",
    inputTag = cms.InputTag( "hltL2TauJets" ),
    MinPt = cms.double( 15.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 2 )
)
hltL1HLTDoubleLooseIsoTau15JetsMatch = cms.EDProducer( "L1HLTJetsMatching",
    JetSrc = cms.InputTag( 'hltL2TauRelaxingIsolationSelector','Isolated' ),
    L1TauTrigger = cms.InputTag( "hltL1sDoubleLooseIsoTau15" ),
    EtMin = cms.double( 15.0 )
)
hltFilterL2EcalIsolationDoubleLooseIsoTau15 = cms.EDFilter( "HLT1Tau",
    inputTag = cms.InputTag( "hltL1HLTDoubleLooseIsoTau15JetsMatch" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 15.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 2 )
)
hltL1sBTagIPJet50U = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet30U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreBTagIPJet50U = cms.EDFilter( "HLTPrescaler" )
hltBJet50U = cms.EDFilter( "HLT1CaloBJet",
    inputTag = cms.InputTag( "hltMCJetCorJetIcone5HF07" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 50.0 ),
    MaxEta = cms.double( 3.0 ),
    MinN = cms.int32( 1 )
)
hltPixelVertices = cms.EDProducer( "PixelVertexProducer",
    Verbosity = cms.int32( 0 ),
    Finder = cms.string( "DivisiveVertexFinder" ),
    UseError = cms.bool( True ),
    WtAverage = cms.bool( True ),
    ZOffset = cms.double( 5.0 ),
    ZSeparation = cms.double( 0.05 ),
    NTrkMin = cms.int32( 2 ),
    PtMin = cms.double( 1.0 ),
    TrackCollection = cms.InputTag( "hltPixelTracks" ),
    Method2 = cms.bool( True ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" )
)
hltSelector4JetsU = cms.EDProducer( "LargestEtCaloJetSelector",
    src = cms.InputTag( "hltMCJetCorJetIcone5HF07" ),
    filter = cms.bool( False ),
    maxNumber = cms.uint32( 4 )
)
hltBLifetimeL25JetsStartupU = cms.EDProducer( "EtMinCaloJetSelector",
    src = cms.InputTag( "hltSelector4JetsU" ),
    filter = cms.bool( False ),
    etMin = cms.double( 15.0 )
)
hltBLifetimeL25AssociatorStartupU = cms.EDProducer( "JetTracksAssociatorAtVertex",
    jets = cms.InputTag( "hltBLifetimeL25JetsStartupU" ),
    tracks = cms.InputTag( "hltPixelTracks" ),
    coneSize = cms.double( 0.5 )
)
hltBLifetimeL25TagInfosStartupU = cms.EDProducer( "TrackIPProducer",
    jetTracks = cms.InputTag( "hltBLifetimeL25AssociatorStartupU" ),
    primaryVertex = cms.InputTag( "hltPixelVertices" ),
    computeProbabilities = cms.bool( False ),
    computeGhostTrack = cms.bool( False ),
    ghostTrackPriorDeltaR = cms.double( 0.03 ),
    minimumNumberOfPixelHits = cms.int32( 2 ),
    minimumNumberOfHits = cms.int32( 3 ),
    maximumTransverseImpactParameter = cms.double( 0.2 ),
    minimumTransverseMomentum = cms.double( 1.0 ),
    maximumChiSquared = cms.double( 5.0 ),
    maximumLongitudinalImpactParameter = cms.double( 17.0 ),
    jetDirectionUsingTracks = cms.bool( False ),
    jetDirectionUsingGhostTrack = cms.bool( False ),
    useTrackQuality = cms.bool( False )
)
hltBLifetimeL25BJetTagsStartupU = cms.EDProducer( "JetTagProducer",
    jetTagComputer = cms.string( "trackCounting3D2nd" ),
    tagInfos = cms.VInputTag( 'hltBLifetimeL25TagInfosStartupU' )
)
hltBLifetimeL25FilterStartupU = cms.EDFilter( "HLTJetTag",
    JetTag = cms.InputTag( "hltBLifetimeL25BJetTagsStartupU" ),
    MinTag = cms.double( 2.5 ),
    MaxTag = cms.double( 99999.0 ),
    MinJets = cms.int32( 1 ),
    SaveTag = cms.bool( False )
)
hltBLifetimeL3JetsStartupU = cms.EDProducer( "GetJetsFromHLTobject",
    jets = cms.InputTag( "hltBLifetimeL25FilterStartupU" )
)
hltBLifetimeRegionalPixelSeedGeneratorStartupU = cms.EDProducer( "SeedGeneratorFromRegionHitsEDProducer",
    ClusterCheckPSet = cms.PSet( 
      MaxNumberOfCosmicClusters = cms.uint32( 50000 ),
      ClusterCollectionLabel = cms.InputTag( "hltSiStripClusters" ),
      doClusterCheck = cms.bool( False )
    ),
    RegionFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "TauRegionalPixelSeedGenerator" ),
      RegionPSet = cms.PSet( 
        precise = cms.bool( True ),
        deltaPhiRegion = cms.double( 0.5 ),
        originHalfLength = cms.double( 0.2 ),
        originRadius = cms.double( 0.2 ),
        deltaEtaRegion = cms.double( 0.5 ),
        ptMin = cms.double( 1.0 ),
        JetSrc = cms.InputTag( "hltBLifetimeL3JetsStartupU" ),
        originZPos = cms.double( 0.0 ),
        vertexSrc = cms.InputTag( "hltPixelVertices" )
      )
    ),
    OrderedHitsFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "StandardHitPairGenerator" ),
      SeedingLayers = cms.string( "PixelLayerPairs" )
    ),
    SeedComparitorPSet = cms.PSet(  ComponentName = cms.string( "none" ) ),
    SeedCreatorPSet = cms.PSet( 
      ComponentName = cms.string( "SeedFromConsecutiveHitsCreator" ),
      propagator = cms.string( "PropagatorWithMaterial" )
    ),
    TTRHBuilder = cms.string( "WithTrackAngle" )
)
hltBLifetimeRegionalCkfTrackCandidatesStartupU = cms.EDProducer( "CkfTrackCandidateMaker",
    src = cms.InputTag( "hltBLifetimeRegionalPixelSeedGeneratorStartupU" ),
    TrajectoryBuilder = cms.string( "bJetRegionalTrajectoryBuilder" ),
    TrajectoryCleaner = cms.string( "TrajectoryCleanerBySharedHits" ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    useHitsSplitting = cms.bool( False ),
    doSeedingRegionRebuilding = cms.bool( False ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    cleanTrajectoryAfterInOut = cms.bool( False )
)
hltBLifetimeRegionalCtfWithMaterialTracksStartupU = cms.EDProducer( "TrackProducer",
    TrajectoryInEvent = cms.bool( True ),
    useHitsSplitting = cms.bool( False ),
    clusterRemovalInfo = cms.InputTag( "" ),
    alias = cms.untracked.string( "ctfWithMaterialTracks" ),
    Fitter = cms.string( "FittingSmootherRK" ),
    Propagator = cms.string( "RungeKuttaTrackerPropagator" ),
    src = cms.InputTag( "hltBLifetimeRegionalCkfTrackCandidatesStartupU" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    NavigationSchool = cms.string( "" )
)
hltBLifetimeL3AssociatorStartupU = cms.EDProducer( "JetTracksAssociatorAtVertex",
    jets = cms.InputTag( "hltBLifetimeL3JetsStartupU" ),
    tracks = cms.InputTag( "hltBLifetimeRegionalCtfWithMaterialTracksStartupU" ),
    coneSize = cms.double( 0.5 )
)
hltBLifetimeL3TagInfosStartupU = cms.EDProducer( "TrackIPProducer",
    jetTracks = cms.InputTag( "hltBLifetimeL3AssociatorStartupU" ),
    primaryVertex = cms.InputTag( "hltPixelVertices" ),
    computeProbabilities = cms.bool( False ),
    computeGhostTrack = cms.bool( False ),
    ghostTrackPriorDeltaR = cms.double( 0.03 ),
    minimumNumberOfPixelHits = cms.int32( 2 ),
    minimumNumberOfHits = cms.int32( 8 ),
    maximumTransverseImpactParameter = cms.double( 0.2 ),
    minimumTransverseMomentum = cms.double( 1.0 ),
    maximumChiSquared = cms.double( 20.0 ),
    maximumLongitudinalImpactParameter = cms.double( 17.0 ),
    jetDirectionUsingTracks = cms.bool( False ),
    jetDirectionUsingGhostTrack = cms.bool( False ),
    useTrackQuality = cms.bool( False )
)
hltBLifetimeL3BJetTagsStartupU = cms.EDProducer( "JetTagProducer",
    jetTagComputer = cms.string( "trackCounting3D2nd" ),
    tagInfos = cms.VInputTag( 'hltBLifetimeL3TagInfosStartupU' )
)
hltBLifetimeL3FilterStartupU = cms.EDFilter( "HLTJetTag",
    JetTag = cms.InputTag( "hltBLifetimeL3BJetTagsStartupU" ),
    MinTag = cms.double( 3.5 ),
    MaxTag = cms.double( 99999.0 ),
    MinJets = cms.int32( 1 ),
    SaveTag = cms.bool( True )
)
hltL1sBTagMuJet10U = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_Mu3_Jet6U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreBTagMuJet10U = cms.EDFilter( "HLTPrescaler" )
hltBJet10U = cms.EDFilter( "HLT1CaloBJet",
    inputTag = cms.InputTag( "hltMCJetCorJetIcone5HF07" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 10.0 ),
    MaxEta = cms.double( 3.0 ),
    MinN = cms.int32( 1 )
)
hltBSoftMuonL25JetsU = cms.EDProducer( "EtMinCaloJetSelector",
    src = cms.InputTag( "hltSelector4JetsU" ),
    filter = cms.bool( False ),
    etMin = cms.double( 10.0 )
)
hltBSoftMuonL25TagInfosU = cms.EDProducer( "SoftLepton",
    jets = cms.InputTag( "hltBSoftMuonL25JetsU" ),
    primaryVertex = cms.InputTag( "nominal" ),
    leptons = cms.InputTag( "hltL2Muons" ),
    refineJetAxis = cms.uint32( 0 ),
    leptonDeltaRCut = cms.double( 0.4 ),
    leptonChi2Cut = cms.double( 0.0 ),
    leptonQualityCut = cms.double( 0.0 ),
    muonSelection = cms.uint32( 0 )
)
hltBSoftMuonL25BJetTagsUByDR = cms.EDProducer( "JetTagProducer",
    jetTagComputer = cms.string( "softLeptonByDistance" ),
    tagInfos = cms.VInputTag( 'hltBSoftMuonL25TagInfosU' )
)
hltBSoftMuonL25FilterUByDR = cms.EDFilter( "HLTJetTag",
    JetTag = cms.InputTag( "hltBSoftMuonL25BJetTagsUByDR" ),
    MinTag = cms.double( 0.5 ),
    MaxTag = cms.double( 99999.0 ),
    MinJets = cms.int32( 1 ),
    SaveTag = cms.bool( False )
)
hltBSoftMuonL3TagInfosU = cms.EDProducer( "SoftLepton",
    jets = cms.InputTag( "hltBSoftMuonL25JetsU" ),
    primaryVertex = cms.InputTag( "nominal" ),
    leptons = cms.InputTag( "hltL3Muons" ),
    refineJetAxis = cms.uint32( 0 ),
    leptonDeltaRCut = cms.double( 0.4 ),
    leptonChi2Cut = cms.double( 0.0 ),
    leptonQualityCut = cms.double( 0.0 ),
    muonSelection = cms.uint32( 0 )
)
hltBSoftMuonL3BJetTagsUByPt = cms.EDProducer( "JetTagProducer",
    jetTagComputer = cms.string( "softLeptonByPt" ),
    tagInfos = cms.VInputTag( 'hltBSoftMuonL3TagInfosU' )
)
hltBSoftMuonL3BJetTagsUByDR = cms.EDProducer( "JetTagProducer",
    jetTagComputer = cms.string( "softLeptonByDistance" ),
    tagInfos = cms.VInputTag( 'hltBSoftMuonL3TagInfosU' )
)
hltBSoftMuonL3FilterUByDR = cms.EDFilter( "HLTJetTag",
    JetTag = cms.InputTag( "hltBSoftMuonL3BJetTagsUByDR" ),
    MinTag = cms.double( 0.5 ),
    MaxTag = cms.double( 99999.0 ),
    MinJets = cms.int32( 1 ),
    SaveTag = cms.bool( True )
)
hltL1sStoppedHSCP8E29 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet10U_NotBptxC" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" ),
    saveTags = cms.untracked.bool( False )
)
hltPreStoppedHSCP8E29 = cms.EDFilter( "HLTPrescaler" )
hltStoppedHSCPHpdFilter = cms.EDFilter( "HLTHPDFilter",
    inputTag = cms.InputTag( "hltHbhereco" ),
    energy = cms.double( -99.0 ),
    hpdSpikeEnergy = cms.double( 10.0 ),
    hpdSpikeIsolationEnergy = cms.double( 1.0 ),
    rbxSpikeEnergy = cms.double( 50.0 ),
    rbxSpikeUnbalance = cms.double( 0.2 )
)
hltStoppedHSCPTowerMakerForAll = cms.EDProducer( "CaloTowersCreator",
    EBThreshold = cms.double( 0.09 ),
    EEThreshold = cms.double( 0.45 ),
    UseEtEBTreshold = cms.bool( False ),
    UseEtEETreshold = cms.bool( False ),
    UseSymEBTreshold = cms.bool( False ),
    UseSymEETreshold = cms.bool( False ),
    HcalThreshold = cms.double( -1000.0 ),
    HBThreshold = cms.double( 0.9 ),
    HESThreshold = cms.double( 1.4 ),
    HEDThreshold = cms.double( 1.4 ),
    HOThreshold0 = cms.double( 1.1 ),
    HOThresholdPlus1 = cms.double( 1.1 ),
    HOThresholdMinus1 = cms.double( 1.1 ),
    HOThresholdPlus2 = cms.double( 1.1 ),
    HOThresholdMinus2 = cms.double( 1.1 ),
    HF1Threshold = cms.double( 1.2 ),
    HF2Threshold = cms.double( 1.8 ),
    EBWeight = cms.double( 1.0 ),
    EEWeight = cms.double( 1.0 ),
    HBWeight = cms.double( 1.0 ),
    HESWeight = cms.double( 1.0 ),
    HEDWeight = cms.double( 1.0 ),
    HOWeight = cms.double( 1.0E-99 ),
    HF1Weight = cms.double( 1.0 ),
    HF2Weight = cms.double( 1.0 ),
    EcutTower = cms.double( -1000.0 ),
    EBSumThreshold = cms.double( 0.2 ),
    EESumThreshold = cms.double( 0.45 ),
    UseHO = cms.bool( False ),
    MomConstrMethod = cms.int32( 1 ),
    MomHBDepth = cms.double( 0.2 ),
    MomHEDepth = cms.double( 0.4 ),
    MomEBDepth = cms.double( 0.3 ),
    MomEEDepth = cms.double( 0.0 ),
    hbheInput = cms.InputTag( "hltHbhereco" ),
    hoInput = cms.InputTag( "" ),
    hfInput = cms.InputTag( "" ),
    AllowMissingInputs = cms.bool( True ),
    HcalAcceptSeverityLevel = cms.uint32( 999 ),
    EcalAcceptSeverityLevel = cms.uint32( 1 ),
    UseHcalRecoveredHits = cms.bool( True ),
    UseEcalRecoveredHits = cms.bool( True ),
    EBGrid = cms.vdouble(  ),
    EBWeights = cms.vdouble(  ),
    EEGrid = cms.vdouble(  ),
    EEWeights = cms.vdouble(  ),
    HBGrid = cms.vdouble(  ),
    HBWeights = cms.vdouble(  ),
    HESGrid = cms.vdouble(  ),
    HESWeights = cms.vdouble(  ),
    HEDGrid = cms.vdouble(  ),
    HEDWeights = cms.vdouble(  ),
    HOGrid = cms.vdouble(  ),
    HOWeights = cms.vdouble(  ),
    HF1Grid = cms.vdouble(  ),
    HF1Weights = cms.vdouble(  ),
    HF2Grid = cms.vdouble(  ),
    HF2Weights = cms.vdouble(  ),
    ecalInputs = cms.VInputTag(  )
)
hltStoppedHSCPIterativeCone5CaloJets = cms.EDProducer( "FastjetJetProducer",
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.5 ),
    src = cms.InputTag( "hltStoppedHSCPTowerMakerForAll" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.5 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    doPUFastjet = cms.bool( False ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 )
)
hltStoppedHSCP1CaloJetEnergy = cms.EDFilter( "HLT1CaloJetEnergy",
    inputTag = cms.InputTag( "hltStoppedHSCPIterativeCone5CaloJets" ),
    saveTag = cms.untracked.bool( True ),
    MinE = cms.double( 20.0 ),
    MaxEta = cms.double( 3.0 ),
    MinN = cms.int32( 1 )
)
hltL1sL1Mu14L1SingleEG10 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMu14 AND L1_SingleEG10" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1Mu14L1SingleEG10 = cms.EDFilter( "HLTPrescaler" )
hltL1sL1Mu14L1SingleJet6U = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMu14 AND L1_SingleJet6U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1Mu14L1SingleJet6U = cms.EDFilter( "HLTPrescaler" )
hltL1sL1Mu14L1ETM30 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMu14 AND L1_ETM30" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1Mu14L1ETM30 = cms.EDFilter( "HLTPrescaler" )
hltL1sZeroBias = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "4" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreZeroBias = cms.EDFilter( "HLTPrescaler" )
hltPreZeroBias1kHz = cms.EDFilter( "HLTPrescaler" )
hltL1sMinBias = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_MinBias_HTT10" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreMinBias = cms.EDFilter( "HLTPrescaler" )
hltL1sMinBiasBSC = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "32 OR 33 OR 40 OR 41" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreMinBiasBSC = cms.EDFilter( "HLTPrescaler" )
hltPreMinBiasBSCNoBPTX = cms.EDFilter( "HLTPrescaler" )
hltL1sMinBiasBSCOR = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "34" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreMinBiasBSCOR = cms.EDFilter( "HLTPrescaler" )
hltL1sMinBiasHcal = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleHfBitCountsRing1_1 OR L1_SingleHfBitCountsRing2_1 OR L1_DoubleHfBitCountsRing1_P1N1 OR L1_DoubleHfBitCountsRing2_P1N1 OR L1_SingleHfRingEtSumsRing1_4 OR L1_DoubleHfRingEtSumsRing1_P4N4 OR L1_SingleHfRingEtSumsRing2_4 OR L1_DoubleHfRingEtSumsRing2_P4N4" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreMinBiasHcal = cms.EDFilter( "HLTPrescaler" )
hltL1sMinBiasEcal = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleEG1 OR L1_SingleEG2 OR L1_DoubleEG1" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreMinBiasEcal = cms.EDFilter( "HLTPrescaler" )
hltPreZeroBiasPixelSingleTrack = cms.EDFilter( "HLTPrescaler" )
hltPixelTracksForMinBias = cms.EDProducer( "PixelTrackProducer",
    useFilterWithES = cms.bool( False ),
    RegionFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "GlobalRegionProducer" ),
      RegionPSet = cms.PSet( 
        precise = cms.bool( True ),
        originHalfLength = cms.double( 30.0 ),
        originRadius = cms.double( 0.5 ),
        originYPos = cms.double( 0.0 ),
        ptMin = cms.double( 0.2 ),
        originXPos = cms.double( 0.0 ),
        originZPos = cms.double( 0.0 )
      )
    ),
    OrderedHitsFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "StandardHitTripletGenerator" ),
      SeedingLayers = cms.string( "PixelLayerTriplets" ),
      GeneratorPSet = cms.PSet( 
        useBending = cms.bool( True ),
        useFixedPreFiltering = cms.bool( False ),
        phiPreFiltering = cms.double( 0.3 ),
        extraHitRPhitolerance = cms.double( 0.06 ),
        useMultScattering = cms.bool( True ),
        ComponentName = cms.string( "PixelTripletHLTGenerator" ),
        extraHitRZtolerance = cms.double( 0.06 )
      )
    ),
    FitterPSet = cms.PSet( 
      ComponentName = cms.string( "PixelFitterByHelixProjections" ),
      TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" )
    ),
    FilterPSet = cms.PSet( 
      chi2 = cms.double( 1000.0 ),
      nSigmaTipMaxTolerance = cms.double( 0.0 ),
      ComponentName = cms.string( "PixelTrackFilterByKinematics" ),
      nSigmaInvPtTolerance = cms.double( 0.0 ),
      ptMin = cms.double( 0.1 ),
      tipMax = cms.double( 1.0 )
    ),
    CleanerPSet = cms.PSet(  ComponentName = cms.string( "PixelTrackCleanerBySharedHits" ) )
)
hltPixelCands = cms.EDProducer( "ConcreteChargedCandidateProducer",
    src = cms.InputTag( "hltPixelTracksForMinBias" ),
    particleType = cms.string( "pi+" )
)
hltMinBiasPixelFilter1 = cms.EDFilter( "HLTPixlMBFilt",
    pixlTag = cms.InputTag( "hltPixelCands" ),
    MinPt = cms.double( 0.0 ),
    MinTrks = cms.uint32( 1 ),
    MinSep = cms.double( 1.0 )
)
hltPreMinBiasPixelSingleTrack = cms.EDFilter( "HLTPrescaler" )
hltPreMinBiasPixelDoubleTrack = cms.EDFilter( "HLTPrescaler" )
hltMinBiasPixelFilter2 = cms.EDFilter( "HLTPixlMBFilt",
    pixlTag = cms.InputTag( "hltPixelCands" ),
    MinPt = cms.double( 0.0 ),
    MinTrks = cms.uint32( 2 ),
    MinSep = cms.double( 1.0 )
)
hltPreMinBiasPixelDoubleIsoTrack5 = cms.EDFilter( "HLTPrescaler" )
hltPixelMBForAlignment = cms.EDFilter( "HLTPixlMBForAlignmentFilter",
    pixlTag = cms.InputTag( "hltPixelCands" ),
    MinPt = cms.double( 5.0 ),
    MinTrks = cms.uint32( 2 ),
    MinSep = cms.double( 1.0 ),
    MinIsol = cms.double( 0.05 )
)
hltL1sCSCBeamHalo = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMuBeamHalo" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreCSCBeamHalo = cms.EDFilter( "HLTPrescaler" )
hltL1sCSCBeamHaloOverlapRing1 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMuBeamHalo" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreCSCBeamHaloOverlapRing1 = cms.EDFilter( "HLTPrescaler" )
hltOverlapsHLTCSCBeamHaloOverlapRing1 = cms.EDFilter( "HLTCSCOverlapFilter",
    input = cms.InputTag( "hltCsc2DRecHits" ),
    minHits = cms.uint32( 4 ),
    xWindow = cms.double( 2.0 ),
    yWindow = cms.double( 2.0 ),
    ring1 = cms.bool( True ),
    ring2 = cms.bool( False ),
    fillHists = cms.bool( False )
)
hltL1sCSCBeamHaloOverlapRing2 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMuBeamHalo" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreCSCBeamHaloOverlapRing2 = cms.EDFilter( "HLTPrescaler" )
hltOverlapsHLTCSCBeamHaloOverlapRing2 = cms.EDFilter( "HLTCSCOverlapFilter",
    input = cms.InputTag( "hltCsc2DRecHits" ),
    minHits = cms.uint32( 4 ),
    xWindow = cms.double( 2.0 ),
    yWindow = cms.double( 2.0 ),
    ring1 = cms.bool( False ),
    ring2 = cms.bool( True ),
    fillHists = cms.bool( False )
)
hltL1sCSCBeamHaloRing2or3 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMuBeamHalo" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreCSCBeamHaloRing2or3 = cms.EDFilter( "HLTPrescaler" )
hltFilter23HLTCSCBeamHaloRing2or3 = cms.EDFilter( "HLTCSCRing2or3Filter",
    input = cms.InputTag( "hltCsc2DRecHits" ),
    minHits = cms.uint32( 4 ),
    xWindow = cms.double( 2.0 ),
    yWindow = cms.double( 2.0 )
)
hltL1sBackwardBSC = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "38 OR 39" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreBackwardBSC = cms.EDFilter( "HLTPrescaler" )
hltL1sForwardBSC = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "36 OR 37" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreForwardBSC = cms.EDFilter( "HLTPrescaler" )
hltL1sHighMultiplicityBSC = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "35" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreHighMultiplicityBSC = cms.EDFilter( "HLTPrescaler" )
hltL1sSplashBSC = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "42 OR 43" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreSplashBSC = cms.EDFilter( "HLTPrescaler" )
hltL1sL1BSC = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "36 OR 37 OR 38 OR 39" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1BSC = cms.EDFilter( "HLTPrescaler" )
hltL1sL1BscMinBiasORBptxPlusORMinus = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_BscMinBiasOR_BptxPlusORMinus" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1BscMinBiasORBptxPlusORMinus = cms.EDFilter( "HLTPrescaler" )
hltPreL1BscMinBiasORBptxPlusORMinusNoBptx = cms.EDFilter( "HLTPrescaler" )
hltL1sRPCBarrelCosmics = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "24" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreRPCBarrelCosmics = cms.EDFilter( "HLTPrescaler" )
hltL1sTrackerCosmics = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "25" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreTrackerCosmics = cms.EDFilter( "HLTPrescaler" )
hltL1sL1TechRPCTTURBst1collisions = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "31" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1TechRPCTTURBst1collisions = cms.EDFilter( "HLTPrescaler" )
hltL1sIsoTrack8E29 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet20U OR L1_SingleJet30U OR L1_SingleJet40U OR L1_SingleJet50U OR L1_SingleJet60U OR L1_SingleTauJet10U OR L1_SingleTauJet20U OR L1_SingleTauJet30U OR L1_SingleTauJet50U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreIsoTrackHE8E29 = cms.EDFilter( "HLTPrescaler" )
hltHITPixelTracksHB = cms.EDProducer( "PixelTrackProducer",
    useFilterWithES = cms.bool( False ),
    RegionFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "GlobalRegionProducerFromBeamSpot" ),
      RegionPSet = cms.PSet( 
        precise = cms.bool( True ),
        originRadius = cms.double( 0.0015 ),
        nSigmaZ = cms.double( 3.0 ),
        beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
        ptMin = cms.double( 0.7 )
      )
    ),
    OrderedHitsFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "StandardHitTripletGenerator" ),
      GeneratorPSet = cms.PSet( 
        useBending = cms.bool( True ),
        useFixedPreFiltering = cms.bool( False ),
        phiPreFiltering = cms.double( 0.3 ),
        extraHitRPhitolerance = cms.double( 0.06 ),
        useMultScattering = cms.bool( True ),
        ComponentName = cms.string( "PixelTripletHLTGenerator" ),
        extraHitRZtolerance = cms.double( 0.06 )
      ),
      SeedingLayers = cms.string( "PixelLayerTripletsHITHB" )
    ),
    FitterPSet = cms.PSet( 
      ComponentName = cms.string( "PixelFitterByConformalMappingAndLine" ),
      TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
      fixImpactParameter = cms.double( 0.0 )
    ),
    FilterPSet = cms.PSet( 
      chi2 = cms.double( 1000.0 ),
      nSigmaTipMaxTolerance = cms.double( 0.0 ),
      ComponentName = cms.string( "PixelTrackFilterByKinematics" ),
      nSigmaInvPtTolerance = cms.double( 0.0 ),
      ptMin = cms.double( 0.7 ),
      tipMax = cms.double( 1.0 )
    ),
    CleanerPSet = cms.PSet(  ComponentName = cms.string( "PixelTrackCleanerBySharedHits" ) )
)
hltHITPixelTracksHE = cms.EDProducer( "PixelTrackProducer",
    useFilterWithES = cms.bool( False ),
    RegionFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "GlobalRegionProducerFromBeamSpot" ),
      RegionPSet = cms.PSet( 
        precise = cms.bool( True ),
        originRadius = cms.double( 0.0015 ),
        nSigmaZ = cms.double( 3.0 ),
        beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
        ptMin = cms.double( 0.35 )
      )
    ),
    OrderedHitsFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "StandardHitTripletGenerator" ),
      GeneratorPSet = cms.PSet( 
        useBending = cms.bool( True ),
        useFixedPreFiltering = cms.bool( False ),
        phiPreFiltering = cms.double( 0.3 ),
        extraHitRPhitolerance = cms.double( 0.06 ),
        useMultScattering = cms.bool( True ),
        ComponentName = cms.string( "PixelTripletHLTGenerator" ),
        extraHitRZtolerance = cms.double( 0.06 )
      ),
      SeedingLayers = cms.string( "PixelLayerTripletsHITHE" )
    ),
    FitterPSet = cms.PSet( 
      ComponentName = cms.string( "PixelFitterByConformalMappingAndLine" ),
      TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
      fixImpactParameter = cms.double( 0.0 )
    ),
    FilterPSet = cms.PSet( 
      chi2 = cms.double( 1000.0 ),
      nSigmaTipMaxTolerance = cms.double( 0.0 ),
      ComponentName = cms.string( "PixelTrackFilterByKinematics" ),
      nSigmaInvPtTolerance = cms.double( 0.0 ),
      ptMin = cms.double( 0.35 ),
      tipMax = cms.double( 1.0 )
    ),
    CleanerPSet = cms.PSet(  ComponentName = cms.string( "PixelTrackCleanerBySharedHits" ) )
)
hltHITPixelVerticesHE = cms.EDProducer( "PixelVertexProducer",
    Verbosity = cms.int32( 0 ),
    Finder = cms.string( "DivisiveVertexFinder" ),
    UseError = cms.bool( True ),
    WtAverage = cms.bool( True ),
    ZOffset = cms.double( 5.0 ),
    ZSeparation = cms.double( 0.05 ),
    NTrkMin = cms.int32( 2 ),
    PtMin = cms.double( 1.0 ),
    TrackCollection = cms.InputTag( "hltHITPixelTracksHE" ),
    Method2 = cms.bool( True ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" )
)
hltIsolPixelTrackProdHE8E29 = cms.EDProducer( "IsolatedPixelTrackCandidateProducer",
    L1eTauJetsSource = cms.InputTag( 'hltL1extraParticles','Tau' ),
    tauAssociationCone = cms.double( 0.0 ),
    tauUnbiasCone = cms.double( 1.2 ),
    ExtrapolationConeSize = cms.double( 1.0 ),
    PixelIsolationConeSizeAtEC = cms.double( 40.0 ),
    L1GTSeedLabel = cms.InputTag( "hltL1sIsoTrack8E29" ),
    MaxVtxDXYSeed = cms.double( 101.0 ),
    MaxVtxDXYIsol = cms.double( 101.0 ),
    VertexLabel = cms.InputTag( "hltHITPixelVerticesHE" ),
    MagFieldRecordName = cms.string( "VolumeBasedMagneticField" ),
    minPTrack = cms.double( 5.0 ),
    maxPTrackForIsolation = cms.double( 3.0 ),
    EBEtaBoundary = cms.double( 1.479 ),
    PixelTracksSources = cms.VInputTag( 'hltHITPixelTracksHB','hltHITPixelTracksHE' )
)
hltIsolPixelTrackL2FilterHE8E29 = cms.EDFilter( "HLTPixelIsolTrackFilter",
    candTag = cms.InputTag( "hltIsolPixelTrackProdHE8E29" ),
    MinPtTrack = cms.double( 3.5 ),
    MaxPtNearby = cms.double( 2.0 ),
    MaxEtaTrack = cms.double( 2.0 ),
    MinEtaTrack = cms.double( 1.4 ),
    filterTrackEnergy = cms.bool( True ),
    MinEnergyTrack = cms.double( 20.0 ),
    NMaxTrackCandidates = cms.int32( 5 ),
    DropMultiL2Event = cms.bool( False )
)
hltHITPixelTripletSeedGeneratorHE8E29 = cms.EDProducer( "SeedGeneratorFromRegionHitsEDProducer",
    ClusterCheckPSet = cms.PSet( 
      MaxNumberOfCosmicClusters = cms.uint32( 50000 ),
      ClusterCollectionLabel = cms.InputTag( "siStripClusters" ),
      doClusterCheck = cms.bool( False )
    ),
    RegionFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "HITRegionalPixelSeedGenerator" ),
      RegionPSet = cms.PSet( 
        useIsoTracks = cms.bool( True ),
        trackSrc = cms.InputTag( "hltHITPixelTracksHE" ),
        isoTrackSrc = cms.InputTag( "hltIsolPixelTrackL2FilterHE8E29" ),
        l1tjetSrc = cms.InputTag( 'hltl1extraParticles','Tau' ),
        originHalfLength = cms.double( 0.2 ),
        precise = cms.bool( True ),
        deltaEtaL1JetRegion = cms.double( 0.3 ),
        useTracks = cms.bool( False ),
        originRadius = cms.double( 0.2 ),
        useL1Jets = cms.bool( False ),
        deltaPhiTrackRegion = cms.double( 0.05 ),
        deltaPhiL1JetRegion = cms.double( 0.3 ),
        vertexSrc = cms.string( "hltHITPixelVerticesHE" ),
        fixedReg = cms.bool( False ),
        etaCenter = cms.double( 0.0 ),
        phiCenter = cms.double( 0.0 ),
        originZPos = cms.double( 0.0 ),
        deltaEtaTrackRegion = cms.double( 0.05 ),
        ptMin = cms.double( 0.5 )
      )
    ),
    OrderedHitsFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "StandardHitTripletGenerator" ),
      GeneratorPSet = cms.PSet( 
        useBending = cms.bool( True ),
        useFixedPreFiltering = cms.bool( False ),
        ComponentName = cms.string( "PixelTripletHLTGenerator" ),
        extraHitRPhitolerance = cms.double( 0.06 ),
        useMultScattering = cms.bool( True ),
        phiPreFiltering = cms.double( 0.3 ),
        extraHitRZtolerance = cms.double( 0.06 )
      ),
      SeedingLayers = cms.string( "PixelLayerTriplets" )
    ),
    SeedComparitorPSet = cms.PSet(  ComponentName = cms.string( "none" ) ),
    SeedCreatorPSet = cms.PSet( 
      ComponentName = cms.string( "SeedFromConsecutiveHitsCreator" ),
      propagator = cms.string( "PropagatorWithMaterial" )
    ),
    TTRHBuilder = cms.string( "WithTrackAngle" )
)
hltHITCkfTrackCandidatesHE8E29 = cms.EDProducer( "CkfTrackCandidateMaker",
    src = cms.InputTag( "hltHITPixelTripletSeedGeneratorHE8E29" ),
    TrajectoryBuilder = cms.string( "CkfTrajectoryBuilder" ),
    TrajectoryCleaner = cms.string( "TrajectoryCleanerBySharedHits" ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    useHitsSplitting = cms.bool( False ),
    doSeedingRegionRebuilding = cms.bool( False ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    cleanTrajectoryAfterInOut = cms.bool( False )
)
hltHITCtfWithMaterialTracksHE8E29 = cms.EDProducer( "TrackProducer",
    TrajectoryInEvent = cms.bool( False ),
    useHitsSplitting = cms.bool( False ),
    clusterRemovalInfo = cms.InputTag( "" ),
    alias = cms.untracked.string( "hltHITCtfWithMaterialTracksHE1E31" ),
    Fitter = cms.string( "hltKFFittingSmoother" ),
    Propagator = cms.string( "PropagatorWithMaterial" ),
    src = cms.InputTag( "hltHITCkfTrackCandidatesHE8E29" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    NavigationSchool = cms.string( "" )
)
hltHITIPTCorrectorHE8E29 = cms.EDProducer( "IPTCorrector",
    corTracksLabel = cms.InputTag( "hltHITCtfWithMaterialTracksHE8E29" ),
    filterLabel = cms.InputTag( "hltIsolPixelTrackL2FilterHE8E29" ),
    associationCone = cms.double( 0.2 )
)
hltIsolPixelTrackL3FilterHE8E29 = cms.EDFilter( "HLTPixelIsolTrackFilter",
    candTag = cms.InputTag( "hltHITIPTCorrectorHE8E29" ),
    MinPtTrack = cms.double( 20.0 ),
    MaxPtNearby = cms.double( 2.0 ),
    MaxEtaTrack = cms.double( 2.0 ),
    MinEtaTrack = cms.double( 1.4 ),
    filterTrackEnergy = cms.bool( True ),
    MinEnergyTrack = cms.double( 20.0 ),
    NMaxTrackCandidates = cms.int32( 15 ),
    DropMultiL2Event = cms.bool( False )
)
hltPreIsoTrackHB8E29 = cms.EDFilter( "HLTPrescaler" )
hltHITPixelVerticesHB = cms.EDProducer( "PixelVertexProducer",
    Verbosity = cms.int32( 0 ),
    Finder = cms.string( "DivisiveVertexFinder" ),
    UseError = cms.bool( True ),
    WtAverage = cms.bool( True ),
    ZOffset = cms.double( 5.0 ),
    ZSeparation = cms.double( 0.05 ),
    NTrkMin = cms.int32( 2 ),
    PtMin = cms.double( 1.0 ),
    TrackCollection = cms.InputTag( "hltHITPixelTracksHB" ),
    Method2 = cms.bool( True ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" )
)
hltIsolPixelTrackProdHB8E29 = cms.EDProducer( "IsolatedPixelTrackCandidateProducer",
    L1eTauJetsSource = cms.InputTag( 'hltL1extraParticles','Tau' ),
    tauAssociationCone = cms.double( 0.0 ),
    tauUnbiasCone = cms.double( 1.2 ),
    ExtrapolationConeSize = cms.double( 1.0 ),
    PixelIsolationConeSizeAtEC = cms.double( 40.0 ),
    L1GTSeedLabel = cms.InputTag( "hltL1sIsoTrack8E29" ),
    MaxVtxDXYSeed = cms.double( 101.0 ),
    MaxVtxDXYIsol = cms.double( 101.0 ),
    VertexLabel = cms.InputTag( "hltHITPixelVerticesHB" ),
    MagFieldRecordName = cms.string( "VolumeBasedMagneticField" ),
    minPTrack = cms.double( 5.0 ),
    maxPTrackForIsolation = cms.double( 3.0 ),
    EBEtaBoundary = cms.double( 1.479 ),
    PixelTracksSources = cms.VInputTag( 'hltHITPixelTracksHB' )
)
hltIsolPixelTrackL2FilterHB8E29 = cms.EDFilter( "HLTPixelIsolTrackFilter",
    candTag = cms.InputTag( "hltIsolPixelTrackProdHB8E29" ),
    MinPtTrack = cms.double( 3.5 ),
    MaxPtNearby = cms.double( 2.0 ),
    MaxEtaTrack = cms.double( 1.45 ),
    MinEtaTrack = cms.double( 0.0 ),
    filterTrackEnergy = cms.bool( True ),
    MinEnergyTrack = cms.double( 8.0 ),
    NMaxTrackCandidates = cms.int32( 10 ),
    DropMultiL2Event = cms.bool( False )
)
hltHITPixelTripletSeedGeneratorHB8E29 = cms.EDProducer( "SeedGeneratorFromRegionHitsEDProducer",
    ClusterCheckPSet = cms.PSet( 
      MaxNumberOfCosmicClusters = cms.uint32( 50000 ),
      ClusterCollectionLabel = cms.InputTag( "siStripClusters" ),
      doClusterCheck = cms.bool( False )
    ),
    RegionFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "HITRegionalPixelSeedGenerator" ),
      RegionPSet = cms.PSet( 
        useIsoTracks = cms.bool( True ),
        trackSrc = cms.InputTag( "hltHITPixelTracksHB" ),
        isoTrackSrc = cms.InputTag( "hltIsolPixelTrackL2FilterHB8E29" ),
        l1tjetSrc = cms.InputTag( 'hltl1extraParticles','Tau' ),
        originHalfLength = cms.double( 0.2 ),
        precise = cms.bool( True ),
        deltaEtaL1JetRegion = cms.double( 0.3 ),
        useTracks = cms.bool( False ),
        originRadius = cms.double( 0.2 ),
        useL1Jets = cms.bool( False ),
        deltaPhiTrackRegion = cms.double( 0.05 ),
        deltaPhiL1JetRegion = cms.double( 0.3 ),
        vertexSrc = cms.string( "hltHITPixelVerticesHB" ),
        fixedReg = cms.bool( False ),
        etaCenter = cms.double( 0.0 ),
        phiCenter = cms.double( 0.0 ),
        originZPos = cms.double( 0.0 ),
        deltaEtaTrackRegion = cms.double( 0.05 ),
        ptMin = cms.double( 1.0 )
      )
    ),
    OrderedHitsFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "StandardHitTripletGenerator" ),
      GeneratorPSet = cms.PSet( 
        useBending = cms.bool( True ),
        useFixedPreFiltering = cms.bool( False ),
        ComponentName = cms.string( "PixelTripletHLTGenerator" ),
        extraHitRPhitolerance = cms.double( 0.06 ),
        useMultScattering = cms.bool( True ),
        phiPreFiltering = cms.double( 0.3 ),
        extraHitRZtolerance = cms.double( 0.06 )
      ),
      SeedingLayers = cms.string( "PixelLayerTriplets" )
    ),
    SeedComparitorPSet = cms.PSet(  ComponentName = cms.string( "none" ) ),
    SeedCreatorPSet = cms.PSet( 
      ComponentName = cms.string( "SeedFromConsecutiveHitsCreator" ),
      propagator = cms.string( "PropagatorWithMaterial" )
    ),
    TTRHBuilder = cms.string( "WithTrackAngle" )
)
hltHITCkfTrackCandidatesHB8E29 = cms.EDProducer( "CkfTrackCandidateMaker",
    src = cms.InputTag( "hltHITPixelTripletSeedGeneratorHB8E29" ),
    TrajectoryBuilder = cms.string( "CkfTrajectoryBuilder" ),
    TrajectoryCleaner = cms.string( "TrajectoryCleanerBySharedHits" ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    useHitsSplitting = cms.bool( False ),
    doSeedingRegionRebuilding = cms.bool( False ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    cleanTrajectoryAfterInOut = cms.bool( False )
)
hltHITCtfWithMaterialTracksHB8E29 = cms.EDProducer( "TrackProducer",
    TrajectoryInEvent = cms.bool( False ),
    useHitsSplitting = cms.bool( False ),
    clusterRemovalInfo = cms.InputTag( "" ),
    alias = cms.untracked.string( "hltHITCtfWithMaterialTracksHB1E31" ),
    Fitter = cms.string( "hltKFFittingSmoother" ),
    Propagator = cms.string( "PropagatorWithMaterial" ),
    src = cms.InputTag( "hltHITCkfTrackCandidatesHB8E29" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    NavigationSchool = cms.string( "" )
)
hltHITIPTCorrectorHB8E29 = cms.EDProducer( "IPTCorrector",
    corTracksLabel = cms.InputTag( "hltHITCtfWithMaterialTracksHB8E29" ),
    filterLabel = cms.InputTag( "hltIsolPixelTrackL2FilterHB8E29" ),
    associationCone = cms.double( 0.2 )
)
hltIsolPixelTrackL3FilterHB8E29 = cms.EDFilter( "HLTPixelIsolTrackFilter",
    candTag = cms.InputTag( "hltHITIPTCorrectorHB8E29" ),
    MinPtTrack = cms.double( 20.0 ),
    MaxPtNearby = cms.double( 2.0 ),
    MaxEtaTrack = cms.double( 2.0 ),
    MinEtaTrack = cms.double( 0.0 ),
    filterTrackEnergy = cms.bool( True ),
    MinEnergyTrack = cms.double( 20.0 ),
    NMaxTrackCandidates = cms.int32( 15 ),
    DropMultiL2Event = cms.bool( False )
)
hltL1EventNumberNZS = cms.EDFilter( "HLTL1NumberFilter",
    rawInput = cms.InputTag( "rawDataCollector" ),
    period = cms.uint32( 4096 ),
    invert = cms.bool( False )
)
hltPreHcalPhiSym = cms.EDFilter( "HLTPrescaler" )
hltL1sHcalNZS8E29 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleEG1 OR L1_SingleEG2 OR L1_SingleEG5 OR L1_SingleEG8 OR L1_SingleEG10 OR L1_SingleEG12 OR L1_SingleEG15 OR L1_SingleEG20 OR L1_SingleIsoEG5 OR L1_SingleIsoEG8 OR L1_SingleIsoEG10 OR L1_SingleIsoEG12 OR L1_SingleIsoEG15 OR L1_SingleJet6U OR L1_SingleJet10U OR L1_SingleJet20U OR L1_SingleJet30U OR L1_SingleJet40U OR L1_SingleJet50U OR L1_SingleJet60U OR L1_SingleTauJet10U OR L1_SingleTauJet20U OR L1_SingleTauJet30U OR L1_SingleTauJet50U OR L1_SingleMuOpen OR L1_SingleMu0 OR L1_SingleMu3 OR L1_SingleMu5 OR L1_SingleMu7 OR L1_SingleMu10 OR L1_SingleMu14 OR L1_SingleMu20 OR L1_MinBias_HTT10 OR L1_ZeroBias" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreHcalNZS8E29 = cms.EDFilter( "HLTPrescaler" )
hltL1sAlCaEcalPhiSym = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_ZeroBias OR L1_SingleEG1 OR L1_SingleEG2 OR L1_SingleEG5 OR L1_DoubleEG1 OR L1_SingleHfBitCountsRing1_1 OR L1_DoubleHfBitCountsRing1_P1N1 OR L1_SingleHfRingEtSumsRing1_4 OR L1_DoubleHfRingEtSumsRing1_P4N4 OR L1_SingleHfRingEtSumsRing2_4 OR L1_DoubleHfRingEtSumsRing2_P4N4" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreAlCaEcalPhiSym = cms.EDFilter( "HLTPrescaler" )
hltAlCaPhiSymStream = cms.EDFilter( "HLTEcalPhiSymFilter",
    barrelHitCollection = cms.InputTag( 'hltEcalRecHitAll','EcalRecHitsEB' ),
    endcapHitCollection = cms.InputTag( 'hltEcalRecHitAll','EcalRecHitsEE' ),
    phiSymBarrelHitCollection = cms.string( "phiSymEcalRecHitsEB" ),
    phiSymEndcapHitCollection = cms.string( "phiSymEcalRecHitsEE" ),
    eCut_barrel = cms.double( 0.15 ),
    eCut_endcap = cms.double( 0.75 ),
    eCut_barrel_high = cms.double( 999999.0 ),
    eCut_endcap_high = cms.double( 999999.0 ),
    statusThreshold = cms.uint32( 3 ),
    useRecoFlag = cms.bool( False )
)
hltL1sAlCaEcalPi0Eta8E29 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleIsoEG5 OR L1_SingleIsoEG8 OR L1_SingleIsoEG10 OR L1_SingleIsoEG12 OR L1_SingleIsoEG15 OR L1_SingleEG2 OR L1_SingleEG5 OR L1_SingleEG8 OR L1_SingleEG10 OR L1_SingleEG12 OR L1_SingleEG15 OR L1_SingleEG20 OR L1_SingleJet6U OR L1_SingleJet20U OR L1_SingleJet30U OR L1_SingleJet40U OR L1_SingleJet50U OR L1_DoubleJet30U OR L1_DoubleEG5" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreAlCaEcalPi08E29 = cms.EDFilter( "HLTPrescaler" )
hltEcalRegionalPi0EtaFEDs = cms.EDProducer( "EcalRawToRecHitRoI",
    sourceTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    type = cms.string( "egamma" ),
    doES = cms.bool( True ),
    sourceTag_es = cms.InputTag( "hltESRawToRecHitFacility" ),
    MuJobPSet = cms.PSet(  ),
    JetJobPSet = cms.VPSet( 
    ),
    EmJobPSet = cms.VPSet( 
      cms.PSet(  Source = cms.InputTag( 'hltL1extraParticles','Isolated' ),
        regionPhiMargin = cms.double( 0.4 ),
        Ptmin = cms.double( 2.0 ),
        regionEtaMargin = cms.double( 0.25 )
      ),
      cms.PSet(  Source = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
        regionPhiMargin = cms.double( 0.4 ),
        Ptmin = cms.double( 2.0 ),
        regionEtaMargin = cms.double( 0.25 )
      )
    ),
    CandJobPSet = cms.VPSet( 
    )
)
hltESRegionalPi0EtaRecHit = cms.EDProducer( "EcalRawToRecHitProducer",
    lazyGetterTag = cms.InputTag( "hltESRawToRecHitFacility" ),
    sourceTag = cms.InputTag( 'hltEcalRegionalPi0EtaFEDs','es' ),
    splitOutput = cms.bool( False ),
    EBrechitCollection = cms.string( "" ),
    EErechitCollection = cms.string( "" ),
    rechitCollection = cms.string( "EcalRecHitsES" )
)
hltEcalRegionalPi0EtaRecHit = cms.EDProducer( "EcalRawToRecHitProducer",
    lazyGetterTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    sourceTag = cms.InputTag( "hltEcalRegionalPi0EtaFEDs" ),
    splitOutput = cms.bool( True ),
    EBrechitCollection = cms.string( "EcalRecHitsEB" ),
    EErechitCollection = cms.string( "EcalRecHitsEE" ),
    rechitCollection = cms.string( "" )
)
hltAlCaPi0RegRecHits = cms.EDFilter( "HLTPi0RecHitsFilter",
    barrelHits = cms.InputTag( 'hltEcalRegionalPi0EtaRecHit','EcalRecHitsEB' ),
    endcapHits = cms.InputTag( 'hltEcalRegionalPi0EtaRecHit','EcalRecHitsEE' ),
    clusSeedThr = cms.double( 0.5 ),
    clusSeedThrEndCap = cms.double( 1.0 ),
    clusEtaSize = cms.int32( 3 ),
    clusPhiSize = cms.int32( 3 ),
    useRecoFlag = cms.bool( False ),
    flagLevelRecHitsToUse = cms.int32( 1 ),
    useDBStatus = cms.bool( True ),
    statusLevelRecHitsToUse = cms.int32( 1 ),
    nMinRecHitsSel1stCluster = cms.int32( 1 ),
    nMinRecHitsSel2ndCluster = cms.int32( 1 ),
    maxNumberofSeeds = cms.uint32( 1000 ),
    maxNumberofClusters = cms.uint32( 200 ),
    doSelForPi0Barrel = cms.bool( True ),
    selePtGamma = cms.double( 1.0 ),
    selePtPi0 = cms.double( 2.0 ),
    seleMinvMaxPi0 = cms.double( 0.22 ),
    seleMinvMinPi0 = cms.double( 0.06 ),
    seleS4S9Gamma = cms.double( 0.83 ),
    selePi0Iso = cms.double( 0.5 ),
    ptMinForIsolation = cms.double( 1.0 ),
    selePi0BeltDR = cms.double( 0.2 ),
    selePi0BeltDeta = cms.double( 0.05 ),
    storeIsoClusRecHitPi0EB = cms.bool( True ),
    pi0BarrelHitCollection = cms.string( "pi0EcalRecHitsEB" ),
    doSelForPi0Endcap = cms.bool( True ),
    region1_Pi0EndCap = cms.double( 2.0 ),
    selePtGammaPi0EndCap_region1 = cms.double( 0.8 ),
    selePtPi0EndCap_region1 = cms.double( 3.0 ),
    region2_Pi0EndCap = cms.double( 2.5 ),
    selePtGammaPi0EndCap_region2 = cms.double( 0.5 ),
    selePtPi0EndCap_region2 = cms.double( 2.0 ),
    selePtGammaPi0EndCap_region3 = cms.double( 0.3 ),
    selePtPi0EndCap_region3 = cms.double( 1.2 ),
    seleS4S9GammaEndCap = cms.double( 0.9 ),
    seleMinvMaxPi0EndCap = cms.double( 0.3 ),
    seleMinvMinPi0EndCap = cms.double( 0.05 ),
    ptMinForIsolationEndCap = cms.double( 0.5 ),
    selePi0BeltDREndCap = cms.double( 0.2 ),
    selePi0BeltDetaEndCap = cms.double( 0.05 ),
    selePi0IsoEndCap = cms.double( 0.5 ),
    storeIsoClusRecHitPi0EE = cms.bool( True ),
    pi0EndcapHitCollection = cms.string( "pi0EcalRecHitsEE" ),
    doSelForEtaBarrel = cms.bool( False ),
    selePtGammaEta = cms.double( 1.2 ),
    selePtEta = cms.double( 4.0 ),
    seleS4S9GammaEta = cms.double( 0.9 ),
    seleS9S25GammaEta = cms.double( 0.8 ),
    seleMinvMaxEta = cms.double( 0.8 ),
    seleMinvMinEta = cms.double( 0.3 ),
    ptMinForIsolationEta = cms.double( 1.0 ),
    seleEtaIso = cms.double( 0.5 ),
    seleEtaBeltDR = cms.double( 0.3 ),
    seleEtaBeltDeta = cms.double( 0.1 ),
    storeIsoClusRecHitEtaEB = cms.bool( True ),
    removePi0CandidatesForEta = cms.bool( True ),
    massLowPi0Cand = cms.double( 0.104 ),
    massHighPi0Cand = cms.double( 0.163 ),
    etaBarrelHitCollection = cms.string( "etaEcalRecHitsEB" ),
    store5x5RecHitEtaEB = cms.bool( True ),
    store5x5IsoClusRecHitEtaEB = cms.bool( True ),
    doSelForEtaEndcap = cms.bool( False ),
    region1_EtaEndCap = cms.double( 2.0 ),
    selePtGammaEtaEndCap_region1 = cms.double( 1.5 ),
    selePtEtaEndCap_region1 = cms.double( 5.0 ),
    region2_EtaEndCap = cms.double( 2.5 ),
    selePtGammaEtaEndCap_region2 = cms.double( 1.2 ),
    selePtEtaEndCap_region2 = cms.double( 4.0 ),
    selePtGammaEtaEndCap_region3 = cms.double( 0.7 ),
    selePtEtaEndCap_region3 = cms.double( 3.0 ),
    seleS4S9GammaEtaEndCap = cms.double( 0.9 ),
    seleS9S25GammaEtaEndCap = cms.double( 0.85 ),
    seleMinvMaxEtaEndCap = cms.double( 0.8 ),
    seleMinvMinEtaEndCap = cms.double( 0.3 ),
    ptMinForIsolationEtaEndCap = cms.double( 0.5 ),
    seleEtaIsoEndCap = cms.double( 0.5 ),
    seleEtaBeltDREndCap = cms.double( 0.3 ),
    seleEtaBeltDetaEndCap = cms.double( 0.1 ),
    storeIsoClusRecHitEtaEE = cms.bool( True ),
    etaEndcapHitCollection = cms.string( "etaEcalRecHitsEE" ),
    store5x5RecHitEtaEE = cms.bool( True ),
    store5x5IsoClusRecHitEtaEE = cms.bool( True ),
    preshRecHitProducer = cms.InputTag( 'hltESRegionalPi0EtaRecHit','EcalRecHitsES' ),
    storeRecHitES = cms.bool( True ),
    preshNclust = cms.int32( 4 ),
    preshClusterEnergyCut = cms.double( 0.0 ),
    preshStripEnergyCut = cms.double( 0.0 ),
    preshSeededNstrip = cms.int32( 15 ),
    preshCalibPlaneX = cms.double( 1.0 ),
    preshCalibPlaneY = cms.double( 0.7 ),
    preshCalibGamma = cms.double( 0.024 ),
    preshCalibMIP = cms.double( 9.0E-5 ),
    debugLevelES = cms.string( " " ),
    pi0ESCollection = cms.string( "pi0EcalRecHitsES" ),
    etaESCollection = cms.string( "etaEcalRecHitsES" ),
    ParameterLogWeighted = cms.bool( True ),
    ParameterX0 = cms.double( 0.89 ),
    ParameterT0_barl = cms.double( 7.4 ),
    ParameterT0_endc = cms.double( 3.1 ),
    ParameterT0_endcPresh = cms.double( 1.2 ),
    ParameterW0 = cms.double( 4.2 ),
    debugLevel = cms.int32( 0 )
)
hltPreAlCaEcalEta8E29 = cms.EDFilter( "HLTPrescaler" )
hltAlCaEtaRegRecHits = cms.EDFilter( "HLTPi0RecHitsFilter",
    barrelHits = cms.InputTag( 'hltEcalRegionalPi0EtaRecHit','EcalRecHitsEB' ),
    endcapHits = cms.InputTag( 'hltEcalRegionalPi0EtaRecHit','EcalRecHitsEE' ),
    clusSeedThr = cms.double( 0.5 ),
    clusSeedThrEndCap = cms.double( 1.0 ),
    clusEtaSize = cms.int32( 3 ),
    clusPhiSize = cms.int32( 3 ),
    useRecoFlag = cms.bool( False ),
    flagLevelRecHitsToUse = cms.int32( 1 ),
    useDBStatus = cms.bool( True ),
    statusLevelRecHitsToUse = cms.int32( 1 ),
    nMinRecHitsSel1stCluster = cms.int32( 1 ),
    nMinRecHitsSel2ndCluster = cms.int32( 1 ),
    maxNumberofSeeds = cms.uint32( 1000 ),
    maxNumberofClusters = cms.uint32( 200 ),
    doSelForPi0Barrel = cms.bool( False ),
    selePtGamma = cms.double( 1.0 ),
    selePtPi0 = cms.double( 2.0 ),
    seleMinvMaxPi0 = cms.double( 0.22 ),
    seleMinvMinPi0 = cms.double( 0.06 ),
    seleS4S9Gamma = cms.double( 0.83 ),
    selePi0Iso = cms.double( 0.5 ),
    ptMinForIsolation = cms.double( 1.0 ),
    selePi0BeltDR = cms.double( 0.2 ),
    selePi0BeltDeta = cms.double( 0.05 ),
    storeIsoClusRecHitPi0EB = cms.bool( True ),
    pi0BarrelHitCollection = cms.string( "pi0EcalRecHitsEB" ),
    doSelForPi0Endcap = cms.bool( False ),
    region1_Pi0EndCap = cms.double( 2.0 ),
    selePtGammaPi0EndCap_region1 = cms.double( 0.8 ),
    selePtPi0EndCap_region1 = cms.double( 3.0 ),
    region2_Pi0EndCap = cms.double( 2.5 ),
    selePtGammaPi0EndCap_region2 = cms.double( 0.5 ),
    selePtPi0EndCap_region2 = cms.double( 2.0 ),
    selePtGammaPi0EndCap_region3 = cms.double( 0.3 ),
    selePtPi0EndCap_region3 = cms.double( 1.2 ),
    seleS4S9GammaEndCap = cms.double( 0.9 ),
    seleMinvMaxPi0EndCap = cms.double( 0.3 ),
    seleMinvMinPi0EndCap = cms.double( 0.05 ),
    ptMinForIsolationEndCap = cms.double( 0.5 ),
    selePi0BeltDREndCap = cms.double( 0.2 ),
    selePi0BeltDetaEndCap = cms.double( 0.05 ),
    selePi0IsoEndCap = cms.double( 0.5 ),
    storeIsoClusRecHitPi0EE = cms.bool( True ),
    pi0EndcapHitCollection = cms.string( "pi0EcalRecHitsEE" ),
    doSelForEtaBarrel = cms.bool( True ),
    selePtGammaEta = cms.double( 1.2 ),
    selePtEta = cms.double( 4.0 ),
    seleS4S9GammaEta = cms.double( 0.9 ),
    seleS9S25GammaEta = cms.double( 0.8 ),
    seleMinvMaxEta = cms.double( 0.8 ),
    seleMinvMinEta = cms.double( 0.3 ),
    ptMinForIsolationEta = cms.double( 1.0 ),
    seleEtaIso = cms.double( 0.5 ),
    seleEtaBeltDR = cms.double( 0.3 ),
    seleEtaBeltDeta = cms.double( 0.1 ),
    storeIsoClusRecHitEtaEB = cms.bool( True ),
    removePi0CandidatesForEta = cms.bool( True ),
    massLowPi0Cand = cms.double( 0.104 ),
    massHighPi0Cand = cms.double( 0.163 ),
    etaBarrelHitCollection = cms.string( "etaEcalRecHitsEB" ),
    store5x5RecHitEtaEB = cms.bool( True ),
    store5x5IsoClusRecHitEtaEB = cms.bool( True ),
    doSelForEtaEndcap = cms.bool( True ),
    region1_EtaEndCap = cms.double( 2.0 ),
    selePtGammaEtaEndCap_region1 = cms.double( 1.5 ),
    selePtEtaEndCap_region1 = cms.double( 5.0 ),
    region2_EtaEndCap = cms.double( 2.5 ),
    selePtGammaEtaEndCap_region2 = cms.double( 1.2 ),
    selePtEtaEndCap_region2 = cms.double( 4.0 ),
    selePtGammaEtaEndCap_region3 = cms.double( 0.7 ),
    selePtEtaEndCap_region3 = cms.double( 3.0 ),
    seleS4S9GammaEtaEndCap = cms.double( 0.9 ),
    seleS9S25GammaEtaEndCap = cms.double( 0.85 ),
    seleMinvMaxEtaEndCap = cms.double( 0.8 ),
    seleMinvMinEtaEndCap = cms.double( 0.3 ),
    ptMinForIsolationEtaEndCap = cms.double( 0.5 ),
    seleEtaIsoEndCap = cms.double( 0.5 ),
    seleEtaBeltDREndCap = cms.double( 0.3 ),
    seleEtaBeltDetaEndCap = cms.double( 0.1 ),
    storeIsoClusRecHitEtaEE = cms.bool( True ),
    etaEndcapHitCollection = cms.string( "etaEcalRecHitsEE" ),
    store5x5RecHitEtaEE = cms.bool( True ),
    store5x5IsoClusRecHitEtaEE = cms.bool( True ),
    preshRecHitProducer = cms.InputTag( 'hltESRegionalPi0EtaRecHit','EcalRecHitsES' ),
    storeRecHitES = cms.bool( True ),
    preshNclust = cms.int32( 4 ),
    preshClusterEnergyCut = cms.double( 0.0 ),
    preshStripEnergyCut = cms.double( 0.0 ),
    preshSeededNstrip = cms.int32( 15 ),
    preshCalibPlaneX = cms.double( 1.0 ),
    preshCalibPlaneY = cms.double( 0.7 ),
    preshCalibGamma = cms.double( 0.024 ),
    preshCalibMIP = cms.double( 9.0E-5 ),
    debugLevelES = cms.string( " " ),
    pi0ESCollection = cms.string( "pi0EcalRecHitsES" ),
    etaESCollection = cms.string( "etaEcalRecHitsES" ),
    ParameterLogWeighted = cms.bool( True ),
    ParameterX0 = cms.double( 0.89 ),
    ParameterT0_barl = cms.double( 7.4 ),
    ParameterT0_endc = cms.double( 3.1 ),
    ParameterT0_endcPresh = cms.double( 1.2 ),
    ParameterW0 = cms.double( 4.2 ),
    debugLevel = cms.int32( 0 )
)
hltPreRPCMuonNoHits = cms.EDFilter( "HLTPrescaler" )
hltRPCMuonNoHitsL1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1SingleMuOpenL1SingleMu0" ),
    MaxEta = cms.double( 1.6 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    SaveTag = cms.untracked.bool( True ),
    SelectQualities = cms.vint32( 6 )
)
hltPreRPCMuonNorma = cms.EDFilter( "HLTPrescaler" )
hltRPCMuonNormaL1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1SingleMuOpenL1SingleMu0" ),
    MaxEta = cms.double( 1.6 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    SaveTag = cms.untracked.bool( True ),
    SelectQualities = cms.vint32(  )
)
hltPreAlCaDTErrors = cms.EDFilter( "HLTPrescaler" )
hltDTROMonitorFilter = cms.EDFilter( "HLTDTROMonitorFilter",
    inputLabel = cms.InputTag( "rawDataCollector" )
)
hltDynAlCaDTErrors = cms.EDFilter( "HLTDynamicPrescaler" )
hltCalibrationEventsFilter = cms.EDFilter( "HLTTriggerTypeFilter",
    SelectedTriggerType = cms.int32( 2 )
)
hltPreCalibration = cms.EDFilter( "HLTPrescaler" )
hltPreEcalCalibration = cms.EDFilter( "HLTPrescaler" )
hltEcalCalibrationRaw = cms.EDProducer( "SubdetFEDSelector",
    getECAL = cms.bool( True ),
    getSiStrip = cms.bool( False ),
    getSiPixel = cms.bool( False ),
    getHCAL = cms.bool( False ),
    getMuon = cms.bool( False ),
    getTrigger = cms.bool( False ),
    rawInputLabel = cms.InputTag( "rawDataCollector" )
)
hltHcalCalibTypeFilter = cms.EDFilter( "HLTHcalCalibTypeFilter",
    InputTag = cms.InputTag( "rawDataCollector" ),
    CalibTypes = cms.vint32( 1, 2, 3, 4, 5, 6 )
)
hltPreHcalCalibration = cms.EDFilter( "HLTPrescaler" )
hltRandomEventsFilter = cms.EDFilter( "HLTTriggerTypeFilter",
    SelectedTriggerType = cms.int32( 3 )
)
hltPreRandom = cms.EDFilter( "HLTPrescaler" )
hltL1sL1HFtech = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "8 OR 9 OR 10" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1HFTech = cms.EDFilter( "HLTPrescaler" )
hltL1sL1TechHFcoincidencePM = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "9" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1TechHFcoincidencePM = cms.EDFilter( "HLTPrescaler" )
hltPreHFThreshold3 = cms.EDFilter( "HLTPrescaler" )
hltHcalSimpleRecHitFilter = cms.EDFilter( "HLTHcalSimpleRecHitFilter",
    threshold = cms.double( 3.0 ),
    HFRecHitCollection = cms.InputTag( "hltHfreco" ),
    maskedChannels = cms.vint32( 8137, 8141, 8146, 8149, 8150, 8153 )
)
hltPreHFThreshold10 = cms.EDFilter( "HLTPrescaler" )
hltHcalSimpleRecHitFilter10 = cms.EDFilter( "HLTHcalSimpleRecHitFilter",
    threshold = cms.double( 10.0 ),
    HFRecHitCollection = cms.InputTag( "hltHfreco" ),
    maskedChannels = cms.vint32( 8137, 8141, 8146, 8149, 8150, 8153 )
)
hltL1sGlobalRunHPDNoise = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet10U_NotBptxC" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreGlobalRunHPDNoise = cms.EDFilter( "HLTPrescaler" )
hltL1sTechTrigHCALNoise = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "(11 OR 12) AND ( NOT 0) AND ( NOT 1) AND ( NOT 2)" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreTechTrigHCALNoise = cms.EDFilter( "HLTPrescaler" )
#hltL1sL1BPTX = cms.EDFilter( "HLTLevel1GTSeed",
#    L1UseL1TriggerObjectMaps = cms.bool( True ),
#    L1NrBxInEvent = cms.int32( 5 ),
#    L1TechTriggerSeeding = cms.bool( True ),
#    L1UseAliasesForSeeding = cms.bool( True ),
#    L1SeedsLogicalExpression = cms.string( "3" ),
#    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
#    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
#    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
#    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
#)

hltL1sL1BPTX = cms.EDFilter( "HLTBool",
                              result = cms.bool( True )
                             )

hltPreL1BPTX = cms.EDFilter( "HLTPrescaler" )
hltL1sL1BPTXMinusOnly = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_BptxMinus AND NOT L1_ZeroBias" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1BPTXMinusOnly = cms.EDFilter( "HLTPrescaler" )
hltL1sL1BPTXPlusOnly = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_BptxPlus AND NOT L1_ZeroBias" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL1BPTXPlusOnly = cms.EDFilter( "HLTPrescaler" )
hltL1sL1SingleMu0 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMu0" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreL2Mu0NoVertex = cms.EDFilter( "HLTPrescaler" )
hltSingleMu0L1Filtered = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1SingleMu0" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    SelectQualities = cms.vint32(  )
)
hltL2MuonCandidatesNoVtx = cms.EDProducer( "L2MuonCandidateProducer",
    InputObjects = cms.InputTag( "hltL2Muons" )
)
hltSingleL2Mu0L2PreFilteredNoVtx = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidatesNoVtx" ),
    PreviousCandTag = cms.InputTag( "hltSingleMu0L1Filtered" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 0.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
hltPreTkMu3NoVertex = cms.EDFilter( "HLTPrescaler" )
hltSingleMu3L2PreFilteredNoVtx = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidatesNoVtx" ),
    PreviousCandTag = cms.InputTag( "hltSingleMu0L1Filtered" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 0.0 ),
    NSigmaPt = cms.double( 0.0 )
)
hltL3TrajectorySeedNoVtx = cms.EDProducer( "TSGFromL2Muon",
    PtCut = cms.double( 1.0 ),
    PCut = cms.double( 2.5 ),
    MuonCollectionLabel = cms.InputTag( "hltL2Muons" ),
    tkSeedGenerator = cms.string( "TSGForRoadSearchOI" ),
    ServiceParameters = cms.PSet( 
      Propagators = cms.untracked.vstring( 'SteppingHelixPropagatorOpposite',
        'SteppingHelixPropagatorAlong' ),
      RPCLayers = cms.bool( True ),
      UseMuonNavigation = cms.untracked.bool( True )
    ),
    MuonTrackingRegionBuilder = cms.PSet(  ),
    TrackerSeedCleaner = cms.PSet(  ),
    TSGFromMixedPairs = cms.PSet(  ),
    TSGFromPixelTriplets = cms.PSet(  ),
    TSGFromPixelPairs = cms.PSet(  ),
    TSGForRoadSearchOI = cms.PSet( 
      propagatorCompatibleName = cms.string( "SteppingHelixPropagatorOpposite" ),
      option = cms.uint32( 3 ),
      maxChi2 = cms.double( 40.0 ),
      errorMatrixPset = cms.PSet( 
        atIP = cms.bool( True ),
        action = cms.string( "use" ),
        errorMatrixValuesPSet = cms.PSet( 
          pf3_V12 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V13 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V11 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 3.0, 3.0, 3.0, 5.0, 4.0, 5.0, 10.0, 7.0, 10.0, 10.0, 10.0, 10.0 )
          ),
          pf3_V14 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V15 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V34 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          yAxis = cms.vdouble( 0.0, 1.0, 1.4, 10.0 ),
          pf3_V33 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 3.0, 3.0, 3.0, 5.0, 4.0, 5.0, 10.0, 7.0, 10.0, 10.0, 10.0, 10.0 )
          ),
          pf3_V45 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V44 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 3.0, 3.0, 3.0, 5.0, 4.0, 5.0, 10.0, 7.0, 10.0, 10.0, 10.0, 10.0 )
          ),
          xAxis = cms.vdouble( 0.0, 13.0, 30.0, 70.0, 1000.0 ),
          pf3_V23 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V22 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 3.0, 3.0, 3.0, 5.0, 4.0, 5.0, 10.0, 7.0, 10.0, 10.0, 10.0, 10.0 )
          ),
          pf3_V55 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 3.0, 3.0, 3.0, 5.0, 4.0, 5.0, 10.0, 7.0, 10.0, 10.0, 10.0, 10.0 )
          ),
          zAxis = cms.vdouble( -3.14159, 3.14159 ),
          pf3_V35 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V25 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V24 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          )
        )
      ),
      propagatorName = cms.string( "SteppingHelixPropagatorAlong" ),
      manySeeds = cms.bool( False ),
      copyMuonRecHit = cms.bool( False ),
      ComponentName = cms.string( "TSGForRoadSearch" )
    ),
    TSGForRoadSearchIOpxl = cms.PSet(  ),
    TSGFromPropagation = cms.PSet(  ),
    TSGFromCombinedHits = cms.PSet(  )
)
hltL3TrackCandidateFromL2NoVtx = cms.EDProducer( "CkfTrajectoryMaker",
    trackCandidateAlso = cms.bool( True ),
    src = cms.InputTag( "hltL3TrajectorySeedNoVtx" ),
    TrajectoryBuilder = cms.string( "muonCkfTrajectoryBuilder" ),
    TrajectoryCleaner = cms.string( "TrajectoryCleanerBySharedHits" ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    useHitsSplitting = cms.bool( False ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    doSeedingRegionRebuilding = cms.bool( False ),
    cleanTrajectoryAfterInOut = cms.bool( False )
)
hltL3TkTracksFromL2NoVtx = cms.EDProducer( "TrackProducer",
    TrajectoryInEvent = cms.bool( False ),
    useHitsSplitting = cms.bool( False ),
    clusterRemovalInfo = cms.InputTag( "" ),
    alias = cms.untracked.string( "" ),
    Fitter = cms.string( "hltKFFittingSmoother" ),
    Propagator = cms.string( "PropagatorWithMaterial" ),
    src = cms.InputTag( "hltL3TrackCandidateFromL2NoVtx" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    NavigationSchool = cms.string( "" )
)
hltL3MuonsNoVtx = cms.EDProducer( "L3TkMuonProducer",
    InputObjects = cms.InputTag( "hltL3TkTracksFromL2NoVtx" )
)
hltL3MuonCandidatesNoVtx = cms.EDProducer( "L3MuonCandidateProducer",
    InputObjects = cms.InputTag( "hltL3MuonsNoVtx" )
)
hltSingleMu3L3PreFilterNoVtx = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidatesNoVtx" ),
    PreviousCandTag = cms.InputTag( "hltSingleMu3L2PreFilteredNoVtx" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 3.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
hltL1sEgammaOnlySC = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 5 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleEG5" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
hltPreEgammaOnlySC = cms.EDFilter( "HLTPrescaler" )
hltEgammaSCOnlyL1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sEgammaOnlySC" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
hltEgammaSCOnlyEtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltEgammaSCOnlyL1MatchFilterRegional" ),
    etcutEB = cms.double( 5.0 ),
    etcutEE = cms.double( 5.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
hltPhysicsDeclared = cms.EDFilter( "HLTPhysicsDeclared",
    invert = cms.bool( False ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
hltPreLogMonitor = cms.EDFilter( "HLTPrescaler" )
hltLogMonitorFilter = cms.EDFilter( "HLTLogMonitorFilter",
    default_threshold = cms.uint32( 10 ),
    categories = cms.VPSet( 
    )
)
hltPreDontNotifyT0 = cms.EDFilter( "HLTPrescaler" )
hltBoolFinalPath = cms.EDFilter( "HLTBool",
    result = cms.bool( False )
)
hltTriggerSummaryAOD = cms.EDProducer( "TriggerSummaryProducerAOD",
    processName = cms.string( "@" )
)
hltPreTriggerSummaryRAW = cms.EDFilter( "HLTPrescaler" )
hltTriggerSummaryRAW = cms.EDProducer( "TriggerSummaryProducerRAW",
    processName = cms.string( "@" )
)
hltPreHLTDQM = cms.EDFilter( "HLTPrescaler" )
hltPreHLTDQMSmart = cms.EDFilter( "HLTHighLevelDev",
    TriggerResultsTag = cms.InputTag( 'TriggerResults','','HLT' ),
    andOr = cms.bool( True ),
    throw = cms.bool( True ),
    eventSetupPathsKey = cms.string( "" ),
    HLTOverallPrescale = cms.uint32( 1 ),
    HLTPaths = cms.vstring( 'HLT_L1_BscMinBiasOR_BptxPlusORMinus_NoBPTX',
      'HLT_MinBiasBSC_NoBPTX',
      'HLT_L1SingleEG1',
      'AlCa_EcalEta_8E29',
      'AlCa_EcalPhiSym',
      'AlCa_EcalPi0_8E29',
      'HLT_Activity_DT',
      'HLT_Activity_Ecal',
      'HLT_Activity_EcalREM',
      'HLT_Activity_L1A',
      'HLT_Activity_PixelClusters',
      'HLT_BTagIP_Jet50U',
      'HLT_BTagMu_Jet10U',
      'HLT_BackwardBSC',
      'HLT_CSCBeamHalo',
      'HLT_CSCBeamHaloOverlapRing1',
      'HLT_CSCBeamHaloOverlapRing2',
      'HLT_CSCBeamHaloRing2or3',
      'HLT_Calibration',
      'HLT_DiJetAve15U_8E29',
      'HLT_DiJetAve30U_8E29',
      'HLT_DoubleEle5_SW_L1R',
      'HLT_DoubleLooseIsoTau15',
      'HLT_DoubleMu0',
      'HLT_DoubleMu3',
      'HLT_DoublePhoton10_L1R',
      'HLT_DoublePhoton5_Jpsi_L1R',
      'HLT_DoublePhoton5_Upsilon_L1R',
      'HLT_DoublePhoton5_eeRes_L1R',
      'HLT_EgammaSuperClusterOnly_L1R',
      'HLT_Ele10_LW_EleId_L1R',
      'HLT_Ele10_LW_L1R',
      'HLT_Ele15_LW_L1R',
      'HLT_Ele15_SC10_LW_L1R',
      'HLT_Ele15_SiStrip_L1R',
      'HLT_Ele20_LW_L1R',
      'HLT_ForwardBSC',
      'HLT_FwdJet20U',
      'HLT_GlobalRunHPDNoise',
      'HLT_HFThreshold10',
      'HLT_HFThreshold3',
      'HLT_HT100U',
      'HLT_HcalNZS_8E29',
      'HLT_HcalPhiSym',
      'HLT_HighMultiplicityBSC',
      'HLT_IsoMu3',
      'HLT_IsoTrackHB_8E29',
      'HLT_IsoTrackHE_8E29',
      'HLT_Jet15U',
      'HLT_Jet30U',
      'HLT_Jet50U',
      'HLT_L1DoubleEG5',
      'HLT_L1DoubleMuOpen',
      'HLT_L1Jet6U',
      'HLT_L1MET20',
      'HLT_L1Mu',
      'HLT_L1Mu14_L1ETM30',
      'HLT_L1Mu14_L1SingleEG10',
      'HLT_L1Mu14_L1SingleJet6U',
      'HLT_L1Mu20',
      'HLT_L1MuOpen',
      'HLT_L1MuOpen_NoBPTX',
      'HLT_L1SingleEG2',
      'HLT_L1SingleEG20_NoBPTX',
      'HLT_L1SingleEG5',
      'HLT_L1SingleEG5_NoBPTX',
      'HLT_L1SingleEG8',
      'HLT_L1SingleForJet',
      'HLT_L1Tech_HCAL_HF_coincidence_PM',
      'HLT_L1Tech_RPC_TTU_RBst1_collisions',
      'HLT_L1_BPTX',
      'HLT_L1_BPTX_MinusOnly',
      'HLT_L1_BPTX_PlusOnly',
      'HLT_L1_BSC',
      'HLT_L1_BscMinBiasOR_BptxPlusORMinus',
      'HLT_L1_HFtech',
      'HLT_L2Mu0_NoVertex',
      'HLT_L2Mu11',
      'HLT_L2Mu9',
      'HLT_MET100',
      'HLT_MET45',
      'HLT_MinBias',
      'HLT_MinBiasBSC',
      'HLT_MinBiasBSC_OR',
      'HLT_MinBiasEcal',
      'HLT_MinBiasHcal',
      'HLT_MinBiasPixel_DoubleIsoTrack5',
      'HLT_MinBiasPixel_DoubleTrack',
      'HLT_MinBiasPixel_SingleTrack',
      'HLT_Mu3',
      'HLT_Mu5',
      'HLT_Mu9',
      'HLT_Photon10_L1R',
      'HLT_Photon15_L1R',
      'HLT_Photon15_LooseEcalIso_L1R',
      'HLT_Photon15_TrackIso_L1R',
      'HLT_Photon20_L1R',
      'HLT_Photon30_L1R_8E29',
      'HLT_QuadJet15U',
      'HLT_RPCBarrelCosmics',
      'HLT_Random',
      'HLT_SingleLooseIsoTau20',
      'HLT_SplashBSC',
      'HLT_SplashEcalSumET',
      'HLT_StoppedHSCP_8E29',
      'HLT_TechTrigHCALNoise',
      'HLT_TkMu3_NoVertex',
      'HLT_TrackerCosmics',
      'HLT_ZeroBias',
      'HLT_ZeroBiasPixel_SingleTrack' ),
    HLTPathsPrescales = cms.vuint32( 1, 1, 1, 1, 100, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 100, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 10, 10, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 10, 10, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 100, 1 )
)
hltOutputHLTDQM = cms.OutputModule( "ShmStreamConsumer",
    SelectEvents = cms.untracked.PSet(  SelectEvents = cms.vstring( 'AlCa_EcalEta_8E29',
  'AlCa_EcalPhiSym',
  'AlCa_EcalPi0_8E29',
  'HLT_Activity_DT',
  'HLT_Activity_Ecal',
  'HLT_Activity_EcalREM',
  'HLT_Activity_L1A',
  'HLT_Activity_PixelClusters',
  'HLT_BTagIP_Jet50U',
  'HLT_BTagMu_Jet10U',
  'HLT_BackwardBSC',
  'HLT_CSCBeamHalo',
  'HLT_CSCBeamHaloOverlapRing1',
  'HLT_CSCBeamHaloOverlapRing2',
  'HLT_CSCBeamHaloRing2or3',
  'HLT_Calibration',
  'HLT_DiJetAve15U_8E29',
  'HLT_DiJetAve30U_8E29',
  'HLT_DoubleEle5_SW_L1R',
  'HLT_DoubleLooseIsoTau15',
  'HLT_DoubleMu0',
  'HLT_DoubleMu3',
  'HLT_DoublePhoton10_L1R',
  'HLT_DoublePhoton5_Jpsi_L1R',
  'HLT_DoublePhoton5_Upsilon_L1R',
  'HLT_DoublePhoton5_eeRes_L1R',
  'HLT_EgammaSuperClusterOnly_L1R',
  'HLT_Ele10_LW_EleId_L1R',
  'HLT_Ele10_LW_L1R',
  'HLT_Ele15_LW_L1R',
  'HLT_Ele15_SC10_LW_L1R',
  'HLT_Ele15_SiStrip_L1R',
  'HLT_Ele20_LW_L1R',
  'HLT_ForwardBSC',
  'HLT_FwdJet20U',
  'HLT_GlobalRunHPDNoise',
  'HLT_HFThreshold10',
  'HLT_HFThreshold3',
  'HLT_HT100U',
  'HLT_HcalNZS_8E29',
  'HLT_HcalPhiSym',
  'HLT_HighMultiplicityBSC',
  'HLT_IsoMu3',
  'HLT_IsoTrackHB_8E29',
  'HLT_IsoTrackHE_8E29',
  'HLT_Jet15U',
  'HLT_Jet30U',
  'HLT_Jet50U',
  'HLT_L1DoubleEG5',
  'HLT_L1DoubleMuOpen',
  'HLT_L1Jet6U',
  'HLT_L1MET20',
  'HLT_L1Mu',
  'HLT_L1Mu14_L1ETM30',
  'HLT_L1Mu14_L1SingleEG10',
  'HLT_L1Mu14_L1SingleJet6U',
  'HLT_L1Mu20',
  'HLT_L1MuOpen',
  'HLT_L1MuOpen_NoBPTX',
  'HLT_L1SingleEG1',
  'HLT_L1SingleEG2',
  'HLT_L1SingleEG20_NoBPTX',
  'HLT_L1SingleEG5',
  'HLT_L1SingleEG5_NoBPTX',
  'HLT_L1SingleEG8',
  'HLT_L1SingleForJet',
  'HLT_L1Tech_HCAL_HF_coincidence_PM',
  'HLT_L1Tech_RPC_TTU_RBst1_collisions',
  'HLT_L1_BPTX',
  'HLT_L1_BPTX_MinusOnly',
  'HLT_L1_BPTX_PlusOnly',
  'HLT_L1_BSC',
  'HLT_L1_BscMinBiasOR_BptxPlusORMinus',
  'HLT_L1_BscMinBiasOR_BptxPlusORMinus_NoBPTX',
  'HLT_L1_HFtech',
  'HLT_L2Mu0_NoVertex',
  'HLT_L2Mu11',
  'HLT_L2Mu9',
  'HLT_MET100',
  'HLT_MET45',
  'HLT_MinBias',
  'HLT_MinBiasBSC',
  'HLT_MinBiasBSC_NoBPTX',
  'HLT_MinBiasBSC_OR',
  'HLT_MinBiasEcal',
  'HLT_MinBiasHcal',
  'HLT_MinBiasPixel_DoubleIsoTrack5',
  'HLT_MinBiasPixel_DoubleTrack',
  'HLT_MinBiasPixel_SingleTrack',
  'HLT_Mu3',
  'HLT_Mu5',
  'HLT_Mu9',
  'HLT_Photon10_L1R',
  'HLT_Photon15_L1R',
  'HLT_Photon15_LooseEcalIso_L1R',
  'HLT_Photon15_TrackIso_L1R',
  'HLT_Photon20_L1R',
  'HLT_Photon30_L1R_8E29',
  'HLT_QuadJet15U',
  'HLT_RPCBarrelCosmics',
  'HLT_Random',
  'HLT_SingleLooseIsoTau20',
  'HLT_SplashBSC',
  'HLT_SplashEcalSumET',
  'HLT_StoppedHSCP_8E29',
  'HLT_TechTrigHCALNoise',
  'HLT_TkMu3_NoVertex',
  'HLT_TrackerCosmics',
  'HLT_ZeroBias',
  'HLT_ZeroBiasPixel_SingleTrack' ) ),
    outputCommands = cms.untracked.vstring( 'drop *_hlt*_*_*',
      'keep edmTriggerResults_*_*_*',
      'keep triggerTriggerEvent_*_*_*',
      'keep triggerTriggerEventWithRefs_*_*_*',
      'keep *_hltBSoftMuonL25JetsU_*_*',
      'keep *_hltIconeCentral1Regional_*_*',
      'keep *_hltBLifetimeL3BJetTagsStartupU_*_*',
      'keep *_hltL1sDoubleLooseIsoTau15_*_*',
      'keep *_hltIconeTau2Regional_*_*',
      'keep *_hltSiPixelClusters_*_*',
      'keep *_hltIconeCentral4Regional_*_*',
      'keep *_hltBLifetimeL3JetsStartupU_*_*',
      'keep *_hltRpcRecHits_*_*',
      'keep *_hltL1NonIsoLargeWindowElectronPixelSeeds_*_*',
      'keep *_hltL1sSingleLooseIsoTau20_*_*',
      'keep *_hltL2TauRelaxingIsolationSelector_*_*',
      'keep *_hltBLifetimeL3TagInfosStartupU_*_*',
      'keep *_hltFilterL2EcalIsolationSingleLooseIsoTau20_*_*',
      'keep *_hltIconeTau4Regional_*_*',
      'keep *_hltMuonCSCDigis_*_*',
      'keep *_hltL2TauJets_*_*',
      'keep *_hltFilterL2EtCutSingleLooseIsoTau20_*_*',
      'keep *_hltBLifetimeRegionalCtfWithMaterialTracksStartupU_*_*',
      'keep *_hltIconeCentral3Regional_*_*',
      'keep *_hltL2MuonCandidates_*_*',
      'keep *_hltL1extraParticles_*_*',
      'keep *_hltTowerMakerForMuons_*_*',
      'keep *_hltDt4DSegments_*_*',
      'keep *_hltBSoftMuonL3BJetTagsUByDR_*_*',
      'keep *_hltBSoftMuonL25TagInfosU_*_*',
      'keep *_hltFilterL2EcalIsolationDoubleLooseIsoTau15_*_*',
      'keep *_hltL2TauNarrowConeIsolationProducer_*_*',
      'keep *_hltMCJetCorJetIcone5HF07_*_*',
      'keep *_hltBSoftMuonL3TagInfosU_*_*',
      'keep *_hltBLifetimeL25JetsStartupU_*_*',
      'keep *_hltL2MuonSeeds_*_*',
      'keep *_hltPixelTracks_*_*',
      'keep *_hltL1NonIsoRecoEcalCandidate_*_*',
      'keep *_hltAlCaPhiSymStream_*_*',
      'keep *_hltBLifetimeL25AssociatorStartupU_*_*',
      'keep *_hltCscSegments_*_*',
      'keep *_hltL3Muons_*_*',
      'keep *_hltIconeCentral2Regional_*_*',
      'keep *_hltBLifetimeL25BJetTagsStartupU_*_*',
      'keep *_hltL1IsoLargeWindowElectronPixelSeeds_*_*',
      'keep *_hltL1IsoSiStripElectronPixelSeeds_*_*',
      'keep *_hltPixelMatchLargeWindowElectronsL1Iso_*_*',
      'keep *_hltIconeTau1Regional_*_*',
      'keep *_hltL1IsoStartUpElectronPixelSeeds_*_*',
      'keep *_hltIterativeCone5CaloJets_*_*',
      'keep *_hltL2Muons_*_*',
      'keep *_hltAlCaPi0RegRecHits_*_*',
      'keep *_hltL1IsoRecoEcalCandidate_*_*',
      'keep *_hltL1IsolatedElectronHcalIsol_*_*',
      'keep *_hltBLifetimeL3AssociatorStartupU_*_*',
      'keep *_hltL3TrajectorySeed_*_*',
      'keep *_hltBLifetimeL25TagInfosStartupU_*_*',
      'keep *_hltOfflineBeamSpot_*_*',
      'keep *_hltMet_*_*',
      'keep *_hltBSoftMuonL25BJetTagsUByDR_*_*',
      'keep *_hltMuonRPCDigis_*_*',
      'keep *_hltL1NonIsolatedElectronHcalIsol_*_*',
      'keep *_hltCsc2DRecHits_*_*',
      'keep *_hltFilterL2EtCutDoubleLooseIsoTau15_*_*',
      'keep *_hltL1NonIsoStartUpElectronPixelSeeds_*_*',
      'keep *_hltL2MuonIsolations_*_*',
      'keep *_hltGtDigis_*_*',
      'keep *_hltPixelMatchLargeWindowElectronsL1NonIso_*_*',
      'keep *_hltL1NonIsoSiStripElectronPixelSeeds_*_*',
      'keep *_hltSiStripRawToClustersFacility_*_*',
      'keep *_hltL3MuonIsolations_*_*',
      'keep *_hltL3MuonCandidates_*_*',
      'keep *_hltIconeTau3Regional_*_*',
      'keep *_hltAlCaEtaRegRecHits_*_*' ),
    use_compression = cms.untracked.bool( True ),
    compression_level = cms.untracked.int32( 1 ),
    max_event_size = cms.untracked.int32( 7000000 )
)

HLTBeginSequenceBPTX = cms.Sequence( hltTriggerType + hltL1EventNumber + hltGtDigis + hltGctDigis + hltL1GtObjectMap + hltL1extraParticles + hltBPTXCoincidence + hltOfflineBeamSpot )
HLTEndSequence = cms.Sequence( hltBoolEnd )
HLTDoLocalPixelSequence = cms.Sequence( hltSiPixelDigis + hltSiPixelClusters + hltSiPixelRecHits )
HLTDoLocalHcalSequence = cms.Sequence( hltHcalDigis + hltHbhereco + hltHfreco + hltHoreco )
HLTDoCaloSequence = cms.Sequence( hltEcalRawToRecHitFacility + hltEcalRegionalRestFEDs + hltEcalRecHitAll + HLTDoLocalHcalSequence + hltTowerMakerForAll )
HLTBeginSequence = cms.Sequence( hltTriggerType + hltL1EventNumber + hltGtDigis + hltGctDigis + hltL1GtObjectMap + hltL1extraParticles + hltOfflineBeamSpot )
HLTRecoMETSequence = cms.Sequence( HLTDoCaloSequence + hltMet )
HLTRecoJetSequenceU = cms.Sequence( HLTDoCaloSequence + hltIterativeCone5CaloJets + hltMCJetCorJetIcone5HF07 )
HLTDoJet15UHTRecoSequence = cms.Sequence( hltJet15UHt )
HLTmuonlocalrecoSequence = cms.Sequence( hltMuonDTDigis + hltDt1DRecHits + hltDt4DSegments + hltMuonCSCDigis + hltCsc2DRecHits + hltCscSegments + hltMuonRPCDigis + hltRpcRecHits )
HLTL2muonrecoNocandSequence = cms.Sequence( HLTmuonlocalrecoSequence + hltL2MuonSeeds + hltL2Muons )
HLTL2muonrecoSequence = cms.Sequence( HLTL2muonrecoNocandSequence + hltL2MuonCandidates )
HLTL2muonisorecoSequence = cms.Sequence( hltEcalRawToRecHitFacility + hltEcalRegionalMuonsFEDs + hltEcalRegionalMuonsRecHit + HLTDoLocalHcalSequence + hltTowerMakerForMuons + hltL2MuonIsolations )
HLTDoLocalStripSequence = cms.Sequence( hltSiStripRawToClustersFacility + hltSiStripClusters )
HLTL3muonTkCandidateSequence = cms.Sequence( HLTDoLocalPixelSequence + HLTDoLocalStripSequence + hltL3TrajectorySeed + hltL3TrackCandidateFromL2 )
HLTL3muonrecoNocandSequence = cms.Sequence( HLTL3muonTkCandidateSequence + hltL3TkTracksFromL2 + hltL3Muons )
HLTL3muonrecoSequence = cms.Sequence( HLTL3muonrecoNocandSequence + hltL3MuonCandidates )
HLTL3muonisorecoSequence = cms.Sequence( hltPixelTracks + hltL3MuonIsolations )
HLTDoRegionalEgammaEcalSequence = cms.Sequence( hltESRawToRecHitFacility + hltEcalRawToRecHitFacility + hltEcalRegionalEgammaFEDs + hltEcalRegionalEgammaRecHit + hltESRegionalEgammaRecHit )
HLTMulti5x5SuperClusterL1Isolated = cms.Sequence( hltMulti5x5BasicClustersL1Isolated + hltMulti5x5SuperClustersL1Isolated + hltMulti5x5EndcapSuperClustersWithPreshowerL1Isolated + hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated )
HLTL1IsolatedEcalClustersSequence = cms.Sequence( hltHybridSuperClustersL1Isolated + hltCorrectedHybridSuperClustersL1Isolated + HLTMulti5x5SuperClusterL1Isolated )
HLTMulti5x5SuperClusterL1NonIsolated = cms.Sequence( hltMulti5x5BasicClustersL1NonIsolated + hltMulti5x5SuperClustersL1NonIsolated + hltMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated + hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolatedTemp + hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated )
HLTL1NonIsolatedEcalClustersSequence = cms.Sequence( hltHybridSuperClustersL1NonIsolated + hltCorrectedHybridSuperClustersL1NonIsolatedTemp + hltCorrectedHybridSuperClustersL1NonIsolated + HLTMulti5x5SuperClusterL1NonIsolated )
HLTDoLocalHcalWithoutHOSequence = cms.Sequence( hltHcalDigis + hltHbhereco + hltHfreco )
HLTSingleElectronLWEt10L1NonIsoHLTNonIsoSequence = cms.Sequence( HLTDoRegionalEgammaEcalSequence + HLTL1IsolatedEcalClustersSequence + HLTL1NonIsolatedEcalClustersSequence + hltL1IsoRecoEcalCandidate + hltL1NonIsoRecoEcalCandidate + hltL1NonIsoHLTNonIsoSingleElectronLWEt10L1MatchFilterRegional + hltL1NonIsoHLTNonIsoSingleElectronLWEt10EtFilter + HLTDoLocalHcalWithoutHOSequence + hltL1IsolatedElectronHcalIsol + hltL1NonIsolatedElectronHcalIsol + hltL1NonIsoHLTNonIsoSingleElectronLWEt10HcalIsolFilter + HLTDoLocalPixelSequence + HLTDoLocalStripSequence + hltL1IsoLargeWindowElectronPixelSeeds + hltL1NonIsoLargeWindowElectronPixelSeeds + hltL1NonIsoHLTNonIsoSingleElectronLWEt10PixelMatchFilter )
HLTPixelMatchLargeWindowElectronL1IsoTrackingSequence = cms.Sequence( hltCkfL1IsoLargeWindowTrackCandidates + hltCtfL1IsoLargeWindowWithMaterialTracks + hltPixelMatchLargeWindowElectronsL1Iso )
HLTPixelMatchLargeWindowElectronL1NonIsoTrackingSequence = cms.Sequence( hltCkfL1NonIsoLargeWindowTrackCandidates + hltCtfL1NonIsoLargeWindowWithMaterialTracks + hltPixelMatchLargeWindowElectronsL1NonIso )
HLTSingleElectronEt10L1NonIsoHLTEleIdSequence = cms.Sequence( HLTDoRegionalEgammaEcalSequence + HLTL1IsolatedEcalClustersSequence + HLTL1NonIsolatedEcalClustersSequence + hltL1IsoRecoEcalCandidate + hltL1NonIsoRecoEcalCandidate + hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdL1MatchFilterRegional + hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdEtFilter + hltL1IsoHLTClusterShape + hltL1NonIsoHLTClusterShape + hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdClusterShapeFilter + HLTDoLocalHcalWithoutHOSequence + hltL1IsolatedElectronHcalIsol + hltL1NonIsolatedElectronHcalIsol + hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdHcalIsolFilter + HLTDoLocalPixelSequence + HLTDoLocalStripSequence + hltL1IsoLargeWindowElectronPixelSeeds + hltL1NonIsoLargeWindowElectronPixelSeeds + hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdPixelMatchFilter + HLTPixelMatchLargeWindowElectronL1IsoTrackingSequence + HLTPixelMatchLargeWindowElectronL1NonIsoTrackingSequence + hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdOneOEMinusOneOPFilter + hltElectronL1IsoLargeWindowDetaDphi + hltElectronL1NonIsoLargeWindowDetaDphi + hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDetaFilter + hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDphiFilter )
HLTSingleElectronLWEt15L1NonIsoHLTNonIsoSequence = cms.Sequence( HLTDoRegionalEgammaEcalSequence + HLTL1IsolatedEcalClustersSequence + HLTL1NonIsolatedEcalClustersSequence + hltL1IsoRecoEcalCandidate + hltL1NonIsoRecoEcalCandidate + hltL1NonIsoHLTNonIsoSingleElectronLWEt15L1MatchFilterRegional + hltL1NonIsoHLTNonIsoSingleElectronLWEt15EtFilter + HLTDoLocalHcalWithoutHOSequence + hltL1IsolatedElectronHcalIsol + hltL1NonIsolatedElectronHcalIsol + hltL1NonIsoHLTNonIsoSingleElectronLWEt15HcalIsolFilter + HLTDoLocalPixelSequence + HLTDoLocalStripSequence + hltL1IsoLargeWindowElectronPixelSeeds + hltL1NonIsoLargeWindowElectronPixelSeeds + hltL1NonIsoHLTNonIsoSingleElectronLWEt15PixelMatchFilter )
HLTDoubleElectronEt5L1NonIsoHLTnonIsoSequence = cms.Sequence( HLTDoRegionalEgammaEcalSequence + HLTL1IsolatedEcalClustersSequence + HLTL1NonIsolatedEcalClustersSequence + hltL1IsoRecoEcalCandidate + hltL1NonIsoRecoEcalCandidate + hltL1NonIsoHLTNonIsoDoubleElectronEt5L1MatchFilterRegional + hltL1NonIsoHLTNonIsoDoubleElectronEt5EtFilter + HLTDoLocalHcalWithoutHOSequence + hltL1IsolatedElectronHcalIsol + hltL1NonIsolatedElectronHcalIsol + hltL1NonIsoHLTNonIsoDoubleElectronEt5HcalIsolFilter + HLTDoLocalPixelSequence + HLTDoLocalStripSequence + hltL1IsoStartUpElectronPixelSeeds + hltL1NonIsoStartUpElectronPixelSeeds + hltL1NonIsoHLTNonIsoDoubleElectronEt5PixelMatchFilter )
HLTDoublePhotonEt5eeResSequence = cms.Sequence( HLTDoRegionalEgammaEcalSequence + HLTL1IsolatedEcalClustersSequence + HLTL1NonIsolatedEcalClustersSequence + hltL1IsoRecoEcalCandidate + hltL1NonIsoRecoEcalCandidate + hltL1NonIsoDoublePhotonEt5eeResL1MatchFilterRegional + hltL1NonIsoDoublePhotonEt5eeResEtFilter + hltL1IsoHLTClusterShape + hltL1NonIsoHLTClusterShape + hltL1NonIsoDoublePhotonEt5eeResClusterShapeFilter + hltL1IsolatedPhotonEcalIsol + hltL1NonIsolatedPhotonEcalIsol + hltL1NonIsoDoublePhotonEt5eeResEcalIsolFilter + HLTDoLocalHcalWithoutHOSequence + hltL1IsolatedElectronHcalIsol + hltL1NonIsolatedElectronHcalIsol + hltL1NonIsoDoublePhotonEt5eeResHcalIsolFilter + hltL1NonIsoDoublePhotonEt5eeResPMMassFilter )
HLTDoublePhotonEt5JpsiSequence = cms.Sequence( HLTDoRegionalEgammaEcalSequence + HLTL1IsolatedEcalClustersSequence + HLTL1NonIsolatedEcalClustersSequence + hltL1IsoRecoEcalCandidate + hltL1NonIsoRecoEcalCandidate + hltL1NonIsoDoublePhotonEt5JpsiL1MatchFilterRegional + hltL1NonIsoDoublePhotonEt5JpsiEtFilter + hltL1IsoHLTClusterShape + hltL1NonIsoHLTClusterShape + hltL1NonIsoDoublePhotonEt5JpsiClusterShapeFilter + hltL1IsolatedPhotonEcalIsol + hltL1NonIsolatedPhotonEcalIsol + hltL1NonIsoDoublePhotonEt5JpsiEcalIsolFilter + HLTDoLocalHcalWithoutHOSequence + hltL1IsolatedElectronHcalIsol + hltL1NonIsolatedElectronHcalIsol + hltL1NonIsoDoublePhotonEt5JpsiHcalIsolFilter + hltL1NonIsoDoublePhotonEt5JpsiPMMassFilter )
HLTDoublePhotonEt5UpsSequence = cms.Sequence( HLTDoRegionalEgammaEcalSequence + HLTL1IsolatedEcalClustersSequence + HLTL1NonIsolatedEcalClustersSequence + hltL1IsoRecoEcalCandidate + hltL1NonIsoRecoEcalCandidate + hltL1NonIsoDoublePhotonEt5UpsL1MatchFilterRegional + hltL1NonIsoDoublePhotonEt5UpsEtFilter + hltL1IsoHLTClusterShape + hltL1NonIsoHLTClusterShape + hltL1NonIsoDoublePhotonEt5UpsClusterShapeFilter + hltL1IsolatedPhotonEcalIsol + hltL1NonIsolatedPhotonEcalIsol + hltL1NonIsoDoublePhotonEt5UpsEcalIsolFilter + HLTDoLocalHcalWithoutHOSequence + hltL1IsolatedElectronHcalIsol + hltL1NonIsolatedElectronHcalIsol + hltL1NonIsoDoublePhotonEt5UpsHcalIsolFilter + hltL1NonIsoDoublePhotonEt5UpsPMMassFilter )
HLTSinglePhoton10L1NonIsolatedHLTNonIsoSequence = cms.Sequence( HLTDoRegionalEgammaEcalSequence + HLTL1IsolatedEcalClustersSequence + HLTL1NonIsolatedEcalClustersSequence + hltL1IsoRecoEcalCandidate + hltL1NonIsoRecoEcalCandidate + hltL1NonIsoHLTNonIsoSinglePhotonEt10L1MatchFilterRegional + hltL1NonIsoHLTNonIsoSinglePhotonEt10EtFilter + HLTDoLocalHcalWithoutHOSequence + hltL1IsolatedPhotonHcalIsol + hltL1NonIsolatedPhotonHcalIsol + hltL1NonIsoHLTNonIsoSinglePhotonEt10HcalIsolFilter )
HLTSinglePhoton15L1NonIsolatedHLTNonIsoSequence = cms.Sequence( HLTDoRegionalEgammaEcalSequence + HLTL1IsolatedEcalClustersSequence + HLTL1NonIsolatedEcalClustersSequence + hltL1IsoRecoEcalCandidate + hltL1NonIsoRecoEcalCandidate + hltL1NonIsoHLTNonIsoSinglePhotonEt15L1MatchFilterRegional + hltL1NonIsoHLTNonIsoSinglePhotonEt15EtFilter + HLTDoLocalHcalWithoutHOSequence + hltL1IsolatedPhotonHcalIsol + hltL1NonIsolatedPhotonHcalIsol + hltL1NonIsoHLTNonIsoSinglePhotonEt15HcalIsolFilter )
HLTDoLocalTrackerSequence = cms.Sequence( HLTDoLocalPixelSequence + HLTDoLocalStripSequence )
HLTL1IsoEgammaRegionalRecoTrackerSequence = cms.Sequence( hltL1IsoEgammaRegionalPixelSeedGenerator + hltL1IsoEgammaRegionalCkfTrackCandidates + hltL1IsoEgammaRegionalCTFFinalFitWithMaterial )
HLTL1NonIsoEgammaRegionalRecoTrackerSequence = cms.Sequence( hltL1NonIsoEgammaRegionalPixelSeedGenerator + hltL1NonIsoEgammaRegionalCkfTrackCandidates + hltL1NonIsoEgammaRegionalCTFFinalFitWithMaterial )
HLTSinglePhoton15L1NonIsolatedHLTHTISequence = cms.Sequence( HLTDoRegionalEgammaEcalSequence + HLTL1IsolatedEcalClustersSequence + HLTL1NonIsolatedEcalClustersSequence + hltL1IsoRecoEcalCandidate + hltL1NonIsoRecoEcalCandidate + hltL1NonIsoSinglePhotonEt15HTIL1MatchFilterRegional + hltL1NonIsoSinglePhotonEt15HTIEtFilter + HLTDoLocalHcalWithoutHOSequence + hltL1IsolatedPhotonHcalIsol + hltL1NonIsolatedPhotonHcalIsol + hltL1NonIsoSinglePhotonEt15HTIHcalIsolFilter + HLTDoLocalTrackerSequence + HLTL1IsoEgammaRegionalRecoTrackerSequence + HLTL1NonIsoEgammaRegionalRecoTrackerSequence + hltL1IsoPhotonHollowTrackIsol + hltL1NonIsoPhotonHollowTrackIsol + hltL1NonIsoSinglePhotonEt15HTITrackIsolFilter )
HLTSinglePhoton15L1NonIsolatedHLTLEISequence = cms.Sequence( HLTDoRegionalEgammaEcalSequence + HLTL1IsolatedEcalClustersSequence + HLTL1NonIsolatedEcalClustersSequence + hltL1IsoRecoEcalCandidate + hltL1NonIsoRecoEcalCandidate + hltL1NonIsoSinglePhotonEt15LEIL1MatchFilterRegional + hltL1NonIsoSinglePhotonEt15LEIEtFilter + hltL1IsolatedPhotonEcalIsol + hltL1NonIsolatedPhotonEcalIsol + hltL1NonIsoSinglePhotonEt15LEIEcalIsolFilter + HLTDoLocalHcalWithoutHOSequence + hltL1IsolatedPhotonHcalIsol + hltL1NonIsolatedPhotonHcalIsol + hltL1NonIsoSinglePhotonEt15LEIHcalIsolFilter )
HLTSinglePhoton20L1NonIsolatedHLTNonIsoSequence = cms.Sequence( HLTDoRegionalEgammaEcalSequence + HLTL1IsolatedEcalClustersSequence + HLTL1NonIsolatedEcalClustersSequence + hltL1IsoRecoEcalCandidate + hltL1NonIsoRecoEcalCandidate + hltL1NonIsoHLTNonIsoSinglePhotonEt20L1MatchFilterRegional + hltL1NonIsoHLTNonIsoSinglePhotonEt20EtFilter + HLTDoLocalHcalWithoutHOSequence + hltL1IsolatedPhotonHcalIsol + hltL1NonIsolatedPhotonHcalIsol + hltL1NonIsoHLTNonIsoSinglePhotonEt20HcalIsolFilter )
HLTDoublePhotonEt10L1NonIsoHLTNonIsoSequence = cms.Sequence( HLTDoRegionalEgammaEcalSequence + HLTL1IsolatedEcalClustersSequence + HLTL1NonIsolatedEcalClustersSequence + hltL1IsoRecoEcalCandidate + hltL1NonIsoRecoEcalCandidate + hltL1NonIsoHLTNonIsoDoublePhotonEt10L1MatchFilterRegional + hltL1NonIsoHLTNonIsoDoublePhotonEt10EtFilter + HLTDoLocalHcalWithoutHOSequence + hltL1IsolatedPhotonHcalIsol + hltL1NonIsolatedPhotonHcalIsol + hltL1NonIsoHLTNonIsoDoublePhotonEt10HcalIsolFilter )
HLTDoRegionalJetEcalSequence = cms.Sequence( hltEcalRawToRecHitFacility + hltEcalRegionalJetsFEDs + hltEcalRegionalJetsRecHit )
HLTCaloTausCreatorRegionalSequence = cms.Sequence( HLTDoRegionalJetEcalSequence + HLTDoLocalHcalSequence + hltTowerMakerForJets + hltCaloTowersTau1Regional + hltIconeTau1Regional + hltCaloTowersTau2Regional + hltIconeTau2Regional + hltCaloTowersTau3Regional + hltIconeTau3Regional + hltCaloTowersTau4Regional + hltIconeTau4Regional + hltCaloTowersCentral1Regional + hltIconeCentral1Regional + hltCaloTowersCentral2Regional + hltIconeCentral2Regional + hltCaloTowersCentral3Regional + hltIconeCentral3Regional + hltCaloTowersCentral4Regional + hltIconeCentral4Regional )
HLTL2TauJetsSequence = cms.Sequence( HLTCaloTausCreatorRegionalSequence + hltL2TauJets )
HLTL2TauEcalIsolationSequence = cms.Sequence( hltL2TauNarrowConeIsolationProducer + hltL2TauRelaxingIsolationSelector )
HLTRecopixelvertexingSequence = cms.Sequence( hltPixelTracks + hltPixelVertices )
HLTBTagIPSequenceL25StartupU = cms.Sequence( HLTDoLocalPixelSequence + HLTRecopixelvertexingSequence + hltSelector4JetsU + hltBLifetimeL25JetsStartupU + hltBLifetimeL25AssociatorStartupU + hltBLifetimeL25TagInfosStartupU + hltBLifetimeL25BJetTagsStartupU )
HLTBTagIPSequenceL3StartupU = cms.Sequence( HLTDoLocalPixelSequence + HLTDoLocalStripSequence + hltBLifetimeL3JetsStartupU + hltBLifetimeRegionalPixelSeedGeneratorStartupU + hltBLifetimeRegionalCkfTrackCandidatesStartupU + hltBLifetimeRegionalCtfWithMaterialTracksStartupU + hltBLifetimeL3AssociatorStartupU + hltBLifetimeL3TagInfosStartupU + hltBLifetimeL3BJetTagsStartupU )
HLTBTagMuSequenceL25U = cms.Sequence( HLTL2muonrecoNocandSequence + hltSelector4JetsU + hltBSoftMuonL25JetsU + hltBSoftMuonL25TagInfosU + hltBSoftMuonL25BJetTagsUByDR )
HLTBTagMuSequenceL3U = cms.Sequence( HLTL3muonrecoNocandSequence + hltBSoftMuonL3TagInfosU + hltBSoftMuonL3BJetTagsUByPt + hltBSoftMuonL3BJetTagsUByDR )
HLTPixelTrackingForMinBiasSequence = cms.Sequence( hltPixelTracksForMinBias )
HLTL2HcalIsolTrackSequenceHE = cms.Sequence( HLTDoLocalPixelSequence + hltHITPixelTracksHB + hltHITPixelTracksHE + hltHITPixelVerticesHE )
HLTL2HcalIsolTrackSequenceHB = cms.Sequence( HLTDoLocalPixelSequence + hltHITPixelTracksHB + hltHITPixelVerticesHB )
HLTBeginSequenceNZS = cms.Sequence( hltTriggerType + hltL1EventNumberNZS + hltGtDigis + hltGctDigis + hltL1GtObjectMap + hltL1extraParticles + hltBPTXCoincidence + hltOfflineBeamSpot )
HLTDoRegionalPi0EtaSequence = cms.Sequence( hltESRawToRecHitFacility + hltEcalRawToRecHitFacility + hltEcalRegionalPi0EtaFEDs + hltESRegionalPi0EtaRecHit + hltEcalRegionalPi0EtaRecHit )
HLTL2muonrecoSequenceNoVtx = cms.Sequence( HLTL2muonrecoNocandSequence + hltL2MuonCandidatesNoVtx )
HLTL3muonTkCandidateSequenceNoVtx = cms.Sequence( HLTDoLocalPixelSequence + HLTDoLocalStripSequence + hltL3TrajectorySeedNoVtx + hltL3TrackCandidateFromL2NoVtx )
HLTL3muonrecoNocandSequenceNoVtx = cms.Sequence( HLTL3muonTkCandidateSequenceNoVtx + hltL3TkTracksFromL2NoVtx + hltL3MuonsNoVtx )
HLTL3muonrecoSequenceNoVtx = cms.Sequence( HLTL3muonrecoNocandSequenceNoVtx + hltL3MuonCandidatesNoVtx )

HLT_Activity_L1A = cms.Path( HLTBeginSequenceBPTX + hltLevel1Activity + hltPreActivityL1A + HLTEndSequence )
HLT_Activity_PixelClusters = cms.Path( HLTBeginSequenceBPTX + hltPreActivityPixelClusters + HLTDoLocalPixelSequence + hltPixelActivityFilter + HLTEndSequence )
HLT_Activity_DT = cms.Path( HLTBeginSequenceBPTX + hltPreActivityDT + hltMuonDTDigis + hltDTTFUnpacker + hltDTActivityFilter + HLTEndSequence )
HLT_Activity_Ecal = cms.Path( HLTBeginSequenceBPTX + hltVetoL1SingleEG2 + hltPreActivityCaloTower + HLTDoCaloSequence + hltTowerMakerForEcal + hltCaloTowerFilter + HLTEndSequence )
HLT_Activity_EcalREM = cms.Path( HLTBeginSequenceBPTX + hltVetoL1SingleEG2 + hltPreActivityEcal + hltEcalDigis + hltEcalFixedAlphaBetaFitUncalibRecHit + hltEcalRecHitAB + hltTowerMakerForEcalABFit + hltEcalTowerFilter + HLTEndSequence )
HLT_SplashEcalSumET = cms.Path( HLTBeginSequence + hltL1sSplashEcalSumET + hltPreSplashEcalSumET + HLTRecoMETSequence + hltSplashEcalEtSum + HLTEndSequence )
HLT_L1Jet6U = cms.Path( HLTBeginSequenceBPTX + hltL1sL1Jet6U + hltPreL1Jet6U + HLTEndSequence )
HLT_Jet15U = cms.Path( HLTBeginSequenceBPTX + hltL1sJet15U + hltPreJet15U + HLTRecoJetSequenceU + hlt1jet15U + HLTEndSequence )
HLT_Jet30U = cms.Path( HLTBeginSequenceBPTX + hltL1sJet30U + hltPreJet30U + HLTRecoJetSequenceU + hlt1jet30U + HLTEndSequence )
HLT_Jet50U = cms.Path( HLTBeginSequenceBPTX + hltL1sJet50U + hltPreJet50U + HLTRecoJetSequenceU + hlt1jet50U + HLTEndSequence )
HLT_L1SingleForJet = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleForJet + hltPreL1SingleForJet + HLTEndSequence )
HLT_FwdJet20U = cms.Path( HLTBeginSequenceBPTX + hltL1sFwdJet20U + hltPreFwdJet20U + HLTRecoJetSequenceU + hltRapGap20U + HLTEndSequence )
HLT_DiJetAve15U_8E29 = cms.Path( HLTBeginSequenceBPTX + hltL1sDiJetAve15U8E29 + hltPreDiJetAve15U8E29 + HLTDoCaloSequence + hltIterativeCone5CaloJets + hltDiJetAve15U8E29 + HLTEndSequence )
HLT_DiJetAve30U_8E29 = cms.Path( HLTBeginSequenceBPTX + hltL1sDiJetAve30U8E29 + hltPreDiJetAve30U8E29 + HLTDoCaloSequence + hltIterativeCone5CaloJets + hltDiJetAve30U8E29 + HLTEndSequence )
HLT_QuadJet15U = cms.Path( HLTBeginSequenceBPTX + hltL1sQuadJet15U + hltPreQuadJet15U + HLTRecoJetSequenceU + hlt4jet15U + HLTEndSequence )
HLT_L1MET20 = cms.Path( HLTBeginSequenceBPTX + hltL1sL1MET20 + hltPreL1MET20 + HLTEndSequence )
HLT_MET45 = cms.Path( HLTBeginSequenceBPTX + hltL1sMET45 + hltPreMET45 + HLTRecoMETSequence + hlt1MET45 + HLTEndSequence )
HLT_MET100 = cms.Path( HLTBeginSequenceBPTX + hltL1sMET100 + hltPreMET100 + HLTRecoMETSequence + hlt1MET100 + HLTEndSequence )
HLT_HT100U = cms.Path( HLTBeginSequenceBPTX + hltL1sHT100 + hltPreHT100 + HLTRecoJetSequenceU + HLTDoJet15UHTRecoSequence + hltHT100U + HLTEndSequence )
HLT_L1MuOpen_NoBPTX = cms.Path( HLTBeginSequence + hltL1sL1SingleMuOpenL1SingleMu0 + hltPreL1MuOpenNoBptx + hltL1MuOpenL1Filtered0 + HLTEndSequence )
HLT_L1MuOpen = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleMuOpenL1SingleMu0 + hltPreL1MuOpen + hltL1MuOpenL1Filtered0 + HLTEndSequence )
HLT_L1Mu = cms.Path( HLTBeginSequenceBPTX + hltL1sL1Mu + hltPreL1Mu + hltL1MuL1Filtered0 + HLTEndSequence )
HLT_L1Mu20 = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleMu20 + hltPreL1Mu20 + hltL1Mu20L1Filtered20 + HLTEndSequence )
HLT_L2Mu9 = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleMu7 + hltPreL2Mu9 + hltL1SingleMu7L1Filtered0 + HLTL2muonrecoSequence + hltL2Mu9L2Filtered9 + HLTEndSequence )
HLT_L2Mu11 = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleMu7 + hltPreL2Mu11 + hltL1SingleMu7L1Filtered0 + HLTL2muonrecoSequence + hltL2Mu11L2Filtered11 + HLTEndSequence )
HLT_IsoMu3 = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleMu3 + hltPreIsoMu3 + hltSingleMuIsoL1Filtered3 + HLTL2muonrecoSequence + hltSingleMuIsoL2PreFiltered3 + HLTL2muonisorecoSequence + hltSingleMuIsoL2IsoFiltered3 + HLTL3muonrecoSequence + hltSingleMuIsoL3PreFiltered3 + HLTL3muonisorecoSequence + hltSingleMuIsoL3IsoFiltered3 + HLTEndSequence )
HLT_Mu3 = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleMuOpenL1SingleMu0L1SingleMu3 + hltPreMu3 + hltL1SingleMu0L1Filtered0 + HLTL2muonrecoSequence + hltSingleMu3L2Filtered3 + HLTL3muonrecoSequence + hltSingleMu3L3Filtered3 + HLTEndSequence )
HLT_Mu5 = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleMu3 + hltPreMu5 + hltL1SingleMu3L1Filtered0 + HLTL2muonrecoSequence + hltSingleMu5L2Filtered4 + HLTL3muonrecoSequence + hltSingleMu5L3Filtered5 + HLTEndSequence )
HLT_Mu9 = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleMu7 + hltPreMu9 + hltL1SingleMu7L1Filtered0 + HLTL2muonrecoSequence + hltSingleMu9L2Filtered7 + HLTL3muonrecoSequence + hltSingleMu9L3Filtered9 + HLTEndSequence )
HLT_L1DoubleMuOpen = cms.Path( HLTBeginSequenceBPTX + hltL1sL1DoubleMuOpen + hltPreL1DoubleMuOpen + hltDoubleMuLevel1PathL1OpenFiltered + HLTEndSequence )
HLT_DoubleMu0 = cms.Path( HLTBeginSequenceBPTX + hltL1sL1DoubleMuOpen + hltPreDoubleMu0 + hltDiMuonL1Filtered0 + HLTL2muonrecoSequence + hltDiMuonL2PreFiltered0 + HLTL3muonrecoSequence + hltDiMuonL3PreFiltered0 + HLTEndSequence )
HLT_DoubleMu3 = cms.Path( HLTBeginSequenceBPTX + hltL1sL1DoubleMu3 + hltPreDoubleMu3 + hltDiMuonL1Filtered + HLTL2muonrecoSequence + hltDiMuonL2PreFiltered + HLTL3muonrecoSequence + hltDiMuonL3PreFiltered + HLTEndSequence )
HLT_L1SingleEG1 = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleEG1 + hltPreL1SingleEG1 + HLTEndSequence )
HLT_L1SingleEG2 = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleEG2 + hltPreL1SingleEG2 + HLTEndSequence )
HLT_L1SingleEG5 = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleEG5 + hltPreL1SingleEG5 + HLTEndSequence )
HLT_L1SingleEG5_NoBPTX = cms.Path( HLTBeginSequence + hltL1sL1SingleEG5 + hltPreL1SingleEG5_NoBPTX + HLTEndSequence )
HLT_L1SingleEG8 = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleEG8 + hltPreL1SingleEG8 + HLTEndSequence )
HLT_L1SingleEG20_NoBPTX = cms.Path( HLTBeginSequence + hltL1sL1SingleEG20 + hltPreL1SingleEG20 + HLTEndSequence )
HLT_Ele10_LW_L1R = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleEG5 + hltPreEle10LWL1R + HLTSingleElectronLWEt10L1NonIsoHLTNonIsoSequence + HLTEndSequence )
HLT_Ele10_LW_EleId_L1R = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleEG5 + hltPreEle10LWEleIdL1R + HLTSingleElectronEt10L1NonIsoHLTEleIdSequence + HLTEndSequence )
HLT_Ele15_LW_L1R = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleEG8 + hltPreEle15LWL1R + HLTSingleElectronLWEt15L1NonIsoHLTNonIsoSequence + HLTEndSequence )
HLT_Ele15_SC10_LW_L1R = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleEG8 + hltPreEle15SC10LWL1R + HLTSingleElectronLWEt15L1NonIsoHLTNonIsoSequence + hltL1NonIsoHLTNonIsoSingleElectronLWEt15ESscWrapper + hltL1NonIsoHLTNonIsoSingleElectronLWEt15ESDoubleSC10 + HLTEndSequence )
HLT_Ele15_SiStrip_L1R = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleEG8 + hltPreEle15SiStripL1R + HLTDoRegionalEgammaEcalSequence + HLTL1IsolatedEcalClustersSequence + HLTL1NonIsolatedEcalClustersSequence + hltL1IsoRecoEcalCandidate + hltL1NonIsoRecoEcalCandidate + hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15L1MatchFilterRegional + hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15EtFilter + HLTDoLocalHcalWithoutHOSequence + hltL1IsolatedElectronHcalIsol + hltL1NonIsolatedElectronHcalIsol + hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15HcalIsolFilter + HLTDoLocalPixelSequence + HLTDoLocalStripSequence + hltL1IsoSiStripElectronPixelSeeds + hltL1NonIsoSiStripElectronPixelSeeds + hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15PixelMatchFilter + HLTEndSequence )
HLT_Ele20_LW_L1R = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleEG8 + hltPreEle20LWL1R + HLTSingleElectronLWEt15L1NonIsoHLTNonIsoSequence + hltL1NonIsoHLTNonIsoSingleElectronLWEt15EtFilterESet20 + HLTEndSequence )
HLT_L1DoubleEG5 = cms.Path( HLTBeginSequenceBPTX + hltL1sL1DoubleEG5 + hltPreL1DoubleEG5 + HLTEndSequence )
HLT_DoubleEle5_SW_L1R = cms.Path( HLTBeginSequenceBPTX + hltL1sL1DoubleEG5 + hltPreDoubleEle5SWL1R + HLTDoubleElectronEt5L1NonIsoHLTnonIsoSequence + HLTEndSequence )
HLT_DoublePhoton5_eeRes_L1R = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleEG8orL1DoubleEG5 + hltPreDoublePhoton5eeResL1R + HLTDoublePhotonEt5eeResSequence + HLTEndSequence )
HLT_DoublePhoton5_Jpsi_L1R = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleEG8orL1DoubleEG5 + hltPreDoublePhoton5JpsiL1R + HLTDoublePhotonEt5JpsiSequence + HLTEndSequence )
HLT_DoublePhoton5_Upsilon_L1R = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleEG8orL1DoubleEG5 + hltPreDoublePhoton5UpsL1R + HLTDoublePhotonEt5UpsSequence + HLTEndSequence )
HLT_Photon10_L1R = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleEG5 + hltPrePhoton10L1R + HLTSinglePhoton10L1NonIsolatedHLTNonIsoSequence + HLTEndSequence )
HLT_Photon15_L1R = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleEG8 + hltPrePhoton15L1R + HLTSinglePhoton15L1NonIsolatedHLTNonIsoSequence + HLTEndSequence )
HLT_Photon15_TrackIso_L1R = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleEG8 + hltPrePhoton15HTIL1R + HLTSinglePhoton15L1NonIsolatedHLTHTISequence + HLTEndSequence )
HLT_Photon15_LooseEcalIso_L1R = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleEG8 + hltPrePhoton15LEIL1R + HLTSinglePhoton15L1NonIsolatedHLTLEISequence + HLTEndSequence )
HLT_Photon20_L1R = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleEG8 + hltPrePhoton20L1R + HLTSinglePhoton20L1NonIsolatedHLTNonIsoSequence + HLTEndSequence )
HLT_Photon30_L1R_8E29 = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleEG8 + hltPrePhoton30L1R8E29 + HLTSinglePhoton15L1NonIsolatedHLTNonIsoSequence + hltL1NonIsoHLTNonIsoSinglePhotonEt15EtFilterESet308E29 + HLTEndSequence )
HLT_DoublePhoton10_L1R = cms.Path( HLTBeginSequenceBPTX + hltL1sL1DoubleEG5 + hltPreDoublePhoton10L1R + HLTDoublePhotonEt10L1NonIsoHLTNonIsoSequence + HLTEndSequence )
HLT_SingleLooseIsoTau20 = cms.Path( HLTBeginSequenceBPTX + hltL1sSingleLooseIsoTau20 + hltPreSingleLooseIsoTau20 + HLTL2TauJetsSequence + hltFilterL2EtCutSingleLooseIsoTau20 + HLTL2TauEcalIsolationSequence + hltL1HLTSingleLooseIsoTau20JetsMatch + hltFilterL2EcalIsolationSingleLooseIsoTau20 + HLTEndSequence )
HLT_DoubleLooseIsoTau15 = cms.Path( HLTBeginSequenceBPTX + hltL1sDoubleLooseIsoTau15 + hltPreDoubleLooseIsoTau15 + HLTL2TauJetsSequence + hltFilterL2EtCutDoubleLooseIsoTau15 + HLTL2TauEcalIsolationSequence + hltL1HLTDoubleLooseIsoTau15JetsMatch + hltFilterL2EcalIsolationDoubleLooseIsoTau15 + HLTEndSequence )
HLT_BTagIP_Jet50U = cms.Path( HLTBeginSequenceBPTX + hltL1sBTagIPJet50U + hltPreBTagIPJet50U + HLTRecoJetSequenceU + hltBJet50U + HLTBTagIPSequenceL25StartupU + hltBLifetimeL25FilterStartupU + HLTBTagIPSequenceL3StartupU + hltBLifetimeL3FilterStartupU + HLTEndSequence )
HLT_BTagMu_Jet10U = cms.Path( HLTBeginSequenceBPTX + hltL1sBTagMuJet10U + hltPreBTagMuJet10U + HLTRecoJetSequenceU + hltBJet10U + HLTBTagMuSequenceL25U + hltBSoftMuonL25FilterUByDR + HLTBTagMuSequenceL3U + hltBSoftMuonL3FilterUByDR + HLTEndSequence )
HLT_StoppedHSCP_8E29 = cms.Path( HLTBeginSequence + hltL1sStoppedHSCP8E29 + hltPreStoppedHSCP8E29 + hltHcalDigis + hltHbhereco + hltStoppedHSCPHpdFilter + hltStoppedHSCPTowerMakerForAll + hltStoppedHSCPIterativeCone5CaloJets + hltStoppedHSCP1CaloJetEnergy + HLTEndSequence )
HLT_L1Mu14_L1SingleEG10 = cms.Path( HLTBeginSequenceBPTX + hltL1sL1Mu14L1SingleEG10 + hltPreL1Mu14L1SingleEG10 + HLTEndSequence )
HLT_L1Mu14_L1SingleJet6U = cms.Path( HLTBeginSequenceBPTX + hltL1sL1Mu14L1SingleJet6U + hltPreL1Mu14L1SingleJet6U + HLTEndSequence )
HLT_L1Mu14_L1ETM30 = cms.Path( HLTBeginSequenceBPTX + hltL1sL1Mu14L1ETM30 + hltPreL1Mu14L1ETM30 + HLTEndSequence )
HLT_ZeroBias = cms.Path( HLTBeginSequence + hltL1sZeroBias + hltPreZeroBias + HLTEndSequence )
HLT_ZeroBias1kHz = cms.Path( HLTBeginSequence + hltL1sZeroBias + hltPreZeroBias1kHz + HLTEndSequence )
HLT_MinBias = cms.Path( HLTBeginSequenceBPTX + hltL1sMinBias + hltPreMinBias + HLTEndSequence )
HLT_MinBiasBSC = cms.Path( HLTBeginSequenceBPTX + hltL1sMinBiasBSC + hltPreMinBiasBSC + HLTEndSequence )
HLT_MinBiasBSC_NoBPTX = cms.Path( HLTBeginSequence + hltL1sMinBiasBSC + hltPreMinBiasBSCNoBPTX + HLTEndSequence )
HLT_MinBiasBSC_OR = cms.Path( HLTBeginSequenceBPTX + hltL1sMinBiasBSCOR + hltPreMinBiasBSCOR + HLTEndSequence )
HLT_MinBiasHcal = cms.Path( HLTBeginSequenceBPTX + hltL1sMinBiasHcal + hltPreMinBiasHcal + HLTEndSequence )
HLT_MinBiasEcal = cms.Path( HLTBeginSequenceBPTX + hltL1sMinBiasEcal + hltPreMinBiasEcal + HLTEndSequence )
HLT_ZeroBiasPixel_SingleTrack = cms.Path( HLTBeginSequence + hltL1sZeroBias + hltPreZeroBiasPixelSingleTrack + HLTDoLocalPixelSequence + HLTPixelTrackingForMinBiasSequence + hltPixelCands + hltMinBiasPixelFilter1 + HLTEndSequence )
HLT_MinBiasPixel_SingleTrack = cms.Path( HLTBeginSequenceBPTX + hltPreMinBiasPixelSingleTrack + HLTDoLocalPixelSequence + HLTPixelTrackingForMinBiasSequence + hltPixelCands + hltMinBiasPixelFilter1 + HLTEndSequence )
HLT_MinBiasPixel_DoubleTrack = cms.Path( HLTBeginSequenceBPTX + hltPreMinBiasPixelDoubleTrack + HLTDoLocalPixelSequence + HLTPixelTrackingForMinBiasSequence + hltPixelCands + hltMinBiasPixelFilter2 + HLTEndSequence )
HLT_MinBiasPixel_DoubleIsoTrack5 = cms.Path( HLTBeginSequenceBPTX + hltPreMinBiasPixelDoubleIsoTrack5 + HLTDoLocalPixelSequence + HLTPixelTrackingForMinBiasSequence + hltPixelCands + hltPixelMBForAlignment + HLTEndSequence )
HLT_CSCBeamHalo = cms.Path( HLTBeginSequence + hltL1sCSCBeamHalo + hltPreCSCBeamHalo + HLTEndSequence )
HLT_CSCBeamHaloOverlapRing1 = cms.Path( HLTBeginSequenceBPTX + hltL1sCSCBeamHaloOverlapRing1 + hltPreCSCBeamHaloOverlapRing1 + hltMuonCSCDigis + hltCsc2DRecHits + hltOverlapsHLTCSCBeamHaloOverlapRing1 + HLTEndSequence )
HLT_CSCBeamHaloOverlapRing2 = cms.Path( HLTBeginSequenceBPTX + hltL1sCSCBeamHaloOverlapRing2 + hltPreCSCBeamHaloOverlapRing2 + hltMuonCSCDigis + hltCsc2DRecHits + hltOverlapsHLTCSCBeamHaloOverlapRing2 + HLTEndSequence )
HLT_CSCBeamHaloRing2or3 = cms.Path( HLTBeginSequenceBPTX + hltL1sCSCBeamHaloRing2or3 + hltPreCSCBeamHaloRing2or3 + hltMuonCSCDigis + hltCsc2DRecHits + hltFilter23HLTCSCBeamHaloRing2or3 + HLTEndSequence )
HLT_BackwardBSC = cms.Path( HLTBeginSequenceBPTX + hltL1sBackwardBSC + hltPreBackwardBSC + HLTEndSequence )
HLT_ForwardBSC = cms.Path( HLTBeginSequenceBPTX + hltL1sForwardBSC + hltPreForwardBSC + HLTEndSequence )
HLT_HighMultiplicityBSC = cms.Path( HLTBeginSequenceBPTX + hltL1sHighMultiplicityBSC + hltPreHighMultiplicityBSC + HLTEndSequence )
HLT_SplashBSC = cms.Path( HLTBeginSequenceBPTX + hltL1sSplashBSC + hltPreSplashBSC + HLTEndSequence )
HLT_L1_BSC = cms.Path( HLTBeginSequenceBPTX + hltL1sL1BSC + hltPreL1BSC + HLTEndSequence )
HLT_L1_BscMinBiasOR_BptxPlusORMinus = cms.Path( HLTBeginSequenceBPTX + hltL1sL1BscMinBiasORBptxPlusORMinus + hltPreL1BscMinBiasORBptxPlusORMinus + HLTEndSequence )
HLT_L1_BscMinBiasOR_BptxPlusORMinus_NoBPTX = cms.Path( HLTBeginSequence + hltL1sL1BscMinBiasORBptxPlusORMinus + hltPreL1BscMinBiasORBptxPlusORMinusNoBptx + HLTEndSequence )
HLT_RPCBarrelCosmics = cms.Path( HLTBeginSequence + hltL1sRPCBarrelCosmics + hltPreRPCBarrelCosmics + HLTEndSequence )
HLT_TrackerCosmics = cms.Path( HLTBeginSequence + hltL1sTrackerCosmics + hltPreTrackerCosmics + HLTEndSequence )
HLT_L1Tech_RPC_TTU_RBst1_collisions = cms.Path( HLTBeginSequenceBPTX + hltL1sL1TechRPCTTURBst1collisions + hltPreL1TechRPCTTURBst1collisions + HLTEndSequence )
HLT_IsoTrackHE_8E29 = cms.Path( HLTBeginSequenceBPTX + hltL1sIsoTrack8E29 + hltPreIsoTrackHE8E29 + HLTL2HcalIsolTrackSequenceHE + hltIsolPixelTrackProdHE8E29 + hltIsolPixelTrackL2FilterHE8E29 + HLTDoLocalStripSequence + hltHITPixelTripletSeedGeneratorHE8E29 + hltHITCkfTrackCandidatesHE8E29 + hltHITCtfWithMaterialTracksHE8E29 + hltHITIPTCorrectorHE8E29 + hltIsolPixelTrackL3FilterHE8E29 + HLTEndSequence )
HLT_IsoTrackHB_8E29 = cms.Path( HLTBeginSequenceBPTX + hltL1sIsoTrack8E29 + hltPreIsoTrackHB8E29 + HLTL2HcalIsolTrackSequenceHB + hltIsolPixelTrackProdHB8E29 + hltIsolPixelTrackL2FilterHB8E29 + HLTDoLocalStripSequence + hltHITPixelTripletSeedGeneratorHB8E29 + hltHITCkfTrackCandidatesHB8E29 + hltHITCtfWithMaterialTracksHB8E29 + hltHITIPTCorrectorHB8E29 + hltIsolPixelTrackL3FilterHB8E29 + HLTEndSequence )
HLT_HcalPhiSym = cms.Path( HLTBeginSequenceNZS + hltLevel1Activity + hltPreHcalPhiSym + HLTEndSequence )
HLT_HcalNZS_8E29 = cms.Path( HLTBeginSequenceNZS + hltL1sHcalNZS8E29 + hltPreHcalNZS8E29 + HLTEndSequence )
AlCa_EcalPhiSym = cms.Path( HLTBeginSequenceBPTX + hltL1sAlCaEcalPhiSym + hltPreAlCaEcalPhiSym + hltEcalRawToRecHitFacility + hltESRawToRecHitFacility + hltEcalRegionalRestFEDs + hltEcalRecHitAll + hltAlCaPhiSymStream + HLTEndSequence )
AlCa_EcalPi0_8E29 = cms.Path( HLTBeginSequenceBPTX + hltL1sAlCaEcalPi0Eta8E29 + hltPreAlCaEcalPi08E29 + HLTDoRegionalPi0EtaSequence + hltAlCaPi0RegRecHits + HLTEndSequence )
AlCa_EcalEta_8E29 = cms.Path( HLTBeginSequenceBPTX + hltL1sAlCaEcalPi0Eta8E29 + hltPreAlCaEcalEta8E29 + HLTDoRegionalPi0EtaSequence + hltAlCaEtaRegRecHits + HLTEndSequence )
AlCa_RPCMuonNoHits = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleMuOpenL1SingleMu0 + hltPreRPCMuonNoHits + hltRPCMuonNoHitsL1Filtered0 + HLTmuonlocalrecoSequence + HLTEndSequence )
AlCa_RPCMuonNormalisation = cms.Path( HLTBeginSequenceBPTX + hltL1sL1SingleMuOpenL1SingleMu0 + hltPreRPCMuonNorma + hltRPCMuonNormaL1Filtered0 + HLTmuonlocalrecoSequence + HLTEndSequence )
HLT_DTErrors = cms.Path( hltPreAlCaDTErrors + hltDTROMonitorFilter + hltDynAlCaDTErrors + HLTEndSequence )
HLT_Calibration = cms.Path( hltCalibrationEventsFilter + hltL1EventNumber + hltPreCalibration + HLTEndSequence )
HLT_EcalCalibration = cms.Path( hltCalibrationEventsFilter + hltL1EventNumber + hltPreEcalCalibration + hltEcalCalibrationRaw + HLTEndSequence )
HLT_HcalCalibration = cms.Path( hltCalibrationEventsFilter + hltL1EventNumber + hltHcalCalibTypeFilter + hltPreHcalCalibration + HLTEndSequence )
HLT_Random = cms.Path( hltRandomEventsFilter + hltL1EventNumber + hltPreRandom + HLTEndSequence )
HLT_L1_HFtech = cms.Path( HLTBeginSequenceBPTX + hltL1sL1HFtech + hltPreL1HFTech + HLTEndSequence )
HLT_L1Tech_HCAL_HF_coincidence_PM = cms.Path( HLTBeginSequenceBPTX + hltL1sL1TechHFcoincidencePM + hltPreL1TechHFcoincidencePM + HLTEndSequence )
HLT_HFThreshold3 = cms.Path( HLTBeginSequenceBPTX + hltPreHFThreshold3 + hltHcalDigis + hltHfreco + hltHcalSimpleRecHitFilter + HLTEndSequence )
HLT_HFThreshold10 = cms.Path( HLTBeginSequenceBPTX + hltPreHFThreshold10 + hltHcalDigis + hltHfreco + hltHcalSimpleRecHitFilter10 + HLTEndSequence )
HLT_GlobalRunHPDNoise = cms.Path( HLTBeginSequence + hltL1sGlobalRunHPDNoise + hltPreGlobalRunHPDNoise + HLTEndSequence )
HLT_TechTrigHCALNoise = cms.Path( HLTBeginSequence + hltL1sTechTrigHCALNoise + hltPreTechTrigHCALNoise + HLTEndSequence )
HLT_L1_BPTX = cms.Path( HLTBeginSequence + hltL1sL1BPTX + hltPreL1BPTX + HLTEndSequence )
HLT_L1_BPTX_MinusOnly = cms.Path( HLTBeginSequence + hltL1sL1BPTX + hltL1sL1BPTXMinusOnly + hltPreL1BPTXMinusOnly + HLTEndSequence )
HLT_L1_BPTX_PlusOnly = cms.Path( HLTBeginSequence + hltL1sL1BPTX + hltL1sL1BPTXPlusOnly + hltPreL1BPTXPlusOnly + HLTEndSequence )
HLT_L2Mu0_NoVertex = cms.Path( HLTBeginSequence + hltL1sL1SingleMu0 + hltPreL2Mu0NoVertex + hltSingleMu0L1Filtered + HLTL2muonrecoSequenceNoVtx + hltSingleL2Mu0L2PreFilteredNoVtx + HLTEndSequence )
HLT_TkMu3_NoVertex = cms.Path( HLTBeginSequence + hltL1sL1SingleMu0 + hltPreTkMu3NoVertex + hltSingleMu0L1Filtered + HLTL2muonrecoSequenceNoVtx + hltSingleMu3L2PreFilteredNoVtx + HLTL3muonrecoSequenceNoVtx + hltSingleMu3L3PreFilterNoVtx + HLTEndSequence )
HLT_EgammaSuperClusterOnly_L1R = cms.Path( HLTBeginSequenceBPTX + hltL1sEgammaOnlySC + hltPreEgammaOnlySC + HLTDoRegionalEgammaEcalSequence + HLTL1IsolatedEcalClustersSequence + HLTL1NonIsolatedEcalClustersSequence + hltL1IsoRecoEcalCandidate + hltL1NonIsoRecoEcalCandidate + hltEgammaSCOnlyL1MatchFilterRegional + hltEgammaSCOnlyEtFilter + HLTEndSequence )
HLT_PhysicsDeclared = cms.Path( HLTBeginSequence + hltPhysicsDeclared + HLTEndSequence )
HLT_LogMonitor = cms.Path( hltPreLogMonitor + hltLogMonitorFilter + HLTEndSequence )
XXX_DontNotifyT0 = cms.Path( hltPreDontNotifyT0 + hltBoolFinalPath )
HLTriggerFinalPath = cms.Path( hltTriggerSummaryAOD + hltPreTriggerSummaryRAW + hltTriggerSummaryRAW + hltBoolFinalPath )
HLTDQMOutput = cms.EndPath( hltPreHLTDQM + hltPreHLTDQMSmart + hltOutputHLTDQM )


HLTSchedule = cms.Schedule( HLT_Activity_L1A, HLT_Activity_PixelClusters, HLT_Activity_DT, HLT_Activity_Ecal, HLT_Activity_EcalREM, HLT_SplashEcalSumET, HLT_L1Jet6U, HLT_Jet15U, HLT_Jet30U, HLT_Jet50U, HLT_L1SingleForJet, HLT_FwdJet20U, HLT_DiJetAve15U_8E29, HLT_DiJetAve30U_8E29, HLT_QuadJet15U, HLT_L1MET20, HLT_MET45, HLT_MET100, HLT_HT100U, HLT_L1MuOpen_NoBPTX, HLT_L1MuOpen, HLT_L1Mu, HLT_L1Mu20, HLT_L2Mu9, HLT_L2Mu11, HLT_IsoMu3, HLT_Mu3, HLT_Mu5, HLT_Mu9, HLT_L1DoubleMuOpen, HLT_DoubleMu0, HLT_DoubleMu3, HLT_L1SingleEG1, HLT_L1SingleEG2, HLT_L1SingleEG5, HLT_L1SingleEG5_NoBPTX, HLT_L1SingleEG8, HLT_L1SingleEG20_NoBPTX, HLT_Ele10_LW_L1R, HLT_Ele10_LW_EleId_L1R, HLT_Ele15_LW_L1R, HLT_Ele15_SC10_LW_L1R, HLT_Ele15_SiStrip_L1R, HLT_Ele20_LW_L1R, HLT_L1DoubleEG5, HLT_DoubleEle5_SW_L1R, HLT_DoublePhoton5_eeRes_L1R, HLT_DoublePhoton5_Jpsi_L1R, HLT_DoublePhoton5_Upsilon_L1R, HLT_Photon10_L1R, HLT_Photon15_L1R, HLT_Photon15_TrackIso_L1R, HLT_Photon15_LooseEcalIso_L1R, HLT_Photon20_L1R, HLT_Photon30_L1R_8E29, HLT_DoublePhoton10_L1R, HLT_SingleLooseIsoTau20, HLT_DoubleLooseIsoTau15, HLT_BTagIP_Jet50U, HLT_BTagMu_Jet10U, HLT_StoppedHSCP_8E29, HLT_L1Mu14_L1SingleEG10, HLT_L1Mu14_L1SingleJet6U, HLT_L1Mu14_L1ETM30, HLT_ZeroBias, HLT_ZeroBias1kHz, HLT_MinBias, HLT_MinBiasBSC, HLT_MinBiasBSC_NoBPTX, HLT_MinBiasBSC_OR, HLT_MinBiasHcal, HLT_MinBiasEcal, HLT_ZeroBiasPixel_SingleTrack, HLT_MinBiasPixel_SingleTrack, HLT_MinBiasPixel_DoubleTrack, HLT_MinBiasPixel_DoubleIsoTrack5, HLT_CSCBeamHalo, HLT_CSCBeamHaloOverlapRing1, HLT_CSCBeamHaloOverlapRing2, HLT_CSCBeamHaloRing2or3, HLT_BackwardBSC, HLT_ForwardBSC, HLT_HighMultiplicityBSC, HLT_SplashBSC, HLT_L1_BSC, HLT_L1_BscMinBiasOR_BptxPlusORMinus, HLT_L1_BscMinBiasOR_BptxPlusORMinus_NoBPTX, HLT_RPCBarrelCosmics, HLT_TrackerCosmics, HLT_L1Tech_RPC_TTU_RBst1_collisions, HLT_IsoTrackHE_8E29, HLT_IsoTrackHB_8E29, HLT_HcalPhiSym, HLT_HcalNZS_8E29, AlCa_EcalPhiSym, AlCa_EcalPi0_8E29, AlCa_EcalEta_8E29, AlCa_RPCMuonNoHits, AlCa_RPCMuonNormalisation, HLT_DTErrors, HLT_Calibration, HLT_EcalCalibration, HLT_HcalCalibration, HLT_Random, HLT_L1_HFtech, HLT_L1Tech_HCAL_HF_coincidence_PM, HLT_HFThreshold3, HLT_HFThreshold10, HLT_GlobalRunHPDNoise, HLT_TechTrigHCALNoise, HLT_L1_BPTX, HLT_L1_BPTX_MinusOnly, HLT_L1_BPTX_PlusOnly, HLT_L2Mu0_NoVertex, HLT_TkMu3_NoVertex, HLT_EgammaSuperClusterOnly_L1R, HLT_PhysicsDeclared, HLT_LogMonitor, XXX_DontNotifyT0, HLTriggerFinalPath, HLTDQMOutput )