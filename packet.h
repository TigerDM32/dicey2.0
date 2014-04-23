#ifndef PACKET_H
#define PACKET_H

#include <iostream>
#include <cstring>
#include <cstdio>

#define PACKET_SIZE 512
#define PACKET_DATA_SIZE 493

typedef bool bit;

namespace dicey2{
	class Packet {
		private:
			int seq_num;
			bit ack;
			int checksum;
			char data[PACKET_SIZE];

		public:
			Packet(int seq_num, char data[PACKET_SIZE]);

			int generate_checksum();

			bool test_checksum();

			int getSeqNum();
			bit getAck();
			int getChecksum();
			char * getData();
			char * getPacketAsCharArray();
			void setAck(char ackBit);
			void setChecksum(int newChecksum);
			void setSeqNum(int newSeqNum);
	};
}

#endif