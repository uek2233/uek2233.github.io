#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define BUF_SIZE 1024

void read_routine(int sock, char* buf);
void write_routine(int sock, char* buf);

int main(int argc, char *argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    int str_len = 0, read_len = 0;
    pid_t pid;
    int status;
    if(argc!=3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        perror("socket() error");
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));    
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) ==-1){
        perror("connect() error");
    }
    else{
        puts("connected......");
    }
    pid = fork();
    if(pid == 0){ // 子进程：发送消息
        write_routine(sock, message);
    }
    else{
        read_routine(sock, message);
    }
    while(!waitpid(pid, &status, WNOHANG)){
        sleep(3);
    }
    if(WIFEXITED(status)){
        printf("message send by client is ended.\n");
    }
    close(sock);
    return 0;
}

void read_routine(int sock, char* buf){
    int str_len = 0;
    while(1){
        str_len = read(sock, buf, BUF_SIZE);
        if(str_len == 0){
            return;
        }
        buf[str_len] = 0;
        printf("Message from server : %s \n", buf);
    }
}

void write_routine(int sock, char* buf){
    while(1){
        fputs("input message(Q or q to quit): ", stdout);
        fgets(buf, BUF_SIZE, stdin);
        if(!strcmp(buf, "Q\n") || !strcmp(buf, "q\n")){
            shutdown(sock, SHUT_WR); // 发送 FIN 包，告诉服务器不再发送数据了
            return;
        }
        write(sock, buf, strlen(buf));
    }  
}

