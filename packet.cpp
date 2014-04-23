#include "packet.h"

using namespace dicey2;

	Packet::Packet(int seq_no, char newData[PACKET_DATA_SIZE]){
		seq_num = seq_no;
		ack = 0;
		std::strcpy(data, newData);
		checksum = generate_checksum();
	}

	int Packet::generate_checksum() {
        int byteTotal = 0;
        for(int i = 0; i < strlen(data); i++){
            byteTotal += data[i];
            std::cout << data[i];
        }
        int divisor = byteTotal/256;
        checksum = byteTotal - (divisor * 256);
        return checksum;
    }

    bool Packet::test_checksum() {
        bool test = checksum == generate_checksum();
        return test;
    }

    void Packet::setAck(char ackBit){
        if(ackBit == '0')
            ack = 0;
        else
            ack = 1;
    }

    void Packet::setSeqNum(int newSeqNum){
    	seq_num = newSeqNum;
    }

    int Packet::getSeqNum(){
        return seq_num;
    }

    bit Packet::getAck(){
        return ack;
    }

    void Packet::setChecksum(int newChecksum){
        checksum = newChecksum;
    }

    int Packet::getChecksum(){
        return checksum;
    }

    char * Packet::getData(){
        return data;
    }

    // Computes and returns the char * representation of entire packet.
    char * Packet::getPacketAsCharArray(){
        //FORMAT - [SEQ_NO0, SEQ_NO1, ACK/NAK, CHECKSUM0, CHECKSUM1, DATA0, ..., DATA506] -- CHARACTERS/BYTES
        //FORMAT - [0      , 1      , 2      , 3        , 4        , 5    , ..., 511] -- ARRAY INDEXES
        char * wholePacket = new char[PACKET_SIZE];

        // Assign sequence number
        if(seq_num)
            wholePacket[0] = seq_num/10;
        else
            wholePacket[1] = seq_num%10;
        //std::cout << "DEBUG (Packet getPacketAsCharArray): wholePacket[0] = " << wholePacket[0] << std::endl;

        // Assign ACK/NAK
        if(ack)
            wholePacket[2] = '1';
        else
            wholePacket[2] = '0';
        //std::cout << "DEBUG (Packet getPacketAsCharArray): wholePacket[1] = " << wholePacket[1] << std::endl;

        // Assign checksum
        char * strCh = new char[sizeof(int)];
        sprintf(strCh, "%16d", checksum);
        for(int i = 3; i < 5; i++){
            if (strCh[i - 3] == ' '){
                wholePacket[i] = '0';
                continue;
            }
            wholePacket[i] = strCh[i - 3];
        }
        //std::cout << "DEBUG (Packet getPacketAsCharArray): strCh = " << strCh << std::endl;

        // Assign data
        for(int j = 5; j < PACKET_SIZE; j++){
            wholePacket[j] = data[j - 5];
        }
        //std::cout << "DEBUG (Packet getPacketAsCharArray): wholePacket = " << wholePacket << std::endl;

        return wholePacket;
    }


