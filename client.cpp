#include "client.h"

using namespace dicey2;

int main(int argc, char* argv[]) {
<<<<<<< HEAD
	dicey2::srv_ip_address = argc > 1 ? argv[1] : "127.0.0.1";
=======
	dicey2::srv_ip_address = argc > 1 ? argv[1] : "127.0.0.1";//"131.204.14.205";
>>>>>>> new-checksum
    dicey2::prob_corrupt = argc > 2 ? std::atof(argv[2]) : 0.2;
    dicey2::prob_loss = argc > 3 ? std::atof(argv[3]) : 0.2;
    dicey2::prob_delay = argc > 4 ? std::atof(argv[4]) : 0.3;
    dicey2::length_delay = argc > 5 ? std::atof(argv[5]) : 4;
    dicey2::filename = argc > 6 ? argv[6] : "TestFile";

    if(!openSocket())
		return 0;
	
	std::cout << "GET " << filename << std::endl << std::endl;
	
	
	std::string getRequestString = "GET " + filename;
	char * getRequestCharArray = (char*)getRequestString.c_str();

	if(!sendMessage(getRequestCharArray)){
		perror("Unable to send GET request.");
		return 0;
	}
	return 0;
}

bool dicey2::openSocket(){
	//create socket    
	if ((skt = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Unable to create socket.");
		return 0;
	}

	memset((char *)&sktaddr, 0, sizeof(sktaddr));
	sktaddr.sin_family = AF_INET;
	sktaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sktaddr.sin_port = htons(0);

	if (bind(skt, (struct sockaddr *)&sktaddr, sizeof(sktaddr)) < 0){
		perror("Unable to bind socket.");
		return 0;
	}

	memset((char *)&srvaddr, 0, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(PORT_NO);
	inet_pton(AF_INET, srv_ip_address.c_str(), &(srvaddr.sin_addr));

	std::cout << "Server IP: " << inet_ntoa(srvaddr.sin_addr) << ":" << ntohs(srvaddr.sin_port) << std::endl;
	return 1;
}

bool dicey2::sendPacket(Packet myPkt){
	char * packetAsArray = myPkt.getPacketAsCharArray();
	if (!sendMessage(packetAsArray))
	{
		std::cout << std::endl << std::endl << "Sending Packet: seq_num = " << myPkt.getSeqNum() << "; ack = " << myPkt.getAck() << "; checksum = " << myPkt.getChecksum() << "; data = " << myPkt.getData() << std::endl;
		return 0;
	}
	else
		return 1;
}

bool dicey2::sendMessage(char * messageArray){
	if(sendto(skt, messageArray, strlen(messageArray), 0, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0){
		perror("Unable to send message.");
		return 0;
	}
	return 1;
}