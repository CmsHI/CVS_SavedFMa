void aliases_fwlite(TTree * Events)
{
  Events->SetAlias("ct","CaloTowersSorted_towerMaker__RECO.obj.obj");
  Events->SetAlias("hltct","CaloTowersSorted_hltTowerMakerForAll__ANALYSIS.obj.obj");

  Events->SetAlias("NHfN","Sum$((ct.energy()>3) * (ct.eta()<3))");
  Events->SetAlias("NHfP","Sum$((ct.energy()>3) * (ct.eta()>3))");
  Events->SetAlias("hltNHfN","Sum$((hltct.energy()>3) * (hltct.eta()<3))");
  Events->SetAlias("hltNHfP","Sum$((hltct.energy()>3) * (hltct.eta()>3))");
}
