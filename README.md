# SecCamp

セキュリティ・キャンプ2020オンラインの「L1暗号解読チャレンジゼミ」で書いたコードを纏めています．  
## コード一覧
【ElGamal.c】  
応募課題で提出したエルガマル暗号の実装（かなり拙い）．  
  
【ElGamal2.c】  
ElGamal.cの改良版．バイナリ法でmod_powを計算．再帰関数で実装． 
  
【ElGamal3.c】  
ElGamal2.cの改良版．バイナリ法をビット操作で実装．   
  
【cca.c】  
選択暗号文攻撃を用いて，ElGamal暗号の暗号文から平文を推測する．
  
【fullsearch.c】  
DLPからのアプローチ．総当たりで鍵を推測する．
  
【pohlig.c】  
DLPからのアプローチ．Pohlig-Hellmanアルゴリズムを用いてfullsearchより高速で秘密鍵を求める． 
  
【pohlig2.c】  
pohlig.cの改良版．素因数分解をポインタを用いて実装．

【samoa.c】  
schmidt-samoa暗号をC言語で簡易実装．  

【samoa2.c】  
schmidt-samoa暗号をGMPを用いて実装．鍵（素数のペア）生成の処理も．  
  
【samoaAttack.c】  
schmidt-samoa暗号に対して，N，d，cが分かっているときに，pqと平文mを推測する処理．

