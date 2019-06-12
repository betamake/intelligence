//
// Created by Dragic on 2018/5/30.
//

#include "ServerSocket.h"

void ServerSocket::creatSocket() {
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket创建出错");
        exit(1);
    }
}

void ServerSocket::configSocket() {
    long flag = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, sizeof(flag));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(SERVPORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(my_addr.sin_zero),8);
    if(bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind出错！");
        exit(1);
    }
    if(listen(sock_fd, BACKLOG) == -1) {
        perror("listen出错！");
        exit(1);
    }
}

int ServerSocket::getSin_size() {
    return sin_size;
}

void ServerSocket::setSin_size(int sin_size) {
    ServerSocket::sin_size = sin_size;
}

int ServerSocket::getClient_fd() const {
    return client_fd;
}

void ServerSocket::setClient_fd(int client_fd) {
    ServerSocket::client_fd = client_fd;
}

int ServerSocket::getSock_fd() const {
    return sock_fd;
}

void ServerSocket::setSock_fd(int sock_fd) {
    ServerSocket::sock_fd = sock_fd;
}

const sockaddr_in &ServerSocket::getMy_addr() const {
    return my_addr;
}

void ServerSocket::setMy_addr(const sockaddr_in &my_addr) {
    ServerSocket::my_addr = my_addr;
}

const sockaddr_in &ServerSocket::getRemote_addr() const {
    return remote_addr;
}

void ServerSocket::setRemote_addr(const sockaddr_in &remote_addr) {
    ServerSocket::remote_addr = remote_addr;
}

ServerSocket::~ServerSocket() {
    close(client_fd);
}

ssize_t ServerSocket::sendInformation(const void *info, size_t size) {
    send(client_fd,info,size,0);
    return 0;
}
