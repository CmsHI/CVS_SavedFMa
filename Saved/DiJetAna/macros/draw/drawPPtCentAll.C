#include "drawPPt.C"
#include "drawTrkEnergy.h"
void drawPPtCentAll()
{
  TCanvas * cAll = new TCanvas("cAll","cAll",1000,500);
  cAll->Divide(2,1);

  cAll->cd(1);
  drawPPt("jfhCorrEtaHighPtv3WideRBin_HCPR_J50U_djcalo_Cent30to100_Aj0to100_SubEtaRefl.root","jfhCorrEtaHighPtv3WideRBin_PyquenUQ80_djcalo_Cent0to100_Aj0to100_SubEtaRefl.root");
  drawText("30-100%",0.22,0.90);

  cAll->cd(2);
  drawPPt("jfhCorrEtaHighPtv3WideRBin_HCPR_J50U_djcalo_Cent0to30_Aj0to100_SubEtaRefl.root","jfhCorrEtaHighPtv3WideRBin_PyquenUQ80_djcalo_Cent0to100_Aj0to100_SubEtaRefl.root",true);
  drawText("0-30%",0.22,0.90);

  cAll->Print("PPt_CentAll.gif");
}
