#include<gmp.h>
#include<time.h>

void createPrime(mpz_t p, mpz_t q, mpz_t bits){
	gmp_randstate_t state;
	int checkPrime = 0;
	gmp_randinit_default(state);
	gmp_randseed_ui(state,(unsigned long)time(NULL));

	while(!(checkPrime == 2)){
		mpz_urandomm(p, state, bits);
		checkPrime = mpz_probab_prime_p(p, 25);
	}
	checkPrime = 0;
	while(!(checkPrime == 2)){
		mpz_urandomm(q, state, bits);
		checkPrime = mpz_probab_prime_p(q, 25);
	}
	mpz_clear(bits);
}

/*鍵生成*/
void KeyGenerate(mpz_t p,mpz_t q, mpz_t n, mpz_t d){
	mpz_t mulp, l, p_minus_1, q_minus_1, one;
	mpz_init(l);
	mpz_init(mulp);
	mpz_init(p_minus_1);
	mpz_init(q_minus_1);
	
	mpz_mul(mulp, p, p);
	mpz_mul(n, mulp, q);      //公開鍵生成
		
	mpz_sub_ui(p_minus_1, p, 1);
	mpz_sub_ui(q_minus_1, q, 1);  //p-1,q-1を格納
	
	mpz_lcm(l, p_minus_1, q_minus_1);  //最小公倍数を求める。
	mpz_invert(d, n, l);   //N^{-1} mod lcm(p-1,q-1)で秘密鍵生成

	mpz_clear(l);
	mpz_clear(mulp);
	mpz_clear(p_minus_1);
	mpz_clear(q_minus_1);
}

/*暗号化*/
void SchmidtEncrypt(mpz_t m,mpz_t n, mpz_t c){
	mpz_powm(c, m, n, n);
}

/*復号化*/
void SchmidtDecrypt(mpz_t mm, mpz_t d, mpz_t p, mpz_t q, mpz_t c){
	mpz_t mulpq;
	mpz_init(mulpq);
	mpz_mul(mulpq, p, q);
	mpz_powm(mm, c, d, mulpq);
	mpz_clear(mulpq);
}

int main(void){
	mpz_t m, p, q, n, d, c, mm, bits;

	mpz_init(m);
 	mpz_init(p);
 	mpz_init(q);
 	mpz_init(n);
 	mpz_init(d);
 	mpz_init(c);
 	mpz_init(mm);
	mpz_init_set_ui(bits,999999999999999);

	gmp_printf("平文を入力してください：");
	gmp_scanf("%Zd",m);

	createPrime(p, q, bits);
	gmp_printf("(p,q)：(%Zd,%Zd)\n", p, q);
 	
 	int check = 1;

 	KeyGenerate(p, q, n, d);
 	gmp_printf("鍵(N,d)： (%Zd,%Zd)\n", n, d);  
 	
 	SchmidtEncrypt(m, n, c);
 	gmp_printf("暗号： %Zd\n", c);

 	SchmidtDecrypt(mm, d, p, q, c);
 	gmp_printf("復号結果 %Zd\n", mm);
 	
 	check = mpz_cmp(m, mm);  //もとの平文と復号文が一致しているか確認
 	if(check == 0){
 		gmp_printf("復号成功\n");
 	}
 	else{
 		gmp_printf("復号失敗…\n");
 	}

 	mpz_clear(m);
 	mpz_clear(p);
 	mpz_clear(q);
 	mpz_clear(n);
 	mpz_clear(d);
 	mpz_clear(c);
 	mpz_clear(mm);
}
