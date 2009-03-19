import os
datadir="/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/yjlee/HYDJET_GEN_X2_MB_4.0TeV/HYDJET_GEN_X2_MB_4.0TeV/6ca09266124c2704b0cdb9d18c9c0d65"
l = os.listdir(datadir)
l.sort()
l[100:] = []
fileNames = []
for i in l:
   fileNames.append("dcache:" + datadir+ "/" + i)

#print fileNames

