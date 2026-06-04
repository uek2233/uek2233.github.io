#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define BUF_SIZE 512

void read_childproc(int sig){
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (id > 0 && WIFEXITED(status)) {
        printf("Removed proc id: %d \n", id);
        printf("Child send: %d \n", WEXITSTATUS(status));
    }
}

int main(int argc, char* argv[]){
    int fds1[2], fds2[2];
    char str1[] = "Who are you?";
    char str2[] = "Thank you for your message.";
    char buf[BUF_SIZE] = {0,};
    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    if(pipe(fds1) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }
    if(pipe(fds2) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }
    pid = fork();
    if(pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if(pid == 0){
        write(fds1[1], str1, sizeof(str1));
        read(fds2[0], buf, BUF_SIZE);
        printf("Child proc output: %s\n", buf);
        return 0;
    }
    else{
        read(fds1[0], buf, BUF_SIZE);
        printf("Parent proc output: %s\n", buf);
        write(fds2[1], str2, sizeof(str2));
        sleep(3);
    }
    return 0;
}
