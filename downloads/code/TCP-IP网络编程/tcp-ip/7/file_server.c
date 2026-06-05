

## ⭐

**示例**::  file_server.c

**说明**::  基于 TCP 半关闭来发送文件

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 32
#define file_name "/root/TCP-IP/7.1/send.txt"

int main(int argc, char *argv[]){
    FILE* fp;
    int serv_sd, clnt_sd;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    char buf[BUF_SIZE];
    int read_cnt;
    if(argc != 2){
        printf("usage %s <port>\n", argv[0]);
    }
    fp = fopen(file_name, "rb");
    if(!fp){
        perror("fopen error\n");
        exit(1);
    }
    serv_sd = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        perror("bind error\n");
    }
    listen(serv_sd, 5);
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sd = accept(serv_sd, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

    while(1){
        read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
        if(read_cnt < BUF_SIZE){
            send(clnt_sd, buf, read_cnt, 0);
            break;
        }
        send(clnt_sd, buf, read_cnt, 0);
    }
    shutdown(clnt_sd, SHUT_WR);
    read_cnt = read(clnt_sd, buf, BUF_SIZE);
    buf[read_cnt] = 0;
    printf("Message from client: %s \n", buf);
    fclose(fp);
    close(clnt_sd);
    close(serv_sd);
    return 0;
}
```