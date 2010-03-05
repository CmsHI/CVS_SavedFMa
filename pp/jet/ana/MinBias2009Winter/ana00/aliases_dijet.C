void aliases_dijet(TTree * dijetTree)
{ 
  // event level
  dijetTree->SetAlias("goodEvent","vtxdof>=5 && abs(vz)<15 && (evtnp<10||fracHP>0.2)");

  // dijet level
  dijetTree->SetAlias("goodDJ3","nljet>10 && aljet>10 && abs(nljeta)<3 && abs(aljeta)<3 && jdphi>2.14 && nljemf>0.01 && aljemf>0.01");
  dijetTree->SetAlias("goodDJ2","nljet>10 && aljet>10 && abs(nljeta)<2 && abs(aljeta)<2 && jdphi>2.14 && nljemf>0.01 && aljemf>0.01");

  // track level
  dijetTree->SetAlias("goodTrk","ppt>0.3 && ppt<25 && trkHP && trkNHits>=8");

  // jet cone info
  dijetTree->SetAlias("NC5NTrk","Sum$(goodTrk && pndr<0.5)");
  dijetTree->SetAlias("AC5NTrk","Sum$(goodTrk && padr<0.5)");
}
