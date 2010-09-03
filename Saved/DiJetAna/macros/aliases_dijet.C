#include "TTree.h"
#include <cassert>

void aliases_dijet(TTree * djTree, int doMC=1)
{ 
  if (!djTree) cout << "djTree not defined, please check" << endl;
  assert(djTree);

  // dijet cleaning
  djTree->SetAlias("goodDJ3","nljet>10 && aljet>10 && abs(nljeta)<3 && abs(aljeta)<3 && jdphi>2.14 && nljemf>0.01 && aljemf>0.01");
  djTree->SetAlias("goodDJ2","nljet>10 && aljet>10 && abs(nljeta)<2 && abs(aljeta)<2 && jdphi>2.14 && nljemf>0.01 && aljemf>0.01");

  // track level
  djTree->SetAlias("goodTrk","ppt>1.2 && ppt<nljet");

  // jet cone info
  djTree->SetAlias("NC5NP","Sum$(goodTrk && pndr<0.5)");
  djTree->SetAlias("AC5NP","Sum$(goodTrk && padr<0.5)");
  djTree->SetAlias("NC5PEt","Sum$((goodTrk && pndr<0.5)*ppt)");
  djTree->SetAlias("AC5PEt","Sum$((goodTrk && padr<0.5)*ppt)");

  djTree->SetAlias("NC5NPBg","Sum$(goodTrk && pndrbg<0.5)");
  djTree->SetAlias("AC5NPBg","Sum$(goodTrk && padrbg<0.5)");
  djTree->SetAlias("NC5PEtBg","Sum$((goodTrk && pndrbg<0.5)*ppt)");
  djTree->SetAlias("AC5PEtBg","Sum$((goodTrk && padrbg<0.5)*ppt)");

  if (doMC==0) return;

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

  djTree->SetAlias("pnrldr","(pndr*NrRefLead+padr*AwRefLead)");
  djTree->SetAlias("pnrldrbg","(pndrbg*NrRefLead+padrbg*AwRefLead)");
  djTree->SetAlias("parldr","(padr*NrRefLead+pndr*AwRefLead)");
  djTree->SetAlias("parldrbg","(padrbg*NrRefLead+pndrbg*AwRefLead)");
  
  djTree->SetAlias("znrl","(zn*NrRefLead+za*AwRefLead)");
  djTree->SetAlias("zarl","(za*NrRefLead+zn*AwRefLead)");
}
