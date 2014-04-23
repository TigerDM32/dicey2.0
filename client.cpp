#include "client.h"

using namespace dicey2;

int main(int argc, char* argv[]) {
	dicey2::srv_ip_address = argc > 1 ? argv[1] : "131.204.14.192";
    dicey2::prob_corrupt = argc > 2 ? std::atof(argv[2]) : 0.2;
    dicey2::prob_loss = argc > 3 ? std::atof(argv[3]) : 0.2;
    dicey2::prob_delay = argc > 4 ? std::atof(argv[4]) : 0.3;
    dicey2::length_delay = argc > 5 ? std::atof(argv[5]) : 4;
    dicey2::filename = argc > 6 ? argv[6] : "TestFile";

    if(!openSocket())
    	return 0;
	
	std::cout << "GET " << filename << std::endl << std::endl;
	//construct packet with 'GET filename' as header


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