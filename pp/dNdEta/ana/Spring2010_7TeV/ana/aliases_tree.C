void aliases_tree(TTree * PixelTree)
{
  PixelTree->SetAlias("MinEPz","(SumEaddEpPos<=SumEsubEpNeg)*SumEaddEpPos + (SumEaddEpPos>SumEsubEpNeg)*SumEsubEpNeg");
}
