import os,sys

AnaV = "1125a1"
RunV = sys.argv[1]
#inFile0Name="~/scratch01/ana/s10-dj80to120/dj1009_Y1JAna011/dj_*.root"
#SrcName="HydjQDJUQ80"
#header="HydjetQDJUQuen80"
inFile0Name="dj_HydjetQ_DJQ80_F10GSR_GoodTrk1123.root"
doMC=0
SrcName="HCPR-GoodTrk1123_All0"
header="HLT_HIJet50U"
AnaType="ff"
NrJEtMin=100
NrJEtMax=200
AwJEtMin=50
JDPhi=3.14159*5./6

SysAnas = {
    #"s00":["djgen",NrJEtMin,NrJEtMax,0,3.,JDPhi,"Ana","AnaSig","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"s01":["djgen",NrJEtMin,NrJEtMax,0,2.,JDPhi,"Ana","AnaSig","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"s02":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,2.,JDPhi,"Ana","AnaSig","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    "s03":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,2.,JDPhi,"Ana","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"s04":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,2.,JDPhi,"AnaMatRefNr","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"], # mat Nr
    #"s05":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,2.,JDPhi,"AnaMatRef","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"], # mat aw
    #"s05b":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,2.,JDPhi,"RefOrderRef","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"], # mat aw
    "s06":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,JDPhi,"Ref","Ana","log\(nlrjet/ppt\)","log\(alrjet/ppt\)"], # calo order
    #"s06b":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,JDPhi,"Ref","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"], # calo order, with calo jes in xi
    #"s06b2":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,JDPhi,"Ref","Ana","log\(nlrjet*1.1/ppt\)","log\(alrjet/ppt\)"], # calo order
    #"s07":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,JDPhi,"RefAwAnaMatNr","Ana","log\(nlrjet/ppt\)","log\(alrjet/ppt\)"], # cut on nr calo et, still matched
    "s08":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,JDPhi,"AnaMatRef","Ana","log\(nlrjet/ppt\)","log\(alrjet/ppt\)"], # cut on aw calo et, still matched
    #"s09":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,JDPhi,"AnaMatRefNr","Ana","log\(nlrjet/ppt\)","log\(alrjet/ppt\)"], # no Aw mat
    "s10":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,JDPhi,"Ana","Ana","log\(nlrjet/ppt\)","log\(alrjet/ppt\)"], # no Nr mat
    "s11":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,JDPhi,"Ana","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"] # xi use Nr JES -- all calojet
    #"sf03a":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,2.,JDPhi,"AnaLz0","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"sf03b":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,2.,JDPhi,"AnaLz1","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"sf03c":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,2.,JDPhi,"AnaLz2","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"sf11a":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,JDPhi,"AnaLz0","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"], 
    #"sf11b":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,JDPhi,"AnaLz1","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"sf11c":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,2.,JDPhi,"AnaLz2","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"]
    }

SysAnas = {
    "d0":["djcalo",NrJEtMin,NrJEtMax,0.,2.,JDPhi,"Ana","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    "d1":["djcalo",NrJEtMin,NrJEtMax,35.,2.,JDPhi,"Ana","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    "d2":["djcalo",NrJEtMin,NrJEtMax,50.,2.,JDPhi,"Ana","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"]
    }

# prepare ana
logdir='log/'+SrcName+'/'+AnaV
os.system("mkdir -p "+logdir)
logfile=logdir+"/ff_"+RunV+'.txt'
os.system('echo '+logdir+' > '+logfile)

# run ana
from operator import itemgetter
for sysana, [mod,NrJEtMin,NrJEtMax,AwJEtMin,AwJEtaMax,JetDPhiMin,DJCutType,TrkCutType,varXiNrJES,varXiAwJES] in sorted(SysAnas.items()):
  os.system('echo;echo;echo --- '+sysana+' --- >> '+logfile)
  cmd='root -l -b -q anaDiJetFF.C+\('
  cmd+=str(doMC)+','
  cmd+='\\"'+AnaV+RunV+'\\",'
  cmd+='\\"'+mod+'\\",'
  cmd+=str(NrJEtMin)+','
  cmd+=str(NrJEtMax)+','
  cmd+=str(AwJEtMin)+','
  cmd+=str(AwJEtaMax)+','
  cmd+=str(JetDPhiMin)+','
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
