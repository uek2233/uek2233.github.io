

## ⭐

**示例**::  wait.c

**说明**::  销毁子进程

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    int  status;
    pid_t pid;
    pid = fork();
    if(pid == 0){
        printf("Hi, I am a child process 1.\n");
        printf("End child process 1.\n");
        return 3;
    }
    else{
        printf("Hi, I am a parent process.\n");
        printf("Child PID: %d \n", pid);
        pid = fork();
        if(pid == 0){
            printf("Hi, I am a child process 2.\n");
            printf("End child process 2.\n");
            exit(7);
        }
        else{
            printf("Child PID: %d \n", pid);
            wait(&status);
            if(WIFEXITED(status)){
                printf("Child process 1 exited with status %d.\n", WEXITSTATUS(status));
            }
            wait(&status);
            if(WIFEXITED(status)){
                printf("Child process 2 exited with status %d.\n", WEXITSTATUS(status));
            }
            sleep(30);
        }
    }
    return 0;
}
```