/**
 *リダイレクトを行う際に必要な操作をまとめたもの
 * 1. file を open
 * 2. file descriptor の複製
 * 3. エラーの確認
**/    
#ifndef REDIRECT_H
#define REDIRECT_H
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "../parser/parse.h"

void redirect_in(char*, int*);
void redirect_out(char*, int*);
void redirect_append(char*, int*);
#endif