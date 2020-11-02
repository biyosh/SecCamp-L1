#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

/* 公開鍵aと乱数rを用いて暗号化① */ 
int ElGamalEncrypt1(int a,int r){
    return pow(a,r);
}

/* 平文mと公開鍵yと乱数rを用いて暗号化② */ 
int ElGamalEncrypt2(int m,int y,int r){
    return m * pow(y,r);
}

/* 暗号文 (u,v) を、秘密鍵 x を用いて復号する */
int ElGamalDecrypt(int u, int v, int x){
    return v / pow(u,x);
}

int main() {
    int m;    //平文
    int a = 6, y;   //公開鍵(aは固定)
    int x = 3;     //秘密鍵
    int r;      //乱数
    int u,v;    //暗号化データ
    int mm;    //復号化データ

    printf("平文を入力してください：");
    scanf("%d",&m);                 //平文を標準入力する
    y = pow(a,x);
    printf("公開鍵(a,y)=(%d,%d)\n",a,y);   //公開鍵を公開
    srand(time(NULL));
    r = rand() % 2 + 1;    //乱数を生成(ここでは試験的にrは0~2の範囲内とする。)

    u = ElGamalEncrypt1(a,r);
    v = ElGamalEncrypt2(m,y,r);
    printf("暗号化(%d,%d)\n",u,v);   //暗号化した（u,v）を標準出力

    mm = ElGamalDecrypt(u,v,x);
    printf("復号化%d\n",mm);      //mを復号化したmmを標準出力（うまくいけばmと同じになる）
}