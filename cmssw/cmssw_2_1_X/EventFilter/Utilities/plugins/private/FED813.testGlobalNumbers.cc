/** \file
 * 
 * 
 * $Date: 2008/06/19 09:36:15 $
 * $Revision: 1.1 $
 * \author N. Amapane - S. Argiro'
 *
*/

#include <FWCore/Framework/interface/MakerMacros.h>
#include <FWCore/Framework/interface/EDAnalyzer.h>
#include <FWCore/Framework/interface/Event.h>
#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h>
#include <DataFormats/FEDRawData/interface/FEDHeader.h>
#include <DataFormats/FEDRawData/interface/FEDTrailer.h>
#include <DataFormats/FEDRawData/interface/FEDNumbering.h>

#include "EventFilter/Utilities/interface/GlobalEventNumber.h"

#include <iostream>
#include <iomanip>

using namespace edm;
using namespace std;

namespace test{

  static const unsigned int GTEVMId= 813;
  class GlobalNumbersAnalysis: public EDAnalyzer{
  private:
  public:
    GlobalNumbersAnalysis(const ParameterSet& pset){
    }

 
    void analyze(const Event & e, const EventSetup& c){
      cout << "--- Run: " << e.id().run()
	   << " Event: " << e.id().event() << endl;
      Handle<FEDRawDataCollection> rawdata;
      e.getByType(rawdata);
      const FEDRawData& data = rawdata->FEDData(GTEVMId);
      size_t size=data.size();

      if (size>0 ) {
	  cout << "FED# " << setw(4) << GTEVMId << " " << setw(8) << size << " bytes " << endl;
	  if(evf::evtn::daq_board_sense(data.data()))
	    {
	      cout << "FED# " << setw(4) << GTEVMId << " is the real GT DAQ block " << endl;
	      cout << "Event # " << evf::evtn::get(data.data(),false) << endl;
	    }
	  }

// 	  CPPUNIT_ASSERT(trailer.check()==true);
// 	  CPPUNIT_ASSERT(trailer.lenght()==(int)data.size()/8);
    }
  };
DEFINE_FWK_MODULE(GlobalNumbersAnalysis);
}

