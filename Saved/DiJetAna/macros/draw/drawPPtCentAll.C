#include "drawPPt.C"
#include "drawTrkEnergy.h"
void drawPPtCentAll()
{
  TCanvas * cAll = new TCanvas("cAll","cAll",1000,500);
  cAll->Divide(3,1);

  cAll->cd(1);
  drawPPt("jfhCorrEtaHighPtv3WideRBin_HCPR_J50U_djcalo_Cent30to100_Aj0to100_SubEtaRefl.root","jfhCorrEtaHighPtv3WideRBin_PyquenUQ80_djcalo_Cent0to100_Aj0to100_SubEtaRefl.root","jfhCorrEtaHighPtv3WideRBin_Hydjet_djcalo_Cent30to100_Aj0to100_SubEtaRefl.root",false);
  drawText("30-100%",0.22,0.90);

  cAll->cd(2);
  drawPPt("jfhCorrEtaHighPtv3WideRBin_HCPR_J50U_djcalo_Cent10to30_Aj0to100_SubEtaRefl.root","jfhCorrEtaHighPtv3WideRBin_PyquenUQ80_djcalo_Cent0to100_Aj0to100_SubEtaRefl.root","jfhCorrEtaHighPtv3WideRBin_Hydjet_djcalo_Cent10to30_Aj0to100_SubEtaRefl.root",true);
  drawText("10-30%",0.22,0.90);

  cAll->cd(3);
  drawPPt("jfhCorrEtaHighPtv3WideRBin_HCPR_J50U_djcalo_Cent0to10_Aj0to100_SubEtaRefl.root","jfhCorrEtaHighPtv3WideRBin_PyquenUQ80_djcalo_Cent0to100_Aj0to100_SubEtaRefl.root","jfhCorrEtaHighPtv3WideRBin_Hydjet_djcalo_Cent0to10_Aj0to100_SubEtaRefl.root",false);
  drawText("0-10%",0.22,0.90);

  cAll->Print("PPt_CentAll.gif");
}
