import os,sys

AnaV = "a1012"
RunV = sys.argv[1]
#inFile0Name="~/scratch01/ana/Hydj_Hard_GSR_NZS/UQDJ80_120_proc0831_10k.root"
inFile0Name="~/scratch01/ana/s10-dj80to120/dj1009_Y1JAna011/dj_*.root"
doMC=1
#SrcName="HydjQDJUQ80"
#header="HydjetQ+DJUQuen80"
SrcName="s10Qcd80"
header="QCD-DiJet80to120"
AnaType="ff"
NrJEtMin=100
NrJEtMax=200
AwJEtMin=50

SysAnas = {
    #"s00":["djgen",NrJEtMin,NrJEtMax,0,3.,"Ana","AnaSig","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"s01":["djgen",NrJEtMin,NrJEtMax,0,2.,"Ana","AnaSig","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"s02":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,2.,"Ana","AnaSig","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    "s03":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,2.,"Ana","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"s04":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,2.,"AnaMatRefNr","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"], # mat Nr
    #"s05":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,2.,"AnaMatRef","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"], # mat aw
    #"s05b":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,2.,"RefOrderRef","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"], # mat aw
    "s06":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"Ref","Ana","log\(nlrjet/ppt\)","log\(alrjet/ppt\)"], # calo order
    #"s06b":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"Ref","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"], # calo order, with calo jes in xi
    #"s06b2":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"Ref","Ana","log\(nlrjet*1.1/ppt\)","log\(alrjet/ppt\)"], # calo order
    #"s07":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"RefAwAnaMatNr","Ana","log\(nlrjet/ppt\)","log\(alrjet/ppt\)"], # cut on nr calo et, still matched
    "s08":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"AnaMatRef","Ana","log\(nlrjet/ppt\)","log\(alrjet/ppt\)"], # cut on aw calo et, still matched
    #"s09":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"AnaMatRefNr","Ana","log\(nlrjet/ppt\)","log\(alrjet/ppt\)"], # no Aw mat
    "s10":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"Ana","Ana","log\(nlrjet/ppt\)","log\(alrjet/ppt\)"], # no Nr mat
    "s11":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"Ana","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"] # xi use Nr JES -- all calojet
    #"sf03a":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,2.,"AnaLz0","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"sf03b":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,2.,"AnaLz1","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"sf03c":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,2.,"AnaLz2","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"sf11a":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"AnaLz0","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"], 
    #"sf11b":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"AnaLz1","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"sf11c":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,"AnaLz2","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"]
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
