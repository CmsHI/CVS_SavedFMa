#ifndef HLTPixelActivityFilter_h
#define HLTPixelActivityFilter_h

#include "HLTrigger/HLTcore/interface/HLTFilter.h"

//
// class declaration
//

class HLTPixelActivityFilter : public HLTFilter {

   public:
      explicit HLTPixelActivityFilter(const edm::ParameterSet&);
      ~HLTPixelActivityFilter();
      virtual bool filter(edm::Event&, const edm::EventSetup&);

   private:
      edm::InputTag pixelTag_;  // input tag identifying product containing pixel clusters
      unsigned int min_clusters_;   // minimum number of clusters
};

#endif //HLTPixelActivityFilter_h
