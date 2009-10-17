#
# add poolsource inputs from a dir
# - for the use of glob:
#   * http://www.diveintopython.org/file_handling/os_module.html
#

import glob
def add_source_dir(process,datadir,prefix):
   l = glob.glob(datadir+"/*.root")
   l.sort()
   #l[8:] = []
   fileNames = []
   for i in l:
      fileNames.append(prefix+i)
   process.source.fileNames = fileNames
   return(process)

datadir="/d01/frankma/scratch/data/pp/dNdEta/Summer09/MinBias900GeV/GEN-SIM-RAW/MC_31X_V3-v1"
prefix="file:"
process = add_source_dir(process,datadir,prefix)

