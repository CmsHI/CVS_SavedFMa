// $Id: createHydjet.C 3010 2006-07-06 11:04:54Z veres $

/* 
   This macro generates a HYDJET HRT tree

   Input:
   ULong_t n: number of events to generate
   ULong_t runno: run number to use
   Float_t bmin:  min. impact parameter
   Float_t bmax:  max. impact parameter
   Bool_t jets:   =true generate jets instead of photons
   Bool_t index:  =true also one-to-one index will be generated
*/   

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <THIDebug.h>
#include <THIConstIndexGenerator.h>
#include <THIHydjet.h>
#include <THIPythia.h>
#include "createHRIT.C"
#endif

void createPythiaDijet30(ULong_t n = 10000, ULong_t runno = 666, 
		     Bool_t index=kFALSE, Float_t energy = 4000)
{
   gDebugMask = THIDebug::kTreeWriting;
   gDebugLevel = 1; //Increase this if needed.
   
   THIPythia h(energy);
   time_t curtime;
   time(&curtime);
   UInt_t myseed=(UInt_t) curtime;
   h.SetWarmupRuns(1000);
   h.SetSeed(myseed,kTRUE);

   h.SetPtHardMin(30);
   h.SetPtHardMax(40); 

   h.SetMSEL(0);        // = 0 for user specification of sub-processes   

   h.SetMSUB(11,1);  // q+q->q+q
   h.SetMSUB(12,1);  // q+qbar->q+qbar
   h.SetMSUB(13,1);  // q+qbar->g+g
   h.SetMSUB(28,1);  // q+g->q+g
   h.SetMSUB(53,1);  // g+g->q+qbar
   h.SetMSUB(68,1);  // g+g->g+g

   h.SetMSTP(2,1);      //which order running alphaS
   h.SetMSTP(33,0);     //unclusion of k factor in cross section
   h.SetMSTP(51,7);     //structure function chosen - CTEQ5M pdf
   h.SetMSTP(81,0);     //multiple parton interactions 1 is Pythia default
   h.SetMSTP(82,4);     //Defines the multi-parton model
   
   h.SetPARP(67,1.);    //amount of initial-state radiation
   h.SetPARP(82,1.9);   //pt cutoff for multiparton interactions
   h.SetPARP(83,0.5);   //fraction of total hadr. matter in core
   h.SetPARP(84,0.4);   //radius of core
   h.SetPARP(85,0.33);  //gluon prod. mechanism in MI
   h.SetPARP(86,0.66);  //gluon prod. mechanism in MI
   h.SetPARP(87,0.7);
   h.SetPARP(88,0.5);
   h.SetPARP(89,1000.); //sqrt(s) for which PARP82 is set
   h.SetPARP(90,0.16);
   h.SetPARP(91,1.0);   //kt distribution
   h.SetPARJ(71,10.);   //for which ctau  10 mm
   
   h.SetMSTJ(11,3);     //Choice of the fragmentation function
   h.SetMSTJ(22,2);     //Decay those unstable particles
   
   h.SetMSTU(21,1);     //Check on possible errors during program execution
   
   h.SetBottomMass(4.8);  //(PYTHIA value)
   h.SetTopMass(175.0);   //(PYTHIA value)
 
   h.Init();

   THIIndexGenerator *igen = 0;
   if(index)
      igen = new THIConstIndexGenerator(1);

   createHRIT(&h,igen,n,runno);

   delete igen;

}
