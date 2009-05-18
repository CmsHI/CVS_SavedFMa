// $Id: createHRIT.C 2837 2006-05-28 20:57:16Z loizides $

/* 
   This macro generates a HRT with index tree

   Input:
   THIGenerator *gen:  configured generator object
   THIGenerator *igen: configured index generator object
   ULong_t n: number of events to generate
   ULong_t runno: run number to use
*/   

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <THIDebug.h>
#include <THIGenerator.h>
#include <THIIndexGenerator.h>
#include <THITreeWriter.h>
#include <TSystem.h>
#endif

//________________________________________________________________________
void createHRIT(THIGenerator *gen, THIIndexGenerator *igen, ULong_t n, ULong_t runno, TString pref = "hrt")
{
   gDebugMask = THIDebug::kTreeWriting;
   gDebugLevel = 1; //Increase this if needed.

   if(!gen) {
      cerr << "Generator must be instantiated!" << endl;
      return;
   }

   THITreeWriter tw(gen,igen);
   if(gSystem->Getenv("HIROOT_TREEPREFIX")) {
      TString pref=gSystem->Getenv("HIROOT_TREEPREFIX");
      tw.SetNamePrefix(Form("%s_hrt",pref.Data()));
      tw.SetIndexNamePrefix(Form("%s_hrit",pref.Data()));
   }
   tw.SetNamePrefix(Form("%s_hrt",pref.Data()));
   tw.SetIndexNamePrefix(Form("%s_hrit",pref.Data()));

   tw.Generate(runno,n);
}
