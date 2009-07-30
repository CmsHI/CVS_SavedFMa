import FWCore.ParameterSet.Config as cms

# main cfg
from MyConfiguration.Sequences.python.skim_raw_template_cfg import *

# dataset
process.load("MyConfiguration.Sequences.python.Local_source_cff")
process.source.fileNames = ['file:Minbias_DIGI_L1_DIGI2RAW.root']

# n of events to run
process.maxEvents.input = 1000

# output file
process.output.fileName = 'MinBias_raw322patch2_Digi2RawFix_Ecal_Dttf_100.root'
process.output.outputCommands = ['drop *','keep FEDRawDataCollection_rawDataCollector_*_DIGI2RAW']
print process.output.fileName
print process.out_step
