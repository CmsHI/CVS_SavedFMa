def customise_p5_online(process):
   process.GlobalTag.connect = "frontier://(proxyurl=http://localhost:3128)(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_COND_31X_GLOBALTAG"
   process.MessageLogger.destinations = ['warnings','errors','infos','debugs','cout','cerr','log4cplus']
   process.HLTHIRecopixelvertexingSequence = cms.Sequence( process.hltHIPixelTracks )
   # delete mu path from OM selection
   del process.HLT_HIDoubleMu
   newsel=[]
   for i in process.hltDefaultOutputWithFEDs.SelectEvents.SelectEvents:
      if (i!="HLT_HIDoubleMu"):
	 newsel.append(i)
   process.hltDefaultOutputWithFEDs.SelectEvents.SelectEvents=newsel
   # delete mu path from Prescale
   # simply pop it, b/c mu is the last path
   process.PrescaleService.prescaleTable.pop() 
   return(process)


# redefine process
process=customise_p5_online(process)

# check
print process.hltDefaultOutputWithFEDs.SelectEvents.SelectEvents
print process.PrescaleService
