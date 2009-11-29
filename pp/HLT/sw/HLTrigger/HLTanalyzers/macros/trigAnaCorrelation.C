#include <iostream>
#include <iomanip>
#include <TFile.h>
#include <TTree.h>
#include <math.h>
#include <string.h>
#include <vector.h>
#include <TH2.h>
#include <TPaletteAxis.h>
#include <TCanvas.h>

using namespace std;

vector <string>* triggers = new vector<string>;
vector <string>* triggerCuts = new vector<string>;
vector< vector <double*> >* results = new vector< vector <double*> >;


double calcEffErr(double nEvt,double nAcc)
{
   double eff =  nAcc / nEvt;
   return sqrt(eff*(1-eff)/nEvt);
}

double* calcEff(TTree* HltTree, char *title, double nEvt,char *cut,int flag=1)
{
   double nAcc = HltTree->GetEntries(cut);
   double effErr = calcEffErr(nEvt,nAcc);
   double eff = nAcc/nEvt;
   double *result = new double[2];
   result[0]=eff;
   result[1]=effErr;
   
   // print out or not
   if (flag) {
      triggers->push_back(string(title));
      triggerCuts->push_back(string(cut));
      cout <<" | "<<setw(20)<<title;
      cout <<" | "<<setw(10)<<fixed<<setprecision(3)<<eff*100.<<"% +-";
      cout        <<setw(10)<<fixed<<setprecision(3)<<effErr*100.<<"%";
      cout <<" | "<<endl;
   }
   
   return result;
}

void printEff(TTree* HltTree,char *cut,char *title, char *projectTitle)
{
   cout <<"   * "<<title<<":"<<endl;
   cout <<"      * Efficiencies:"<<endl;
   double nEvt = HltTree->GetEntries(cut);
      
   cout <<" | "<<setw(20)<<"HLT Path";
   cout <<" | "<<setw(25)<<"Efficiency";
   cout <<" | "<<endl;
   
   triggers->clear();
   triggerCuts->clear();
   
   vector <double*> effs;
   
   // calculate the efficiency //   
   effs.push_back(calcEff(HltTree,"HLT_MinBiasBSC",nEvt,Form("(%s)&&HLT_MinBiasBSC==1",cut)));
   effs.push_back(calcEff(HltTree,"HLT_MinBiasBSC_OR",nEvt,Form("(%s)&&HLT_MinBiasBSC_OR==1",cut)));
   effs.push_back(calcEff(HltTree,"HLT_MinBiasHcal",nEvt,Form("(%s)&&HLT_MinBiasHcal==1",cut)));
   effs.push_back(calcEff(HltTree,"HLT_MinBiasEcal",nEvt,Form("(%s)&&HLT_MinBiasEcal==1",cut)));
   effs.push_back(calcEff(HltTree,"HLT_MinBiasPixel_SingleTrack",nEvt,Form("(%s)&&HLT_MinBiasPixel_SingleTrack==1",cut)));
   effs.push_back(calcEff(HltTree,"HLT_MinBiasPixel_DoubleTrack",nEvt,Form("(%s)&&HLT_MinBiasPixel_DoubleTrack==1",cut)));
   effs.push_back(calcEff(HltTree,"HLT_HFThreshold3",nEvt,Form("(%s)&&HLT_HFThreshold3==1",cut)));
   effs.push_back(calcEff(HltTree,"HLT_Activity_EcalREM",nEvt,Form("(%s)&&HLT_Activity_EcalREM==1",cut)));
   effs.push_back(calcEff(HltTree,"HLT_Activity_PixelClusters",nEvt,Form("(%s)&&HLT_Activity_PixelClusters==1",cut)));
   effs.push_back(calcEff(HltTree,"All",nEvt,Form("(%s)&&(HLT_MinBiasBSC||HLT_MinBiasBSC_OR||HLT_MinBiasHcal||HLT_MinBiasEcal||HLT_MinBiasPixel_SingleTrack||HLT_MinBiasPixel_DoubleTrack||HLT_HFThreshold3||HLT_Activity_EcalREM||HLT_Activity_PixelClusters)",cut)));
   results->push_back(effs);

   cout <<"      * Correlation Matrix:"<<endl;
   int tsize = (int)triggers->size();
   TH2D *h = new TH2D(Form("h%s",title),"",tsize,0,tsize,tsize,0,tsize);
 
   for (int i=tsize-1;i>=0;i--){
      int nEvtAfterCut = HltTree->GetEntries((*triggerCuts)[i].c_str());
      for (int j=0;j<tsize;j++){
         string cut ="("+(*triggerCuts)[i]+")&&("+(*triggerCuts)[j]+")";
         double* eff = calcEff(HltTree,"",nEvtAfterCut,Form("%s",cut.c_str()),0);
	 if (nEvtAfterCut==0) eff[0]=0;
	 h->SetBinContent(j+1,i+1,int(eff[0]*100000)/1000.);
         h->GetXaxis()->SetBinLabel(j+1,(*triggers)[j].c_str());
      }
      h->GetYaxis()->SetBinLabel(i+1,(*triggers)[i].c_str());
   }
   
   TCanvas *c1 = new TCanvas(Form("c%s",title), Form("c_%s",title),800,600);
   c1->Range(-3.609756,-1.910995,12.7561,10.60209);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(0);
   c1->SetTickx();
   c1->SetTicky();
   c1->SetLeftMargin(0.2205663);
   c1->SetRightMargin(0.1684054);
   c1->SetTopMargin(0.04811715);
   c1->SetBottomMargin(0.1527197);
   c1->SetFrameLineColor(0);
   c1->SetFrameBorderMode(0);
   c1->SetFrameLineColor(0);
   c1->SetFrameBorderMode(0);
   
   h->Draw("col text");

   TPaletteAxis *palette = new TPaletteAxis(tsize*1.02,0,tsize*1.1,tsize,h);
   palette->SetLabelColor(1);
   palette->SetLabelFont(42);
   palette->SetLabelOffset(0.005);
   palette->SetLabelSize(0.045);
   palette->SetTitleOffset(1);
   palette->SetTitleSize(0.04);
   palette->SetFillColor(100);
   palette->SetFillStyle(1001);
   h->GetListOfFunctions()->Add(palette,"br");
   h->Draw("col text z");

   string fname(Form("plot_%s_%s.gif",projectTitle,title));
   c1->SaveAs(fname.c_str());
   c1->SaveAs(Form("plot_%s_%s.C",projectTitle,title));

   cout <<"<img src=\"%ATTACHURLPATH%/"<<fname<<"\" alt=\""<<fname<<"\" width='671'   height='478' />"<<endl;   
}

