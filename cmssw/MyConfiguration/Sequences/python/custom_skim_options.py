#
# Python skim options

# === Addition of Customization ===
print "\nBegin Skim Customization\n"
# now define the customization
def customise_skim(process):
   process.output.outputCommands = ['drop *','keep FEDRawDataCollection_rawDataCollector_*_DIGI2RAW']
   return(process)

# redefine process
process=customise_skim(process)

