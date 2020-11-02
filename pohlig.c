#include<stdio.h>
#include<math.h>
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

/*numの素因数の数を数える*/
int countFactors(int num, int sqrtnum){
  int count = 0;

  while (num % 2 == 0){ // 偶数の素数は2のみ
    count++;
    num /= 2;
  }
  for (int i = 3; i <= sqrtnum; i+=2){ // 他の奇数で割っていく
    while (num % i == 0){
      count++;
      num /= i;
    }
  }
  return count;
}

/*numを素因数分解する*/
int findFactors(int num, int sqrtnum){
  while (num % 2 == 0){ // 偶数の素数は2のみ
    return 2;
    num /= 2;
  }

  for (int i = 3; i <= sqrtnum; i+=2){ 
    while (num % i == 0){
      return i;
      num /= i;
    }
  }
  return 0;
}

/* x mod qを求める*/
int pohligHellman(int p, int q,int y,int g){
  int n = (p - 1)/q;
  int i_n;
  int i = 0;
  int y_n = modPow(y,n,p);
  printf("y_n=%d\n",y_n);
  while(i < q){
    i_n = modPow(g,n * i,p);
    if(y_n == i_n){
      break;
    }
    i++;
  }
  return i;
}

/* 全てのmの積を返す */
int prod_array(const int a[], int size){
  int i, res;
  res = 1;
  for ( i = 0; i < size; i++ ) { 
    res *= a[i]; 
  }
  return res;
}

/*a^{-1} mod p を求める*/
int invMod(int a, int p){
  return modPow(a,p - 2,p);
}

/* 中国剰余定理  https://yoshiiz.blog.fc2.com/blog-entry-417.html を参考 */
void chrem(const int a[], const int m[], int size, int *x, int *M ){
  int M_i, t_i, i;

  *M = prod_array(m, size);
  *x = 0;
  for( i = 0; i < size; i++) {
    M_i = *M / m[i];
    t_i = invMod( M_i, m[i] );
    *x =  modPow(*x + a[i] * M_i * t_i, 1, *M);
  }
}

int main() {
  int a = 24680;
  int x = 45537;
  int p = 64439;
  int y = modPow(a,x,p);
  int size = countFactors(p - 1,sqrt(p - 1));
  int factor[size];
  int pohlig[size];
  int attackX, M;
  long cpuTime;
  double sec;

  printf("size=%d\n",size);

  factor[0] = findFactors(p - 1,sqrt(p - 1));
  int qq = factor[0];
  
  for(int i = 1; i < size; i++){
    factor[i]=findFactors((p-1)/qq,sqrt(p - 1));
    qq *= factor[i];
  }
  
  for(int i = 0; i < size; i++){
    pohlig[i] = pohligHellman(p,factor[i],y,a);
    printf("i=%d\n",pohlig[i]);
  }

  chrem(pohlig, factor, size, &attackX, &M );
  printf("推定鍵＝%d\n", attackX);

  if(y == modPow(a, attackX, p)){
    printf("解読成功\n");
  }
  else{
    printf("解読失敗…\n");
  }
  
  cpuTime = clock();
  sec = (double)cpuTime / CLOCKS_PER_SEC;
  printf("実行時間は%f秒でした\n",sec);
  return 0;
}