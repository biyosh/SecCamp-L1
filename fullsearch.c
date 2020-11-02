#include<stdio.h>
#include<time.h>

/*x^y mod zを求める*/
int modPow(int x, int y, int z) {  
  long long int ret = 1;
  long long int mul = x;
  while(y > 0){
    if((y & 1)== 1)
      ret = ret * mul % z;
    mul = mul * mul % z;
    y >>= 1;
  }
  return ret;
}

int fullSearch(int g, int y, int p){
  for(int i = 0; i < p; i++){
    if(y == modPow(g,i,p))
      return i;
  }
  return -1;
}

int main() {
  int a = 24680;
  int x = 45537;
  int p = 64439;
  int y = modPow(a,x,p);

  int attackX, M;
  long cpuTime;
  double sec;

  attackX = fullSearch(a,y,p);
  printf("推定鍵＝%d\n",attackX);

  if(y == modPow(a, attackX, p))
    printf("解読成功\n");
  cpuTime = clock();
  sec = (double)cpuTime / CLOCKS_PER_SEC;
  printf("実行時間は%f秒でした\n",sec);
  return 0;
}