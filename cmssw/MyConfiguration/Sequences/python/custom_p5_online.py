def customise_p5_online(process):
   process.GlobalTag.connect = "frontier://(proxyurl=http://localhost:3128)(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_COND_31X_GLOBALTAG"
   process.MessageLogger.destinations = ['warnings','errors','infos','debugs','cout','cerr','log4cplus']
   process.HLTHIRecopixelvertexingSequence = cms.Sequence( process.hltHIPixelTracks )
   return(process)

# redefine process
process=customise_p5_online(process)

