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

#pragma comment(lib, "WS2_32.lib")
#define BUF_SIZE    6000

SOCKET sockClient;// = socket(AF_INET,SOCK_DGRAM,0);
SOCKADDR_IN servAddr;       // 服务器套接字地址
int nServAddLen = sizeof(servAddr);
int count1 = 0;
int datalength=0;

int udp_output(const char *buf, int len, ikcpcb *kcp, void *user) {
    int res = sendto(sockClient, buf, len, 0, (sockaddr *) user, nServAddLen);


    if (res > 0) {
        count1 = count1 + 1;
        datalength=datalength+res;
        printf("sent msg count1: %d,datalength is %d KB\n", count1,datalength/1024);

    }
    return 0;
}

int main(void) {
    WSADATA wsd;
    SOCKET s;

    // 初始化套接字动态库
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0) {
        printf("WSAStartup failed !\n");
        return 1;
    }

    // 创建套接字
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == INVALID_SOCKET) {
        printf("socket() failed, Error Code:%d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    char buf[BUF_SIZE];  // 接受数据
    char recvbuf[BUF_SIZE];  // 接受数据
    int nRet;
    ZeroMemory(buf, BUF_SIZE);
    ZeroMemory(recvbuf, BUF_SIZE);
    sockClient = socket(AF_INET, SOCK_DGRAM, 0);
    strcpy(buf, "Hello world!");

/*    int i;
    //buf 1KB
    printf("buf 1KB  start\n");
    for(i=0;i<=400;i++){
        buf[i]='A';
        printf("buf 1KB %d \n",i);
    }
    printf("buf 1KB \n");*/

    // 设置服务器地址
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(5000);
    int imode = 1;
    nRet = ioctlsocket(sockClient, FIONBIO, (u_long *) &imode);
    if (nRet == SOCKET_ERROR) {
        printf("ioctlsocket failed!");
        closesocket(sockClient);
        WSACleanup();
        return -1;
    }

    ikcpcb *kcp = ikcp_create(1, (void *) &servAddr);
    kcp->output = udp_output;
    ikcp_nodelay(kcp, 1, 10, 2, 1);
    ikcp_wndsize(kcp, 512, 512);
    kcp->rx_minrto = 10;


    long start, current;

    start = clock();
    long slap = clock();
    int index = 1;
    while (1) {
        Sleep(100);
        //printf("OK\n");
        current = clock();
        //
        ikcp_update(kcp, clock());

        int res;

        //if (current - start < 1000) {
            for (; current >= slap; slap += 1000) {
                //char        temp[BUF_SIZE];
                //sprintf(temp,"%s_%d",buf,index);
                printf("ikcp send: %d  ", index);
                char temp[1024] = {0};//与服务器初次通信
                strcpy(temp,"This is string.h library function");
                char longtemp[6000]={0};
                memset(temp,97,990);
                printf("temp length %d\n",strlen(temp));
                int writecount=sprintf(longtemp,"%s_%d",temp,index);
                printf("writecount=%d\n",writecount);
                printf("puts(temp)输出的字符"); puts(longtemp);
                printf("sprintf后temp数组的大小 %d\n",strlen(longtemp));
                printf("Client send buf:%s\n", longtemp);

                int waitpacket=ikcp_waitsnd(kcp);
                printf("waitpacket 数量 %d\n",waitpacket);

                if(waitpacket<100){
                    ikcp_send(kcp, longtemp, strlen(longtemp)+1);
                    printf("sprintf后temp数组的大小 %d\n",strlen(longtemp));
                }

                ikcp_update(kcp, clock());
                ikcp_flush(kcp);
                index++;

                nRet = recvfrom(sockClient, recvbuf, BUF_SIZE, 0, (sockaddr *) &servAddr, &nServAddLen);
                //if(nRet < 0) break;
                ikcp_input(kcp, recvbuf, nRet);
                ikcp_update(kcp, clock());
                ikcp_flush(kcp);

                printf("RTT 值为 %d\n",kcp->rx_rttval);



                char kcpbuf[BUF_SIZE];
                ikcp_recv(kcp, kcpbuf, BUF_SIZE);
                //if(hr < 0) break;
                // 打印来自服务端发送来的数据
                printf("Recv From Server:%s\n", kcpbuf);
                ikcp_update(kcp, clock());
                ikcp_flush(kcp);
            //}
        }

/*        while(1){
            nRet = recvfrom(sockClient,recvbuf,BUF_SIZE,0,(sockaddr *)&servAddr,&nServAddLen);
            if(nRet < 0) break;
            ikcp_input(kcp, recvbuf, nRet);
        }
        while (1) {
            char kcpbuf[BUF_SIZE];
            int hr = ikcp_recv(kcp, kcpbuf, BUF_SIZE);
            if(hr < 0) break;
            // 打印来自服务端发送来的数据
            printf("Recv From Server:%s\n",kcpbuf);
        }*/
    }
    closesocket(s);
    WSACleanup();
    return 0;
}

