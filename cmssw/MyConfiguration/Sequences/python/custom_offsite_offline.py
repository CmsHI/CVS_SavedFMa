def customise_offsite_offline(process):
   process.GlobalTag.connect = 'frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'
   process.GlobalTag.globaltag = 'MC_31X_V3::All'
   return(process)

# redefine process
process=customise_offsite_offline(process)

