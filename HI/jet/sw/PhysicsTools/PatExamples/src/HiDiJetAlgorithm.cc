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
    evtJetMul_(0),
    ldjAwayJetMul_(0),
    foundDijet_(false),
    verbosity_(0)
  {
    // emtpy
  } 
  HiDiJetAlgorithm::HiDiJetAlgorithm(double nearThresh, double awayThresh, double dPhiMin):
    nearThreshold_(nearThresh),
    awayThreshold_(awayThresh),
    dPhiMin_(dPhiMin),
    evtJetMul_(0),
    ldjAwayJetMul_(0),
    foundDijet_(false),
    verbosity_(0)
  { 
    // empty
  }

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

    // initialize counters/records
    double max = 0;

    // loop over away candidates
    for (InputCollection::iterator icand=others.begin(); icand!=others.end(); ++icand) {
      if (verbosity_>=3)
	cout << "processing: " << *icand << endl;
      if (!PassAwayJetCriterion(*icand))
	continue;
      double dphi=absDPhi(near,*icand);
      // check away side jet mul within dphi strip for the first time away sides candidates are available
      if (!foundDijet_ && dphi>dPhiMin_) {
	++ldjAwayJetMul_;
	if (verbosity_>=3) cout << "first away jet within dphi (" << dPhiMin_ << ") of now lead jet. ldjAwayJetMul_ is now: " << ldjAwayJetMul_ << endl;
      }
      // find best away jet
      if (dphi>dPhiMin_ && max<dphi)
	it_away = icand;
    }

    // done print some info
    if (verbosity_>=2) {
      if (it_away!=NULL) cout << "found away: " << *it_away << endl << endl;
      else cout << endl;
    }
    return it_away;
  }
  //  Run the algorithm
  //  ------------------
  int HiDiJetAlgorithm::Group(InputCollection& input, OutputCollection* output)
  {
    if (!output) return -1;
    // record jet event mul
    evtJetMul_=input.size();
    // need at least 2 input items
    if (input.size()<2) return -1;
    // print out of entire input
    if (verbosity_==1) {
      cout << "group algo input items: " << endl;
      mystd::print_elements(input);
    }

    // loop over the input collection as long as it has more than 2 items.
    while (input.size()>=2) {
      // find leading jet
      InputCollection::iterator imaxPt = max_element(input.begin(),input.end(),lessPt);
      // check leading jet, if does not pass near jet criterion then nothing
      // can be done for this event
      if (!PassNearJetCriterion(*imaxPt)) break;

      // Will now run algorithm
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
      // jet, save to dijet and remove from list of candidates
      if (iaway!=NULL) {
	dj.SetAwayJet(*iaway);
	input.erase(iaway);
	foundDijet_=true;

	// save paired dijet with some quality info
	dj.leadAwayMul_= ldjAwayJetMul_;
	output->push_back(dj);
      }
    } // end of loop over input items

    // done. print out details if wanted
    if (verbosity_>=1) {
      if (output->size()>0) cout << "lead dijet awayside mul: " << ldjAwayJetMul_ << endl;
      mystd::print_elements(*output);
    }
    return ldjAwayJetMul_;
  }

} //jetana
