import os,sys

AnaV = "a1006"
RunV = sys.argv[1]
inFile0Name="~/scratch01/ana/Hydj_Hard_GSR_NZS/UQDJ80_120_proc0831_10k.root"
doMC=1
SrcName="HydjQDJUQ80"
header="HydjetQ+DJUQuen80"
AnaType="ff"
NrJEtMin=100
NrJEtMax=200
AwJEtMin=50

SysAnas = {
    "s00":["dijetAna_mc_genjet_genp",NrJEtMin,NrJEtMax,0,3.,"Ana","AnaSig","log\(1./zn\)","log\(1./za\)"],
    "s01":["dijetAna_mc_genjet_genp",NrJEtMin,NrJEtMax,0,2.,"Ana","AnaSig","log\(1./zn\)","log\(1./za\)"],
    "s02":["dijetAna_mc_genjet_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"Ana","AnaSig","log\(1./zn\)","log\(1./za\)"],
    "s03":["dijetAna_mc_genjet_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"Ana","Ana","log\(1./zn\)","log\(1./za\)"],
    "s04":["dijetAna_mc_genjet_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"AnaMatRefNr","Ana","log\(1./zn\)","log\(1./za\)"], # mat Nr
    "s05":["dijetAna_mc_genjet_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"AnaMatRef","Ana","log\(1./zn\)","log\(1./za\)"], # mat aw
    "s06":["dijetAna_mc_calojet_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"Ref","Ana","log\(nlrjet/ppt\)","log\(alrjet/ppt\)"], # calo order
    "s07":["dijetAna_mc_calojet_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"RefAwAnaMatNr","Ana","log\(nlrjet/ppt\)","log\(alrjet/ppt\)"], # cut on nr calo et, still matched
    "s08":["dijetAna_mc_calojet_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"AnaMatRef","Ana","log\(nlrjet/ppt\)","log\(alrjet/ppt\)"], # cut on aw calo et, still matched
    "s09":["dijetAna_mc_calojet_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"AnaMatRefNr","Ana","log\(nlrjet/ppt\)","log\(alrjet/ppt\)"], # no Aw mat
    "s10":["dijetAna_mc_calojet_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"Ana","Ana","log\(nlrjet/ppt\)","log\(alrjet/ppt\)"], # no Nr mat
    "s11":["dijetAna_mc_calojet_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"Ana","Ana","log\(1./zn\)","log\(1./za\)"] # xi use Nr JES -- all calojet
    }

# prepare ana
logdir='log/'+SrcName+'/'+AnaV
os.system("mkdir -p "+logdir)
logfile=logdir+"/ff_"+RunV+'.txt'
os.system('echo '+logdir+' > '+logfile)

# run ana
from operator import itemgetter
for sysana, [mod,NrJEtMin,NrJEtMax,AwJEtMin,AwJEtaMax,DJCutType,TrkCutType,varXiNrJES,varXiAwJES] in sorted(SysAnas.items()):
  os.system('echo;echo;echo --- '+sysana+' --- >> '+logfile)
  cmd='root -l -b -q anaDiJetFF.C+\('
  cmd+=str(doMC)+','
  cmd+='\\"'+AnaV+RunV+'\\",'
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
