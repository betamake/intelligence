//
// Created by Dragic on 2018/5/30.
//

#include "ClientSocket.h"

ClientSocket::ClientSocket() :sock_fd(-1) {
    server_ip = inet_addr(SERVER_IP);
    server_port = atoi(SERVER_PORT);
    client_ip = inet_addr(CLIENT_IP);
    client_port = atoi(CLIENT_PORT);
}

void ClientSocket::creatSocket() {
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket error");
        exit(1);
    }
}

void ClientSocket::configSocket() {
    long flag = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, sizeof(flag));
    // 綁定自己的ip
    client_addr.sin_addr.s_addr = client_ip;
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(client_port);
    if (bind(sock_fd, (sockaddr*)&client_addr, sizeof(sockaddr)) < 0) {
        perror("bind error");
        exit(1);
    }
}



ClientSocket::~ClientSocket() {
    close(sock_fd);
}

void ClientSocket::connectServer() {
    int recvbytes;
    char buf[MAXDATASIZE];
    Instructions instructions;
    Information information;

    server_addr.sin_addr.s_addr = server_ip;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
//    printf("Try to connect server(%s:%u)\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

    if(connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("connect出错！");
        exit(1);
    }
    printf("Connect server success(%s:%u)\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
    if((recvbytes=recv(sock_fd, buf, MAXDATASIZE, 0)) == -1) {
        perror("recv出错！");
        exit(1);
    }
    buf[recvbytes] = '\0';
    instructions.parseInformation(buf);
    information = instructions.getInformation();
    cout<<information<<endl;
}
