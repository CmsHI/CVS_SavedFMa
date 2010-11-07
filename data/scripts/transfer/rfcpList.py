import os
import sys                    # For exiting program
import commands               # Allows running of shell commands
import os                     # For making directories


storage  = "/castor/cern.ch/cms/store/caf/user/frankma"
scratch = "/d101/frankma/data"
samples  = {
    #"HR10All-PR1-v0":"HIAllPhysics"
    "HR10Exp3/r150304":"HIExpress"
    }

for sample,PD in samples.items():
  print "===== sample: ",PD,sample," =====\n"
  outdir=scratch+"/"+PD+"/"+sample
  os.system('mkdir -p '+outdir)

  fromDir=storage+"/"+sample
  filelist = commands.getoutput("rfdir %s | awk '{print $NF}'" % fromDir).split('\n')

  for file in filelist:
    infile=fromDir+"/"+file
    if not os.path.isfile(outdir + "/" + file):
      cmd = 'rfcp ' + infile + ' ' + outdir
      print cmd
      os.system(cmd)
    else:
      print file + " already exists in '" + outdir + "' directory"

