#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include <sys/time.h>

void proc_exit()
{
    int wstat;
    pid_t pid;

    while (1) {
        pid = wait (&wstat);
        if (pid == 0)
            return;
        else if (pid == -1)
            return;
    }
}

int fib(char* argv[]){
    int arg = atoi(argv[1]);
    u_int64_t fib_0 = 1, fib_1 = 1, fib_n;
    for(int i = 1; i < arg - 1; i++) {
        fib_n = fib_0 + fib_1;
        fib_0 = fib_1;
        fib_1 = fib_n;
        if(i == arg-2)
            return fib_n%10;
    }
}

int main (int argc, char* argv[])
{
    signal (SIGCHLD, proc_exit);
    int result = 0;
    switch (fork()) {
        case -1:
            perror ("Error");
            exit(1);
        case 0:
            result = fib(argv);
            printf("%d\n", result);
            exit(0);
        default:
            pause();
    }
    return 0;
}
