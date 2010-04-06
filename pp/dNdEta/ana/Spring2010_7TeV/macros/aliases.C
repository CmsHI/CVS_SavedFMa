void aliases(TTree * PixelTree)
{
  // Event Selection
  PixelTree->SetAlias("trigger","L1A[124]");
  PixelTree->SetAlias("noHalo","(!L1T[36] && !L1T[37] && !L1T[38] && !L1T[39])");
  PixelTree->SetAlias("edscut2","(vtxqual>(0.0045*npxhits) || npxhits<150 || vtxqual>2)");

  PixelTree->SetAlias("bscSelColBx","trigger&&L1T[0]&&L1T[34]&&noHalo");
  PixelTree->SetAlias("looseSelColBx","bscSelColBx&&edscut2");
  PixelTree->SetAlias("anaSelColBx","looseSelColBx&&vz[1]>-99");

  PixelTree->SetAlias("bscSelMc","L1T[34]&&noHalo");
  PixelTree->SetAlias("looseSelMc","bscSelMc&&edscut2");
  PixelTree->SetAlias("anaSelMc","looseSelMc&&vz[1]>-99");

  PixelTree->SetAlias("bscSelEmpBx","trigger&&L1T[34]&&noHalo");
  PixelTree->SetAlias("looseSelEmpBx","bscSelEmpBx&&edscut2");
  PixelTree->SetAlias("anaSelEmpBx","looseSelEmpBx&&vz[1]>-99");

  // Obs
  PixelTree->SetAlias("MinEPz","(SumEaddEpPos<=SumEsubEpNeg)*SumEaddEpPos + (SumEaddEpPos>SumEsubEpNeg)*SumEsubEpNeg");
}
