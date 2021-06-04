#include "../parser/parse.h"
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

extern node* BG_H, *BG_B;

void wait_on_background(void){
    pid_t tmp;
    int status;
    node* cur = BG_H;
    node* nxt;

    /* wait some number of times */
    while(cur != NULL){
        nxt = cur->next;
        tmp = waitpid(cur->pid, &status, WNOHANG);
        if(tmp < 0) perror("waitpid error"), exit(1);
        else if(WIFEXITED(status)){
            if(cur->prev == NULL){
                if(cur->next == NULL) BG_B = BG_H = NULL;
                else{
                    cur->next->prev = NULL;
                    BG_H = cur->next;
                }
            }
            else{
                if(cur->next == NULL){
                    cur->prev->next = NULL;
                    BG_B = cur->prev; 
                }
                else{
                    cur->prev->next = cur->next;
                    cur->next->prev = cur->prev;
                }
            }
            free(cur);
        }
        cur = nxt;
    }
}
