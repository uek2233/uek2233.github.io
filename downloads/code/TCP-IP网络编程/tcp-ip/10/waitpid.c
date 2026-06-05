

## ⭐

**示例**::  waitpid.c

**说明**::  通过非阻塞的方法来结束子进程

```c
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
```