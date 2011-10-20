class Frac
{
public:
  Frac(float t) : total(t) {}
  GetFrac(float num)
  {
    printf("frac: %.1f +- %.1f\% \n",num/total*100,sqrt(num)/total*100);
  }
  
  float total;
};
