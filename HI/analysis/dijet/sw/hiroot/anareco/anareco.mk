#
# $Id: anareco.mk 3971 2007-05-15 15:57:29Z loizides $
#
# See $HIROOT/build/template/module.mk.example for a list
# of all possible variables and their meaning.
# 

anarecoDH := \
     THIAnaEcalMod.h \
     THICaloUtil.h \
     THIGamJetTruthAnaMod.h \
     THIGamTaggedJetAnaMod.h \
     THIGenPartConvMod.h \
     THIMCGammaJetSignalDef.h \
     THIMatchedParticles.h \
     THIPhotonMCType.h \
     THIPrimaryRecTracksMod.h \
     THISimTrackConvMod.h \
     THISuperClusAccFilterMod.h \
     THISuperClusEcalFilterMod.h \
     THISuperClusIsolationMod.h \
     THISuperClusProperties.h \
     THITrackEffMod.h \
     THIDiJetTruthAnaMod.h \


anarecoLIBEXTRA := \
     -L$(LIBDIR) -lana \


anarecoLIBDEP := \
     lib/libana.$(SOEXT) \

