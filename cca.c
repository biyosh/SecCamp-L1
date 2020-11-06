/*攻撃者：ElGamal3について、暗号文(33869,3499907416776)の元の平文を知りたい*/

#include<stdio.h>

/* x^yをバイナリ法で求める */ 
int modPow(int x, int y, int z) {  
  long long int ret = 1;
  long long int mul = x;
  while(y > 0){
    if((y & 1) == 1)
      ret = ret * mul % z;
    mul = mul * mul % z;
    y >>= 1;
  }
  return ret;
}

/* 暗号文 (u,v) を、秘密鍵 x を用いて復号する */
int ElGamalDecrypt(int u, long long int v, int x,int z){
    return v / modPow(u,x,z);
}

int main(void) {
    int x = 53537;     //正規の秘密鍵

    int attackX;       //攻撃者が推測する秘密鍵
    int chosenU = 7195;    
    long long int chosenV = 76152453639453;    //攻撃者が選択した暗号文
    
    int u;    
    long long int v;    //推測したい平文の暗号化データ

    int m;    //正規の秘密鍵で復号した平文
    int attackM;    //攻撃者がattackXで復号した平文
    int z = 57223;  //16ビット以下の素数（公開情報）
    int answerM;

    printf("解読対象の暗号文を入力：\n");
    printf("C1：");
    scanf("%d",&u);
    printf("C2：");
    scanf("%lld",&v);
    printf("＝＝＝＝＝＝＝＝＝＝＝＝＝＝\n");

    m = ElGamalDecrypt(chosenU,chosenV,x,z);  //復号オラクルにchosenUとchosenVを復号させる
    printf("復号オラクルの復号結果：%d\n",m);

    for(attackX = 0;attackX < z;attackX++){
        attackM = ElGamalDecrypt(chosenU,chosenV,attackX,z);  //総当たりでattackXを用いて復号化する
        if(attackM == m){
            printf("推定した秘密鍵attackX：%d\n",attackX);
            break;             //条件を満たすattackXを１つ抽出（正規の秘密鍵で復号した文と同じ復号結果になるような秘密鍵）
        }
    }
    attackM = ElGamalDecrypt(u,v,attackX,z);   //推測した秘密鍵で復号化
    answerM = ElGamalDecrypt(u,v,x,z);    //正規の秘密鍵で復号化（正しい復号文）

    printf("＝＝＝＝＝＝＝＝＝＝＝＝＝＝\n");
    printf("正しい秘密鍵xの復号文：%d\n",answerM);
    printf("attackXの復号文：%d\n",attackM);
    if(attackM == answerM){  //ElGamal3の元の平文と一致したら成功
        printf("解読成功！\n");
        return 0;
    }else{  //ElGamal3の元の平文と一致したら成功
        printf("解読失敗…\n");
        return -1;
    }
}