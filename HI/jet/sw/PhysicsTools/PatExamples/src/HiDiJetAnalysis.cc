#include "PhysicsTools/PatExamples/interface/HiDiJetAnalysis.h"
// root
#include "TFile.h"
#include "TTree.h"
// helpers
#include "/home/frankma/UserCode/SavedFMa/analysis/cpp/templates/stl_helper_fuctions.h"

using namespace std;

namespace jetana
{
  HiDiJetAnalysis::HiDiJetAnalysis() :
    anaOnlyLeadDijet_(true),
    verbosity_(0)
  {
    TTree * tree_ = new TTree("dijetTree","dijet tree");
    jd_.SetTree(tree_);

    // set branches of tree to jd_'s vars
    jd_.SetBranches();
  }

  void HiDiJetAnalysis::CalcJetVars(const DiJet & dijet)
  {
    if (verbosity_>=3)
      cout << "dijet: " << dijet << endl;

    // fill jet info
    jd_.mass_ = dijet.m();
  }

  void HiDiJetAnalysis::CalcFragVars(const DiJet & dijet, const AnaInputCollection & fragments)
  {
    if (verbosity_>=3) {
      cout << "dijet: " << dijet << endl;
      cout << "fragments: " << endl;
      mystd::print_elements(fragments);
    }
  }

  bool HiDiJetAnalysis::isFrag(const DiJet & dijet, const AnaInputItem & track)
  {
    return true;
  }

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
      cout << "tree address: " << tree_ << endl;
      //tree_->Fill();
    }
  }
} // jetana namespace
