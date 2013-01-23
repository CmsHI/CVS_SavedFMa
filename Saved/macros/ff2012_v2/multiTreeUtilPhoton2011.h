#ifndef MULTITREETUIL_H
#define MULTITREETUIL_H

// =========================================================================
// Multi Tree Utility
// Author: Yen-Jie Lee (MIT)
// Modified by Yongsun Kim (MIT)
//       A tool for multiTree handling
// =========================================================================
#include <TTree.h>
#include <TCut.h>
#include <TFile.h>
#include <vector>
#include <TH1.h>
#include <cassert>


class multiTreeUtil
{
public:
  multiTreeUtil() {
    TH1::SetDefaultSumw2();
    nTrees=0;
    for (int i=0;i<100;i++)
    {
      color[i]=i;
    }
    color[0]=kOrange-3;
    color[1]=3;
    color[2]=4;
    color[3]=5;
    color[4]=6;
    color[5]=9;
    color[6]=1;
    color[7]=kAzure+3;
    color[8]=kOrange+3;
    color[9]=kSpring-7;
    color[10]=kPink+4;
  }
  ~multiTreeUtil() {
    for (int i=0;i<(int)trees_.size();i++) {
      tfiles_[i]->Close();
    }
  }
  void addFile(TString filename, TString treeName, TCut cut, Float_t scaleFactor=1);
  void NormalizeScales(Int_t normMode=1, TCut cut="", TString name="");
  void Draw(TH1D* h,TString expression,TCut cut = "" , TString indWeight="", TString opt="");
  void Draw2(TH1D* h,TString expression,TCut cut = "", TString indWeight="", TString opt="",bool hist=true);
  void Draw3(TH1D* h,TString expression,TCut cut = "", TString indWeight="", TString opt="",bool hist=true);
  void Draw2D(TH2D* h,TString expression,TCut cut = "", TString indWeight="", TString opt="",bool hist=true);
  void Print(Option_t* option = "");
  void AddFriend(TString friendname="");
  void SetAlias(TString nameAl="", TString varAl="");
  TTree *getTree(int i) { return trees_[i]; };

private:
  int nTrees;
  vector<TFile*>  tfiles_;                // TFile 
  vector<TTree*>  trees_;                 // Trees
  vector<TString>   tfileNames_;            // File names
  vector<TCut>    tcuts_;
  vector<Float_t> scaleFactors_;
  std::vector<Float_t> nEvts_;
  int color[100];

  void cleanHist(TH1* h);
};

// =========================================================================
// Add file
// =========================================================================
void multiTreeUtil::addFile(TString filename, TString treeName, TCut cut, Float_t scaleFactor) {
  TFile *f = new TFile(filename.Data());
  TTree *t = (TTree*) f->FindObjectAny(treeName.Data());

  if (t!=0) {
    nTrees++;
    tfileNames_.push_back(filename);
    tfiles_.push_back(f);
    trees_.push_back(t);
    tcuts_.push_back(cut);
    scaleFactors_.push_back(scaleFactor);
  }
  else
    cout << " there is no such tree in this file :" << treeName << endl;
}

void multiTreeUtil::NormalizeScales(Int_t normMode, TCut cut, TString name) {
  // Any additional normalization to the scale factors of trees
  for (int i=0;i<(int)trees_.size();i++) {
    if (normMode==1) {
      nEvts_.push_back(trees_[i]->GetEntries(cut));
    } else if (normMode==2) {
      TH1D * h = (TH1D*)tfiles_[i]->Get(name);
      assert(h);
      nEvts_.push_back(h->GetEntries());
    }
    if (normMode) {
      cout << "xsec: " << scaleFactors_[i] << " nEvt: " << nEvts_[i] << endl;
      scaleFactors_[i]/=nEvts_[i];
    }
  }
}

// =========================================================================
// Draw
// =========================================================================
void multiTreeUtil::Draw(TH1D *h, TString expression, TCut cut,TString indWeight, TString opt) {
  h->Reset();
  for (int i=0;i<(int)trees_.size();i++) {
    TH1D *htmp = (TH1D*)h->Clone();
    htmp->SetName(Form("my_htmp_%d",i));
    htmp->Reset();
    TCut fCut = cut&&tcuts_[i];
    if ( indWeight=="") {
      trees_[i]->Draw(Form("%s>>my_htmp_%d",expression.Data(),i),  Form("(%s)", fCut.GetTitle() ));
    } else {
      trees_[i]->Draw(Form("%s>>my_htmp_%d",expression.Data(),i),  Form("(%s) * (%s)", fCut.GetTitle(), indWeight.Data() ) );
    }
    cout << "cut * weight :  " << endl << Form("   ( %s )   *   (%s)", fCut.GetTitle(),  indWeight.Data()) << endl;
    cout << " and scale factor = " << double(scaleFactors_[i]) << endl;
    htmp->Scale(scaleFactors_[i]);
    h->Add(htmp);
    delete htmp;
  }
  if (opt!="") h->Draw(opt.Data());
}

