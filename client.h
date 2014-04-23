#ifndef CLIENT_H
#define CLIENT_H

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/poll.h>

#include "packet.h"

#define PORT_NO 10022
#define PACKET_SIZE 512
#define PACKET_DATA_SIZE 507
#define BUFFER_SIZE 2048

namespace dicey2 {
	std::string srv_ip_address;
    double prob_loss;
    double prob_corrupt;
    double prob_delay;
    int length_delay;
    std::string filename;
    int skt;
    struct timeval timeout; 
	struct sockaddr_in sktaddr;
	struct sockaddr_in srvaddr;
	socklen_t srvaddrLen = sizeof(srvaddr);
	struct hostent *h;
	unsigned char buffer[BUFFER_SIZE];

    bool openSocket();
    bool sendPacket(Packet myPkt);
    bool sendMessage(char * messageArray);
    bool rcvPacket();
    void gremlin(Packet gremPkt);
}

#endif