#ifndef PACKET_H
#define PACKET_H

#include <iostream>
#include <cstring>
#include <cstdio>

#define PACKET_SIZE 512
#define PACKET_DATA_SIZE 507

typedef bool bit;

namespace dicey2{
	class Packet {
		private:
			int seq_num;
			bit ack;
			uint16_t checksum;
			char data[PACKET_SIZE];

		public:
			Packet(int seq_num = 0, char data[PACKET_SIZE] = new char[PACKET_SIZE]);

			uint16_t generate_checksum();

			bool test_checksum();

			int getSeqNum();
			bit getAck();
			uint16_t getChecksum();
			char * getData();
			char * getPacketAsCharArray();
			void setAck(char ackBit);
			void setChecksum(uint16_t newChecksum);
			void setSeqNum(int newSeqNum);
	};
}

#endif
