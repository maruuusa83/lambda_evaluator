lambda_evaluator
================

## 概要
ラムダ項を与えるとラムダ計算します。  
lex/yaccを使っています。


## 使い方
``` bash
$ make all
$ ./lambda
```
でコンパイルして実行すればよいです。  
lex, yacc, gccが必要です。


## ちょっとした仕様
 * 変数\[a-zA-Z\]\[a-zA-Z0-9\]?はlambda項
 * xが変数、Mがlambda項のとき、(lambda x.M)はlambda項
 * MとNがともにlambda項のとき、(M N)はlambda項

この処理系では、lambda項でよく用いられる簡略記法は許容していません。厳密にこの構文に従っています。  
つまり、(lambda xy.x)は、(lambda x.(lambda y.x))と記述されなければなりません。

与えられたlambda項を最左最内戦略でベータ簡約します。


## フォルダ構成
```
/ -- lambda.l
  |- lambda.y
  |- lambda_utils.h
  |- lambda_utils.c
  |
  |- README.md
  |- LISENCE
```

