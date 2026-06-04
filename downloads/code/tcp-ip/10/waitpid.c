#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    int status;
    pid_t pid = fork();

    if(pid == 0){
        sleep(15);
        return 24;
    }
    else{
        while(!waitpid(pid, &status, WNOHANG)){
            sleep(3);
            printf("Child process is still running...\n");
        }
        if(WIFEXITED(status)){
            printf("Child process exited with status %d.\n", WEXITSTATUS(status));
        }
    }
    return 0;
}
