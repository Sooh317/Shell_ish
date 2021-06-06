#include "../parser/parse.h"
#include <stdio.h>
#include <stdlib.h>

extern node* BG_H, *BG_B;
extern node* SUS_H, *SUS_B;

void make_sus(process pro, pid_t pid, pid_t pgid, int make){
    node* nw;
    nw = (node*)malloc(sizeof(node));
    nw->next = NULL;
    nw->prev = NULL;
    nw->proc = pro;
    nw->pid = pid;
    nw->pgid = pgid;

    if(SUS_H == NULL){
        SUS_H = nw;
        SUS_B = nw;
    }
    else{
        nw->prev = SUS_B;
        SUS_B->next = nw;
        SUS_B = nw;
    }
}

void make_bg(process proc, pid_t pid, pid_t pgid){
    node* nw;
    nw = (node*)malloc(sizeof(node));
    nw->next = NULL;
    nw->prev = NULL;
    nw->proc = proc;
    nw->pid = pid;
    nw->pgid = pgid;

    if(BG_H == NULL){
        BG_H = nw;
        BG_B = nw;
    }
    else{
        nw->prev = BG_B;
        BG_B->next = nw;
        BG_B = nw;
    }
}

void give_node(node* cur, pid_t id, int flag){ // if flag = 0 then give node to background, otherwize to suspended
    if(cur->pgid == id){
        if(cur->prev == NULL){
            if(cur->next == NULL){ // only one process in the list
                if(flag == 0){
                    if(BG_B == NULL) BG_H = BG_B = cur; 
                    else{
                        BG_B->next = cur;
                        cur->prev = BG_B;
                        BG_B = cur;
                        SUS_H = SUS_B = NULL;
                    }
                }
                else{
                    if(SUS_B == NULL) SUS_H = SUS_B = cur; 
                    else{
                        SUS_B->next = cur;
                        cur->prev = SUS_B;
                        SUS_B = cur;
                        BG_H = BG_B = NULL;
                    }
                }
            }
            else{                  // first element in many suspenede processes
                cur->next->prev = NULL; // next node becomes the first node in the list
                cur->next = NULL;     // put this node to the tail of the other list
                if(flag == 0){
                    SUS_H = cur->next;
                    if(BG_B == NULL) BG_H = BG_B = cur;
                    else{
                        cur->prev = BG_B;
                        BG_B->next = cur;
                        BG_B = cur;
                    }
                }
                else{
                    BG_H = cur->next;
                    if(SUS_B == NULL) SUS_H = SUS_B = cur;
                    else{
                        cur->prev = SUS_B;
                        SUS_B->next = cur;
                        SUS_B = cur;
                    }
                }
            }
        }   
        else{
            if(cur->next == NULL){  // last element in many suspended processes
                if(flag == 0){ 
                    SUS_B = cur->prev;
                    cur->prev = BG_B;    
                    if(BG_B == NULL) BG_H = BG_B = cur;
                    else{
                        BG_B->next = cur;
                        BG_B = cur;
                    }
                }
                else{
                    BG_B = cur->prev;
                    cur->prev = SUS_B;    
                    if(SUS_B == NULL) SUS_H = SUS_B = cur;
                    else{
                        SUS_B->next = cur;
                        SUS_B = cur;
                    }
                }
            } 
            else{                   // have left and right suspended process node
                cur->next->prev = cur->prev;
                cur->prev->next = cur->next;
                cur->next = NULL;
                if(flag == 0){
                    cur->prev = BG_B;
                    if(BG_B == NULL) BG_H = BG_B = cur;
                    else{
                        BG_B->next = cur;
                        BG_B = cur;
                    }
                }
                else{
                    cur->prev = SUS_B;
                    if(SUS_B == NULL) SUS_H = SUS_B = cur;
                    else{
                        SUS_B->next = cur;
                        SUS_B = cur;
                    }
                }
            }
        }                     
    }
}
