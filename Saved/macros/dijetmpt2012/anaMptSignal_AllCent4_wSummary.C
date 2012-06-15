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
   int iptbin;

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
         cout << "# " << name << ": useWeight: " << weight << " dataSrcType: " << dataSrcType << " pt1: " << minPt1 << " jet: " << minPt2 << " subJet|doMixBkg: " << subDPhiSide << "|" << doMixBkg << " iptbin: " << iptbin << endl;
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
         
         // binning
         float nxbins=2500, xmin=-2500, xmax=2500, dx=1;
         if (iptbin>=3&&iptbin<=4) { nxbins=4000; xmin=-500; xmax=500; }
//          float xmin=-2500, xmax=2500, dx=1;
//          if (iptbin>=2&&iptbin<=4) { xmin=-500; xmax=500; dx=0.5;}
//          vector<double> mptbins;
//          double small=1e-8;
//          mptbins.push_back(xmin-small);
//          for (int i=0; mptbins.back()<-dx; ++i) {
//             mptbins.push_back(mptbins.back()+dx);
//          }
//          mptbins.push_back(0);
//          mptbins.push_back(small);
//          for (int i=0; mptbins.back()<=(xmax+small); ++i) {
//             mptbins.push_back(mptbins.back()+dx);
//          }

         // output histograms
         TFile* hout = new TFile(outfname,"update");
         hout->cd();

         // analyze tree
         if (verbosity>0) vana[ib]->SetVerbosity(1);
//         vana[ib]->rSigAllLeading.Init(nt,250,0,500);
         vana[ib]->rSigAll.Init(nt,nxbins,xmin,xmax);
