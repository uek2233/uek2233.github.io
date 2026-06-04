#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

int acpt_sock, recv_sock;

// 只会收到一个字节
void urg_handler(int sig){
    char buffer[BUF_SIZE];
    int str_len;
    str_len = recv(recv_sock, buffer, BUF_SIZE-1, MSG_OOB);
    buffer[str_len] = 0;
    printf("Urgent message: %s\n", buffer);
}


int main(int argc, char* argv[]){
    struct sockaddr_in recv_addr, send_addr;
    socklen_t send_addr_sz;
    struct sigaction act;
    char buffer[BUF_SIZE];
    int str_len;
    if(argc != 2){
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGURG, &act, 0);
    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(acpt_sock, (struct sockaddr*)&recv_addr, sizeof(recv_addr)) == -1){
        printf("bind() error!\n");
        exit(1);
    }
    if(listen(acpt_sock, 5) == -1){
        printf("listen() error!\n");
        exit(1);
    }
    send_addr_sz = sizeof(send_addr);
    printf("Waiting for a connection...\n");
    recv_sock = accept(acpt_sock, (struct sockaddr*)&send_addr, &send_addr_sz);
    if(recv_sock == -1){
        printf("accept() error!\n");
        exit(1);
    }
    // 控制文件描述符的所有权，使得当有紧急数据到达时，SIGURG 信号能够发送给当前进程
    fcntl(recv_sock, F_SETOWN, getpid());
    while((str_len = recv(recv_sock, buffer, BUF_SIZE-1, 0)) != 0){
        if(str_len == -1){
            continue;
        }
        buffer[str_len] = 0;
        printf("Received data: %s\n", buffer);
    }
    close(recv_sock);
    close(acpt_sock);
    return 0;
}
