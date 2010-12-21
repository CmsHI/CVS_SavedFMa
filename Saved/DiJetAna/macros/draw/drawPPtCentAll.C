#include "drawPPt.C"
#include "drawTrkEnergy.h"
void drawPPtCentAll(
    TString anaV = "CorrEtaTrkEffPtBin5RBin20v4",
    TString module = "djcalo",
    TString moduleGen = "djcalo_genp",
    Int_t RatType=2)
{
  TCanvas * cAll = new TCanvas("cAll","cAll",1400,800);
  cAll->Divide(3,2);

  cAll->cd(1);
  drawPPt("jfh"+anaV+"_Hydjet_"+module+"_Cent30to100_Aj0to100_SubEtaRefl.root","jfh"+anaV+"_Hydjet_"+moduleGen+"_Cent30to100_Aj0to100_SubEtaRefl.root","jfh"+anaV+"_PyquenUQ80_"+moduleGen+"_Cent0to100_Aj0to100_SubEtaRefl.root",true);
  drawText("30-100%",0.22,0.90);

  cAll->cd(2);
  drawPPt("jfh"+anaV+"_Hydjet_"+module+"_Cent10to30_Aj0to100_SubEtaRefl.root","jfh"+anaV+"_Hydjet_"+moduleGen+"_Cent10to30_Aj0to100_SubEtaRefl.root","jfh"+anaV+"_PyquenUQ80_"+moduleGen+"_Cent0to100_Aj0to100_SubEtaRefl.root",false);
  drawText("10-30%",0.22,0.90);

  cAll->cd(3);
  drawPPt("jfh"+anaV+"_Hydjet_"+module+"_Cent0to10_Aj0to100_SubEtaRefl.root","jfh"+anaV+"_Hydjet_"+moduleGen+"_Cent0to10_Aj0to100_SubEtaRefl.root","jfh"+anaV+"_PyquenUQ80_"+moduleGen+"_Cent0to100_Aj0to100_SubEtaRefl.root",false);
  drawText("0-10%",0.22,0.90);

  cAll->cd(4);
  drawPPt("jfh"+anaV+"_Hydjet_"+module+"_Cent30to100_Aj0to100_SubEtaRefl.root","jfh"+anaV+"_Hydjet_"+moduleGen+"_Cent30to100_Aj0to100_SubEtaRefl.root","jfh"+anaV+"_PyquenUQ80_"+moduleGen+"_Cent0to100_Aj0to100_SubEtaRefl.root",true,2);

  cAll->cd(5);
  drawPPt("jfh"+anaV+"_Hydjet_"+module+"_Cent10to30_Aj0to100_SubEtaRefl.root","jfh"+anaV+"_Hydjet_"+moduleGen+"_Cent10to30_Aj0to100_SubEtaRefl.root","jfh"+anaV+"_PyquenUQ80_"+moduleGen+"_Cent0to100_Aj0to100_SubEtaRefl.root",false,2);

  cAll->cd(6);
  drawPPt("jfh"+anaV+"_Hydjet_"+module+"_Cent0to10_Aj0to100_SubEtaRefl.root","jfh"+anaV+"_Hydjet_"+moduleGen+"_Cent0to10_Aj0to100_SubEtaRefl.root","jfh"+anaV+"_PyquenUQ80_"+moduleGen+"_Cent0to100_Aj0to100_SubEtaRefl.root",false,2);

  cAll->Print(Form("%sPPt_CentAll_Rat%d.gif",anaV.Data(),RatType));
}
