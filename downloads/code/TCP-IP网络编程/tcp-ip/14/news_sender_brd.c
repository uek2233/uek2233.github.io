

## ⭐

**示例**::  news_sender_brd.c

**说明**::  广播发送者

**链接**::  https://cloudreve.xxh123.online/s/D4uw

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main(int argc, char* argv[]){
    int send_sock;
    FILE* fp;
    char buf[BUF_SIZE];
    int so_brd = 1;
    struct sockaddr_in broad_adr;
    if(argc != 3){
        printf("Usage : %s <GroupIP> <PORT>\n", argv[0]);
        exit(1);
    }
    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(send_sock == -1){
        perror("socket() error");
        exit(1);
    }
    memset(&broad_adr, 0, sizeof(broad_adr));
    broad_adr.sin_family = AF_INET;
    broad_adr.sin_addr.s_addr = inet_addr(argv[1]);
    broad_adr.sin_port = htons(atoi(argv[2]));
    setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));
    if((fp = fopen("/root/TCP-IP/14.2/news.txt", "r")) == NULL){
        perror("fopen() error");
        exit(1);
    }
    while(fgets(buf, BUF_SIZE, fp) != NULL){
        sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*)&broad_adr, sizeof(broad_adr));
        sleep(2);
    }
    sendto(send_sock, "EOF", 3, 0, (struct sockaddr*)&broad_adr, sizeof(broad_adr));
    close(send_sock);
    return 0;
}
```