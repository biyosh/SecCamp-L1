#include<stdio.h>
#include<time.h>

/*x^y mod zを求める*/
int modPow(int x, int y, int z) {  
  long long int ret = 1;
  long long int mul = x;
  while(y > 0){
    if((y & 1)== 1){
      ret = ret * mul % z;
    }
    mul = mul * mul % z;
    y >>= 1;
  }
  return ret;
}

int fullSearch(int g, int y, int p){
  for(int i = 0; i < p; i++){
    if(y == modPow(g,i,p)){
      return i;
    }
  }
  return -1;
}

int main() {
  int a = 23631;
  int x = 55163;
  int p = 57223;
  int y = modPow(a,x,p);

  int attackX, M;
  long cpuTime;
  double sec;

  printf("【公開情報】(a,y,p)=(%d,%d,%d)\n",a,y,p);
  printf("＝＝＝＝＝＝＝＝＝＝＝＝＝＝\n");

  attackX = fullSearch(a,y,p);
  printf("推定鍵attackX＝%d\n", attackX);
  printf("y(=a^x mod p)=%d\n",y);
  printf("y(=a^attackX mod p)=%d\n",y);

  if(y == modPow(a, attackX, p)){
    printf("解読成功\n");
  }
  else{
    printf("解読失敗…\n");
  }
  printf("＝＝＝＝＝＝＝＝＝＝＝＝＝＝\n");

  cpuTime = clock();
  sec = (double)cpuTime / CLOCKS_PER_SEC;
  printf("実行時間は%f秒でした\n",sec);
  return 0;
}