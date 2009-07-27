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
	 TString tNPtnEt_;
	 TString tAPtnEt_;
	 TString tDPhi_;
	 TString tNJEta_;
	 TString tAJEta_;
	 // particle level
	 TString tPPt_;
	 TString tNJPDR_;
	 TString tAJPDR_;

	 TreeData();
   };

   // constructor
   TreeData::TreeData() :
      // tree branch names
      tNJEt_("nljet"),
      tAJEt_("aljet"),
      tNPtnEt_("nlpet"),
      tAPtnEt_("alpet"),
      tDPhi_("jdphi"),
      tNJEta_("nljeta"),
      tAJEta_("aljeta"),
      tPPt_("ppt"),
      tNJPDR_("pndr"),
      tAJPDR_("padr")
   {
   }
}

#endif // TREEDATA_H
