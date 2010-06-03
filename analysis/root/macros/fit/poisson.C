//
// - cf
//   * http://root.cern.ch/root/html522/TH1.html
//   * http://root.cern.ch/root/html/TMath.html#TMath:Poisson
//   * http://root.cern.ch/root/roottalk/roottalk03/0205.html
// 
Double_t poissonf(Double_t*x,Double_t*par) 
{                                                                              
  return par[0]*TMath::Poisson(x[0],par[1]);
}                                                                              

void poisson()
{                                      
  //TF1 * pois = new TF1("pois",poissonf,0,10,2); // x in [0;10], 2 parameters 
  TF1 * pois = new TF1("pois","[0]*TMath::Poisson(x,[1])",0,10); // x in [0;10], 2 parameters 

  pois->SetParName(0,"Const");                                                
  pois->SetParName(1,"#mu");                                                  
  // Create histogram with poisson distribution                               
  TH1D*testhi = new TH1D("testhi","Poisson distribution",100,0,5);            
  pois->SetParameter(0,3.75654);                                              
  pois->SetParameter(1,2.95437);                                              
  testhi->FillRandom("pois",20000);                                           

  // Fitting it                                                               
  pois->SetParameter(0,1); // not the best shots                             
  // 'cause we fill with 20000 events                
  // so constant will be in 1000 times bigger        
  pois->SetParameter(1,1); // :)                                             
  TCanvas *c1 = new TCanvas("c1","Canvas",600,600);                           

  gStyle->SetOptStat(0);                                                      
  gStyle->SetOptFit(111);

  testhi->Fit("pois");                                                        

  testhi->Draw();                                                             
}

