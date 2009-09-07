#process.load("MyConfiguration/Sequences/python/Local_source_cff")
from dbs.python.DBSFetch import DBSFetch
def add_source_from_dbs_cff(process):
   process.source.fileNames = DBSFetch.getFiles('/RelValHydjetQ_MinBias_4TeV/CMSSW_3_2_4-MC_31X_V3-v1/GEN-SIM-RAW')
   return(process)

process = add_source_from_dbs_cff(process)

