#include "myshell.h"

void doRedirection(int flag, int is_bg, char *argv[], char* input, char * output){
    int input_fd, output_fd;
    int status, pid;
    
    if ((pid=fork()) == -1)
        perror("fork failed");
    else if (pid != 0) {
        if(is_bg==0)
            pid = wait(&status);
        else {
            printf("[1] %d\n", getpid());
            waitpid(pid, &status, WNOHANG);
        }
    } else {
        if (flag == 2) {
            if((input_fd = open(input, O_RDONLY))==-1){
                perror(argv[0]);
                exit(2);
            }
            dup2(input_fd, 0);
            close(input_fd);
            execvp(argv[0], argv);
        } else if (flag == 3) {
            output_fd = open(output, O_CREAT|O_TRUNC|O_WRONLY, 0600);
            dup2(output_fd, 1);
            close(output_fd);
            execvp(argv[0], argv);
        } else {
            if (input != NULL && output != NULL) {
                input_fd = open(input, O_RDONLY);
                dup2(input_fd, 0);
                close(input_fd);
                
                output_fd = open(output, O_CREAT|O_TRUNC|O_WRONLY, 0600);
                dup2(output_fd, 1);
                close(output_fd);
                execvp(argv[0], argv);
            }
        }
    }
}


