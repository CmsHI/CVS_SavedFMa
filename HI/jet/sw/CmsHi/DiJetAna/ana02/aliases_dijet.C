#include "TTree.h"

void aliases_dijet(TTree * djTree)
{ 
  // dijet level
  djTree->SetAlias("goodDJ3","nljet>10 && aljet>10 && abs(nljeta)<3 && abs(aljeta)<3 && jdphi>2.14 && nljemf>0.01 && aljemf>0.01");
  djTree->SetAlias("goodDJ2","nljet>10 && aljet>10 && abs(nljeta)<2 && abs(aljeta)<2 && jdphi>2.14 && nljemf>0.01 && aljemf>0.01");

  // track level
  djTree->SetAlias("goodTrk","ppt>0.5 && ppt<nljet");

  // jet cone info
  djTree->SetAlias("NC5NTrk","Sum$(goodTrk && pndr<0.5)");
  djTree->SetAlias("AC5NTrk","Sum$(goodTrk && padr<0.5)");
  djTree->SetAlias("NC5TrkEt","Sum$((goodTrk && pndr<0.5)*ppt)");
  djTree->SetAlias("AC5TrkEt","Sum$((goodTrk && padr<0.5)*ppt)");

  djTree->SetAlias("NC5NTrkBg","Sum$(goodTrk && pndrbg<0.5)");
  djTree->SetAlias("AC5NTrkBg","Sum$(goodTrk && padrbg<0.5)");
  djTree->SetAlias("NC5TrkEtBg","Sum$((goodTrk && pndrbg<0.5)*ppt)");
  djTree->SetAlias("AC5TrkEtBg","Sum$((goodTrk && padrbg<0.5)*ppt)");

  // refjet leading vars
  djTree->SetAlias("NrRefLead","(nlrjet>=alrjet)");
  djTree->SetAlias("AwRefLead","(nlrjet<alrjet)");
  djTree->SetAlias("nrljet","(nljet*NrRefLead+aljet*AwRefLead)");
  djTree->SetAlias("arljet","(aljet*NrRefLead+nljet*AwRefLead)");
  djTree->SetAlias("nrlrjet","(nlrjet*NrRefLead+alrjet*AwRefLead)");
  djTree->SetAlias("arlrjet","(alrjet*NrRefLead+nlrjet*AwRefLead)");

  djTree->SetAlias("nrljeta","(nljeta*NrRefLead+aljeta*AwRefLead)");
  djTree->SetAlias("arljeta","(aljeta*NrRefLead+nljeta*AwRefLead)");
  djTree->SetAlias("nrlrjeta","(nlrjeta*NrRefLead+alrjeta*AwRefLead)");
  djTree->SetAlias("arlrjeta","(alrjeta*NrRefLead+nlrjeta*AwRefLead)");

  djTree->SetAlias("nrljphi","(nljphi*NrRefLead+aljphi*AwRefLead)");
  djTree->SetAlias("arljphi","(aljphi*NrRefLead+nljphi*AwRefLead)");
  djTree->SetAlias("nrlrjphi","(nlrjphi*NrRefLead+alrjphi*AwRefLead)");
  djTree->SetAlias("arlrjphi","(alrjphi*NrRefLead+nlrjphi*AwRefLead)");

  djTree->SetAlias("prlndr","(pndr*NrRefLead+padr*AwRefLead)");
  djTree->SetAlias("prlndrbg","(pndrbg*NrRefLead+padrbg*AwRefLead)");
  djTree->SetAlias("prladr","(padr*NrRefLead+pndr*AwRefLead)");
  djTree->SetAlias("prladrbg","(padrbg*NrRefLead+pndrbg*AwRefLead)");
  djTree->SetAlias("zrln","(zn*NrRefLead+za*AwRefLead)");
  djTree->SetAlias("zrla","(za*NrRefLead+zn*AwRefLead)");
}
