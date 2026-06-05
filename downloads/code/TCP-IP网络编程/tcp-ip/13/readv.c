

## ⭐

**示例**::  readv.c

**说明**::  readv 函数对数据进行整合传输

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>

#define BUF_SIZE 100

int main(int argc, char* argv[]){
    int str_len;
    struct iovec vec[2];
    char buf1[BUF_SIZE] = {0,};
    char buf2[BUF_SIZE] = {0,};;
    vec[0].iov_base = buf1;
    vec[0].iov_len = 5;
    vec[1].iov_base = buf2;
    vec[1].iov_len = BUF_SIZE;
    str_len = readv(0, vec, 2);
    printf("str_len: %d \n", str_len);
    printf("first message: %s \n", buf1);
    printf("second message: %s \n", buf2);
    return 0;
}
    
```