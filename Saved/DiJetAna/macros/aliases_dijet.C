#include "TTree.h"
#include <cassert>

void aliases_dijet(TTree * djTree, float pptMin=4.5, int doMC=1, TString refjet="djgen")
{ 
  if (!djTree) cout << "djTree not defined, please check" << endl;
  assert(djTree);

  // jet matching
  djTree->SetAlias("PI","3.1415926535897932");
  djTree->SetAlias("nlrjdphi","PI-abs(PI-abs(nljphi-nlrjphi))");
  djTree->SetAlias("alrjdphi","PI-abs(PI-abs(aljphi-alrjphi))");
  djTree->SetAlias("nlrjdeta",Form("abs(nljeta-nlrjeta)"));
  djTree->SetAlias("alrjdeta",Form("abs(aljeta-alrjeta)"));
  djTree->SetAlias("nlrjdr","sqrt(nlrjdphi*nlrjdphi+nlrjdeta*nlrjdeta)");
  djTree->SetAlias("alrjdr","sqrt(alrjdphi*alrjdphi+alrjdeta*alrjdeta)");

  djTree->SetAlias("nltr2nljdphi",Form("PI-abs(PI-abs(nljphi-%s.nljphi))",refjet.Data()));
  djTree->SetAlias("altr2aljdphi",Form("PI-abs(PI-abs(aljphi-%s.aljphi))",refjet.Data()));
  djTree->SetAlias("nltr2nljdeta",Form("abs(nljeta-%s.nljeta)",refjet.Data()));
  djTree->SetAlias("altr2aljdeta",Form("abs(aljeta-%s.aljeta)",refjet.Data()));
  djTree->SetAlias("nltr2nljdr","sqrt(nltr2nljdphi*nltr2nljdphi+nltr2nljdeta*nltr2nljdeta)");
  djTree->SetAlias("altr2aljdr","sqrt(altr2aljdphi*altr2aljdphi+altr2aljdeta*altr2aljdeta)");

  djTree->SetAlias("nltr2aljdphi",Form("PI-abs(PI-abs(nljphi-%s.aljphi))",refjet.Data()));
  djTree->SetAlias("altr2nljdphi",Form("PI-abs(PI-abs(aljphi-%s.nljphi))",refjet.Data()));
  djTree->SetAlias("nltr2aljdeta",Form("abs(nljeta-%s.aljeta)",refjet.Data()));
  djTree->SetAlias("altr2nljdeta",Form("abs(aljeta-%s.nljeta)",refjet.Data()));
  djTree->SetAlias("nltr2aljdr","sqrt(nltr2aljdphi*nltr2aljdphi+nltr2aljdeta*nltr2aljdeta)");
  djTree->SetAlias("altr2nljdr","sqrt(altr2nljdphi*altr2nljdphi+altr2nljdeta*altr2nljdeta)");

  djTree->SetAlias("tr2nljdr","(nltr2nljdr*(nltr2nljdr<=nltr2aljdr)+nltr2aljdr*(nltr2nljdr>nltr2aljdr))");
  djTree->SetAlias("tr2aljdr","(altr2aljdr*(altr2aljdr<=altr2nljdr)+altr2nljdr*(altr2aljdr>altr2nljdr))");
  djTree->SetAlias("nlmatrjet",Form("(%s.nljet*(nltr2nljdr<=nltr2aljdr)+%s.aljet*(nltr2nljdr>nltr2aljdr))",refjet.Data(),refjet.Data()));
  djTree->SetAlias("almatrjet",Form("(%s.aljet*(nltr2nljdr<=nltr2aljdr)+%s.nljet*(nltr2nljdr>nltr2aljdr))",refjet.Data(),refjet.Data()));

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
