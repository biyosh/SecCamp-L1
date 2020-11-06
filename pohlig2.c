#include<stdio.h>
#include<math.h>
#include<time.h>
#define N 10

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

/*素因数分解を行う*/
int next_prime_factor(int m, int* q, int* e){
  int count = 0;

  //2で割る
  if(m % 2 == 0){
    while (m % 2 == 0){ 
      m /= 2;
      count++;
      *e = count;
    }
    *q = 2;
    return m;
  }

  //3以上の奇数で割る
  for (int i = 3; i <= m; i+=2){ 
    if(m % i == 0){
      while (m % i == 0){
        m /= i;
        count++;
        *e = count;
      }
      *q = i;
      return m;
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
  int x[N];
  int y_n = modPow(y,n,p);
  int sum = 0;

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
      j_n = modPow(y * modPow(invMod(g,p),sum,p),n / modPow(q,j,p),p);
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
  int q = 1, e = 1; // 素数とべき
  int m = p - 1; // 残り
  int t; // テンポラリ変数
  int pohlig[N];
  int fe[N];
  int attackX, M;
  long cpuTime;
  double sec;

  printf("【公開情報】(a,y,p)=(%d,%d,%d)\n",a,y,p);
  printf("＝＝＝＝＝＝＝＝＝＝＝＝＝＝\n");

  //素因数分解をし、x mod q_iを満たすx_iを求める
  int count = -1;
  while (1) {
    t = next_prime_factor(m, &q, &e);
    printf("%d = %d * %d^%d\n", m, t, q, e);
    count++;
    m = t;

    fe[count]= pow(q,e);
    pohlig[count] = pohligHellman(p,q,e,y,a);
    printf("【x mod %d^%dとなるx_%d】%d\n",q,e,count,pohlig[count]);

    if (m == 1) {
      break;
    }
  }
  printf("＝＝＝＝＝＝＝＝＝＝＝＝＝＝\n");

  //連立合同式で秘密鍵の推測鍵を計算
  chrem(pohlig, fe, count + 1, &attackX, &M );
  printf("推定鍵attackX＝%d\n", attackX);

  printf("y(=a^x mod p)=%d\n",y);
  printf("y(=a^attackX mod p)=%d\n",y);
  //推測した鍵の結果が正しければtrueを返す
  if(y == modPow(a, attackX, p)){
    printf("解読成功\n");
  }
  else{
    printf("解読失敗…\n");
  }
  printf("＝＝＝＝＝＝＝＝＝＝＝＝＝＝\n");
  
  //実行時間の計測
  cpuTime = clock();
  sec = (double)cpuTime / CLOCKS_PER_SEC;
  printf("実行時間は%f秒でした\n",sec);
  return 0;
}