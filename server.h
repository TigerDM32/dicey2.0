#ifndef SERVER_H
#define SERVER_H

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
#define PACKET_DATA_SIZE 493
#define BUFFER_SIZE 2048

namespace dicey2 {
	struct sockaddr_in addr1;
	struct sockaddr_in addr2;
	socklen_t addr2Len = sizeof(addr2);
	int rlen;
	int skt;
	int expectedSeq = 0;
	unsigned char buffer[BUFFER_SIZE];
}

#endif