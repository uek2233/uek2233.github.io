#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main(int argc, char* argv[]){
    int recv_sock;
    char buf[BUF_SIZE];
    struct sockaddr_in adr;
    int str_len;
    if(argc != 2){
        printf("Usage : %s <PORT>\n", argv[0]);
        exit(1);
    }
    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(recv_sock == -1){
        perror("socket() error");
        exit(1);
    }
    memset(&adr, 0, sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    adr.sin_port = htons(atoi(argv[1]));
    if(bind(recv_sock, (struct sockaddr*)&adr, sizeof(adr)) == -1){
        perror("bind() error");
        exit(1);
    }
    while(1){
        str_len = recvfrom(recv_sock, buf, BUF_SIZE-1, 0, NULL, NULL);
        if(str_len < 0){
            perror("recvfrom() error");
            break;
        }
        buf[str_len] = 0;
        if(strcmp(buf, "EOF") == 0){
            printf("recvfrom() end");
            break;
        }
        printf("Received message: %s\n", buf);

    }
    close(recv_sock);
    return 0;
}
