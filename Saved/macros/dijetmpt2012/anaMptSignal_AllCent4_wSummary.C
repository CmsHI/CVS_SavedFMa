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

         if (doMixBkg) {
            vana[ib]->rBkgDPhi.cut        = mycutInBin && leadingSel && awayMixSel && sigMixSel;
            vana[ib]->rBkgDPhi.var        = bkgvar;
         }
         
         // analyze tree
         if (verbosity>0) vana[ib]->SetVerbosity(1);
         float nxbins=200, xmin=-400, xmax=400;
//         vana[ib]->rSigAllLeading.Init(nt,250,0,500);
         vana[ib]->rSigAll.Init(nt,nxbins,xmin,xmax);
//         vana[ib]->rBkgDPhi.Init(nt,nxbins,xmin,xmax);
         
         // output histograms
         TFile* hout = new TFile(outfname,"update");
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
void anaMptSignal_AllCent4_wSummary(
                                    int normMode=0, // 0=no norm, 1=unity, 2=per photon
                                    int subDPhiSide = 0,
                                    float minPt1=120,
                                    float minPt2=50,
                                    float sigDPhi=3.1415926*7./8,
                                    TString outdir = "./fig/04.14_dijetmpt_compare"
                                    )
{
   TH1::SetDefaultSumw2();
   
   AnaMpt::outfname=Form("%s/HisOutput_DiJetv7_v1_icPu5_MptCone_%.0f_%.0f_%.0f_Norm%d.root",outdir.Data(),minPt1,minPt2,sigDPhi*1000,normMode);
   TFile* hout = new TFile(AnaMpt::outfname,"RECREATE");
   hout->Close();

   const int nBin = 4;
   float m[nBin+1] = {0.0001,0.11,0.22,0.33,0.49999};

   ////////////////////////////////////////////////////////////////////////////////////////////////
   // Analysis
   ////////////////////////////////////////////////////////////////////////////////////////////////
   TString inputTree_data="../ntout/output-data-DiJet-v7-noDuplicate_v2_icPu5.root";
   TString inputTree_mc="../ntout/norewt-output-hy18dj80_v2_xsec_icPu5.root";

   TCut mycut="cBin>=0&&cBin<12";

   vector<TCut> vcutAnaBin,vcutAnaBinPp;
   for (int ib=0; ib<nBin; ++ib) {
      vcutAnaBin.push_back(Form("Aj>=%f&&Aj<%f",m[ib],m[ib+1]));
      cout << "anaBin cut: " << vcutAnaBin[ib] << endl;
   }
   
   TCut leadingSel  = Form("abs(eta1)<1.6&&pt1>%.3f",minPt1);
   TCut awaySel     = Form("abs(eta2)<1.6&&pt2>%.3f",minPt2);
   TCut sigSel      = Form("acos(cos(phi2-phi1))>%.3f",sigDPhi);

   // MPT
   vector<TString> mptCand;
   vector<TString> mptType;
   vector<float> cones;
//   mptCand.push_back("trk");
   mptCand.push_back("trkCorr");
   cones.push_back(4);
   cones.push_back(8);
//   cones.push_back(12);
//   cones.push_back(14);

   mptType.push_back("AllAcc");
   mptType.push_back("InCone");
   mptType.push_back("OutCone");

   TString mcweight = "((samplePtHat==30&&pthat<50)||(samplePtHat==50&&pthat<80)||(samplePtHat==80))*weight*sampleWeight";
   mcweight = "(1==1)";

   for (int c=0; c<mptCand.size(); ++c) {
      for (int m=0; m<mptType.size(); ++m) {
         for (int ir=0; ir<cones.size(); ++ir) {
            TString mpttag=Form("mptx%s%s%.0f",mptCand[c].Data(),mptType[m].Data(),cones[ir]);
            if (mptType[m]=="AllAcc") {
               if (ir>0) continue;
               mpttag=Form("mptx%s%s",mptCand[c].Data(),mptType[m].Data());
            }
            vector<TString> xmptObs;
            xmptObs.push_back("-1*("+mpttag+"_pt[0])");
            xmptObs.push_back("-1*("+mpttag+"_pt[1])");
            xmptObs.push_back("-1*("+mpttag+"_pt[2])");
            xmptObs.push_back("-1*("+mpttag+"_pt[3])");
            xmptObs.push_back("-1*("+mpttag+"_pt[4]+"+mpttag+"_pt[5])");
            xmptObs.push_back("-1*("+mpttag+")");
            
            for (int k=0; k<xmptObs.size(); ++k) {
               AnaMpt hypho(inputTree_mc,Form("hypho_%s_merge%d",mpttag.Data(),k), xmptObs[k],0,1);
               if (k<=1) hypho.verbosity=1;
               hypho.GetHistograms("offlSel"&&mycut,vcutAnaBin,leadingSel,awaySel,sigSel,mcweight);
               
               AnaMpt hi(inputTree_data,Form("hi_%s_merge%d",mpttag.Data(),k), xmptObs[k],1,1);
               if (k<=1) hi.verbosity=1;
               hi.GetHistograms("anaEvtSel"&&mycut,vcutAnaBin,leadingSel,awaySel,sigSel);
            }
         }
      }
   }
}