// TCP通信的服务器端
#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
int main()
{

    // 1.创建socket（用于监听的套接字）
    int lfd = socket(AF_INET,SOCK_STREAM,0);

    if(lfd==-1)
    {
        perror("socket");
        exit(-1);
    }

    // 2.绑定
    struct sockaddr_in saddr;
    saddr.sin_family = PF_INET;
    //转化成网络字节序
    //inet_pton(AF_INET,"192.168.227.129", saddr.sin_addr.s_addr);
    saddr.sin_addr.s_addr = INADDR_ANY;    //表示机器上所有网卡ip都可以进行连接
    saddr.sin_port = htons(9999);
    int ret = bind(lfd,(struct sockaddr *)&saddr,sizeof(saddr));
    if(ret==-1)
    {
        perror("bind");
        exit(-1);
    }

    //3.监听
    ret = listen(lfd,10);
    if(ret==-1)
    {
        perror("listen");
        exit(-1);
    }

    //4.接受客户端连接
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    //accept 是阻塞的
    int cfd = accept(lfd,(struct sockaddr *)&clientaddr,&len);
    if(cfd==-1)
    {
        perror("accept");
        exit(-1);
    }

    //输出客户端的信息
    char clientIP[16];
    inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,clientIP,sizeof(clientIP));
    unsigned short clientPort = ntohs(clientaddr.sin_port);
    printf("client ip is %s,port is %d\n",clientIP,clientPort);

    // 5.通信
    // 获取客户端的数据
    char recvBuf[1024] = {0};
    while(1)
    {
        //read 是阻塞的
        int len_recv = read(cfd,recvBuf,sizeof(recvBuf));
        if(len_recv==-1)
        {
            perror("read");
            exit(-1);
        }
        else if(len_recv>0)
        {
            printf("recv client data : %s\n",recvBuf);
        }
        else if(len_recv==0)
        {
            //表示客户端断开连接
            printf("client closed\n");
            break;
        }

        // 给客户端发送数据
        char *data = "hello,i am server";
        write(cfd,data,strlen(data));
    }

    //关闭文件描述符
    close(lfd);
    close(cfd);
    return 0;
}
