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
#include "npart.h"
#endif

TString outfname;

//---------------------------------------------------------------------
void getHistograms(TString myname, TString var, TString bkgvar,
                   vector<SignalCorrector*> & vana,
                   vector<TCut> vcutCent, TCut mycut, TCut jetSel, TCut jetBkgSel,
                   int isolScheme, int normMode,
                   TString infname,
                   TString weight,
                   int dataSrcType, // 0=mc, 1=pbpb data, 2= pp data
                   int dataType, // 0=mc gen, 1=reco
                   int subDPhiSide = 1,
                   int subSShapeSide = 1,
                   float minPhoton=60,
                   float minJet=30,
                   float sigDPhi=2.0944
                   )
{
   // open the data file
   TFile *inf = TFile::Open(infname);
   TTree *nt =(TTree*)inf->FindObjectAny("tgj");
   cout << endl << "# " << endl;
   cout << "# " << infname << endl;
   cout << "# " << myname << ": useWeight: " << weight << " dataSrcType: " << dataSrcType << " photon: " << minPhoton << " jet: " << minJet << " subJet|subPho: " << subDPhiSide << "|" << subSShapeSide << endl;
   cout << "# " << endl;
   
   // mixing trees
   TString mixfname=infname;
   bool doMixBkg=false;
//   if (dataSrcType<=1&&dataType==1) {
//      doMixBkg=true;
//      mixfname.ReplaceAll("_akPu3PF","_mixmb_akPu3PF");
//      cout << "Mix file: " << mixfname << endl;
//   }
   if (doMixBkg) nt->AddFriend("tmix=tgj",mixfname);
   
   // loop over centrality bins
   for (int ib=0; ib<vcutCent.size(); ++ib) {
      TString name = myname+Form("_dataSrc%d_reco%d_%d",dataSrcType,dataType,ib);
      int cBin = ib;
      if (dataSrcType>1) cBin==vcutCent.size()-1;
      // initialize ana
      vana.push_back(new SignalCorrector(nt,name,var,Form("pt1>%.3f",minPhoton)&&mycut&&vcutCent[ib],weight,cBin,dataSrcType));
      // setup cuts
      vana[ib]->cutSigAllLeading     = "abs(eta1)<1.6";
      vana[ib]->cutBkgDPhi       = vana[ib]->cutSigAllLeading    && jetSel && Form("acos(cos(phi2-phi2))>0.7 && acos(cos(phi2-phi2))<3.14159/2.");
      if (doMixBkg) {
         vana[ib]->cutBkgDPhi    = vana[ib]->cutSigAllLeading    && jetBkgSel && Form("acos(cos(phi2-tmix.phi2))>%f",sigDPhi);
         vana[ib]->rBkgDPhi.var = bkgvar;
      }
      
      // analyze tree
      float nxbins=80, xmin=-400, xmax=400;
      if (dataType==0) {
         vana[ib]->subDPhiSide = false;
         vana[ib]->MakeHistograms(vana[ib]->cutSigAllLeading&&jetSel&&Form("acos(cos(phi2-phi1))>%f",sigDPhi),nxbins,xmin,xmax);
      } else {
         vana[ib]->subDPhiSide = subDPhiSide;
         vana[ib]->MakeHistograms(vana[ib]->cutSigAllLeading&&jetSel&&Form("acos(cos(phi2-phi1))>%f",sigDPhi),nxbins,xmin,xmax);
      }
      
      if (!doMixBkg) {
         vana[ib]->Extrapolate((3.14159-sigDPhi)/(3.14159/2.-0.7));
      } else {
         vana[ib]->Extrapolate(1./40);
      }
      vana[ib]->SubtractBkg();
      vana[ib]->Normalize(normMode);
      
      // output histograms
      TFile* hout = new TFile(outfname,"update");
      vana[ib]->rSubtracted.hExtrapNorm->Write();
      if (vana[ib]->rSigAllLeading.h) vana[ib]->rSigAllLeading.h->Write();
      if (vana[ib]->rSigAllLeading.hExtrap) vana[ib]->rSigAllLeading.hExtrap->Write();
      if (vana[ib]->rSigAll.hExtrapNorm) vana[ib]->rSigAll.hExtrapNorm->Write();
      if (vana[ib]->rBkgDPhi.hExtrapNorm) vana[ib]->rBkgDPhi.hExtrapNorm->Write();
      hout->Close();
   }
//   inf->Close();
}

