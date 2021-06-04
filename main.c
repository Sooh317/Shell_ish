#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser/parse.h"
#ifndef MAX_PROCESS
#define MAX_PROCESS 32  // 1つのjob内のプロセスは最大でこの数だとする
#endif

void update_bg(pid_t, job*); // definition if jpgc is in parse.h
void myexec(job*, char*[]);
void print_job_list(job*);
void sig_init_ign(struct sigaction* sig);
void wait_on_background(void);

node* BG_H = NULL, *BG_B = NULL;
node* SUS_H = NULL, *SUS_B = NULL;

int main(int argc, char *argv[], char *envp[]) {
    char s[LINELEN];
    job *curr_job;
    
    struct sigaction sig;
    sig_init_ign(&sig);

    while(get_line(s, LINELEN)) {
        if(!strcmp(s, "exit\n")) break;

        curr_job = parse_line(s);
		//print_job_list(curr_job); 

        myexec(curr_job, envp); 

        wait_on_background();
        
        free_job(curr_job);
    }

    return 0;
}
