import os,sys

AnaV = "1125AwJEtMax"
RunV = sys.argv[1]
doMC=0
#inFile0Name="dj_HydjetQ_DJQ80_F10GSR_GoodTrk1123.root"
#SrcName="HydjQDJQ80"
#inFile0Name="dj_HydjetQ_DJUQ80_F10GSR_GoodTrk1123.root"
#SrcName="HydjQDJUQ80"
inFile0Name="dj_HCPR-GoodTrk1123_All0.root"
SrcName="HCPR-GoodTrk1123_All0"
evtBase="S1"
AnaType="ff"
NrJEtMin=120
NrJEtMax=500
AwJEtMin=50
AwJEtMax=NrJEtMax
JDPhi=3.14159*5./6
AwJEtaMax=2.

SysAnas = {
    #"s00":["djgen",NrJEtMin,NrJEtMax,0,AwJEtMax,3.,JDPhi,"Ana","AnaSig","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"s01":["djgen",NrJEtMin,NrJEtMax,0,AwJEtMax,AwJEtaMax,JDPhi,"Ana","AnaSig","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"s02":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,AwJEtaMax,JDPhi,"Ana","AnaSig","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    "s03":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,AwJEtaMax,JDPhi,"Ana","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"s04":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,AwJEtaMax,JDPhi,"AnaMatRefNr","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"], # mat Nr
    #"s05":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,AwJEtaMax,JDPhi,"AnaMatRef","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"], # mat aw
    #"s05b":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,AwJEtaMax,JDPhi,"RefOrderRef","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"], # mat aw
    "s06":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,AwJEtaMax,JDPhi,"Ref","Ana","log\(nlrjet/ppt\)","log\(alrjet/ppt\)"], # calo order
    #"s06b":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,AwJEtaMax,JDPhi,"Ref","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"], # calo order, with calo jes in xi
    #"s06b2":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,AwJEtaMax,JDPhi,"Ref","Ana","log\(nlrjet*1.1/ppt\)","log\(alrjet/ppt\)"], # calo order
    #"s07":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,AwJEtaMax,JDPhi,"RefAwAnaMatNr","Ana","log\(nlrjet/ppt\)","log\(alrjet/ppt\)"], # cut on nr calo et, still matched
    "s08":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,AwJEtaMax,JDPhi,"AnaMatRef","Ana","log\(nlrjet/ppt\)","log\(alrjet/ppt\)"], # cut on aw calo et, still matched
    #"s09":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,AwJEtaMax,JDPhi,"AnaMatRefNr","Ana","log\(nlrjet/ppt\)","log\(alrjet/ppt\)"], # no Aw mat
    "s10":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,AwJEtaMax,JDPhi,"Ana","Ana","log\(nlrjet/ppt\)","log\(alrjet/ppt\)"], # no Nr mat
    "s11":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,AwJEtaMax,JDPhi,"Ana","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"] # xi use Nr JES -- all calojet
    #"sf03a":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,AwJEtaMax,JDPhi,"AnaLz0","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"sf03b":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,AwJEtaMax,JDPhi,"AnaLz1","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"sf03c":["djgen",NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,AwJEtaMax,JDPhi,"AnaLz2","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"sf11a":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,AwJEtaMax,JDPhi,"AnaLz0","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"], 
    #"sf11b":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,AwJEtaMax,JDPhi,"AnaLz1","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    #"sf11c":["djcalo_genp",NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,AwJEtaMax,JDPhi,"AnaLz2","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"]
    }

SysAnas = {
    "d0":["djcalo",NrJEtMin,NrJEtMax,0.,30,AwJEtaMax,JDPhi,"Ana","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    "d1":["djcalo",NrJEtMin,NrJEtMax,30.,50,AwJEtaMax,JDPhi,"Ana","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"],
    "d2":["djcalo",NrJEtMin,NrJEtMax,50.,AwJEtMax,AwJEtaMax,JDPhi,"Ana","Ana","log\(nljet/ppt\)","log\(aljet/ppt\)"]
    }

# prepare ana
logdir='log/'+SrcName+'/'+AnaV
os.system("mkdir -p "+logdir)
logfile=logdir+"/ff_"+RunV+'.txt'
os.system('echo '+logdir+' > '+logfile)

# run ana
from operator import itemgetter
for sysana, [mod,NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,AwJEtaMax,JetDPhiMin,DJCutType,TrkCutType,varXiNrJES,varXiAwJES] in sorted(SysAnas.items()):
  os.system('echo;echo;echo --- '+sysana+' --- >> '+logfile)
  cmd='root -l -b -q anaDiJetFF.C+\('
  cmd+=str(doMC)+','
  cmd+='\\"'+evtBase+'\\",'
  cmd+='\\"'+AnaV+RunV+'\\",'
  cmd+='\\"'+mod+'\\",'
  cmd+=str(NrJEtMin)+','
  cmd+=str(NrJEtMax)+','
  cmd+=str(AwJEtMin)+','
  cmd+=str(AwJEtMax)+','
  cmd+=str(AwJEtaMax)+','
  cmd+=str(JetDPhiMin)+','
  cmd+='\\"'+DJCutType+'\\",'
  cmd+='\\"'+TrkCutType+'\\",'
  cmd+='\\"'+varXiNrJES+'\\",'
  cmd+='\\"'+varXiAwJES+'\\",'
  cmd+='\\"'+inFile0Name+'\\",'
  cmd+='\\"'+SrcName+'\\",'
  cmd+='\\"'+AnaType+sysana+'\\"\)'
  cmd+=' >> '+logfile
  print cmd
  os.system(cmd)

sys.exit("all done");
