#include "TTree.h"
#include <cassert>

void aliases_dijet(TTree * djTree, float pptMin=4.5, int doMC=1)
{ 
  if (!djTree) cout << "djTree not defined, please check" << endl;
  assert(djTree);

  // dijet cleaning
  djTree->SetAlias("goodDJ3","nljet>10 && aljet>10 && abs(nljeta)<3 && abs(aljeta)<3 && jdphi>2.14 && nljemf>0.01 && aljemf>0.01");
  djTree->SetAlias("goodDJ2","nljet>10 && aljet>10 && abs(nljeta)<2 && abs(aljeta)<2 && jdphi>2.14 && nljemf>0.01 && aljemf>0.01");

  // track level
  djTree->SetAlias("goodTrk",Form("ppt>%f && ppt<nljet*1.5 &&psube==0",pptMin));

  // jet cone info
  double dR=0.5;
  djTree->SetAlias("NC5NP",Form("Sum$(goodTrk && pndr<%f)",dR));
  djTree->SetAlias("AC5NP",Form("Sum$(goodTrk && padr<%f)",dR));
  djTree->SetAlias("NC5Pt",Form("Sum$((goodTrk && pndr<%f)*ppt)",dR));
  djTree->SetAlias("AC5Pt",Form("Sum$((goodTrk && padr<%f)*ppt)",dR));

  djTree->SetAlias("NC5NPBg",Form("Sum$(goodTrk && pndrbg<%f)",dR));
  djTree->SetAlias("AC5NPBg",Form("Sum$(goodTrk && padrbg<%f)",dR));
  djTree->SetAlias("NC5PtBg",Form("Sum$((goodTrk && pndrbg<%f)*ppt)",dR));
  djTree->SetAlias("AC5PtBg",Form("Sum$((goodTrk && padrbg<%f)*ppt)",dR));

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
