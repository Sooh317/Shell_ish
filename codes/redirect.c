#include "redirect.h"

void redirect_in(char* filename, int* fd){
	if((*fd = open(filename, O_RDONLY)) < 0){
		perror("open error");
		close(*fd);
		exit(1);
	}
	if(dup2(*fd, 0) == -1){ 
		perror("dup error");
		exit(1);
	}
	if(close(*fd) == -1){
		perror("close error");
		exit(1);
	}
	return;
}

void redirect_out(char* filename, int* fd){
	if((*fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0){
		perror("open error");
		close(*fd);
		exit(1);
	}
	if(dup2(*fd, 1) == -1){
		perror("dup error");
		exit(1);
	}
	if(close(*fd) == -1){
		perror("close error");
		exit(1);
	}
	return;
}

void redirect_append(char* filename, int* fd){
	if((*fd = open(filename, O_WRONLY | O_APPEND)) < 0){
		perror("open error");
		close(*fd);
		exit(1);
	}
	if(dup2(*fd, 1) == -1){
		perror("dup error");
		exit(1);
	}
	if(close(*fd) == -1){
		perror("close error");
		exit(1);
	}
	return;
}
