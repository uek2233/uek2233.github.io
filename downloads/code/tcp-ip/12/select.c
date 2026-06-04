#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 512

int main(int argc, char* argv[]){
    fd_set reads, temps;
    int res, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;
    FD_ZERO(&reads);
    FD_SET(0, &reads); // 0 is the file descriptor for standard input


    while(1){
        temps = reads;
        timeout.tv_sec = 5; // 5 seconds
        timeout.tv_usec = 0;
        res = select(1, &temps, 0, 0, &timeout);
        if(res == -1){
            perror("select() error");
            break;
        }
        else if(res == 0){
            printf("Time-out! \n");
        }
        else{
            if(FD_ISSET(0, &temps)){
                str_len = read(0, buf, BUF_SIZE);
                if(str_len == -1){
                    perror("read() error");
                    break;
                }
                buf[str_len] = 0; // null terminate the string
                printf("Message from console: %s", buf);
            }
        }

    }

    return 0;
}
