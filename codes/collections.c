/**
 * 重要ではない関数をまとめたファイル
 * check_fork : fork()のエラー判定
 * pipecount  : パイプの数をカウント
**/
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include "../parser/parse.h"

void check_fork(pid_t pid){
	if(pid == -1){
		perror("fork error");
		exit(1);
	}
}

int pipecount(process p){
	int res = 0;
	while(p.next != NULL){
		++res;
		p = *p.next;
	}
	return res;
}
