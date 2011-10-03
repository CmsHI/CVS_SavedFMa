import os

dcache  = "/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/frankma"
#dcache  = "/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/yilmaz"
scratch = "/net/hidsk0001/d00/scratch/frankma/data"

samples  = {
    #"HI2010_Run152957_test17":"HAZS",
    #"pp2760_AllPhysics_prod06":"pp2760",
    #"HI2010_SD_Jet35_prod05":"HAZS",
    "hcpr-j50u-itertrkcalo-v2_4":"HCPR",
    "hazs-j50u-itertrkcalo-v2_4":"HAZS",
    }

for sample,PD in samples.items():
  print "===== sample: ",PD,sample," =====\n"
  indir=dcache+"/"+PD+"/"+sample
  #indir=dcache+"/"+"/"+sample
  outdir=scratch+"/"+PD+"/"+sample
  os.system('mkdir -p '+outdir)

  for file in os.listdir(indir):
    if "root" not in file:
      continue
    infile=indir+"/"+file
    if not os.path.isfile(outdir + "/" + file):
      cmd = 'dccp ' + infile + ' ' + outdir
      print cmd
      os.system(cmd)
    else:
      print file + " already exists in '" + outdir + "' directory"