void trigAnaCorrelation(char *infile="openhlt-900GeV.root",char *projectTitle = "900GeV")
{
   TFile *inf = new TFile(infile);
   
   TTree *HltTree =  (TTree*) inf->FindObjectAny("HltTree");
   
   int nEvent = HltTree->GetEntries();
   int nSD = HltTree->GetEntries("evtType==92||evtType==93");
   int nDD = HltTree->GetEntries("evtType==94");
   int nND = nEvent-nSD-nDD;
   int nNSD = nEvent-nSD;


   cout <<"<pre>"<<endl;
   cout <<std::right<<std::setw(20)<<"Event: "<<nEvent<<endl;
   cout <<std::right<<std::setw(20)<<"SD Event: "<<nSD<<endl;
   cout <<std::right<<std::setw(20)<<"DD Event: "<<nDD<<endl;
   cout <<std::right<<std::setw(20)<<"ND Event: "<<nND<<endl;
   cout <<std::right<<std::setw(20)<<"NSD Event: "<<nNSD<<endl;
   cout <<"</pre>"<<endl;
   

   printEff(HltTree,"1==1","All",projectTitle);
   printEff(HltTree,"(evtType==92||evtType==93)","Single_Diffractive",projectTitle);
   printEff(HltTree,"evtType==94","Double_Diffractive",projectTitle);
   printEff(HltTree,"evtType!=92&&evtType!=93","Non_Single_Diffractive",projectTitle);
   printEff(HltTree,"evtType!=92&&evtType!=93&&evtType!=94","Non_Diffractive",projectTitle);

   string evtType[5];
   evtType[0]="All";
   evtType[1]="SD";
   evtType[2]="DD";
   evtType[3]="NSD";
   evtType[4]="ND";
   
   cout <<" | "<<setw(20)<<" ";
   for (int i=0;i<5;i++) {
      cout <<" | "<<setw(8)<<evtType[i];
   }
   
   cout <<" | " <<endl;

   for (int i=0;i<(int)triggers->size();i++){
      cout <<" | " <<setw(20)<<(*triggers)[i];
      for (int j=0;j<(int)results->size();j++) {
         double eff=(((*results)[j])[i])[0];
         double effErr=(((*results)[j])[i])[1];
         cout <<" | "<<setw(10)<<fixed<<setprecision(3)<<eff*100.<<"% +-";
         cout        <<setw(10)<<fixed<<setprecision(3)<<effErr*100.<<"% ";
      }
      cout <<" | "<<endl;
   }   
}