//---------------------------------------------------------------------
void anaMptSignal_AllCent4_wSummary(
                                         int isolScheme=0, // 0=sumIsol, 1=cutIsol, 2=fisherIsol
                                         int normMode=0, // 0=no norm, 1=unity, 2=per photon
                                         int subDPhiSide = 0,
                                         int subSShapeSide = 0,
                                         float minPhoton=120,
                                         float minJet=50,
                                         int log=0,
                                         int drawCheck = 0,
                                         TString outdir = "./fig/04.11_dijetmpt_compare"
                                         )
{
   TH1::SetDefaultSumw2();
   float sigDPhi=3.1415926*7./8;
   
   outfname=Form("%s/HisOutput_DiJetv7_v1_icPu5_MptCone_%.0f_%.0f_%.0f_Norm%d.root",outdir.Data(),minPhoton,minJet,sigDPhi*1000,normMode);
   TFile* hout = new TFile(outfname,"RECREATE");
   hout->Close();

   const int nBin = 4;
   float m[nBin+1] = {0.0001,0.11,0.22,0.33,0.49999};

   ////////////////////////////////////////////////////////////////////////////////////////////////
   // Analysis
   ////////////////////////////////////////////////////////////////////////////////////////////////
   TString inputTree_data="../ntout/output-data-DiJet-v7_v1_icPu5.root";
   TString inputTree_mc="../ntout/norewt-output-hy18dj80_v1_xsec_icPu5.root";

   TCut mycut="cBin>=0&&cBin<12";

   vector<TCut> vcutCent,vcutCentPp;
   for (int ib=0; ib<nBin; ++ib) vcutCent.push_back(Form("Aj>=%f&&Aj<%f",m[ib],m[ib+1]));
   vcutCentPp.push_back("1==1");
   
   TCut jetSel = Form("abs(eta2)<1.6&&pt2>%.3f",minJet);

   // MPT
   vector<TString> mptCand;
   vector<TString> mptType;
   vector<float> cones;
   mptCand.push_back("trk");
   //mptCand.push_back("trkCorr");
   cones.push_back(4);
   cones.push_back(8);
   cones.push_back(12);
   //cones.push_back(14);

   //mptType.push_back("AllAcc");
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
	    xmptObs.push_back("-1*("+mpttag+")");
	    xmptObs.push_back("-1*("+mpttag+"_pt[1])");
	    xmptObs.push_back("-1*("+mpttag+"_pt[4]+"+mpttag+"_pt[5])");
	    
	    for (int k=0; k<xmptObs.size(); ++k) {
	       vector<SignalCorrector*> vanahyphompt;
	       getHistograms(Form("hypho%s_ptmerge%d",mpttag.Data(),k),xmptObs[k],"",vanahyphompt, vcutCent,"offlSel"&&mycut,jetSel,"",isolScheme,normMode,inputTree_mc,mcweight,0,1,0,0,minPhoton,minJet,sigDPhi);
	       
	       vector<SignalCorrector*> vanahimpt;
	       getHistograms(Form("hi%s_ptmerge%d",mpttag.Data(),k),xmptObs[k],"",vanahimpt, vcutCent,"anaEvtSel"&&mycut,jetSel,"",isolScheme,normMode,inputTree_data,"(1==1)",1,1,0,0,minPhoton,minJet,sigDPhi);
	    }
	 }
      }
   }
}
