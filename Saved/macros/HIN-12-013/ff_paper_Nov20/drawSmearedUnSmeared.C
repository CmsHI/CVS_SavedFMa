#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <TMath.h>
#include "CutAndBinCollection2011.h"
#include <time.h>
#include <TRandom3.h>

using namespace std;

#define PI 3.141592653589
static const int MAXTRK = 30000;

void drawSmearedUnSmeared() {
  TH1::SetDefaultSumw2();
  std::string Input_="dijetFF_output_histograms_trkPtProjectOnJetAxis_trackPtCut1_FinaletaCut2.00_NOSMEARING.root";
  std::string InputSys_="dijetFF_output_histograms_trkPtProjectOnJetAxis_trackPtCut1_FinaletaCut2.00_pfJet.root";
//   std::string InputSys_="dijetFF_output_histograms_trkPtProjectOnJetAxis_trackPtCut1_FinaletaCut2.00_pas.root";
  
  bool doJetPt = 0;

  TH1D* ffraw[3][5], *ffsm[3][5], *ffsmrew[3][5];
  TLegend* l1;
  
  for (int binMode=2; binMode<=2; ++binMode) {  // 1 : aj, 2 : cent
    TCanvas * c2 = new TCanvas(Form("c2_binMode%d",binMode),"c2",1200,600);
    c2->Divide(4,2);
    for ( int ijet=1; ijet<=1; ijet++){ 
      for ( int iaj=1;iaj<=4;iaj++) {
        TString jetName[3] = {"","lJet","slJet"};
        TString jetNamePt[3] = {"","LjetPt","SljetPt"};
        
        if ( binMode == 2 ) {
          if (!doJetPt) {
            ffraw[ijet][iaj]  = (TH1D*)load(Input_.data(),Form("hpt_%s_sigTrk_ppdata_icent%d_irj999_fragMode2_closure-1",jetName[ijet].Data(),iaj));
            ffsm[ijet][iaj]  = (TH1D*)load(InputSys_.data(),Form("hpt_%s_sigTrk_ppdata_icent%d_irj999_fragMode2_closure100",jetName[ijet].Data(),iaj));
//             ffsm[ijet][iaj]  = (TH1D*)load(InputSys_.data(),Form("hpt_%s_sigTrk_ppdata_icent%d_irj999_fragMode2_closure100_wtmode0_pt1to500",jetName[ijet].Data(),iaj));
//             ffsmrew[ijet][iaj]  = (TH1D*)load(InputSys_.data(),Form("hpt_%s_sigTrk_ppdata_icent%d_irj999_fragMode2_closure100",jetName[ijet].Data(),iaj));
           } else {
            ffraw[ijet][iaj]  = (TH1D*)load(Input_.data(),Form("h%s_ppdata_icent%d_irj999_fragMode2_closure-1",jetNamePt[ijet].Data(),iaj));
            ffsm[ijet][iaj]  = (TH1D*)load(InputSys_.data(),Form("h%s_ppdata_icent%d_irj999_fragMode2_closure100",jetNamePt[ijet].Data(),iaj));
//             ffsmrew[ijet][iaj]  = (TH1D*)load(InputSys_.data(),Form("h%s_ppdata_icent%d_irj999_fragMode2_closure100",jetNamePt[ijet].Data(),iaj));
           }
        } 
    
        handsomeTH1(ffraw[ijet][iaj],kBlack);
        handsomeTH1(ffsm[ijet][iaj],kBlack,1.2,kFullCircle);
//         handsomeTH1(ffsmrew[ijet][iaj],kBlack,1.2,kFullCircle);
        
        if (doJetPt) {
          ffraw[ijet][iaj]->SetAxisRange(100,300);
          ffsm[ijet][iaj]->SetAxisRange(100,300);
//           ffsmrew[ijet][iaj]->SetAxisRange(100,300);
          ffraw[ijet][iaj]->SetAxisRange(0.5,30000,"Y");
          ffsm[ijet][iaj]->SetAxisRange(0.5,30000,"Y");
//           ffsmrew[ijet][iaj]->SetAxisRange(0.5,30000,"Y");
          ffraw[ijet][iaj]->Scale(1./ffraw[ijet][iaj]->Integral());
          ffsm[ijet][iaj]->Scale(1./ffsm[ijet][iaj]->Integral());
        }
        else {
      	  ffraw[ijet][iaj]->SetAxisRange(0.001,3000,"Y");
          ffsm[ijet][iaj]->SetAxisRange(0.001,3000,"Y");
//           ffsmrew[ijet][iaj]->SetAxisRange(0.001,3000,"Y");
       	}
	
        c2->cd(5- iaj);
        gPad->SetLogy();
      	ffraw[ijet][iaj]->SetNdivisions(505);
        ffraw[ijet][iaj]->DrawClone("hist");
        ffsm[ijet][iaj]->DrawClone("sameE");
//         ffsmrew[ijet][iaj]->DrawClone("sameE");
        int lowCent = centBin1[iaj-1];
        int highCent = centBin1[iaj]-1;
        drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)), 0.5,0.63,1);
        l1 = new TLegend(0.185546,0.7039627,0.9998546,0.9261364,NULL,"brNDC");
        easyLeg(l1,"");
        l1->AddEntry(ffraw[ijet][iaj],"Before","l");
        l1->AddEntry(ffsm[ijet][iaj],"After","pl");
        l1->Draw();
        cout << Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)) << endl;
        cout << "Draw: " << ffsm[ijet][iaj]->GetName() << endl;
        cout << "Draw: " << ffraw[ijet][iaj]->GetName() << endl;
	
        c2->cd(9 - iaj);
        ffsm[ijet][iaj]->SetNdivisions(505);
      	ffsm[ijet][iaj]->Divide(ffraw[ijet][iaj]);
        ffsm[ijet][iaj]->SetAxisRange(0,2,"Y");
        ffsm[ijet][iaj]->SetYTitle("Ratio");
        ffsm[ijet][iaj]->Draw();
      }
    }
    c2->SaveAs(Form("pp_beforeAfter_semaringAndReweighting%d_noreweight.pdf",doJetPt));
    c2->SaveAs(Form("pp_beforeAfter_semaringAndReweighting%d_noreweight.gif",doJetPt));
  }
  
//   outf->Write();
}
