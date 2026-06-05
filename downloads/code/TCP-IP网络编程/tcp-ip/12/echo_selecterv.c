

## ⭐

**示例**::  echo_selecterv.c

**说明**::  通过 `select` 实现 IO 复用服务器

**链接**:: https://cloudreve.xxh123.online/s/G7fG

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 1024

int main(int argc, char* argv[]){
    int serv_sock, clnt_sock;
    char buf[BUF_SIZE];
    struct sockaddr_in serv_adr, clnt_adr;
    struct timeval timeout;
    socklen_t adr_sz;
    fd_set reads, cpy_reads;
    int fd_max, fd_num, i, str_len;

    if(argc != 2){
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    printf("Server socket is %d\n", serv_sock);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1){
        perror("bind() error");
        exit(1);
    }
    if(listen(serv_sock, 5) == -1){
        perror("listen() error");
        exit(1);
    }
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock + 1;
    while(1){
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        cpy_reads = reads;
        if((fd_num = select(fd_max, &cpy_reads, NULL, NULL, &timeout)) == -1){
            perror("select() error");
            break;
        }
        if(fd_num == 0){
            printf("Time-out! \n");
            continue;
        }
        // 有客户端连接请求
        for(i=serv_sock;i<fd_max;i++){
            // 判断是否是有连接请求或者数据传输请求
            if(FD_ISSET(i, &cpy_reads)){
                // 有连接请求
                if(i == serv_sock){
                    adr_sz = sizeof(clnt_adr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                    // 将新的客户端套接字加入到监视集合中
                    FD_SET(clnt_sock, &reads);
                    // 更新 fd_max 的值
                    if(fd_max<clnt_sock+1){
                        fd_max = clnt_sock + 1;
                    }
                    printf("Connected client: %d \n", clnt_sock);
                }
                // 有数据传输请求
                else{
                    str_len = read(i, buf, BUF_SIZE);
                    // EOF 表示客户端关闭了连接
                    if(str_len == 0){
                        FD_CLR(i, &reads);
                        close(i);
                        printf("Closed client: %d \n", i);
                    }
                    // 回声服务
                    else{
                        write(i, buf, str_len);
                    }
                }
            }
        }
    }
    close(serv_sock);
    return 0;
}
```