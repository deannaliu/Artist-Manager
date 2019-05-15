#include "defs.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// Assigned = 1 //
// Waiting = 0 //

typedef struct artists{
    int pid;
    int status;
    struct artists* next;
}artist;

artist* head = NULL;

int isNumber(char* input){
    int length = strlen (input);
    for (int i = 0; i < length; i++)
        if (!isdigit(input[i]))
            return 1;
    return 0;
}

void printHelpInformation(){
    cse320_printf("%-30s \t %s\n", "help:", "You are here.");
    cse320_printf("%-30s \t %s\n", "date:", "Prints the date.");
    cse320_printf("%-30s \t %s\n", "hire [N]:", "Hire [N] number of artists to do nothing.");
    cse320_printf("%-30s \t %s\n", "fire [X]:", "Fire artist [X].");
    cse320_printf("%-30s \t %s\n", "fireall:", "Fire all the artists.");
    cse320_printf("%-30s \t %s\n", "withdraw [X]:", "Announce withdrawal of artist [X].");
    cse320_printf("%-30s \t %s\n", "list:", "Lists all current artists and their status.");
    cse320_printf("%-30s \t %s\n", "exit:", "Exits the program");
}

void toString(char* to, char* from);

void printDate(char* command[]){
    if((fork()) == 0) { //child process
        char *env[] = {NULL};
        char *cmd = malloc(sizeof(char) * 100);
        strncat(cmd, "/bin/", sizeof("/bin/"));
        strncat(cmd, command[0], sizeof(command[0]));
        strncat(cmd, "\0", 1);
        execve(cmd, &command[0], env);
        exit(0);
    }
	sleep(1);
}

void hire(int N){
    for(int i = 0; i < N; i++){
        int x = fork();
        if(x == 0){
            while(1); //they are doing nothing -status = NOTHING
        }
        else{
            artist* current = head;
            artist* newArtist = malloc(sizeof(artist));
            newArtist->pid = x;
            newArtist->status = 0;
            if(current == NULL){
                newArtist->next = head;
                head = newArtist;
            }
            else {
                while(current->next != NULL) {
                    current = current->next;
                }
                newArtist->next = current->next;
                current->next = newArtist;
            }
        }
    }
}

void list(){
    artist* current = head;
    char *str = (char*)malloc(20 * sizeof(char));
    while(current!=NULL) {
        if (current->status == 1) {
            sprintf(str, "%d %s\n", current->pid, "ASSIGNED");
            cse320_print(str);
        } else if (current->status == 0) {
            sprintf(str, "%d %s\n", current->pid, "WAITING");
            cse320_print(str);
        }
        current = current->next;
    }
    free(current);
    free(str);
}

void handler(int sig) {
    switch (sig){
        case SIGUSR1:
            cse320_printf("ASSIGN");
        case SIGUSR2:
            cse320_printf("WITHDRAW");
    }
}

void withdraw(int id) {
    if (head == NULL) {
        cse320_printf("ARTIST %d DOES NOT EXIST\n", id);
        return;
    }
    artist *current = head;

    while (current != NULL) {
        if (current->pid == id && current->status == 0) {
            cse320_printf("ARTIST %d CANNOT BE WITHDRAWN FROM A NONEXISTENT JOB\n", id);
            return;
        }
        else if (current->pid == id && current->status == 1){
            cse320_printf("ARTIST %d IS WITHDRAWN FROM A JOB\n", id);
            current->status = 0;
            return;
        }
        else if(current->next == NULL && current->pid != id){ //accounting for assigning a nonexistent process
            cse320_printf("ARTIST %d DOES NOT EXIST\n", id);
            return;
        }
        else
            current = current->next;
    }
}

void assign(int id){
    signal(SIGUSR1, handler);
    if (head == NULL) {
        cse320_printf("ARTIST %d DOES NOT EXIST\n", id);
        return;
    }
    artist *current = head;

    while (current != NULL) {
        if (current->pid == id && current->status == 0) {
            current->status = 1;
            cse320_printf("ARTIST %d IS ASSIGNED TO A JOB\n", id);
            return;
        }
        else if (current->pid == id && current->status == 1){
            cse320_printf("ARTIST %d IS ALREADY ASSIGNED TO A JOB\n", id);
            return;
        }
        else if(current->next == NULL && current->pid != id){ //accounting for assigning a nonexistent process
            cse320_printf("ARTIST %d DOES NOT EXIST\n", id);
            return;
        }
        else
            current = current->next;
    }

}

void freeList(){
    artist* tmp;
    while (head != NULL){
        tmp = head;
        head = head->next;
        free(tmp);
    }
    head = NULL;
}

void fireall(){
    artist* current = head;
    while (current != NULL) {
        if(current->status == 0)
            kill(current->pid, SIGKILL);
        else {
            withdraw(current->pid);
            kill(current->pid, SIGKILL);
        }
        current = current->next;
    }
    freeList();
}

void fire(int id){
    if(head == NULL)
        return;
    if(head->pid == id){
        if(head->status == 1)
            withdraw(head->pid); // if status is assigned, need to withdraw firste
        artist* ptr = head;
        head = head->next;
        ptr->next = NULL;
        if(ptr == head)
            head = NULL;
        free(ptr);
        return;
    }

    artist* current = head->next;
    artist* prev = head;
    artist* tmp = NULL;

    while(current != NULL && prev != NULL){
        if(current->pid == id){
            if(current->status == 1)
                withdraw(head->pid); // if status is assigned, need to withdraw first
            kill(current->pid, SIGKILL);
            tmp = current;
            prev->next = current->next;
            free(tmp);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void exitthis(){
    fireall();
}



