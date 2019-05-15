#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>
#include <fcntl.h>

#define EXIT "exit"
#define ECHO_PATH "echo $PATH"
#define PATH "PATH"

char *my_argv[100];
char *env[100];
char *path[10];

void fill_argv(char *tmp_argv){
    char *commands = tmp_argv;
    int i = 0; //looking for how many flags
    char ret[100];
    memset(ret, 0, 100);
    while(*commands != '\0') {
        if(*commands == ' ') {
            if(my_argv[i] == NULL)
                my_argv[i] = (char *)malloc(sizeof(char) * strlen(ret) + 1);
            else
                memset(my_argv[i], 0, strlen(my_argv[i]));
            strncpy(my_argv[i], ret, strlen(ret));
            strncat(my_argv[i], "\0", 1);
            memset(ret, 0, 100);
            i++;
        } else
            strncat(ret, commands, 1);
        commands++;
    }
    my_argv[i] = (char *)malloc(sizeof(char) * strlen(ret) + 1);
    strncpy(my_argv[i], ret, strlen(ret));
    strncat(my_argv[i], "\0", 1);
}

void copy_env(char **env1){
    for(int i = 0; env[i] != NULL; i++) {
        env1[i] = (char *)malloc(sizeof(char) * (strlen(env1[i]) + 1));
        memcpy(env1[i], env1[i], strlen(env1[i]));
    }
}

void get_path(char **tmp_env, char *path_str){
    int i = 0;
    char *tmp;
    while(1) {
        tmp = strstr(tmp_env[i], PATH);
        if(tmp == NULL)
            i++;
        else
            break;
    }
    strncpy(path_str, tmp, strlen(tmp));

    i = 0;
    tmp = path_str;
    char ret[100];

    while(*tmp != '=')
        tmp++;
    tmp++;

    while(*tmp != '\0') {
        if(*tmp == ':') {
            strncat(ret, "/", 1);
            path[i] = (char *) malloc(sizeof(char) * (strlen(ret) + 1));
            strncat(path[i], ret, strlen(ret));
            strncat(path[i], "\0", 1);
            i++;
            memset(ret, 0, 100);
        } else
            strncat(ret, tmp, 1);
        tmp++;
    }
}

void free_end(char* tmp, char* path_str){
    free(tmp);
    free(path_str);
    for(int i = 0; env[i] != NULL; i++)
        free(env[i]);
    for(int i = 0; i < 10; i++)
        free(path[i]);
}

void free_argv(){
    for(int i = 0; my_argv[i] != NULL; i++) {
        memset(my_argv[i], 0, strlen(my_argv[i])+1);
        my_argv[i] = NULL;
        free(my_argv[i]);
    }
}

void start_signal(){
    printf("\nshell> ");
    fflush(stdout);
}

int main(int argc, char *argv[], char *env1[]){
    char c;
    int fd;
    char *tmp = (char *)malloc(sizeof(char) * 100);
    char *path_str = (char *)malloc(sizeof(char) * 256);
    char *cmd = (char *)malloc(sizeof(char) * 100);
    char *newcmd = (char *)malloc(sizeof(char) * 100);

    signal(SIGINT, SIG_IGN);
    signal(SIGINT, start_signal);

    copy_env(env1);
    get_path(env1, path_str);

    if(fork() == 0) {
        execve("/usr/bin/clear", argv, env1);
        exit(1);
    } else {
        wait(NULL);
    }
    printf("shell> ");
    fflush(stdout);
    while(c != EOF) {
        c = getchar();
        switch(c) {
            case '\n':
                if(tmp[0] == '\0')
                    printf("shell> ");
                else {
                    fill_argv(tmp);
                    strncpy(cmd, my_argv[0], strlen(my_argv[0]));
                    strncat(cmd, "\0", 1);
                    strncpy(newcmd, "/bin/", strlen("/bin/"));
                    strncat(newcmd, cmd, strlen(cmd));

                    if (strcmp(EXIT, cmd) == 0) {
                        free_end(tmp, path_str);
                        return 0;
                    } else if (strcmp(ECHO_PATH, tmp) == 0) {
                        printf("%s\n", path_str);
                        free_argv();
                        memset(newcmd, 0, 100);
                        memset(cmd, 0, 100);
                        printf("shell> ");

                    } else {
                        if ((fd = open(newcmd, O_RDONLY)) > 0) { //read only O_RDONLY
                            close(fd);
                            pid_t pid;
                            int i;
                            pid = fork();
                            if (pid == 0) {
                                i = execve(newcmd, my_argv, env1);
                                if (i < 0) {
                                    printf("Failed to Execute: %s\n", newcmd);
                                    exit(1);
                                }
                            } else
                                wait(NULL);
                        } else
                            printf("Failed to Execute: %s\n", cmd);
                        free_argv();
                        memset(newcmd, 0, 100);
                        memset(cmd, 0, 100);
                        printf("shell> ");
                    }
                    memset(tmp, 0, 100);
                    break;
                }
            default: strncat(tmp, &c, 1);
                break;
        }
    }
    free_end(tmp, path_str);
    return 0;
}
