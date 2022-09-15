/*
//////////////////////////////////////////////////////////////////////////
// UDPClient.cpp
//
#include <iostream>
#include <stdio.h>
#include <WINSOCK2.H>
#include <fcntl.h>
#include <time.h>
#include "ikcp.h"

using namespace std;

#pragma comment(lib,"WS2_32.lib")
#define BUF_SIZE    128

SOCKET      sockClient;// = socket(AF_INET,SOCK_DGRAM,0);
SOCKADDR_IN servAddr;       // 服务器套接字地址
int nServAddLen = sizeof(servAddr);

int udp_output(const char *buf, int len, ikcpcb *kcp, void *user){
    int res = sendto(sockClient,buf,len,0,(sockaddr *)user, nServAddLen);
    printf("sent msg len: %d\n", res);
    printf("sent msg: %s\n", buf+24);
    return 0;
}

int main(void)
{
    WSADATA wsd;
    SOCKET  s;

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
        printf("socket() failed, Error Code:%d\n",WSAGetLastError());
        WSACleanup();
        return 1;
    }

    char        buf[BUF_SIZE];  // 接受数据
    char        recvbuf[BUF_SIZE];  // 接受数据
    int         nRet;
    ZeroMemory(buf,BUF_SIZE);
    ZeroMemory(recvbuf,BUF_SIZE);
    sockClient = socket(AF_INET,SOCK_DGRAM,0);
    strcpy(buf,"Hello world!");
    printf("strcpy(buf   %s\n",buf);

    // 设置服务器地址
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(5000);
    int imode=1;
    nRet=ioctlsocket(sockClient,FIONBIO,(u_long *)&imode);
    if(nRet == SOCKET_ERROR)
    {
        printf("ioctlsocket failed!");
        closesocket(sockClient);
        WSACleanup();
        return -1;
    }

    ikcpcb *kcp = ikcp_create(1, (void*)&servAddr);
    kcp->output = udp_output;
    ikcp_nodelay(kcp, 2, 10, 2, 1);
    ikcp_wndsize(kcp, 128, 128);

    long current;
    long slap = clock() + 1000;;
    int index = 1;
    while(1){
        strcpy(buf,"Hello world!");
        //sendto(sockClient,buf,strlen(buf),0,(sockaddr *)&servAddr, nServAddLen);
        //Sleep(100);
        //printf("OK\n");
        current = clock();
        int res;

        for (; current >= slap; slap += 1) {
            printf("ikcp send: %d \n ", index);
            ikcp_send(kcp, buf, strlen(buf)+1);
            printf("ikcp_send buf: %s\n  ", buf);
            ikcp_update(kcp, clock());
            ikcp_flush(kcp);
            index++;
        }

        while(1){
            nRet = recvfrom(sockClient,recvbuf,BUF_SIZE,0,(sockaddr *)&servAddr,&nServAddLen);
            if(nRet < 0) break;
            ikcp_input(kcp, recvbuf, nRet);
        }

        memset(buf, '\0', sizeof(buf));
    }
    closesocket(s);
    WSACleanup();
    return 0;
}

*/


//////////////////////////////////////////////////////////////////////////
// UDPClient.cpp
//
#include <iostream>
#include <stdio.h>
#include <WINSOCK2.H>
#include <fcntl.h>
#include <time.h>
#include "ikcp.h"

using namespace std;

#pragma comment(lib,"WS2_32.lib")
#define BUF_SIZE    1024*500

SOCKET      sockClient;// = socket(AF_INET,SOCK_DGRAM,0);
SOCKADDR_IN servAddr;       // 服务器套接字地址
int nServAddLen = sizeof(servAddr);
int count1 = 0;
int datalength=0;

int udp_output(const char *buf, int len, ikcpcb *kcp, void *user){
    int res = sendto(sockClient,buf,len,0,(sockaddr *)user, nServAddLen);
    if (res > 0) {
        count1 = count1 + 1;
        datalength=datalength+res;
        printf("sent msg count1: %d,datalength is %d KB\n", count1,datalength/1024);

    }
    return 0;
}

int main(void)
{
    WSADATA wsd;
    SOCKET  s;

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
        printf("socket() failed, Error Code:%d\n",WSAGetLastError());
        WSACleanup();
        return 1;
    }

    char        buf[BUF_SIZE];  // 接受数据
    char        recvbuf[BUF_SIZE];  // 接受数据
    int         nRet;
    ZeroMemory(buf,BUF_SIZE);
    ZeroMemory(recvbuf,BUF_SIZE);
    sockClient = socket(AF_INET,SOCK_DGRAM,0);
    strcpy(buf,"Hello world!");

    // 设置服务器地址
    servAddr.sin_family = AF_INET;
    //172.16.31.55
    servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(5000);
    int imode=1;
    nRet=ioctlsocket(sockClient,FIONBIO,(u_long *)&imode);
    if(nRet == SOCKET_ERROR)
    {
        printf("ioctlsocket failed!");
        closesocket(sockClient);
        WSACleanup();
        return -1;
    }

    ikcpcb *kcp = ikcp_create(1, (void*)&servAddr);
    kcp->output = udp_output;
    ikcp_nodelay(kcp, 1, 5, 2, 1);
    ikcp_wndsize(kcp, 2048, 2048);
    kcp->rx_minrto = 10;

    long current;
    long slap = clock() + 1000;;
    int index = 1;
    while(1){
        //Sleep(100);
        //printf("OK\n");
        current = clock();
        ikcp_update(kcp, clock());

        int res;

        for (; current >= slap; slap += 10) {

            printf("currenttime 1 is %d\n",current);

            memset(buf,97,1024*10-20);
            char        temp[BUF_SIZE];
            sprintf(temp,"%s_%d",buf,index);
            printf("ikcp send: %d  ", index);

            int waitpacket=ikcp_waitsnd(kcp);
            printf("waitpacket 数量 %d\n",waitpacket);
            if(waitpacket<1024){
                ikcp_send(kcp, temp, strlen(temp) + 1);
                ikcp_update(kcp, clock());
                ikcp_flush(kcp);
                index++;
            }
            ikcp_update(kcp, clock());
            ikcp_flush(kcp);
            printf("RTT 值为 %d\n",kcp->rx_srtt);
            printf("currenttime 2 is %d\n",current);
            printf("slap  is %d\n",slap);
        }

        while(1){
            nRet = recvfrom(sockClient,recvbuf,BUF_SIZE,0,(sockaddr *)&servAddr,&nServAddLen);
            printf("recvfrom nRet %d\n",nRet);
            if(nRet < 0) break;
            ikcp_input(kcp, recvbuf, nRet);
        }
        while (1) {
            char kcpbuf[BUF_SIZE];
            int hr = ikcp_recv(kcp, kcpbuf, BUF_SIZE);
            printf("ikcp_recv hr is %d\n",hr);

            int seg=kcp->snd_una;
            printf("第一个未收到Ack包的序号 %d\n",seg);

            if(hr < 0) break;
            // 打印来自服务端发送来的数据
            printf("Recv From Server:%s\n",kcpbuf);
            ZeroMemory(kcpbuf,BUF_SIZE);
        }
        ZeroMemory(buf,BUF_SIZE);
        ZeroMemory(recvbuf,BUF_SIZE);

    }
    closesocket(s);
    WSACleanup();
    return 0;
}

