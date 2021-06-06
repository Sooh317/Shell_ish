#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "../parser/parse.h"
#include "redirect.h"
#ifndef MAX_PROCESS
#define MAX_PROCESS 32
#endif

void check_fork(pid_t);
int pipecount(process);
void sig_init_dfl(struct sigaction*);
void make_sus(process, pid_t, pid_t);
void make_bg(process, pid_t, pid_t);
void resume(job*);
void fix(process*);
void change_dir(process*);


void myexec(job* jb, char *envp[]){	
	if(jb == NULL) return;
	if(!strcmp(jb->process_list->program_name, "bg")){
		resume(jb);
		return;
	}
	else if(!strcmp(jb->process_list->program_name, "cd")){
		if(chdir(jb->process_list->argument_list[1]) < 0) perror("cd error\n");
		return;
	}
	else if(!strcmp(jb->process_list->program_name, "fg")){
		perror("this feature not supported\n");
		return;
	}

	/* declaration */
	pid_t pid[MAX_PROCESS]; // child process id
	int status[MAX_PROCESS];
	int pfd[MAX_PROCESS * 2]; // 偶数番目が read, 奇数番目が write を担当する pipe_fd
	int i, j;
	int fd_in = -1, fd_out = -1; // for redirect
	int pipe_num, process_num;   // for pipe 

	process* cur_process = jb->process_list;
	pipe_num = pipecount(*cur_process);
	process_num = pipe_num + 1;
		
	for(i = 0; i < process_num; i++){
		if(cur_process->program_name[0] != '/') fix(cur_process);
		//sigprocmask ? -> cond = RUNNING
		if(i < pipe_num && pipe(pfd + 2*i) == -1) perror("pipe error"), exit(1);

		pid[i] = fork();
		check_fork(pid[i]);

		if(pid[i] == 0){
			struct sigaction sig_child;
			sig_init_dfl(&sig_child);

			if(i ==  0 && cur_process->input_redirection != NULL) redirect_in(cur_process->input_redirection, &fd_in);
			if(i == pipe_num && cur_process->output_redirection != NULL){
				if(cur_process->output_option == APPEND) redirect_append(cur_process->output_redirection, &fd_out);
				else redirect_out(cur_process->output_redirection, &fd_out);
			}
			if(i < pipe_num) dup2(pfd[2*i + 1], 1);
			if(i > 0) dup2(pfd[2*(i - 1)], 0);
			
			if(fd_in != -1) close(fd_in);
			if(fd_out != -1) close(fd_out);
			//for(j = 0; j < pipe_num * 2; j++) close(pfd[j]);
			execve(cur_process->program_name, cur_process->argument_list, envp);
			perror("execve error");
			exit(1);
		}
		if(i < pipe_num) close(pfd[2*i + 1]);
		if(i > 0) close(pfd[2*(i - 1)]);

		cur_process = cur_process->next;
	}
	
	if(fd_in != -1) close(fd_in);
	if(fd_out != -1) close(fd_out);

	cur_process = jb->process_list;
	for(i = 0; i < process_num; i++){
		if(setpgid(pid[i], pid[0]) < 0){
			perror("set pgid error\n");
			exit(1);
		}
	}
	if(jb->mode == FOREGROUND && tcsetpgrp(0, pid[0]) < 0){
		perror("tcsetpgrp error");
		exit(1);
	}
	for(i = 0; i < process_num; i++){
		if(jb->mode == FOREGROUND){
			if(waitpid(pid[i], &status[i], WUNTRACED) < 0) perror("wait error"), exit(1);
			if(WIFSTOPPED(status[i])){
				make_sus(*cur_process, pid[i], pid[0]);
				if(tcsetpgrp(0, getpgrp()) < 0) perror("tcsetpgid error"), exit(1);
			}
		}
		else make_bg(*cur_process, pid[i], pid[0]);  // background
		cur_process = cur_process->next;
	}

	if(jb->mode == FOREGROUND && tcsetpgrp(0, getpgrp()) < 0) perror("tcsetpgid error"), exit(1);
	return;
}
