import os
l = os.listdir("/pnfs/cmsaf.mit.edu/t2bat/cms/store/mc/Summer08/MinBias/GEN-SIM-RAW/STARTUP_V5_STARTUP_V5_v1/0002/")
l.sort()
l[8:] = []
fileNames = []
for i in l:
   fileNames.append("dcache:/pnfs/cmsaf.mit.edu/t2bat/cms/store/mc/Summer08/MinBias/GEN-SIM-RAW/STARTUP_V5_STARTUP_V5_v1/0002/" + i)
#print fileNames

