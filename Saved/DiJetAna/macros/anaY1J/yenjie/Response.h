
#include "TF1.h"


TF1* responseLead[5];
TF1* resolutionLead[5];

TF1* responseAway[5];
TF1* resolutionAway[5];

void initializeResponse(){


   for(int i = 0; i < 5; ++i){

      responseLead[i] = new TF1(Form("responseLead_%d",i),"[0]+[1]/x",50,200);
      responseAway[i] = new TF1(Form("responseLead_%d",i),"[0]+[1]/x",50,200);
      resolutionLead[i] = new TF1(Form("resolutionLead_%d",i),"sqrt(pow([0],2)+pow([1]/sqrt(x),2)+pow([2]/x,2))",0,400);
      resolutionAway[i] = new TF1(Form("resolutionAway_%d",i),"sqrt(pow([0],2)+pow([1]/sqrt(x),2)+pow([2]/x,2))",0,400);

   }

      responseLead[0]->SetParameter(0,0.849386);
      responseLead[0]->SetParameter(1,14.225168);
      responseAway[0]->SetParameter(0,0.870845);
      responseAway[0]->SetParameter(1,11.685759);

      resolutionLead[0]->SetParameter(0,0.121613);
      resolutionLead[0]->SetParameter(1,0.000002);
      resolutionLead[0]->SetParameter(2,11.043124);

      resolutionAway[0]->SetParameter(0,0.125431);
      resolutionAway[0]->SetParameter(1,0.000017);
      resolutionAway[0]->SetParameter(2,10.826459);


 
      responseLead[1]->SetParameter(0,0.887987);
      responseLead[1]->SetParameter(1,9.277797);
      responseAway[1]->SetParameter(0,0.890609);
      responseAway[1]->SetParameter(1,8.988989);
      resolutionLead[1]->SetParameter(0,0.118642);
      resolutionLead[1]->SetParameter(1,0.000006);
      resolutionLead[1]->SetParameter(2,10.294323);
      resolutionAway[1]->SetParameter(0,0.134175);
      resolutionAway[1]->SetParameter(1,-0.000010);
      resolutionAway[1]->SetParameter(2,9.386174);


      responseLead[2]->SetParameter(0,0.899135);
      responseLead[2]->SetParameter(1,7.657971);
      responseAway[2]->SetParameter(0,0.895807);
      responseAway[2]->SetParameter(1,7.138731);
      resolutionLead[2]->SetParameter(0,0.050643);
      resolutionLead[2]->SetParameter(1,1.379138);
      resolutionLead[2]->SetParameter(2,4.243481);
      resolutionAway[2]->SetParameter(0,0.062224);
      resolutionAway[2]->SetParameter(1,1.428237);
      resolutionAway[2]->SetParameter(2,0.000041);


      responseLead[3]->SetParameter(0,0.921483);
      responseLead[3]->SetParameter(1,5.084390);
      responseAway[3]->SetParameter(0,0.919994);
      responseAway[3]->SetParameter(1,4.437062);
      resolutionLead[3]->SetParameter(0,0.059450);
      resolutionLead[3]->SetParameter(1,1.361898);
      resolutionLead[3]->SetParameter(2,0.000007);
      resolutionAway[3]->SetParameter(0,0.036663);
      resolutionAway[3]->SetParameter(1,1.397659);
      resolutionAway[3]->SetParameter(2,0.000007);


      responseLead[4]->SetParameter(0,0.952637);
      responseLead[4]->SetParameter(1,1.444436);
      responseAway[4]->SetParameter(0,0.965141);
      responseAway[4]->SetParameter(1,-0.195233);
      resolutionLead[4]->SetParameter(0,0.071335);
      resolutionLead[4]->SetParameter(1,1.176171);
      resolutionLead[4]->SetParameter(2,0.000005);
      resolutionAway[4]->SetParameter(0,0.095181);
      resolutionAway[4]->SetParameter(1,1.059669);
      resolutionAway[4]->SetParameter(2,-0.000002);

}












