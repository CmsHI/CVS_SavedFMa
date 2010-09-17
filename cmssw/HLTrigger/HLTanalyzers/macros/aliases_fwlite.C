void aliases_fwlite(TTree * Events)
{
  Events->SetAlias("ct","CaloTowersSorted_towerMaker__RECO.obj.obj");

  Events->SetAlias("NHfN","Sum$((ct.energy()>3) * (ct.eta()<3))");
  Events->SetAlias("NHfP","Sum$((ct.energy()>3) * (ct.eta()>3))");
}
