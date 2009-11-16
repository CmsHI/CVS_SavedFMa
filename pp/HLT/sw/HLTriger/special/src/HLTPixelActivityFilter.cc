#include "HLTrigger/special/interface/HLTPixelActivityFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

//
// constructors and destructor
//
 
HLTPixelActivityFilter::HLTPixelActivityFilter(const edm::ParameterSet& iConfig) :
    pixelTag_ (iConfig.getParameter<edm::InputTag>("pixelTag")),
    min_clusters_ (iConfig.getParameter<unsigned int>("minClusters"))
{
  LogDebug("") << "Using the " << pixelTag_ << " input collection ";
  LogDebug("") << "Requesting " << min_clusters_ << " clusters";

   //register your products
  produces<trigger::TriggerFilterObjectWithRefs>();
}

HLTPixelActivityFilter::~HLTPixelActivityFilter()
{
}

//
// member functions
//

// ------------ method called to produce the data  ------------
bool HLTPixelActivityFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace std;
   using namespace edm;
   using namespace reco;
   using namespace trigger;

   // All HLT filters must create and fill an HLT filter object,
   // recording any reconstructed physics objects satisfying (or not)
   // this HLT filter, and place it in the Event.



   // Specific filter code

   // get hold of products from Event

   Handle< edmNew::DetSetVector<SiPixelCluster> > clusterColl;
   iEvent.getByLabel(pixelTag_,clusterColl);

   bool accept = false;
   unsigned int clusterSize = clusterColl.product()->size();
   if(clusterSize > min_clusters_)
     accept = true ;


   // The filter object
   auto_ptr<TriggerFilterObjectWithRefs> filterobject (new TriggerFilterObjectWithRefs(path(),module()));
/*
   if(accept) {
     for(unsigned int i=0; i < clusterSize; i++) {
//       filterobject->addObject(TriggerCluster,SiPixelCluster(clusterColl,i));
//       SiPixelClusterRef ref(SiPixelClusterRef(clusterColl,i));
       SiPixelClusterRef ref(SiPixelClusterRef(clusterColl));
       filterobject->addObject(TriggerCluster,ref);
//       filterobject->addObject(TriggerCluster,SiPixelClusterRef(clusterColl,i));
     }
   }
   // put filter object into the Event
   iEvent.put(filterobject);
*/

   LogDebug("") << "Number of clusters accepted:" << clusterSize;

   // return with final filter decision
   return accept;

}
