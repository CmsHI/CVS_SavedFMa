//////////////////////////////////////////////////////////
//
// Class to print rates
//
//////////////////////////////////////////////////////////

#ifndef OHltRatePrinter_h
#define OHltRatePrinter_h

#include <vector>
#include "OHltMenu.h"
#include "OHltConfig.h"
#include "OHltTree.h"

using namespace std;

class OHltRatePrinter {
 public:
  
  OHltRatePrinter(){};
  OHltRatePrinter(OHltConfig *cfg, OHltMenu *menu);
  virtual ~OHltRatePrinter(){};

  void SetupAll(vector<float> Rate,vector<float> RateErr,vector<float> spureRate,
		vector<float> spureRateErr,vector<float> pureRate,
		vector<float> pureRateErr,vector< vector<float> >coMa,
		vector< vector<float> > RatePerLS,vector<int> tRunID,vector<int> tLumiSection,
		vector<float> tTotalRatePerLS);
  void ReorderRunLS();

  void printRatesASCII(OHltConfig *cfg,OHltMenu *menu);
  void printCorrelationASCII();
  
  void printRatesTex(OHltConfig *cfg, OHltMenu *menu);
  void printHltRatesTex(OHltConfig *cfg, OHltMenu *menu);
  void printL1RatesTex(OHltConfig *cfg, OHltMenu *menu);
  void printRatesTwiki(OHltConfig *cfg, OHltMenu *menu); 
  void printHltRatesTwiki(OHltConfig *cfg, OHltMenu *menu); 
  void printL1RatesTwiki(OHltConfig *cfg, OHltMenu *menu); 
  void printHltRatesBocci(OHltConfig *cfg, OHltMenu *menu);
  void writeHistos(OHltConfig *cfg, OHltMenu *menu);
  TString GetFileName(OHltConfig *cfg, OHltMenu *menu);
  void printPrescalesCfg(OHltConfig *cfg, OHltMenu *menu);
  void printHLTDatasets(OHltConfig *cfg, OHltMenu *menu
  		, HLTDatasets &hltDatasets
  		, TString   &fullPathTableName
  		, const Int_t     significantDigits);
  int ivecMax(vector<int> ivec);
  int ivecMin(vector<int> ivec);

  vector<float> Rate;
  vector<float> RateErr;
  vector<float> spureRate;
  vector<float> spureRateErr;
  vector<float> pureRate;
  vector<float> pureRateErr;
  vector< vector<float> >coMa;

  // output
  TFile * fr_;

  vector< vector<float> > RatePerLS;
  vector<float> totalRatePerLS;
  vector<int> runID;
  vector<int> lumiSection;

};

#endif