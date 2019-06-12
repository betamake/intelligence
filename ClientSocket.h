//
// Created by Dragic on 2018/5/30.
//

#ifndef IDCARDREADER_CLIENTSOCKET_H
#define IDCARDREADER_CLIENTSOCKET_H
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include "Information.h"
#include "Instructions.h"
#define MAXDATASIZE 10000
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT "3333"
#define CLIENT_IP "127.0.0.1"
#define CLIENT_PORT "4444"

using namespace std;

class ClientSocket {
public:
    ClientSocket();

    virtual ~ClientSocket();

    void creatSocket();
    void configSocket();
    void connectServer();

private:
    int sock_fd;
    in_addr_t server_ip;
    in_port_t server_port;
    in_port_t client_ip;
    in_port_t client_port;
    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;

};


#endif //IDCARDREADER_CLIENTSOCKET_H
