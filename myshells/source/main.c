#include "myshell.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAXARG 7
int main(){
    char buffer[256];
    int num_args = 0, i=0, num_arg=0;
    char* args[MAXARG];
    char* arg[MAXARG];
    char* s;
    char* saveptr;
    int pid, status;
    
    int num = 0;
    int j = 0;
    int m = 0;
    int brpoint = 0;
    char *pipe_arg1[MAXARG];
    char *pipe_arg2[MAXARG];
    char input[20] = "";
    char output[20] = "";
    int is_bg = 0;
    static const char delim[] = " \t\n";
    while (1) {
        printf("myshell$ ");
        gets(buffer);
        
        if (buffer != NULL && checkInput(buffer)==0) {
            //multiple instruction
            num_args =0;
            if (strchr(buffer, ';') != NULL) {
                num_args = tokenized(buffer, args, ";");
            } else {
                num_args = 1;
                args[0] = buffer;
                args[1] = (char *)0;
            }
            
            if (!strcmp(args[0], "quit")) {
                printf("Goodbye from myshell::\n");
                break;
            }
            
            for (i = 0; i<num_args; i++) {
                num = 0;
                brpoint = 0;
                m = 0;
                is_bg = 0;
                s = strtok_r(args[i], delim, &saveptr);
                
                while(s) {
                    if (strcmp(s, "|") == 0) {
                        m = 1;
                        brpoint = num;
                        for (j =0; j < brpoint; j++) {
                            pipe_arg1[j] =  arg[j];
                        }
                        pipe_arg1[brpoint] = (char *) 0;
                    } else if (strcmp(s, "<") == 0) {
                        m = 2;
                        brpoint = num;
                    } else if (strcmp(s, ">") == 0) {
                        if (m == 2)
                            m = 4;
                        else
                            m = 3;
                    } else if(strcmp(s, "&") == 0) {
                        is_bg = 1;
                    } else {
                        if(m < 2)
                            arg[num++] = s;
                        else if(m ==2)
                            strcpy(input, s);
                        else
                            strcpy(output, s);
                    }
                    
                    s = strtok_r(NULL, delim, &saveptr);
                    
                }
                arg[num] = (char *)0;
                
                if (m == 0) {
                    executesLine(is_bg, arg);
                }
                else if (m == 1) {
                    for (j =0; j < num-brpoint; j++) {
                        pipe_arg2[j]= arg[j+brpoint];
                    }
                    pipe_arg2[num-brpoint] = (char *) 0;
                    doPipe(pipe_arg1, pipe_arg2);
                } else if(m >= 2) {
                    doRedirection(m, is_bg, arg, input, output);
                }       
            }
        }
    }
    exit(0);
}