// =========================================================================
// Draw components
// =========================================================================
void multiTreeUtil::Draw2(TH1D *h, TString expression, TCut cut, TString indWeight, TString opt,bool hist) {
  TH1D *hComponent[100];

  h->Reset();
  for (int i=0;i<(int)trees_.size();i++) {
    TString hName = Form("%s_Draw_%d",h->GetName(),i);
    TH1D *htmp= (TH1D*)h->Clone();
    htmp->SetName(hName.Data());
    htmp->Reset();
    TCut fCut = cut&&tcuts_[i];
    if ( indWeight=="" ) {
      trees_[i]->Draw(Form("%s>>%s",expression.Data(),hName.Data()), Form("(%s)", fCut.GetTitle() ));
    } else {
      trees_[i]->Draw(Form("%s>>%s",expression.Data(),hName.Data()), Form("( %s ) * (%s)", fCut.GetTitle(), indWeight.Data()) );
    }
    cout << "cut * weight :  " << endl << Form("   ( %s )   *   (%s)", fCut.GetTitle(),  indWeight.Data()) << endl;
    cout << " and scale factor = " << double(scaleFactors_[i]) << endl;
    htmp->Scale(scaleFactors_[i]);
    h->Add(htmp);
    hComponent[i]=htmp;
  }
  h->DrawCopy(opt.Data());

  for (int i=0;i<(int)trees_.size();i++) {
    hComponent[i]->SetLineColor(color[i]);
    hComponent[i]->SetMarkerColor(color[i]);
    hComponent[i]->SetFillColor(color[i]);
    if ( hist) opt ="hist";
    hComponent[i]->DrawCopy(Form("%s same",opt.Data()));
    delete hComponent[i];
  }
}


void multiTreeUtil::Draw2D(TH2D *h, TString expression, TCut cut, TString indWeight, TString opt,bool hist) {
  TH2D *hComponent[20];
  h->Reset();
  for (int i=0;i<(int)trees_.size();i++) {
    TString hName = Form("%s_Draw_%d",h->GetName(),i);
    TH2D *htmp= (TH2D*)h->Clone();
    htmp->SetName(hName.Data());
    TCut fCut = cut&&tcuts_[i];
    if ( indWeight=="" ) {
      trees_[i]->Draw(Form("%s>>%s",expression.Data(),hName.Data()), Form("(%s)", fCut.GetTitle() ));
    } else {
      trees_[i]->Draw(Form("%s>>%s",expression.Data(),hName.Data()), Form("( %s ) * %s", fCut.GetTitle(),  indWeight.Data()) );
    }
    htmp->Scale(scaleFactors_[i]);
    h->Add(htmp);
    hComponent[i]=htmp;
  }
   h->DrawCopy(opt.Data());

  for (int i=0;i<(int)trees_.size();i++) {
    hComponent[i]->SetLineColor(color[i]);
    hComponent[i]->SetMarkerColor(color[i]);
    hComponent[i]->SetFillColor(color[i]);
    if ( hist) opt ="hist";
    hComponent[i]->DrawCopy(Form("%s same",opt.Data()));
    delete hComponent[i];
  }
}




void multiTreeUtil::Draw3(TH1D *h, TString expression, TCut cut, TString indWeight, TString opt,bool hist) {
  TH1D *hComponent[100];
  h->Reset();
  for (int i=0;i<(int)trees_.size();i++){
    TString hName = Form("%s_Draw_%d",h->GetName(),i);
    TH1D *htmp= (TH1D*)h->Clone();
    htmp->SetName(hName.Data());
    TCut fCut = cut&&tcuts_[i];
    if ( indWeight=="" ) {
      trees_[i]->Draw(Form("%s>>%s",expression.Data(),hName.Data()), Form("%s", fCut.GetTitle()));
    } else {
      trees_[i]->Draw(Form("%s>>%s",expression.Data(),hName.Data()), Form("(%s) * %s", fCut.GetTitle(),  indWeight.Data()) );
    }
    htmp->Scale(scaleFactors_[i]);
    h->Add(htmp);
    hComponent[i]=htmp;
    hComponent[i]->SetLineColor(color[i]);
    hComponent[i]->SetMarkerColor(color[i]);
    hComponent[i]->SetFillColor(color[i]);
  }
  h->DrawCopy(opt.Data());

  for (int i=0;i<(int)trees_.size();i++) {
    if ( i<(int)trees_.size() -1 ) {
      hComponent[i+1]->Add( hComponent[i]);
    }
  }

  for (int i=0;i<(int)trees_.size();i++) {
    if ( hist) opt ="hist";
    hComponent[trees_.size()-i-1]->DrawCopy(Form("%s same",opt.Data()));
  }
}


// =========================================================================
// Print variables from the first tree
// =========================================================================
void multiTreeUtil::AddFriend(TString friendname) {
  for (int i=0;i<(int)trees_.size();i++) {
    trees_[i]->AddFriend(friendname.Data(), tfileNames_[i]);
  }
}

void multiTreeUtil::SetAlias(TString nameAl, TString varAl) {
  for (int i=0;i<(int)trees_.size();i++) {
    trees_[i]->SetAlias( nameAl.Data(), varAl.Data());
  }
}



void multiTreeUtil::Print(Option_t* option)
{
 trees_[0]->Print(option);   
}

//--------------------------------PRIVATE-----------------------------------

// =========================================================================
// Clear
// =========================================================================
void multiTreeUtil::cleanHist(TH1* h) {
  for (int i=0;i<=h->GetNbinsX()+1;i++) {
    h->SetBinContent(i,0);
    h->SetBinError(i,0);
  }
}

#endif
