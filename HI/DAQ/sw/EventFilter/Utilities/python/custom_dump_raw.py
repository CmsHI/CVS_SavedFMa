def local_mod(process):
   process.dump.feds = [54]
   process.dtRawDump = cms.Path(process.dump)
   return (process)

process=local_mod(process)
