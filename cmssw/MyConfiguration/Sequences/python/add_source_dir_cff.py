#
# add poolsource inputs from a dir
#

import os
def add_source_dir(process,datadir):
   l = os.listdir(datadir)
   l.sort()
   #l[8:] = []
   fileNames = []
   for i in l:
      fileNames.append(prefix+datadir+"/"+i)
   process.source.fileNames = fileNames
   return(process)

datadir="/pnfs/cmsaf.mit.edu/hibat/cms/users/cer/OpenHLT324"
prefix="dcache:"
process = add_source_dir(process,datadir)

