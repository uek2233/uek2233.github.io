#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig){
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status)){
        printf("Removed proc id: %d \n", id);
        printf("Child send: %d \n", WEXITSTATUS(status));
    }
}


int main(int argc, char* argv[]){
    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);
    pid = fork();
    if(pid == 0){
        printf("Hi, I am a child process 1.\n");
        sleep(2);
        return 12;
    }
    else{
        printf("Child proc id: %d\n", pid);
        pid = fork();
        if(pid == 0){
            printf("Hi, I am a child process 2.\n");
            sleep(9);
            return 24;
        }
        else{
            int i;
            for(i=0; i<5; i++){
                printf("wait...\n");
                sleep(5);
            }
        }
    }
    return 0;
}
