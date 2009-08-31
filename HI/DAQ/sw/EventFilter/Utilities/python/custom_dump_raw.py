def local_mod(process):
   # sistrip min: 50
   # ecal min: 600
   # dttf min: 780
   process.dump.feds = [54,605,780]
   process.dtRawDump = cms.Path(process.dump)
   return (process)

process=local_mod(process)
