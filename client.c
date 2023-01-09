//TCP通信客户端
#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
int main()
{
    //1.创建套接字
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd==-1)
    {
        perror("socket");
        exit(-1);
    }
    //2.连接服务器
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    //转换成网络字节序
    inet_pton(AF_INET,"192.168.227.129",&serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(9999);
    int ret = connect(fd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
    if(ret==-1)
    {
        perror("connect");
        exit(-1);
    }   
    
    //3.通信

    char recvBuf[1024] = {0};
    char *data;
    while(1)
    {
        scanf("%s",data);
        //给服务器发送数据
        write(fd,data,strlen(data));
        //read 是阻塞的
        int len_recv = read(fd,recvBuf,sizeof(recvBuf));
        if(len_recv==-1)
        {
            perror("read");
            exit(-1);
        }
        else if(len_recv>0)
        {
            printf("recv server data : %s\n",recvBuf);
        }
        else if(len_recv==0)
        {
            //表示服务器端断开连接
            printf("server closed\n");
            break;
        }
    }
    //4.断开连接
    close(fd);
    return 0;
}