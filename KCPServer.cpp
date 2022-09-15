/*
////////////////////////////////////////////////////////////////////////////
//// UDPServer.cpp
//https://www.cnblogs.com/findumars/p/5928712.html

#include <stdio.h>
#include <WINSOCK2.H>
#include <time.h>
#include <fcntl.h>
#include <strings.h>
#include <stdlib.h>

#include "ikcp.h"

#pragma comment(lib, "WS2_32.lib")
#define BUF_SIZE    128

SOCKET socketSrv;
//存储地址和IP的结构体  服务端地址、IP的结构体变量
SOCKADDR_IN addrSrv;
int hr;
//客户端地址、IP的结构体
SOCKADDR_IN addrClient;
char buf[6000];
char kcpbuf[6000];
char recvbuf[BUF_SIZE];

int udp_output(const char *buf, int len, ikcpcb *kcp, void *user) {
    int nRet = sendto(socketSrv, buf, len, 0, (SOCKADDR *) user, sizeof(addrClient));
    //printf("KCP start udp_output callback by ikcp_output\n");
    //printf("total data:  %d byte \n", nRet);
    return 0;
}


int main(void) {
    //WSA全称是Windows Sockets API
    WSADATA wsd;
    int nRet;

    // 初始化套接字动态库
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0) {
        printf("WSAStartup failed !\n");
        return 1;
    }

    // 创建套接字
    socketSrv = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketSrv == INVALID_SOCKET) {
        printf("socket() failed ,Error Code:%d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    int len = sizeof(SOCKADDR);

    //设置为非阻塞模式
    int imode = 1;
    //ioctl(input/output control)
    // If iMode = 0, blocking is enabled;  If iMode != 0, non-blocking mode is enabled.
    nRet = ioctlsocket(socketSrv, FIONBIO, (u_long *) &imode);
    if (nRet == SOCKET_ERROR) {
        printf("ioctlsocket failed!");
        closesocket(socketSrv);
        WSACleanup();
        return -1;
    }

    // 设置服务器地址
    ZeroMemory(buf, BUF_SIZE);
    //设置Socket　本机地址IP
    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    //AF_INET 是IPv4的地址系列
    addrSrv.sin_family = AF_INET;
    //设置Socket　端口
    addrSrv.sin_port = htons(5000);

    // 绑定套接字
    //将socket绑定（bind)一个本地地址和端口上  绑定待链接套接字bind(Socket,服务器ip和端口)
    nRet = bind(socketSrv, (SOCKADDR *) &addrSrv, sizeof(SOCKADDR));
    if (SOCKET_ERROR == nRet) {
        printf("bind failed !\n");
        closesocket(socketSrv);
        WSACleanup();
        return -1;
    }

    // 从客户端接收数据
    printf(" server started..\n");

    //ikcp_create返回的是分配内存的地址
    ikcpcb *kcp = ikcp_create(1, (void *) &addrClient);
    ikcp_setoutput(kcp, udp_output);
    //kcp->output = udp_output;
    int count;
    ikcp_wndsize(kcp, 128, 128);

    // 打印来自客户端发送来的数据
    while (1) {
        //strcpy(buf,"Hello world!");
        long current = clock();
        //Sleep(1000);
        //printf("buf value:%s\n",buf);
        printf(" recvfrom  start\n");
        //等待信息recvfrom(Socket,对端地址)
        nRet = recvfrom(socketSrv, buf, 6000, 0, (SOCKADDR *) &addrClient, &len);
        printf("receive content:%s, length of buf:%d\n", buf + 24, strlen(buf));
        printf(" recvfrom end,nRet return value %d\n", nRet);
        if (nRet < 0) {
            int y = WSAGetLastError();
            printf(" WSAGetLastError();   %d\n", y);
        }
        //if(nRet < 0)break;
        */
