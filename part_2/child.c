#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int i = 1;
    sigset_t mask, prev_mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    /* Block SIGINT and save previous blocked set */
    sigprocmask(SIG_BLOCK, &mask, &prev_mask);

    while(i <= 5){
        printf("%d\n", i);
	sleep(1);
        i++;
    }

    sigprocmask(SIG_SETMASK, &prev_mask, NULL);

    while(i <= 10){
        printf("%d\n", i);
	if(i != 10)
	   sleep(1);
        i++;
	
    }
printf("shell> ");

}
