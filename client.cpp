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