#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define BUF_SIZE 1024

int main(int argc, char* argv[]){
    int sock;
    struct sockaddr_in recv_addr;
    char buffer[BUF_SIZE];
    if(argc != 3){
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    recv_addr.sin_port = htons(atoi(argv[2]));
    if(connect(sock, (struct sockaddr*)&recv_addr, sizeof(recv_addr)) == -1){
        printf("connect() error!\n");
        exit(1);
    }
    sleep(2);
    strcpy(buffer, "123");
    write(sock, buffer, strlen(buffer));
    strcpy(buffer, "4");
    send(sock, buffer, strlen(buffer), MSG_OOB);
    strcpy(buffer, "567");
    write(sock, buffer, strlen(buffer));
    strcpy(buffer, "890");
    send(sock, buffer, strlen(buffer), MSG_OOB);
    close(sock);
    return 0;
}
