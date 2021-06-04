#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser/parse.h"
#ifndef MAX_PROCESS 
#define MAX_PROCESS 32
#endif

void sig_init_ign(struct sigaction* sig){
    memset(sig, 0, sizeof(*sig));
    sig->sa_handler = SIG_IGN;
    sig->sa_flags = 0;
    sigemptyset(&(sig->sa_mask));  
    if(sigaction(SIGINT, sig, NULL) < 0){
        perror("SIGINT error");
        exit(1);
    }
    if(sigaction(SIGTSTP, sig, NULL) < 0){
        perror("SIGTSTP error");
        exit(1);
    }
    if(sigaction(SIGTTOU, sig, NULL) < 0){
        perror("SIGTTOU error");
        exit(1);
    }
    /*]if(sigaction(SIGCHLD, sig, NULL) < 0){
        perror("SIGCHLD error");
        exit(1);
    }*/
}


void sig_init_dfl(struct sigaction* sig){
    memset(sig, 0, sizeof(*sig));
    sig->sa_handler = SIG_DFL;
    sig->sa_flags = 0;
    sigemptyset(&(sig->sa_mask));  
    if(sigaction(SIGINT, sig, NULL) < 0){
        perror("SIGINT error");
        exit(1);
    }
    if(sigaction(SIGTSTP, sig, NULL) < 0){
        perror("SIGTSTP error");
        exit(1);
    }
}