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
#include "TSystem.h"

#include "commonUtility.h"
#include "SignalCorrector.h"
#include "loopMpt.h"
#endif

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
         cout << "# " << infname << ": " << nt->GetEntries() << " events." << endl;
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
//          float nxbins=1600, xmin=-800, xmax=800;
//         vana[ib]->rSigAllLeading.Init(nt,250,0,500);

         vector<float> xbins;
         int nxbins=1600;
         float xmin=-800, xmax=800;
         float dx=(xmax-xmin)/nxbins;
         for (int i=0; i<=nxbins; ++i) xbins.push_back(xmin+dx*i);
         
         vana[ib]->rSigAll.Init(nt,nAjBin,AjBins,xbins.size()-1,&xbins[0]);
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
                                    TString outdir = "./fig/06.23Mpt2DAna"
                                    )
{
   TH1::SetDefaultSumw2();
   gSystem->mkdir(outdir,kTRUE);
   
   const int nCentBin = 2;
   int centBins[3] = {0,12,40};

//    TString inputTree_data="../ntout/output-data-DiJetTrk_v0_v6_icPu5.root";
//    TString inputTree_data="../ntout/output-data-DiJetTrk_v0_v7_trkAlgo45_icPu5.root";
//    TString inputTree_data="../ntout/output-data-DiJetTrk_v0_v9_hgt_icPu5.root";
//    TString inputTree_data="../ntout/output-data-Forest2v2v3_saveTrks_v0_icPu5.root";
//    TString inputTree_data = "../ntout/output-hy18dj80_forest2_v0_xsec_icPu5.root";
//    TString inputTree_data="../ntout/output-data-Forest2v2v3_saveTrks_v0_icPu5.root";
//    TString inputTree_mc = "../ntout/output-hy18dj80_forest2_v0_xsec_icPu5.root";
   TString inputTree_data="../ntout/output-data-Forest2v2v3_saveTrks_v0_icPu5.root";
   TString inputTree_mc = "../ntout/output-hy18dj80_forest2_v0_xsec_icPu5.root";

   vector<TCut> vcutAnaBin,vcutAnaBinPp;
   for (int ib=0; ib<nCentBin; ++ib) vcutAnaBin.push_back(Form("cBin>=%d&&cBin<%d",centBins[ib],centBins[ib+1]));
   vcutAnaBinPp.push_back("1==1");
   
   TCut leadingSel  = Form("pt1>%.3f&&abs(eta1)<1.6",minPt1);
   TCut awaySel     = Form("pt2>%.3f&&abs(eta2)<1.6",minPt2);
   TCut sigSel      = Form("acos(cos(phi2-phi1))>%.3f",sigDPhi);
   AnaMpt::minPt1 = minPt1;
   AnaMpt::minPt2 = minPt2;
   AnaMpt::sigDPhi = sigDPhi;
   float etamax=2.4;

   bool isMC=false;
   TString tag = Form("%s/HisData_icPu5_trkHPCorr_%.0f_%.0f_%.0f_eta%.0f",outdir.Data(),minPt1,minPt2,sigDPhi*1000,etamax*10);
   if (isMC) tag.ReplaceAll("HisData","HisMc");

   TString recMptVar = "-trkPt*cos(trkPhi-phi1)";
   TString trkSel = Form("(abs(trkEta)<%.1f&&(trkNHit<8||vtrkQual[][0]))",etamax);
   TString trkWt = "vtrkWt[][0]";

   // MPT
   TString mcweight;
//   TString mcweight = "((samplePtHat==50&&pthat<80)||(samplePtHat==80&&pthat<120)||(samplePtHat==120&&pthat<170)||(samplePtHat==170))*weight*sampleWeight";
//    TString mcweight = "((samplePtHat==80&&pthat<120)||(samplePtHat==120&&pthat<170)||(samplePtHat==170))*weight*sampleWeight"; // not enough statistics for lower pt hat when we have a 120 pt1 cut

   TString mpttag=Form("mptx%s%s","trkCorr","AllAcc");

   vector<TString> xmptObs;
   TString mpt = Form("%s*%s*%s",recMptVar.Data(),trkSel.Data(),trkWt.Data());
   for (int i=0; i<nptrange; ++i) {
      xmptObs.push_back(Form("Sum$(%s*(trkPt>=%.2f&&trkPt<%.2f))",mpt.Data(),ptranges[i],ptranges[i+1]));
   }
   xmptObs.push_back(Form("Sum$(%s*(trkPt>=%.2f&&trkPt<%.2f))",mpt.Data(),ptranges[0],ptranges[nptrange]));
   
   for (int i=0; i<xmptObs.size(); ++i) {
      xmptObs[i]+=":Aj";
   }
   
   
   ////////////////////////////////////////////////////////////////////////////////////////////////
   // Analysis
   ////////////////////////////////////////////////////////////////////////////////////////////////
   // Output
   AnaMpt::outfname = tag+".root";
   cout << "Output file: " << AnaMpt::outfname << endl;
   TFile* hout = new TFile(AnaMpt::outfname,"RECREATE");
   hout->Close();
   
   for (int k=0; k<xmptObs.size(); ++k) {
      if (isMC) {
        AnaMpt hypho(inputTree_mc,Form("hypho_%s_merge%d",mpttag.Data(),k), xmptObs[k],0,1);
         if (k==0||k==(xmptObs.size()-1)) hypho.verbosity=1;
        hypho.GetHistograms("offlSel",vcutAnaBin,leadingSel,awaySel,sigSel,mcweight);
      } else {
         AnaMpt hi(inputTree_data,Form("hi_%s_merge%d",mpttag.Data(),k), xmptObs[k],1,1);
         if (k==0||k==(xmptObs.size()-1)) hi.verbosity=1;
         hi.GetHistograms("anaEvtSel",vcutAnaBin,leadingSel,awaySel,sigSel);
      }
   }
}