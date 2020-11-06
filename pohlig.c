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
  int count=0;
  if(num % 2 == 0){
    while (num % 2 == 0){ // 偶数の素数は2のみ
      num /= 2;
    }
    count++;
  }

  for (int i = 3; i <= sqrtnum; i+=2){ 
    if(num % i == 0){
      while (num % i == 0){ // 3以上の奇数でわる
        num /= i;
      }
      count++;
    }
  }
  return count;
}

/*numを素因数分解する*/
int findFactors(int num, int sqrtnum, int* yoso){
  int count = 0;
  if(num % 2 == 0){
    while (num % 2 == 0){ 
      *yoso = count +1;
      count += 1;
      num /= 2;
    }
    return 2;
  }

  for (int i = 3; i <= num; i+=2){ 
    *yoso = 0;
    int count = 0;
    if(num % i == 0){
      while (num % i == 0){
        *yoso =count + 1;
        count += 1;
        num /= i;
      }
      return i;
    }
  }
  return 0;
}

/*拡張ユークリッド互除法を用いてa^{-1} mod p を求める*/
int invMod(int a, int p){
  int b, x, u, q, tmp; 

  b = p; x = 1; u = 0; 
  while ( b > 0 ) {
    q = a / b; 
    tmp = u; 
    u = x - q * u; 
    x = tmp;
    tmp = b; 
    b = a - q * b; 
    a = tmp;
  }
  return ( x < 0 ) ? p + x : x;
}

/* x_i = x mod q_iを求める*/
int pohligHellman(int p, int q,int e,int y,int g){
  int n = (p - 1)/q;
  int i_n = 0;
  int j_n = 0;
  int x[e];
  int y_n = modPow(y,n,p);
  int sum = 0;

  printf("p=%d,q=%d,e=%d,y=%d,g=%d\n",p,q,e,y,g);

  for(int i = 0; i < e; i++){
    x[i] = 0;
  }
  //指数部分が１つの処理
  while(x[0] < q){
    i_n = modPow(g,n * x[0],p);
    if(y_n == i_n){
      break;
    }
    x[0]++;
  }
  sum = x[0];

  //指数部分が２以上の処理
  if(e > 1){
    for(int j = 1; j < e; j++){
      j_n = modPow(y*modPow(invMod(g,p),sum,p),n/modPow(q,j,p),p);
      while(x[j] < q){
        i_n = modPow(g,n * x[j],p);
        if(j_n == i_n){
          break;
        }
        x[j]++;
      }
      sum += x[j]*modPow(q,j,p);
      
    }
  }
  return sum;
}

/* 全てのmの積を返す */
int prod_array(const int a[], int size){
  int i, res;
  res = 1;
  for ( i = 0; i < size; i++) { 
    res *= a[i]; 
  }
  return res;
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
  int a = 23631;    //Zpの原始根
  int x = 55163;    //秘密情報
  int p = 57223;    //素数
  int y = modPow(a,x,p);
  int size = countFactors(p - 1,sqrt(p - 1));
  int factor[size];
  int pohlig[size];
  int exponent[size];
  int fe[size];
  int attackX, M;
  long cpuTime;
  double sec;
  int yoso = 0;

  printf("【公開情報】(a,y,p)=(%d,%d,%d)\n",a,y,p);

  //素因数分解を行う
  printf("＝＝＝＝＝＝＝＝\n");
  printf("【p-1の素因数分解】\n");
  factor[0] = findFactors(p - 1, sqrt(p - 1), &yoso);
  exponent[0]=yoso;
  printf("%d^%d\n",factor[0],exponent[0]);
  int qq = pow(factor[0],yoso);
  
  for(int i = 1; i < size; i++){
    factor[i] = findFactors((p-1)/qq, sqrt(p - 1),&yoso);
    exponent[i] = yoso;
    printf("%d^%d\n",factor[i],exponent[i]);
    qq *= pow(factor[i],exponent[i]);
  }
  printf("＝＝＝＝＝＝＝＝\n");
  
  //x mod q_iとなるx_i求める
  for(int i = 0; i < size; i++){
    pohlig[i] = pohligHellman(p,factor[i],exponent[i],y,a);
    printf("【x mod q_%dとなるx_%d】%d\n",i,i,pohlig[i]);
  }

  for(int i = 0; i < size; i++){
    fe[i]= pow(factor[i],exponent[i]);
  }

  //連立合同式で秘密鍵の推測鍵を計算
  chrem(pohlig, fe, size, &attackX, &M );
  printf("推定鍵＝%d\n", attackX);

  //推測した鍵の結果が正しければtrueを返す
  if(y == modPow(a, attackX, p)){
    printf("解読成功\n");
  }
  else{
    printf("解読失敗…\n");
  }
  
  //実行時間の計測
  cpuTime = clock();
  sec = (double)cpuTime / CLOCKS_PER_SEC;
  printf("実行時間は%f秒でした\n",sec);
  return 0;
}