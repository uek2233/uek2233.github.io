#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/socket.h>

volatile sig_atomic_t running = 1;
#define BUF_SIZE 1024

void read_childproc(int sig);
void keyctrlC(int sig);

int main(int argc, char* argv[]){
    int fds[2];
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    struct sigaction act;
    int recv_len;
    char buf[BUF_SIZE];
    pid_t pid = 0;
    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    // 子进程结束信号函数注册
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);
    // ctrl+c 信号函数注册
    act.sa_handler = keyctrlC;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;          // 不加 SA_RESTART
    sigaction(SIGINT, &act, 0);

    // 创建服务器套接字
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        perror("bind() error\n");
    }
    if(listen(serv_sock, 5) == -1){
        perror("listen() error\n");
    }
    // 创建管道
    if(pipe(fds) == -1){
        perror("pipe() error\n");
    }
    // 写文件子进程，子进程从 fork 执行 到 return 0
    pid = fork();
    if(pid == 0){
        FILE* fp = fopen("echomsg.txt", "wt");
        char msgbuf[BUF_SIZE];
        int i, len;
        for(i=0;i<10;i++){
            len = read(fds[0], msgbuf, BUF_SIZE);
            fwrite((void*)msgbuf, 1, len, fp);
        }
        fclose(fp);
        return 0;
    }
    // 写文件子进程结束
    while(running){
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if(clnt_sock == -1){
            continue;
        }
        else{
            printf("Connected new client %d \n", clnt_sock);
        }
        pid_t pid = fork();
        if(pid == -1){
            close(serv_sock);
            close(clnt_sock);
            continue;
        }
        // 接收数据子进程
        if(pid == 0){
            close(serv_sock);
            while((recv_len = recv(clnt_sock, buf, BUF_SIZE, 0)) != 0){
                send(clnt_sock, buf, recv_len, 0);
                write(fds[1], buf, recv_len);
            }
            close(clnt_sock);
            printf("Client %d disconnected.\n", clnt_sock);
            return 0;
        }
        // 接收数据子进程结束
        // 父进程
        else{
            close(clnt_sock);
        }
    }
    close(serv_sock);
    return 0;
}

void read_childproc(int sig){
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status)){
        printf("Removed proc id: %d \n", id);
        printf("Child send: %d \n", WEXITSTATUS(status));
    }
}

void keyctrlC(int sig){
    if(sig == SIGINT){
        printf("Ctrl-C pressed!\n");
        running = 0;
    }
}
