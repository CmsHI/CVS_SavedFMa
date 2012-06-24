void DrawTick(Double_t Y, Double_t Erru, Double_t Errd, Double_t X, Double_t Tick_x=0.012, Double_t Tick_y=1.8, Int_t TickColor=1)
{
  TLine *line;
  line = new TLine(X-Tick_y,Y+Erru,X-Tick_y,Y+Erru-Tick_x);
  line->SetLineColor(TickColor);
  line->Draw();
  line = new TLine(X+Tick_y,Y+Erru,X+Tick_y,Y+Erru-Tick_x);
  line->SetLineColor(TickColor);
  line->Draw();
  line = new TLine(X-Tick_y,Y+Erru,X+Tick_y,Y+Erru);
  line->SetLineColor(TickColor);
  line->Draw();
  
  line = new TLine(X-Tick_y,Y-Errd,X-Tick_y,Y-Errd+Tick_x);
  line->SetLineColor(TickColor);
  line->Draw();
  line = new TLine(X+Tick_y,Y-Errd,X+Tick_y,Y-Errd+Tick_x);
  line->SetLineColor(TickColor);
  line->Draw();
  line = new TLine(X-Tick_y,Y-Errd,X+Tick_y,Y-Errd);
  line->SetLineColor(TickColor);
  line->Draw();
}
