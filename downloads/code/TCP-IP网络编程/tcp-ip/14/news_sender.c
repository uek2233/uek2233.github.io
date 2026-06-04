

## ⭐

**示例**::  news_sender.c

**说明**::  多播的发送者

**链接**::  https://cloudreve.xxh123.online/s/jmcb

```c
#define BUF_SIZE 1024

int main(int argc, char* argv[]){
    int send_sock;
    FILE* fp;
    char buf[BUF_SIZE];
    int time_live = TTL;
    struct sockaddr_in mul_adr;
    if(argc != 3){
        printf("Usage : %s <GroupIP> <PORT>\n", argv[0]);
        exit(1);
    }
    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(send_sock == -1){
        perror("socket() error");
        exit(1);
    }
    memset(&mul_adr, 0, sizeof(mul_adr));
    mul_adr.sin_family = AF_INET;
    mul_adr.sin_addr.s_addr = inet_addr(argv[1]);
    mul_adr.sin_port = htons(atoi(argv[2]));
    setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, &time_live, sizeof(time_live));
    if((fp = fopen("/root/TCP-IP/14.1/news.txt", "r")) == NULL){
        perror("fopen() error");
        exit(1);
    }
    while(fgets(buf, BUF_SIZE, fp) != NULL){
        sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*)&mul_adr, sizeof(mul_adr));
        sleep(2);
    }
    return 0;
}
```

