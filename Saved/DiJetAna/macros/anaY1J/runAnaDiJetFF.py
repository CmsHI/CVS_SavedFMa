import os

doMC=1
AnaV = "a7j2t0"
inFile0Name="~/scratch01/ana/Hydj_Hard_GSR_NZS/UQDJ80_120_proc0831_10k.root"
SrcName="HydUQDJ80"
header="Hydjet+UQDJ80"
SysAnas = {
    AnaV+"/s0":["dijetAna_mc_calojet_genp",100,200,50,2.,"Ana","Ana","log\(1./zn\)","log\(1./za\)"]
    }

# run
for sysAna, [mod,NrJEtMin,NrJEtMax,AwJEtMin,AwJEtaMax,DJCutType,TrkCutType,varXiNrJES,varXiAwJES] in SysAnas.items():
  #print sysAna, ": ",mod,NrJEtMin,NrJEtMax,AwJEtMin,AwJEtaMax,DJCutType,TrkCutType,varXiNrJES,varXiAwJES
  cmd='root -b -q anaDiJetFF.C+\('
  cmd+=str(doMC)+','
  cmd+='\\"'+sysAna+'\\",'
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
  cmd+='\\"'+header+'\\"\)'
  print cmd
  os.system(cmd)
