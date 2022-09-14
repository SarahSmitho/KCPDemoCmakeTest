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
#define BUF_SIZE    6000

SOCKET socketSrv;
SOCKADDR_IN addrSrv;
int hr;
SOCKADDR_IN addrClient;
char buf[BUF_SIZE];
char kcpbuf[BUF_SIZE];
char blankbuf[BUF_SIZE];

int udp_output(const char *buf, int len, ikcpcb *kcp, void *user) {
    int res = sendto(socketSrv, buf, len, 0, (SOCKADDR *) user, sizeof(addrClient));
    printf("返回ack包 大小为 %d\n",res);
    return 0;
}


int main(void) {
    WSADATA wsd;
    SOCKET s;
    int nRet;

    // 初始化套接字动态库
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0) {
        printf("WSAStartup failed !\n");
        return 1;
    }

    // 创建套接字
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == INVALID_SOCKET) {
        printf("socket() failed ,Error Code:%d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    socketSrv = socket(AF_INET, SOCK_DGRAM, 0);
    int len = sizeof(SOCKADDR);

    //设置为非阻塞模式
    int imode = 1;
    nRet = ioctlsocket(socketSrv, FIONBIO, (u_long *) &imode);
    if (nRet == SOCKET_ERROR) {
        printf("ioctlsocket failed!");
        closesocket(socketSrv);
        WSACleanup();
        return -1;
    }

    // 设置服务器地址
    ZeroMemory(buf, BUF_SIZE);
    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(5000);

    // 绑定套接字
    nRet = bind(socketSrv, (SOCKADDR *) &addrSrv, sizeof(SOCKADDR));
    if (SOCKET_ERROR == nRet) {
        printf("bind failed !\n");
        closesocket(s);
        WSACleanup();
        return -1;
    }

    // 从客户端接收数据
    printf(" server started..\n");
    ikcpcb *kcp = ikcp_create(1, (void *) &addrClient);
    kcp->output = udp_output;
    ikcp_nodelay(kcp, 1, 10, 2, 1);
    ikcp_wndsize(kcp, 512, 512);
    kcp->rx_minrto = 10;


    int count=0;
    while (1) {
        //Sleep(100);
        ikcp_update(kcp, clock());
        // 打印来自客户端发送来的数据
        while (1) {
            nRet = recvfrom(socketSrv, buf, BUF_SIZE, 0, (SOCKADDR *) &addrClient, &len);
            if (nRet < 0){
                int y = WSAGetLastError();
                printf(" WSAGetLastError();   %d\n", y);
            }
            printf(" udp收到数据  %s\n", buf+24);
            ikcp_input(kcp, buf, nRet);
            ikcp_update(kcp, clock());
            ikcp_flush(kcp);

            hr = ikcp_recv(kcp, kcpbuf, BUF_SIZE);
            if (hr > 0) {
                count = count + 1;
            }
            printf("I recv :%s,count is  %d  \n", kcpbuf, count);
            // 没有收到包就退出
            //if (hr < 0) break;
            // 如果收到包就回射一个空包，里面应该是有KCP里处理好的包序号以及Ack信息
            ikcp_send(kcp, blankbuf, strlen(blankbuf) + 1);
            ikcp_update(kcp, clock());
            ikcp_flush(kcp);
        }

        while (1) {
            hr = ikcp_recv(kcp, kcpbuf, BUF_SIZE);
            if (hr > 0) {
                count = count + 1;
            }
            printf("I recv :%s,count is  %d  \n", kcpbuf, count);
            // 没有收到包就退出
            if (hr < 0) break;
            // 如果收到包就回射一个空包，里面应该是有KCP里处理好的包序号以及Ack信息
            ikcp_send(kcp, blankbuf, strlen(blankbuf) + 1);
        }
    }
    closesocket(s);
    WSACleanup();
    return 0;
}
