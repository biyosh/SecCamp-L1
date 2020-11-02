#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<time.h>

/* x^yをバイナリ法で求める */ 
int modPow(int x, int y, int z) {
    if(y == 0)       //①の処理
        return 1;   

    else if(y == 1)       //②の処理
        return (int)(x % z);

    else if(y % 2 == 1){   //yが奇数のとき
        int b = modPow(x,y - 1,z);
        long long int lb = b; 
        lb = ((x * lb) % z);
        return (int)lb;
    }
    else{
        int b = modPow(x,y/2,z);
        long long int lb = b;
        lb = ((lb * lb) % z);
        return (int)lb;
    }
}

/* 公開鍵aと乱数rを用いて暗号化① */ 
int ElGamalEncrypt1(int a,int r,int z){
    return modPow(a,r,z);
}
 
/* 平文mと公開鍵yと乱数rを用いて暗号化② */ 
long long int ElGamalEncrypt2(int m,int y,int r,int z){
    long long int E2 = modPow(y,r,z);
    E2 = E2 * m;
    return E2;
}

/* 暗号文 (u,v) を、秘密鍵 x を用いて復号する */
int ElGamalDecrypt(int u, long long int v, int x,int z){
    return v / modPow(u,x,z);
}

int main() {
    int m;    //平文(int型を想定)
    int a = 24680, y;   //公開鍵①②(aはZpの原始元)
    int z = 64439;      //公開鍵③(16ビット以下の素数)
    int x = 65537;     //秘密鍵
    int r;      //乱数
    int u;    
    long long int v;    //暗号化データ
    int mm;    //復号化データ
    
    printf("平文を入力してください：");
    scanf("%d",&m);                 //平文を標準入力する(不正な値はエラー処理)
    if(!(0 < m && m <= INT_MAX)){
        printf("不正な値です\n");
        return -1;
    }

    y = modPow(a,x,z);
    printf("公開鍵(a,y,z)=(%d,%d,%d)\n",a,y,z);   //公開鍵を計算して公開する
    srand(time(NULL));
    r = rand();    //乱数を生成(0〜2147483647)

    u = ElGamalEncrypt1(a,r,z);
    v = ElGamalEncrypt2(m,y,r,z);
    printf("暗号化(%d,%lld)\n",u,v);   //暗号化した（u,v）を標準出力

    mm = ElGamalDecrypt(u,v,x,z);
    printf("復号化%d\n",mm);      //暗号文（u,v）を復号化したmmを標準出力（うまくいけばmと同じになる）
    return 0;
}