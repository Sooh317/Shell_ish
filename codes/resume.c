#include "../parser/parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void give_node(node*, pid_t, int flag);

extern node* BG_H, *BG_B;
extern node* SUS_H, *SUS_B;

void resume(job* jb){
    int id;    
    if(jb->process_list->argument_list[1] != NULL){ // pid is indicated
        perror("this feature not supported\n");
    }  
    else{
        if(SUS_B == NULL) perror("no process to resume\n");
        else{
            id = SUS_H->pgid;
            if(kill(-id, SIGCONT) < 0) perror("kill error\n"), exit(1);
            else{
                node* cur = SUS_H;
                node* nxt;
                while(cur != NULL){
                    nxt = cur->next;
                    give_node(cur, id, 0); // give node to background
                    cur = nxt;
                }
            }
        }                
    }
    return;
}