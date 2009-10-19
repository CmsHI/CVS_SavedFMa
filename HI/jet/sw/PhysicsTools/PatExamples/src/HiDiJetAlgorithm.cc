//
// Based on http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/RecoJets/JetAlgorithms/src/CMSIterativeConeAlgorithm.cc?hideattic=0&view=markup
//

#include "PhysicsTools/PatExamples/interface/HiDiJetAlgorithm.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "PhysicsTools/PatExamples/interface/DiJet.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "RecoJets/JetAlgorithms/interface/JetAlgoHelper.h"

// helpers
#include "/home/frankma/UserCode/SavedFMa/analysis/cpp/templates/stl_helper_fuctions.h"

using namespace std;
using namespace reco;

namespace jetana
{
  // constructors
  HiDiJetAlgorithm::HiDiJetAlgorithm():
    nearThreshold_(20),
    awayThreshold_(20),
    dPhiMin_(3.14-0.5),
    verbosity_(0)
  {
  } 
  HiDiJetAlgorithm::HiDiJetAlgorithm(double nearThresh, double awayThresh, double dPhiMin):
    nearThreshold_(nearThresh),
    awayThreshold_(awayThresh),
    dPhiMin_(dPhiMin),
    verbosity_(0)
  { /* empty */ }

  // helpers
  InputCollection::iterator HiDiJetAlgorithm::FindPair(const InputItem & near, InputCollection & others)
  { 
    if (verbosity_>=2) {
      cout << "pair algo input near: " << near << endl;
      cout << "pair algo input others: " << endl;
      mystd::print_elements(others);
    }

    InputCollection::iterator it_away=NULL;
    if (others.size() == 0) return it_away;

    double max = 0;
    for (InputCollection::iterator icand=others.begin(); icand!=others.end(); ++icand) {
      if (verbosity_>3)
	cout << "processing: " << *icand << endl;
      if (!PassAwayJetCriterion(*icand))
	continue;
      double dphi=absDPhi(near,*icand);
      if (dphi>dPhiMin_ && max<dphi)
	it_away = icand;
    }

    if (verbosity_>=2) {
      if (it_away!=NULL) cout << "found away: " << *it_away << endl << endl;
      else cout << endl;
    }
    return it_away;
  }
  //  Run the algorithm
  //  ------------------
  void HiDiJetAlgorithm::Group(InputCollection& input, OutputCollection* output)
  {
    if (!output) return;
    // need at least 2 input items
    if (input.size()<2) return;

    InputCollection::iterator imaxPt = max_element(input.begin(),input.end(),lessPt);
    while ( input.size()>=2 && PassNearJetCriterion(*imaxPt)) {
      if (verbosity_>=2) {
	cout << "group algo input items: " << endl;
	mystd::print_elements(input);
	cout << "max pt item: " << *imaxPt << endl;
	cout << "pass criterio? " << PassNearJetCriterion(*imaxPt) << endl;;
      }

      // save and remove leading jet from input list
      OutputItem dj;
      dj.SetNearJet(*imaxPt);
      input.erase(imaxPt);

      // find away jet
      InputCollection::iterator iaway = FindPair(*imaxPt,input);
      // if found an away jet passing away criterion and paired to the leading
      // jet, save to dijet
      if (iaway!=NULL) {
	dj.SetAwayJet(*iaway);
	input.erase(iaway);
	// saved paired dijet
	output->push_back(dj);
      }

      // update next leading item
      imaxPt = max_element(input.begin(),input.end(),lessPt);
    } // end of loop over input items
  }

} //jetana
