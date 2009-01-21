import os
datapath = "/net/pstore01/d00/scratch/frankma/data/cmssw/217/l1skim/prod/prod_digi_l1_digi2raw/STARTUP_V5/crab240_return_local_250k"
l = os.listdir(datapath)
l.sort()
#l[8:] = []
fileNames = []
for i in l:
   fileNames.append("file:" + datapath + "/" + i)
#print fileNames

