#ifndef HiJetAna_DiJet_h
#define HiJetAna_DiJet_h

//
// based on http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/RecoJets/JetAlgorithms/interface/ProtoJet.h?hideattic=0&view=markup
//

#include <iostream>
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaPhi.h"

class DiJet
{
  public:
    typedef math::XYZTLorentzVector LorentzVector;

    DiJet();
    void SetNearJet(LorentzVector nj) {nj_ = nj;};
    void SetAwayJet(LorentzVector aj) {
      aj_ = aj;
      cm_ = nj_+aj_;
      dphi_ = reco::deltaPhi(aj_.phi(), nj_.phi());
    };

    // Standard quantities derived from the dijet Lorentz vector
    /** Returns the modulus of the momentum of the dijet */
    double p() const {return cm_.P();}
    /** Returns the transverse momentum of the dijet*/
    double pt() const {return cm_.Pt();}
    /** Returns the transverse energy of the dijet*/
    double et() const {return cm_.Et();}
    /** Returns the dijet mass of the dijet*/
    double m() const {return cm_.M();}
    /** Returns the azimuthal angle of the dijet, Phi*/
    double phi() const {return cm_.Phi();}
    /** Returns the pseudorapidity of the dijet*/
    double eta() const {return cm_.Eta();}
    /** Returns the rapidity of the dijet*/
    double y() const {return p() > 0 ? cm_.Rapidity() : 0;}

    // Freind Functions
    friend std::ostream& operator <<(std::ostream& outs, const DiJet& dj);


  private:
    // Near jet
    LorentzVector nj_;
    // Away jet
    LorentzVector aj_;
    // dijet properties
    double dphi_;
    LorentzVector cm_;
};

#endif //HiJetAna_DiJet_h
