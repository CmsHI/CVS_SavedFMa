{
  TCanvas* c = new TCanvas("c","",500,500);
  TH2D* h = new TH2D("h","",200,0,5,200,0,3);
  h->Draw();

  TGraph *graph = new TGraph(5);
  graph->SetName("");
  graph->SetTitle("");
  graph->SetFillColor(90);
  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(1.3);

  int ient = 0;
  graph->SetPoint(ient,0,0.827905485);  ient++;
  graph->SetPoint(ient,0,2.321370321);   ient++;
  graph->SetPoint(ient,0.5,2.321370321);   ient++;
  graph->SetPoint(ient,0.5,1.431080934); ient++;
  graph->SetPoint(ient,1,1.431080934);  ient++;
  graph->SetPoint(ient,1,1.187297662);  ient++;
  graph->SetPoint(ient,1.5,1.187297662);  ient++;
  graph->SetPoint(ient,1.5,1.123097754);  ient++;
  graph->SetPoint(ient,2,1.123097754);  ient++;
  graph->SetPoint(ient,2,1.022374646);  ient++;
  graph->SetPoint(ient,2.5,1.022374646);  ient++;
  graph->SetPoint(ient,2.5,1.141584158);  ient++;
  graph->SetPoint(ient,3,1.141584158);  ient++;
  graph->SetPoint(ient,3,1.152100906);  ient++;
  graph->SetPoint(ient,3.5,1.152100906);  ient++;
  graph->SetPoint(ient,3.5,1.260215016);  ient++;
  graph->SetPoint(ient,4,1.260215016);  ient++;
  graph->SetPoint(ient,4,1.865199831); ient++;
  graph->SetPoint(ient,4.5,1.865199831); ient++;
  // half way!
  graph->SetPoint(ient,4.5,0.6033820821); ient++;
  graph->SetPoint(ient,4,0.6033820821); ient++;
  graph->SetPoint(ient,4,0.62840846);   ient++;
  graph->SetPoint(ient,3.5,0.62840846); ient++;
  graph->SetPoint(ient,3.5,0.7333786452); ient++;
  graph->SetPoint(ient,3,0.7333786452); ient++;
  graph->SetPoint(ient,3,0.8043352398); ient++;
  graph->SetPoint(ient,2.5,0.8043352398); ient++;
  graph->SetPoint(ient,2.5,0.7257791577); ient++;
  graph->SetPoint(ient,2,0.7257791577); ient++;
  graph->SetPoint(ient,2,0.7548269812); ient++;
  graph->SetPoint(ient,1.5,0.7548269812); ient++;
  graph->SetPoint(ient,1.5,0.690144622); ient++;
  graph->SetPoint(ient,1,0.690144622); ient++;
  graph->SetPoint(ient,1,0.6262867487); ient++;
  graph->SetPoint(ient,0.5,0.6262867487); ient++;
  graph->SetPoint(ient,0.5,0.827905485); ient++;
  graph->SetPoint(ient,0,0.827905485); ient++;

  
  
  graph->Draw("l");

}
