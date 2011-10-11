#include <iostream>
#include "TTree.h"
#include "TFile.h"
#include "TCut.h"
using namespace std;

void ScanTracks(
                int run=151088,
                int evt=646838,
                float ptmin=30)
{
  TFile * fold = new TFile("../trees/zs01-7.root");
  TFile * fnew = new TFile("../trees/zs01-8.root");
  
  TTree * told = (TTree*)fold->Get("anaTrack/trackTree");
  TTree * tnew = (TTree*)fnew->Get("anaTrack/trackTree");
  
  TString trkHgtSel="(trkPtError/trkPt<=0.06&&trkNHit>=13&&trkChi2/trkNdof/trkNlayer<=0.15&&abs(trkDz1/trkDzError1)<3&&abs(trkDxy1/trkDxyError1)<3)";
  TTree * ts[2] = {told,tnew};
  for (int i=0; i<2; ++i) {
    ts[i]->SetAlias("isHGT",trkHgtSel);
    ts[i]->SetAlias("normPtError","trkPtError/trkPt");
    ts[i]->SetAlias("normChi2","trkChi2/trkNdof/trkNlayer");
    ts[i]->SetAlias("normDz","trkDz1/trkDzError1");
    ts[i]->SetAlias("normD0","trkDxy1/trkDxyError1");
    ts[i]->SetAlias("caloComp","(pfSumHcal+pfSumEcal)/trkPt>0.15");
  }

  TString evtvars = "nRun:nEv:cbin";
  TString trkvars = "trkPt:trkEta:trkPhi:isHGT:trkNHit:normPtError:normChi2:normDz:normD0";
  
  TCut evtsel = Form("nRun==%d&&nEv==%d",run,evt);
  TCut trksel = Form("trkPt>=%.1f",ptmin);
  
  cout << "Evt selection: " << evtsel << endl;
  cout << "Track collection: hiGlobalPrimTracks" << endl;
  cout << "Track selection: " << trksel << endl;

  cout << endl;
  cout << "Old ZS:" << endl;
  told->Scan(evtvars+":"+trkvars,evtsel&&trksel);
  cout << "New ZS:" << endl;
  tnew->Scan(evtvars+":"+trkvars,evtsel&&trksel);
}