import os,sys

AnaV = "a7"
inFile0Name="~/scratch01/ana/Hydj_Hard_GSR_NZS/UQDJ80_120_proc0831_10k.root"
doMC=1
SrcName="HydUQDJ80"
header="Hydjet+UQDJ80"
AnaType="ff"

SysAnas = {
    "s0":["dijetAna_mc_genjet_genp",100,200,0,3.,"Ana","Ana","log\(1./zn\)","log\(1./za\)"],
    "s1":["dijetAna_mc_genjet_genp",100,200,0,2.,"Ana","Ana","log\(1./zn\)","log\(1./za\)"],
    "s8":["dijetAna_mc_calojet_genp",100,200,50,2.,"Ana","Ana","log\(1./zn\)","log\(1./za\)"]
    }

# prepare ana
logdir='log/'+SrcName+'/'+AnaV
os.system("mkdir -p "+logdir)
logfile=logdir+"/ff_"+sys.argv[1]+'.txt'
os.system('echo '+logdir+' > '+logfile)

# run ana
from operator import itemgetter
for sysana, [mod,NrJEtMin,NrJEtMax,AwJEtMin,AwJEtaMax,DJCutType,TrkCutType,varXiNrJES,varXiAwJES] in sorted(SysAnas.items()):
  os.system('echo --- '+sysana+' --- >> '+logfile)
  cmd='root -l -b -q anaDiJetFF.C+\('
  cmd+=str(doMC)+','
  cmd+='\\"'+AnaV+'\\",'
  cmd+='\\"'+mod+'\\",'
  cmd+=str(NrJEtMin)+','
  cmd+=str(NrJEtMax)+','
  cmd+=str(AwJEtMin)+','
  cmd+=str(AwJEtaMax)+','
  cmd+='\\"'+DJCutType+'\\",'
  cmd+='\\"'+TrkCutType+'\\",'
  cmd+='\\"'+varXiNrJES+'\\",'
  cmd+='\\"'+varXiAwJES+'\\",'
  cmd+='\\"'+inFile0Name+'\\",'
  cmd+='\\"'+SrcName+'\\",'
  cmd+='\\"'+header+'\\",'
  cmd+='\\"'+AnaType+sysana+'\\"\)'
  cmd+=' >> '+logfile
  print cmd
  os.system(cmd)

sys.exit("all done");
