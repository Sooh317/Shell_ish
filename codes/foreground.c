#include "../parser/parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern node* BG_H, *BG_B;

void give_node(node*, pid_t, int flag);

void to_fg(){ // bring earliest added process group to foreground
    if(BG_H == NULL){
        perror("No background process\n");
        return;
    }
    int status;
    pid_t id;
    node* cur, *nxt;


    id = BG_H->pgid;
    if(tcsetpgrp(0, id) < 0) perror("tcsetpgrp error"), exit(1);

    cur = BG_H;
    while(cur != NULL){
        nxt = cur->next;
        if(cur->pgid == id){
            if(waitpid(cur->pid, &status, WUNTRACED) < 0) perror("wait error\n"), exit(1);
            if(WIFSTOPPED(status)){
                /* this function */
				give_node(cur, cur->pid, 1);  // give node to suspended
				if(tcsetpgrp(0, getpgrp()) < 0) perror("tcsetpgrp error"), exit(1);
			}
            else free(cur);  // this process finished
        }
        else{
            if(tcsetpgrp(0, getpgrp()) < 0) perror("tcsetpgrp error"), exit(1);
            BG_H = cur;
            cur->prev = NULL;
            return;
        }
        cur = nxt;
    }
    BG_H = BG_B = NULL;
    if(tcsetpgrp(0, getpgrp()) < 0) perror("tcsetpgrp error"), exit(1);
    return;
}