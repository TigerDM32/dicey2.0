#include "server.h"

using namespace dicey2;

bool dicey2::sendMessage(char * messageArray){
	if(sendto(skt, messageArray, strlen(messageArray), 0, (struct sockaddr *)&addr2, sizeof(addr2)) < 0){
		perror("Unable to send message.");
		return 0;
	}
	return 1;
}

bool dicey2::sendPacket(Packet myPkt){
	if(sendto(skt, &myPkt, PACKET_SIZE, 0, (struct sockaddr *)&addr2, sizeof(addr2)) < 0){
		perror("Unable to send message.");
		return 0;
	}
	return 1;
}

int main(int argc, char* argv[]) {
    dicey2::prob_corrupt = argc > 1 ? std::atof(argv[1]) : 0.2;
    dicey2::prob_loss = argc > 2 ? std::atof(argv[2]) : 0.2;
    dicey2::prob_delay = argc > 3 ? std::atof(argv[3]) : 0.3;
    dicey2::length_delay = argc > 4 ? std::atof(argv[4]) : 4;
	
	if((skt = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Unable to create socket.");
		return 0;
	}

	memset((char *)&addr1, 0, sizeof(addr1));
	addr1.sin_family = AF_INET;
	addr1.sin_addr.s_addr = htonl(INADDR_ANY);
	addr1.sin_port = htons(PORT_NO);

	if (bind(skt, (struct sockaddr *)&addr1, sizeof(addr1)) < 0){
		perror("Unable to bind socket.");
		return 0;
	}

	bit rcvdRequest = 0;
	while(!rcvdRequest){
		rlen = recvfrom(skt, buffer, PACKET_DATA_SIZE, 0, (struct sockaddr *)&addr2, &addr2Len);
		if (rlen > 0){
			rcvdRequest = 1;
			buffer[rlen] = 0;

			char * requestData = new char[rlen];
			char * filename = new char[rlen - 4];
			for(int i = 0; i < rlen; i++){
				requestData[i] = buffer[i];
				filename[i] = buffer[i + 4];
			}
			std::cout << "Received request: " << requestData << ", filename = " << filename << std::endl;
		
			//attempt to open file
			std::ifstream dataFile;
			dataFile.open(filename);
			if(dataFile.is_open()){
			
				//get filesize
  				dataFile.seekg (0, dataFile.end);
  				int filesize = dataFile.tellg();
  				dataFile.seekg(0, dataFile.beg);

				//is file 80KB or more? if not, send error
				if(filesize <= 80000){
					dataFile.close();
					std::cout << "ERROR: File is too small.";
					return 0;
				}

				//calculate how many times to pull out 507 bytes
				int numPackets = 1 + ((filesize - 1)/PACKET_DATA_SIZE);

				std::cout << "File size: " << filesize << ", packets to send = " << numPackets << std::endl;

				//create a buffer of packets that comprise the entire file to send
				Packet * fileBuffer = new Packet[numPackets];
				
				for (int i = 0; i < numPackets; i++){
					char * pktData = new char[PACKET_DATA_SIZE];
					dataFile.read(pktData, PACKET_DATA_SIZE);

                    if (i == numPackets - 1) { // this is the last packet
                      pktData[506] = '\0';
                    }
                  
					Packet filePkt(i%32,  pktData);
					
					
					fileBuffer[i] = filePkt;
				}
              	

				int currentSeqNum = 0;
				while(currentSeqNum < numPackets) {
					//bool itworked = rcvAck(currentSeqNum);
					//if (itworked) {
						//std::cout << "it worked" << std::endl;
						//currentSeqNum++;
					//}
					for (int j = 0; j < 16; j++) {
						Packet printPkt = fileBuffer[currentSeqNum + j];
							char * pktData = printPkt.getData();
							sendPacket(printPkt);
							char * sampleData = new char[48];
							for (int k = 0; k < 48; k++){
								sampleData[k] = pktData[k];
							}
							std::cout << std::endl << std::endl << "Packet: seq_num = " << printPkt.getSeqNum() << "; ack = " << printPkt.getAck() << "; checksum = " << printPkt.getChecksum() << "; data = \"" << sampleData << "\"" << std::endl;
					}
					currentSeqNum++;
				}


				//acks we have already received
				//int ackCount = 0;
				//int base = 0;

				//while (ackCount < numPackets/16){
					
					//while (!rcvAck(base)){
                		//for (int j = base; j < base + 16; j++){
                			//Packet printPkt = fileBuffer[ackCount];
							//char * pktData = printPkt.getData();
							//sendPacket(printPkt);
							//char * sampleData = new char[48];
							//for (int k = 0; k < 48; k++){
								//sampleData[k] = pktData[k];
							//}
							//std::cout << std::endl << std::endl << "Packet: seq_num = " << printPkt.getSeqNum() << "; ack = " << printPkt.getAck() << "; checksum = " << printPkt.getChecksum() << "; data = \"" << sampleData << "\"" << std::endl;
                		//}
                	//}
					
					//ackCount++;
					//base += 16;
				//}


                //for (int i = 0; i < numPackets; i++) {
                  //  sendPacket(fileBuffer[i]);
                    //if (i%32 == 16 || i%32 == 0) {
                      //  std::cout << "ACK RECEIVED. SENDING NEXT WINDOW." << std::endl;
                    //}
                //}
			}
            dataFile.close();		
		}
	}

	return 0;
}

bool dicey2::rcvAck(int expectSeq){
	int rcvPoll = 0;
	struct pollfd ufds;
	time_t timer;

	ufds.fd = skt;
	ufds.events = POLLIN;
	rcvPoll = poll(&ufds, 1, 20);

	if( rcvPoll == -1 ) {
		perror("Unable to poll socket.");
		return 0; 
	} 
	else if( rcvPoll == 0 ) {
		perror("Timeout");
		return 0;
	} 
	else {
		int recvLen;
		recvLen = recvfrom(skt, buffer, PACKET_SIZE, 0, (struct sockaddr *)&addr2, &addr2Len);
		if (recvLen > 1){
			Packet* ackPkt = new Packet;                 
			buffer[recvLen] = 0;
			memcpy(ackPkt, buffer, PACKET_SIZE);
			
			if (ackPkt->getSeqNum() == expectSeq && ackPkt->getAck() == 1){
				std::cout << std::endl << std::endl << "Received ACK: seq_num = " << expectSeq << "; ack = " << ackPkt->getAck() << std::endl;
				return 1;
			}
			else
				return 0;
			
			//if (pktData[PACKET_DATA_SIZE] == char(0))
			//	return 1;
		}
	}
	return 0;
}
