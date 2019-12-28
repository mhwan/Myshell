#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#define MAXARG 7

void doRedirection(int flag, int is_bg, char *argv[], char* input, char * output);
int checkInput(char buf[]);
int tokenized(char buff[], char* arg[] ,char delim[]);
void executesLine(int isbg, char * argv[]);
void doPipe(char* arg1[], char* arg2[]);

