//
// Created by Dragic on 2018/5/30.
//

#ifndef IDCARDREADER_SERVERSOCKET_H
#define IDCARDREADER_SERVERSOCKET_H
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SERVPORT 3333   // 服务器监听端口号
#define BACKLOG 10  // 最大同时连接请求数
#define MAXDATASIZE 1000
using namespace std;

class ServerSocket {
public:
    void creatSocket();

    void configSocket();

    ssize_t sendInformation(const void* info, size_t size);

    int getSock_fd() const;

    void setSock_fd(int sock_fd);

    int getClient_fd() const;

    void setClient_fd(int client_fd);

    int getSin_size();

    void setSin_size(int sin_size);

    const sockaddr_in &getMy_addr() const;

    void setMy_addr(const sockaddr_in &my_addr);

    const sockaddr_in &getRemote_addr() const;

    void setRemote_addr(const sockaddr_in &remote_addr);

    virtual ~ServerSocket();

private:
    int sock_fd,client_fd;  // sock_fd：监听socket；client_fd：数据传输socket
    int sin_size;
    struct sockaddr_in my_addr; // 本机地址信息
    struct sockaddr_in remote_addr; // 客户端地址信息
};


#endif //IDCARDREADER_SERVERSOCKET_H
