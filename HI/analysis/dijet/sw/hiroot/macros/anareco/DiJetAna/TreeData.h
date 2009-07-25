#ifndef TREEDATA_H
#define TREEDATA_H
namespace DiJetAna
{
   //
   // DiJet ana Trees Data definition
   //
   class TreeData
   {
      public:
	 // tree branch names
	 // jet level
	 TString tNJEt_;
	 TString tAJEt_;
	 TString tDPhi_;
	 TString tNJEta_;
	 TString tAJEta_;
	 // particle level
	 TString tPPt_;
	 TString tNJPDR2_;
	 TString tAJPDR2_;

	 TreeData();
   };

   // constructor
   TreeData::TreeData() :
      // tree branch names
      tNJEt_("nljet"),
      tAJEt_("aljet"),
      tDPhi_("jdphi"),
      tNJEta_("nljeta"),
      tAJEta_("aljeta"),
      tPPt_("ppt"),
      tNJPDR2_("pndr"),
      tAJPDR2_("padr")
   {
   }
}

#endif // TREEDATA_H
