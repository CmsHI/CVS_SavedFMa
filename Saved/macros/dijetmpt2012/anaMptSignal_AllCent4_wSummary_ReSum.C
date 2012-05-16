#if !defined(__CINT__) || defined(__MAKECINT__)

#include <iostream>
#include "TCanvas.h"
#include "TError.h"
#include "TPad.h"
#include "TString.h"
#include "TRandom.h"
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TString.h"
#include "TCut.h"

#include "commonUtility.h"
#include "SignalCorrector.h"
#endif

// MPT Ranges
const int nptrange = 5;
float ptranges[nptrange+1]={0.5,1.0,2,4,8,180};

class AnaMpt {
public:
   // Members
   TString infname,mixfname;
   TString name,var,bkgvar;
   int dataSrcType; // 0=mc, 1=pbpb data, 2= pp data
   int dataType; // 0=mc gen, 1=reco
   bool subDPhiSide,doMixBkg;
   static float minPt1,minPt2,sigDPhi;
   vector<SignalCorrector*> vana;
   static TString outfname;
   int verbosity;

   // Methods
   AnaMpt(TString inf, TString nm, TString v, int src, int type) :
   infname(inf),
   name(nm),
   var(v),
   dataSrcType(src),
   dataType(type),
   verbosity(0)
   {}

   void GetHistograms(
                      TCut mycut, vector<TCut> vcutAnaBin,
                      TCut leadingSel, TCut awaySel, TCut sigSel,
                      TString weight="(1==1)",
                      TCut awayMixSel="", TCut sigMixSel=""
                      )
   {
      // open the data file
      TFile *inf = TFile::Open(infname);
      TTree *nt =(TTree*)inf->FindObjectAny("tgj");
      if (verbosity>0) {
         cout << endl << "# " << endl;
         cout << "# " << infname << endl;
         cout << "# " << name << ": useWeight: " << weight << " dataSrcType: " << dataSrcType << " pt1: " << minPt1 << " jet: " << minPt2 << " subJet|doMixBkg: " << subDPhiSide << "|" << doMixBkg << endl;
         cout << "# " << endl;
      }
      
      // mixing trees
      if (doMixBkg) nt->AddFriend("tmix=tgj",mixfname);
      
      // loop over analysis bins
      for (int ib=0; ib<vcutAnaBin.size(); ++ib) {
         TString myname = name+Form("_%d",ib);
         // initialize ana
         vana.push_back(new SignalCorrector(myname));
         TCut mycutInBin = mycut && vcutAnaBin[ib];
         // setup regions
         vana[ib]->rSigAllLeading.var     = "pt1";
         vana[ib]->rSigAll.var            = var;
         vana[ib]->rBkgDPhi.var           = var;

         vana[ib]->rSigAllLeading.cut     = mycutInBin && leadingSel;
         vana[ib]->rSigAll.cut            = mycutInBin && leadingSel && awaySel && sigSel;
         vana[ib]->rBkgDPhi.cut           = mycutInBin && leadingSel && awaySel && "acos(cos(phi2-phi2))>0.7 && acos(cos(phi2-phi2))<3.14159/2.";

         vana[ib]->rSigAllLeading.weight     = weight;
         vana[ib]->rSigAll.weight            = weight;
         vana[ib]->rBkgDPhi.weight           = weight;

         if (doMixBkg) {
            vana[ib]->rBkgDPhi.cut        = mycutInBin && leadingSel && awayMixSel && sigMixSel;
            vana[ib]->rBkgDPhi.var        = bkgvar;
         }
         
         // analyze tree
         TFile* hout = new TFile(outfname,"update");
         hout->cd();
         if (verbosity>0) vana[ib]->SetVerbosity(1);
         float nxbins=800, xmin=-800, xmax=800;
//         vana[ib]->rSigAllLeading.Init(nt,250,0,500);
         vana[ib]->rSigAll.Init(nt,nxbins,xmin,xmax);
//         vana[ib]->rBkgDPhi.Init(nt,nxbins,xmin,xmax);
         
         // output histograms
//         if (vana[ib]->rSigAllLeading.h) vana[ib]->rSigAllLeading.h->Write();
         if (vana[ib]->rSigAll.h) vana[ib]->rSigAll.h->Write();
//         if (vana[ib]->rBkgDPhi.h) vana[ib]->rBkgDPhi.h->Write();
         hout->Close();
      }
   }
};

TString AnaMpt::outfname="HisOutput.root";
float AnaMpt::minPt1;
float AnaMpt::minPt2;
float AnaMpt::sigDPhi;

