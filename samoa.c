#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<time.h>

/* x^yをバイナリ法で求める */ 
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

/*最大公約数*/
int gcd(int a, int b){
  if(a % b == 0){
    return b;
  }else{
    return gcd(b, a % b);
  }
}

/*最小公倍数*/
int lcm(int a, int b){
  return a * b / gcd(a, b);
}

/*拡張ユークリッド互除法を用いてa^{-1} mod p を求める*/
int invMod(int a, int p){
  long long int b, x, u, q, tmp; 
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

/* 鍵生成 */ 
void KeyGenerate(int p,int q,long long int* n, int* d){
  long long int l;
  *n = p * p * q;
  l = lcm(p - 1,q - 1);
  *d = invMod(*n,l);
}
/* 暗号化 */ 
int SchmidtEncrypt(int m,long long int n){
    return modPow(m,n,n);
}

/* 復号化 */
int SchmidtDecrypt(int c, int d, int p,int q){
    return modPow(c, d, p * q);
}

int main() {
    int m;
    int p = 383;      
    int q = 2137;
    long long int n; //公開鍵
    int d; //秘密鍵
    int c;
    int mm;

    printf("平文を入力してください：");
    scanf("%d",&m);                 //平文を標準入力する(不正な値はエラー処理)
    if(!(0 < m && m <= INT_MAX)){
      printf("不正な値です\n");
      return -1;
    }

    KeyGenerate(p,q,&n,&d);     //鍵生成

    printf("鍵(n,d)=(%lld,%d)\n",n,d);   

    c = SchmidtEncrypt(m,n);   //暗号化
    printf("暗号化：%d\n",c);   
    mm = SchmidtDecrypt(c,d,p,q); //復号化
    printf("復号化：%d\n",mm);

}