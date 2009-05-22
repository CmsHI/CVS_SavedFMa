// $Id: runJFPythia.C 3420 2006-10-12 13:46:41Z loizides $

/* 
   This macro demonstrates the usage of TAM. It runs on 3 modes
   that require a different inputs. Modes 0 and 1 run on files with
   HRT trees produced by THIPythia or some other generator. Mode 2
   runs on files with HRT that include the jets in a "CaloJets" branch.
   If ran with mode 1, THITreeWriterMod produces a tree that can be used 
   for mode 2. It will produce various histograms as output.

   Input:
   const char* hrts: file name(s) of data trees.
   Int_t mode: if 0 runs without loading from branch instead gets 
                    jets from TAModule THICaloJetFinder
               if 1 runs like 0 but uses TreeWriterMod- saves an 
                    output hrt with jet to hrtjetoutput*.root.
               if 2 runs by loading branch to get jets.
   const char *outfile: name of the output file.
   const char *myjetsname: name of the jet arrays array container
   Bool_t useProof: true if analysis to be run with PROOF.
*/   

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <THIDebug.h>
#include <THIIterativeConeFinder.h>
#include <THICreateJetComponentsMod.h>
#include <THICaloJetFinderMod.h>
#include <THITreeWriterMod.h>
#include <THIJetAnaMod.h>
#include <THIAnalysis.h>
#include <THIExampleAnaMod.h>
#include <THIDiJetTruthAnaMod.h>
#include "ana/runAnaGeneric.C"
#endif

void runJFPythia(Int_t rnum,
                 const char * genIn,
                 const char *hrts, 
                 Int_t mode = 0,
		 const char *outfile="jfpythia_output.root",
                 const char *myjetsname="myjets",
                 Bool_t useProof=kFALSE)
{
   gDebugMask = THIDebug::kAnalysis;
   gDebugLevel = 0; //Increase this if needed.
   
   // set up top-level analysis class
   THIAnalysis *ana = new THIAnalysis(useProof);
   ana->AddFile(hrts);          
   ana->SetAnaOutputName(outfile); 

   if (mode <= 1) {
      // setup the jet finder
      THIIterativeConeFinder *jf= new THIIterativeConeFinder();
      jf->SetRadius(0.7);
      jf->SetJetTh(3.);
      jf->SetSeedTh(1.);
   
      THICreateJetComponentsMod *createComponentsMod = new
         THICreateJetComponentsMod("createJC", "Create Jet Components Module");
      createComponentsMod->SetDoSubEvents(kFALSE);
      createComponentsMod->SetJCName("myjetcomps");

      THICaloJetFinderMod  *findJetsMod = new 
         THICaloJetFinderMod(jf, 0, "findJets", "Jet Finder Module");
      findJetsMod->SetJCName(createComponentsMod->GetJCName());
      findJetsMod->SetJetName(myjetsname);
      findJetsMod->SetJetEventName(Form("%s_event",myjetsname));
      createComponentsMod->Add(findJetsMod);
   
      THIJetAnaMod *anaJetsMod = new 
         THIJetAnaMod(kFALSE, "anaJets", "Jet Finder Analysis Module");
      anaJetsMod->SetJetName(findJetsMod->GetJetName());
      findJetsMod->Add(anaJetsMod);

      // add test
      THIDiJetTruthAnaMod *anaDijetFFMod = new 
         THIDiJetTruthAnaMod("anaJets", "Jet Finder Analysis Module");
      anaDijetFFMod->SetJetName(findJetsMod->GetJetName());
//      anaDijetFFMod->SetJetDeltaRMatch(TMath::Pi()/4);
      anaDijetFFMod->SetJetDeltaRMatch(TMath::Pi()/3);
      findJetsMod->Add(anaDijetFFMod);

      ana->SetAnaOutputName(Form("%s/%s_dijet_%06d.root",getenv("HIROOT_BASEURL"),genIn,rnum));

      if(mode == 1) {
         THITreeWriterMod *twMod = new THITreeWriterMod("treeewrite", "Tree Writing Module");
         twMod->SetEventName(findJetsMod->GetJetEventName());
         twMod->SetPrefix("hrtjetout");
         findJetsMod->Add(twMod);
      }

      ana->SetSuperModule(createComponentsMod);

   } else { 

      THIJetAnaMod *anaJetsMod = new 
	 THIJetAnaMod(kTRUE, "anaJets", "Jet Finder Analysis Module");
      ana->SetSuperModule(anaJetsMod);
      anaJetsMod->SetJetName(myjetsname);
   }

   // run generic analysis macro
   runAnaGeneric(ana);
}
