#process.load("MyConfiguration/Sequences/python/Local_source_cff")
from dbs.python.DBSFetch import DBSFetch
def add_source_from_dbs_cff(process):
   process.source.fileNames = DBSFetch.getFiles('/RelValQCD_Pt_80_120/CMSSW_3_1_2-MC_31X_V3-v1/GEN-SIM-DIGI-RAW-HLTDEBUG')
   return(process)

process = add_source_from_dbs_cff(process)

