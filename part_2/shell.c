#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void toString(char* from, char* to){
    while(*to != '\0'){
        *from = *to;
        to++;
        from++;
    }
    *from = 0;
}

#define RUN "run"
#define EXIT "exit"
#define CHILD "./child"

int main(void){
	printf("shell> ");
    while(1) {
        char* usercommand = malloc(255* sizeof(char));
        scanf("%[^\n]%*c", usercommand);
        size_t len = strlen(usercommand);
        char *userCommand = malloc(len + 1 + 1); /* one for extra char, one for trailing zero */
        toString(userCommand, usercommand);
        free(usercommand);
        userCommand[len] = '\0';
        char* command[1];
        FILE* fp;
        pid_t pid;

        if (strcmp(userCommand, RUN) == 0){
            if((pid = fork()) == 0) {
                char* newargv[] = {CHILD, NULL, NULL};
                char* env[] = {NULL};
                execve(CHILD,newargv,env);
                exit(0);
            }
            else if(pid < 0) {
                printf("Failed to fork process\n");
                exit(1);
            }
            sleep(1);
        }
        else if (strcmp(userCommand, EXIT) == 0){
	    kill(pid, SIGKILL);
            break;
        }
	else{ //commands are not "exit" nor "run"
	    printf("Invalid Commands. This program only accepts: run & exit\n");
	    printf("shell> ");
	}
    }
    return 0;
}
