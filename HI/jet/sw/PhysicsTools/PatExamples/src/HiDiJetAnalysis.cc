#include "PhysicsTools/PatExamples/interface/HiDiJetAnalysis.h"
// root
#include "TFile.h"
#include "TTree.h"
// ROOT Mathematical Libraries
#include "Math/GenVector/VectorUtil.h"
// helpers
#include "/home/frankma/UserCode/SavedFMa/analysis/cpp/templates/stl_helper_fuctions.h"

using namespace std;
using namespace ROOT::Math;

namespace jetana
{
  //  constructor/destructor =======================================
  HiDiJetAnalysis::HiDiJetAnalysis() :
    anaOnlyLeadDijet_(true),
    anaFF_(false),
    fragDRMax_(1.),
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
  // hi event vars
  void HiDiJetAnalysis::StoreHiEventVars(fwlite::Handle<pat::HeavyIon> * hievt)
  {
    pat::HeavyIon hievtcp = *(*hievt);
    jd_.b_	     = hievtcp.generatedB();
    jd_.npart_	     = hievtcp.generatedNpart();
    jd_.ncoll_	     = hievtcp.generatedNcoll();
  }

  // dijets related
  void HiDiJetAnalysis::CalcJetVars(const DiJet & dijet)
  {
    if (verbosity_>=3)
      cout << "cal jet - dijet: " << dijet << endl;

    // fill dijet info
    jd_.mass_	     = dijet.m();
    jd_.cmeta_	     = dijet.eta();
    jd_.jdphi_	     = dijet.dphi_;

    // dijet qualities
    jd_.leadAwayMul_ = dijet.leadAwayMul_;

    // near/away info
    jd_.nljet_	     = dijet.nj_.pt();
    jd_.nljeta_	     = dijet.nj_.eta();
    jd_.nljphi_	     = dijet.nj_.phi();

    jd_.aljet_	     = dijet.aj_.pt();
    jd_.aljeta_	     = dijet.aj_.eta();
    jd_.aljphi_	     = dijet.aj_.phi();
  }

  // fragmenation related
  bool HiDiJetAnalysis::isFrag(const DiJet & dijet, const AnaInputItem & track)
  {
    bool result = false;
    if ( VectorUtil::DeltaR(dijet.nj_,track)<fragDRMax_ || VectorUtil::DeltaR(dijet.aj_,track)<fragDRMax_) {
      result = true;
    }
    return result;
  }

  void HiDiJetAnalysis::CalcFragVars(const DiJet & dijet, const AnaInputCollection & fcands)
  {
    if (verbosity_>=3) {
      cout << "calc frag - dijet: " << dijet << endl;
      cout << "fragment candidates: " << endl;
      mystd::print_elements(fcands);
    }

    // counter for # particles saved for each dijet pair
    int ct = 0;

    // loop over fragment candidates
    for (unsigned int ip=0; ip<fcands.size(); ++ip) {
      // saved only tracks associated with dijet
      if ( !isFrag(dijet,fcands[ip]) ) continue;

      // fill frag candidates basic info
      jd_.ppt_[ct]	= fcands[ip].pt();
      jd_.peta_[ct]     = fcands[ip].eta();
      jd_.pphi_[ct]     = fcands[ip].phi();

      // Relations to jet
      jd_.pndphi_[ct]	= VectorUtil::DeltaPhi(fcands[ip],dijet.nj_);
      jd_.pndeta_[ct]	= fcands[ip].eta() - dijet.nj_.eta();
      jd_.pndr_[ct]	= VectorUtil::DeltaR(fcands[ip],dijet.nj_);

      jd_.padphi_[ct]	= VectorUtil::DeltaPhi(fcands[ip],dijet.aj_);
      jd_.padeta_[ct]	= fcands[ip].eta() - dijet.aj_.eta();
      jd_.padr_[ct]	= VectorUtil::DeltaR(fcands[ip],dijet.aj_);

      // fragmentation variables
      jd_.zn_[ct]	= fcands[ip].pt()/dijet.nj_.pt();
      jd_.za_[ct]	= fcands[ip].pt()/dijet.aj_.pt();

      // saved one frag candidate, update counter
      ++ct;
    } // end of loop over fragment candidates

    // save counter
    jd_.evtnp_		= ct;
  }

  // main methods ==================================================
  void HiDiJetAnalysis::Fill(const AnaDiJetCollection & dijets, const AnaInputCollection & tracks, fwlite::Handle<pat::HeavyIon> * hievt)
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

      // load hi event info
      if (hievt!=NULL) StoreHiEventVars(hievt);

      // calc jet vars
      CalcJetVars(dijets[idj]);
      if (anaFF_) CalcFragVars(dijets[idj],tracks);

      // fill dijet by dijet
      // - note
      //   * should take care only the particles counted as frag should be filled
      //cout << "tree address: " << tree_ << endl;
      tree_->Fill();
    }
  }
} // jetana namespace
