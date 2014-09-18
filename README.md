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

