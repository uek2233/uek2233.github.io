#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

void timeout(int sig){
    if(sig == SIGALRM){
       printf("Time out!\n");
    }
    alarm(2);
}

int main(int argc, char* argv[]){
    int i;
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = timeout;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, NULL);
    alarm(2);
    for(i=0;i<3;i++){
        printf("wait...\n");
        sleep(10);
    }
    return 0;
}
