#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig){
    if(sig == SIGALRM){
       printf("Time out!\n");
    }
    alarm(2);
}

void keyctl(int sig){
    if(sig == SIGINT){
        printf("Ctrl-C pressed!\n");
    }
}
int main(int argc, char* argv[]){
    int i;
    signal(SIGALRM, timeout);
    signal(SIGINT, keyctl);
    alarm(2);
    for(i=0;i<3;i++){
        printf("wait...\n");
        sleep(100);
    }
    return 0;
}
