#include "PhysicsTools/PatExamples/interface/HiDiJetAnalysis.h"
// root
#include "TFile.h"
#include "TTree.h"
// helpers
#include "/home/frankma/UserCode/SavedFMa/analysis/cpp/templates/stl_helper_fuctions.h"

using namespace std;

namespace jetana
{
  //  constructor/destructor =======================================
  HiDiJetAnalysis::HiDiJetAnalysis() :
    anaOnlyLeadDijet_(true),
    verbosity_(0)
  {
    // allocate memory for tree
    tree_ = new TTree("dijetTree","dijet tree");
    //cout << "initialize HiDiJetAnalysis instance with tree: " << tree_ << endl;

    // set tree address and branches to jet ana data
    jd_.SetTree(tree_);
    jd_.SetBranches();
  }

  HiDiJetAnalysis::~HiDiJetAnalysis() { delete tree_; }

  // helpers ======================================================
  void HiDiJetAnalysis::CalcJetVars(const DiJet & dijet)
  {
    if (verbosity_>=3)
      cout << "cal jet - dijet: " << dijet << endl;

    // fill jet info
    jd_.mass_ = dijet.m();
  }

  void HiDiJetAnalysis::CalcFragVars(const DiJet & dijet, const AnaInputCollection & fragments)
  {
    if (verbosity_>=3) {
      cout << "calc frag - dijet: " << dijet << endl;
      cout << "fragments: " << endl;
      mystd::print_elements(fragments);
    }
  }

  bool HiDiJetAnalysis::isFrag(const DiJet & dijet, const AnaInputItem & track)
  {
    return true;
  }

  // main methods ==================================================
  void HiDiJetAnalysis::Fill(const AnaDiJetCollection & dijets, const AnaInputCollection & tracks)
  {
    if (verbosity_>=2) {
      cout << "dijets: " << endl;
      mystd::print_elements(dijets);
      cout << "tracks: " << endl;
      mystd::print_elements(tracks);
    }

    for (unsigned int idj=0; idj<dijets.size(); ++idj) {
      // in the beginning we'll only look at the lead dijet in the event
      if (anaOnlyLeadDijet_ && idj==1) break;

      CalcJetVars(dijets[idj]);
      CalcFragVars(dijets[idj],tracks);
      // fill dijet by dijet
      // - note
      //   * should take care only the particles counted as frag should be filled
      //cout << "tree address: " << tree_ << endl;
      tree_->Fill();
    }
  }
} // jetana namespace
