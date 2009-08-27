// $Id: rootlogon.C 2637 2006-03-06 23:53:38Z loizides $

{
   gROOT->Macro("$HIROOT/macros/setRootEnv.C+");

   gROOT->ProcessLine(".L ana/runAnaGeneric.C+");
}