//         vana[ib]->rBkgDPhi.Init(nt,nxbins,xmin,xmax);
//          vana[ib]->rSigAll.Init(nt,mptbins.size(),&mptbins[0]);
         
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
//                                     float sigDPhi=3.1415926*2./3,
                                    float sigDPhi=3.1415926*5./6,
                                    TString outdir = "./fig/06.12_mithig"
                                    )
{
   TH1::SetDefaultSumw2();
   
   const int nBin = 4;
   float m[nBin+1] = {0.0001,0.11,0.22,0.33,0.49999};
   const int nCentBin = 2;
   int centBins[nCentBin+1] = {0,12,40};

   ////////////////////////////////////////////////////////////////////////////////////////////////
   // Analysis
   ////////////////////////////////////////////////////////////////////////////////////////////////
//    TString inputTree_data="../ntout/output-data-DiJetTrk_v0_v6_icPu5.root";
//    TString inputTree_data="../ntout/output-data-DiJetTrk_v0_v7_trkAlgo45_icPu5.root";
//    TString inputTree_data="../ntout/output-data-DiJetTrk_v0_v8_icPu5.root";
//    TString inputTree_data="../ntout/output-data-DiJetTrk_v0_v9hgtOnly_hgtcorr_icPu5.root";
//    TString inputTree_mc="../ntout/output-hy18dj80_IterPixTrkv1_v6_xsec_icPu5.root";
   TString inputTree_data="../ntout/output-data-Forest2v2v3_saveTrks_v0_icPu5.root";
   TString inputTree_mc = "../ntout/output-hy18dj80_forest2_v0_xsec_icPu5.root";

   // Output
   bool isMC=false;
   TString inputtag = "DiJetTrk_f2v0_icPu5";
   if (isMC) inputtag = "Hy18DJ_f2v0_icPu5";

   // Cuts
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

   // MPT
   vector<TString> mptSrc;
   mptSrc.push_back("mptx");
//    mptSrc.push_back("mptx1");
//    mptSrc.push_back("mptx2");

   vector<TString> mptCand;
   mptCand.push_back("trk");
   mptCand.push_back("trkCorr");
   if (isMC)  {
      mptCand.push_back("genpAll");
//       mptCand.push_back("genpSig");
   }

   vector<TString> mptType;
   mptType.push_back("AllAcc");
   mptType.push_back("InCone");
   mptType.push_back("OutCone");

   vector<float> cones;
//    cones.push_back(4);
   cones.push_back(8);
//    cones.push_back(12);
//   cones.push_back(14);

//    TString mcweight;
//    TString mcweight = "((samplePtHat==50&&pthat<120)||(samplePtHat==120&&pthat<170)||(samplePtHat==170&&pthat<200)||(samplePtHat==200))*weight*sampleWeight";
//    TString mcweight = "samplePtHat==120&&weight";
//    TString mcweight = "((samplePtHat==120&&pthat<170)||(samplePtHat==170&&pthat<200)||(samplePtHat==200))*weight*sampleWeight"; // not enough statistics for lower pt hat when we have a 120 pt1 cut
   TString mcweight = "weight"; // not 
   ////////////////////////////////////////////////////////////////////////////////////////////////
   // Analysis
   ////////////////////////////////////////////////////////////////////////////////////////////////
   for (int c=0; c<nCentBin; ++c) {
      TCut mycut=Form("cBin>=%d&&cBin<%d",centBins[c],centBins[c+1]);
      AnaMpt::outfname=Form("%s/HisOutput_%s_%.0f_%.0f_%.0f_Norm%d_c%d_%d.root",outdir.Data(),inputtag.Data(),minPt1,minPt2,sigDPhi*1000,normMode,centBins[c],centBins[c+1]);

      cout << "Outout: " << AnaMpt::outfname << endl;
      TFile* hout = new TFile(AnaMpt::outfname,"RECREATE");
      hout->ls();
      hout->Close();
      
      for (int s=0; s<mptSrc.size(); ++s) {
         for (int c=0; c<mptCand.size(); ++c) {
            for (int m=0; m<mptType.size(); ++m) {
               for (int ir=0; ir<cones.size(); ++ir) {
                  TString mpttag=Form("%s%s%s%.0f",mptSrc[s].Data(),mptCand[c].Data(),mptType[m].Data(),cones[ir]);
                  if (mptType[m]=="AllAcc") {
                     mpttag=Form("%s%s%s",mptSrc[s].Data(),mptCand[c].Data(),mptType[m].Data());
                     if (ir>0) continue;
                  }
                  vector<TString> xmptObs;
                  TString pixtrkmpttag = mpttag;
                  TString sign="-1";
                  if (mptSrc[s].Contains("1")||mptSrc[s].Contains("2")) sign="1";
                  // old pt bins
                  xmptObs.push_back(sign+"*("+mpttag+"_pt[0])"); // 0.5-1
                  xmptObs.push_back(sign+"*("+mpttag+"_pt[1]+"+mpttag+"_pt[2])"); //1-2
                  xmptObs.push_back(sign+"*("+mpttag+"_pt[3]+"+mpttag+"_pt[4])"); //2-4
                  xmptObs.push_back(sign+"*("+mpttag+"_pt[5]+"+mpttag+"_pt[6])"); //4-8
                  xmptObs.push_back(sign+"*("+mpttag+"_pt[7]+"+mpttag+"_pt[8]+"+mpttag+"_pt[9]+"+mpttag+"_pt[10]+"+mpttag+"_pt[11])");            
                  // new pt bins
//                   for (int ipt=0; ipt<12; ++ipt) {
//                      xmptObs.push_back(sign+"*("+mpttag+Form("_pt[%d])",ipt));
//                   }
   //                xmptObs.push_back(sign+"*("+mpttag+"_pt[0]+"+mpttag+"_pt[1]+"+mpttag+"_pt[2]+"+mpttag+"_pt[3]+"+mpttag+"_pt[4]+"+mpttag+"_pt[5])");            
                  TString AllPtTag = mpttag;
                  xmptObs.push_back(sign+"*("+AllPtTag+")");
                  
                  for (int k=0; k<xmptObs.size(); ++k) {
                     if (isMC) {
                        AnaMpt hypho(inputTree_mc,Form("hypho_%s_merge%d",mpttag.Data(),k), xmptObs[k],0,1);
                        if (k==0||k>=(xmptObs.size()-2)) hypho.verbosity=1;
                        hypho.iptbin=k;
                        hypho.GetHistograms("offlSel"&&mycut,vcutAnaBin,leadingSel,awaySel,sigSel,mcweight);
                     } else {
                        AnaMpt hi(inputTree_data,Form("hi_%s_merge%d",mpttag.Data(),k), xmptObs[k],1,1);
                        hi.iptbin=k;
                        if (k==0||k==(xmptObs.size()-2)) hi.verbosity=1;
                        hi.GetHistograms("anaEvtSel"&&mycut,vcutAnaBin,leadingSel,awaySel,sigSel);
                     }
                  }
               }
            }
         }
      } // end of for mptSrc
   } // end of for each cBin
}