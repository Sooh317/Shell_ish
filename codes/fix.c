#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../parser/parse.h"

void fix(process* cur_process){
    int i;
    int prev = 0, len, namelen = strlen(cur_process->program_name);
    FILE* fp;
    char* env = getenv("PATH");
    char path[LINELEN];

    for(i = 0; i < strlen(env); i++){
        if(env[i] == ':'){
            len = i - prev;
            memcpy(path, env + prev, len);
            path[len] = '/';
            memcpy(path + len + 1, cur_process->program_name, namelen);
            path[len + namelen + 1] = '\0';

            if((fp = fopen(path, "r")) == NULL){
                prev = i + 1;
                continue;
            }
            strcpy(cur_process->argument_list[0], path);
            strcpy(cur_process->program_name, path);
            fclose(fp);
            return;
        }
    }
}
