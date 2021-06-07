int n;
int calc(int x){
  if (x>10)
    n=4*x;
  else
    n=++x;
  return n;
}