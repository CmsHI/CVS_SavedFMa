import FWCore.ParameterSet.Config as cms
# os var's
from  os import getenv
cvsdir=getenv("SavedFMa")
import sys
sys.path.append(cvsdir)
# sys path
import sys
cfgpath=cvsdir+'/cmssw/Configuration/Sequences/python'
sys.path.append(cfgpath)

# main cfg
from skim_raw_template_cfg import *

# dataset
process.load("Local_source_cff")
process.source.fileNames = ['file:MinBias_cfi_GEN_SIM_DIGI_L1_DIGI2RAW.root']

# n of events to run
process.maxEvents.input = 100

# output file
process.output.fileName = 'MinBias_raw322patch2.root'
print process.output.fileName
print process.out_step