/* ikcp_input 是一个从网络接收数据的函数。 *//*

        ikcp_input(kcp, buf, nRet);
        printf(" ikcp_input receive content:%s \n", buf + 24);

        ikcp_update(kcp, current);
        ikcp_flush(kcp);

        //最后数据从kcp给到了buf,你要用新的buf来装
        int size = ikcp_recv(kcp, kcpbuf, 6000);
        printf(" ikcp_recv  receive content:%s \n", kcpbuf);
        if (size > 0) {
            count = count + 1;
            //printf("I recv :%s,count is  %d  \n", buf,count);
        }
        printf("I recv :%s,count is  %d  \n", kcpbuf, count);
        ikcp_send(kcp, recvbuf, strlen(buf)+1);

        //memset(buf, '\0', sizeof(buf));
        //memset(kcpbuf, '\0', sizeof(kcpbuf));
    }

    closesocket(socketSrv);
    WSACleanup();
    return 0;
}
*/


////////////////////////////////////////////////////////////////////////////
//// UDPServer.cpp
//https://www.cnblogs.com/findumars/p/5928712.html

#include <stdio.h>
#include <WINSOCK2.H>
#include <time.h>
#include <fcntl.h>
#include <strings.h>
#include <stdlib.h>

#include "ikcp.h"

#pragma comment(lib,"WS2_32.lib")
#define BUF_SIZE   1024*500

SOCKET      socketSrv ;
SOCKADDR_IN addrSrv;
int hr;
SOCKADDR_IN addrClient;
char        buf[BUF_SIZE];

int udp_output(const char *buf, int len, ikcpcb *kcp, void *user)
{
    int res = sendto(socketSrv,buf, len,0,(SOCKADDR*)user,sizeof(addrClient));
    return 0;
}


int main(void)
{
    WSADATA wsd;
    SOCKET  s;
    int     nRet;

    // 初始化套接字动态库
    if(WSAStartup(MAKEWORD(2,2),&wsd) != 0)
    {
        printf("WSAStartup failed !\n");
        return 1;
    }

    // 创建套接字
    s = socket(AF_INET,SOCK_DGRAM,0);
    if(s == INVALID_SOCKET)
    {
        printf("socket() failed ,Error Code:%d\n",WSAGetLastError());
        WSACleanup();
        return 1;
    }

    socketSrv = socket(AF_INET,SOCK_DGRAM,0);
    int         len = sizeof(SOCKADDR);

    //设置为非阻塞模式
    int imode=1;
    nRet=ioctlsocket(socketSrv,FIONBIO,(u_long *)&imode);
    if(nRet == SOCKET_ERROR)
    {
        printf("ioctlsocket failed!");
        closesocket(socketSrv);
        WSACleanup();
        return -1;
    }

    // 设置服务器地址
    ZeroMemory(buf,BUF_SIZE);
    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(5000);

    // 绑定套接字
    nRet = bind(socketSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
    if(SOCKET_ERROR == nRet)
    {
        printf("bind failed !\n");
        closesocket(s);
        WSACleanup();
        return -1;
    }

    // 从客户端接收数据
    printf(" server started..\n");
    ikcpcb *kcp = ikcp_create(1, (void*)&addrClient);
    kcp->output = udp_output;
    ikcp_nodelay(kcp, 1, 10, 2, 1);
    ikcp_wndsize(kcp, 2048, 2048);
    kcp->rx_minrto = 10;

    while(1)
    {
        //Sleep(100);
        ikcp_update(kcp, clock());
        // 打印来自客户端发送来的数据
        while(1)
        {
            nRet = recvfrom(socketSrv,buf,BUF_SIZE,0,(SOCKADDR*)&addrClient,&len);
            if(nRet < 0)break;
            ikcp_input(kcp,buf,nRet);
        }

        while (1)
        {
            hr = ikcp_recv(kcp, buf, BUF_SIZE);
            // 没有收到包就退出
            if (hr < 0) break;
            // 如果收到包就回射
            printf("I recv :%s \n",buf);
            ikcp_send(kcp, buf, strlen(buf)+1);
            ikcp_update(kcp, clock());
        }
        ZeroMemory(buf,BUF_SIZE);
    }
    closesocket(s);
    WSACleanup();
    return 0;
}