//---------------------------------------------------------------------
void anaMptSignal_AllCent4_wSummary_ReSum(
                                    int normMode=0, // 0=no norm, 1=unity, 2=per photon
                                    int subDPhiSide = 0,
                                    float minPt1=120,
                                    float minPt2=50,
                                    float sigDPhi=3.1415926*2./3,
                                    TString outdir = "./fig/05.16_hgtcorr"
                                    )
{
   TH1::SetDefaultSumw2();
   
   const int nBin = 4;
   float m[nBin+1] = {0.0001,0.11,0.22,0.33,0.49999};
   const int nCentBin = 2;
   int centBins[3] = {0,12,40};

//    TString inputTree_data="../ntout/output-data-DiJetTrk_v0_v6_icPu5.root";
//    TString inputTree_data="../ntout/output-data-DiJetTrk_v0_v7_trkAlgo45_icPu5.root";
//    TString inputTree_data="../ntout/output-data-DiJetTrk_v0_v9_hgt_icPu5.root";
   TString inputTree_data="../ntout/output-data-DiJetTrk_v0_v9_hgtcorr_icPu5.root";
   TString inputTree_mc="../ntout/output-hy18dj80_IterPixTrkv1_v6_xsec_icPu5.root";

   vector<TCut> vcutAnaBin,vcutAnaBinPp;
   for (int ib=0; ib<nBin; ++ib) {
      vcutAnaBin.push_back(Form("Aj>=%f&&Aj<%f",m[ib],m[ib+1]));
      cout << "anaBin cut: " << vcutAnaBin[ib] << endl;
   }
   
   TCut leadingSel  = Form("abs(eta1)<1.6&&pt1>%.3f",minPt1);
   TCut awaySel     = Form("abs(eta2)<1.6&&pt2>%.3f",minPt2);
   TCut sigSel      = Form("acos(cos(phi2-phi1))>%.3f",sigDPhi);
   AnaMpt::minPt1 = minPt1;
   AnaMpt::minPt2 = minPt2;
   AnaMpt::sigDPhi = sigDPhi;

   bool isMC=false;

   TString selTrk = "trkAlgo<5&&trkHP";
   TString tag = Form("%s/HisOutput_DiJetTrkv0_v9_icPu5_trkAlgo4HPCorr_%.0f_%.0f_%.0f",outdir.Data(),minPt1,minPt2,sigDPhi*1000);
   if (isMC) tag =Form("%s/HisOutput_Hy18DJv1_v6_icPu5_trkAlgo4HPCorr_%.0f_%.0f_%.0f",outdir.Data(),minPt1,minPt2,sigDPhi*1000);

   // MPT
   TString mcweight;
//   TString mcweight = "((samplePtHat==50&&pthat<80)||(samplePtHat==80&&pthat<120)||(samplePtHat==120&&pthat<170)||(samplePtHat==170))*weight*sampleWeight";
//    TString mcweight = "((samplePtHat==80&&pthat<120)||(samplePtHat==120&&pthat<170)||(samplePtHat==170))*weight*sampleWeight"; // not enough statistics for lower pt hat when we have a 120 pt1 cut

   TString mpttag=Form("mptx%s%s","trk","AllAcc");

   vector<TString> xmptObs;
   for (int i=0; i<nptrange; ++i) {
      xmptObs.push_back(Form("Sum$(-trkPt*cos(trkPhi-phi1)*(trkPt>=%.2f&&trkPt<%.2f&&%s)*trkWt)",ptranges[i],ptranges[i+1],selTrk.Data()));
   }
   xmptObs.push_back(Form("Sum$(-trkPt*cos(trkPhi-phi1)*(trkPt>=%.2f&&%s))*trkWt",ptranges[0],selTrk.Data()));
   
   ////////////////////////////////////////////////////////////////////////////////////////////////
   // Analysis
   ////////////////////////////////////////////////////////////////////////////////////////////////
   for (int c=0; c<nCentBin; ++c) {
      TCut mycut=Form("cBin>=%d&&cBin<%d",centBins[c],centBins[c+1]);
   
      // Output
      AnaMpt::outfname = tag+Form("_c%d_%d.root",centBins[c],centBins[c+1]);
      cout << "Output file: " << AnaMpt::outfname << endl;
      TFile* hout = new TFile(AnaMpt::outfname,"RECREATE");
      hout->Close();
      
      for (int k=0; k<xmptObs.size(); ++k) {
         if (isMC) {
           AnaMpt hypho(inputTree_mc,Form("hypho_%s_merge%d",mpttag.Data(),k), xmptObs[k],0,1);
            if (k==0||k==(xmptObs.size()-1)) hypho.verbosity=1;
           hypho.GetHistograms("offlSel"&&mycut,vcutAnaBin,leadingSel,awaySel,sigSel,mcweight);
         } else {
            AnaMpt hi(inputTree_data,Form("hi_%s_merge%d",mpttag.Data(),k), xmptObs[k],1,1);
            if (k==0||k==(xmptObs.size()-1)) hi.verbosity=1;
            hi.GetHistograms("anaEvtSel"&&mycut,vcutAnaBin,leadingSel,awaySel,sigSel);
         }
      }
   }
}