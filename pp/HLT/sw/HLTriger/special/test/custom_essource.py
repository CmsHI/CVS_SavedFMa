#
# Use a local es source db file
#
def custom_essource(process):
   # EventSetup
   process.GlobalTag.globaltag = 'GR09_E_V3::All'
   return(process)

custom_essource(process)

