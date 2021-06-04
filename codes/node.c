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

void give_node_to_bg(node* cur, pid_t id){
    if(cur->pgid == id){ // free cur
        if(cur->prev == NULL){
            if(cur->next == NULL){ // only one suspended process
                if(BG_B == NULL) BG_H = BG_B = cur; 
                else{
                    BG_B->next = cur;
                    cur->prev = BG_B;
                    BG_B = cur;
                    SUS_H = SUS_B = NULL;
                }
            }
            else{                  // first element in many suspenede processes
                cur->next->prev = NULL;
                SUS_H = cur->next;
                cur->next = NULL;
                if(BG_B == NULL) BG_H = BG_B = cur;
                else{
                    cur->prev = BG_B;
                    BG_B->next = cur;
                    BG_B = cur;
                }
            }
        }   
        else{
            if(cur->next == NULL){  // last element in many suspended processes
                SUS_B = cur->prev;
                cur->prev = BG_B;    
                if(BG_B == NULL) BG_H = BG_B = cur;
                else{
                    BG_B->next = cur;
                    BG_B = cur;
                }    
            }
            else{                   // have left and right suspended process node
                cur->next->prev = cur->prev;
                cur->prev->next = cur->next;
                cur->prev = BG_B;
                cur->next = NULL;
                if(BG_B == NULL) BG_H = BG_B = cur;
                else{
                    BG_B->next = cur;
                    BG_B = cur;
                }
            }
        }                     
    }
}
