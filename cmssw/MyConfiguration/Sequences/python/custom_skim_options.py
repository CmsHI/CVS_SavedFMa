#
# Python skim options

# === Addition of Customization ===
print "\nBegin Skim Customization\n"
# now define the customization
def customise_skim(process):
   classname="FEDRawDataCollection"
   modulename="rawDataCollector"
   processname=process.name_()
   rawtag=classname+"_"+modulename+"_*_"+processname

   process.output.outputCommands = cms.untracked.vstring('drop *','keep '+rawtag)
   return(process)

# redefine process
process=customise_skim(process)

