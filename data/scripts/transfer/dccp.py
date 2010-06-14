import os

prefix = "/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/edwenger"
dir    = "MinBias_TuneD6T_7TeV-pythia6/MB7TEV-D6T-V26B-v0/0a4687a61a8ef51e73b340419a6958d4"
out    = "mergeD6T.root"
cpdir  = "/net/hibat0007/d00/scratch/edwenger/d6t"

filelist = os.listdir(prefix + "/" + dir)

for file in filelist:
    print 'preparing to copy ' + file + ' from dcache'
    fullpath = prefix + "/" + dir + "/" + file
    if not os.path.isfile(cpdir + "/" + file):
        os.system('dccp ' + fullpath + ' ' + cpdir)
    else:
        print file + " already exists in 'cpdir'"
                     
