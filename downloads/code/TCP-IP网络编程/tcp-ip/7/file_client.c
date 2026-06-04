

## ⭐

**示例**::  file_client.c

**说明**::  基于 TCP 半关闭来接受文件

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 32

int main(int argc, char *argv[]){
    int sd;
    char buf[BUF_SIZE] = {0};
    int read_cnt;
    struct sockaddr_in serv_addr;
    FILE* fd;
    if(argc!=3){
        printf("Usage : %s <ip> <port>\n", argv[0]);
        exit(1);
    }
    fd = fopen("./received_file.txt", "wb");
    if(!fd){
        perror("fopen error");
        exit(1);
    }
    sd = socket(PF_INET, SOCK_STREAM, 0);
    if(sd == -1){
        perror("socket() error");
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if(connect(sd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        perror("connect() error");
    }
    printf("等待消息......\n");
    while((read_cnt = recv(sd, buf, BUF_SIZE, 0)) != 0){
        fwrite((void*)buf, 1, read_cnt, fd);
    }
    strcpy(buf, "文件接收成功！");
    printf("消息接收完毕！\n");
    send(sd, buf, sizeof(buf), 0);
    close(sd);
    fclose(fd);
    return 0;
}
```