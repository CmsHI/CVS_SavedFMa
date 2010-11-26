#include "TTree.h"
#include <cassert>

void aliases_dijet(TTree * djTree, float pptMin=4.5, int doMC=1)
{ 
  if (!djTree) cout << "djTree not defined, please check" << endl;
  assert(djTree);

  // jet matching
  djTree->SetAlias("PI","3.1415926535897932");
  TString refjet("djgen");
  djTree->SetAlias("tr2nljdphiRaw",Form("abs(nljphi-%s.nljphi)",refjet.Data()));
  djTree->SetAlias("tr2aljdphiRaw",Form("abs(aljphi-%s.aljphi)",refjet.Data()));
  djTree->SetAlias("tr2nljdphi","((tr2nljdphiRaw<=PI)*tr2nljdphiRaw+(tr2nljdphiRaw>PI)*(2*PI-tr2nljdphiRaw))");
  djTree->SetAlias("tr2aljdphi","((tr2aljdphiRaw<=PI)*tr2aljdphiRaw+(tr2aljdphiRaw>PI)*(2*PI-tr2aljdphiRaw))");
  djTree->SetAlias("tr2nljdeta",Form("abs(nljeta-%s.nljeta)",refjet.Data()));
  djTree->SetAlias("tr2aljdeta",Form("abs(aljeta-%s.aljeta)",refjet.Data()));
  djTree->SetAlias("tr2nljdr","sqrt(tr2nljdphi*tr2nljdphi+tr2nljdeta*tr2nljdeta)");
  djTree->SetAlias("tr2aljdr","sqrt(tr2aljdphi*tr2aljdphi+tr2aljdeta*tr2aljdeta)");

  // dijet cleaning
  djTree->SetAlias("goodDJ3","nljet>10 && aljet>10 && abs(nljeta)<3 && abs(aljeta)<3 && jdphi>2.14 && nljemf>0.01 && aljemf>0.01");
  djTree->SetAlias("goodDJ2","nljet>10 && aljet>10 && abs(nljeta)<2 && abs(aljeta)<2 && jdphi>2.14 && nljemf>0.01 && aljemf>0.01");

  // track level
  // psube==0 for genp signal
  djTree->SetAlias("goodTrk",Form("ppt>%f && ppt<nljet*1.5",pptMin));

  // leading partile
  djTree->SetAlias("lpndphiRaw","abs(nljphi-lp[0].Phi())");
  djTree->SetAlias("lpadphiRaw","abs(nljphi-lp[1].Phi())");
  djTree->SetAlias("lpndphi","((lpndphiRaw<=PI)*lpndphiRaw+(lpndphiRaw>PI)*(2*PI-lpndphiRaw))");
  djTree->SetAlias("lpadphi","((lpadphiRaw<=PI)*lpadphiRaw+(lpadphiRaw>PI)*(2*PI-lpadphiRaw))");
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

  djTree->SetAlias("NC5NPSub","(NC5NP-NC5NPBg)");
  djTree->SetAlias("AC5NPSub","(AC5NP-AC5NPBg)");
  djTree->SetAlias("NC5PtSub","(NC5Pt-NC5PtBg)");
  djTree->SetAlias("AC5PtSub","(AC5Pt-AC5PtBg)");

  // sum pt^2
  djTree->SetAlias("NC5Pt2",Form("Sum$((goodTrk && pndr<%f)*ppt*ppt)",dR));
  djTree->SetAlias("AC5Pt2",Form("Sum$((goodTrk && padr<%f)*ppt*ppt)",dR));

  djTree->SetAlias("NC5Pt2Bg",Form("Sum$((goodTrk && pndrbg<%f)*ppt*ppt)",dR));
  djTree->SetAlias("AC5Pt2Bg",Form("Sum$((goodTrk && padrbg<%f)*ppt*ppt)",dR));

  djTree->SetAlias("NC5Pt2Sub","(NC5Pt2-NC5Pt2Bg)");
  djTree->SetAlias("AC5Pt2Sub","(AC5Pt2-AC5Pt2Bg)");

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
