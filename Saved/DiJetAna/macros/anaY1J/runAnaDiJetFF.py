import os
import sys

AnaV = "a7"
inFile0Name="~/scratch01/ana/Hydj_Hard_GSR_NZS/UQDJ80_120_proc0831_10k.root"
doMC=1
SrcName="HydUQDJ80"
header="Hydjet+UQDJ80"
AnaType="ff"

SysAnas = {
    "s0":["dijetAna_mc_genjet_genp",100,200,50,3.,"Ana","Ana","log\(1./zn\)","log\(1./za\)"],
    "s1":["dijetAna_mc_genjet_genp",100,200,50,2.,"Ana","Ana","log\(1./zn\)","log\(1./za\)"],
    "s5":["dijetAna_mc_calojet_genp",100,200,50,2.,"Ana","Ana","log\(1./zn\)","log\(1./za\)"]
    }

# run ana
from operator import itemgetter
for sysana, [mod,NrJEtMin,NrJEtMax,AwJEtMin,AwJEtaMax,DJCutType,TrkCutType,varXiNrJES,varXiAwJES] in sorted(SysAnas.items()):
  print '--- ',sysana,' ---'
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
  print cmd
  os.system(cmd)

sys.exit("all done");
