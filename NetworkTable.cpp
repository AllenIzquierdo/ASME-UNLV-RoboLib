
#include "NetworkTable.h"
#define NETWORK_CMD_HELLOWORLD 0 
#define NETWORK_CMD_FLUSHBYTES 1

#define PACKET_COMMAND 2
#define PACKET_HASHKEY 1
#define PACKET_BYTECOUNT 0

#define HMAP_HELLOWORLD 0 // first index of byteMap is reserved for debuggin.

#define HMAP_BUTTONS 5
#define CMND_HELLOWORLD 0

NetworkTable::NetworkTable(int byteSize, int floatSize)
{
	this->byteMap = new byte[byteSize+3];
	byteMapSize = byteSize;
}


void NetworkTable::processPacketFromSender(const PacketSerial& sender, const uint8_t* buffer, size_t size)
{
	if(size < 3)
	{
		return;
	}

	switch(buffer[2])
	{
		case NETWORK_CMD_FLUSHBYTES:
			for(byte i = 0; i < byteMapSize; i++)
			{
				byteMap[i] = buffer[i+3];
			}
		case NETWORK_CMD_HELLOWORLD:
			byteMap[HMAP_HELLOWORLD] = buffer[3];
	}

}

void NetworkTable::setByte(byte key, byte value)
{
	this->byteMap[key] = value;
}

byte NetworkTable::getByte(byte key)
{
	return this->byteMap[key];
}

void NetworkTable::flushBytes(PacketSerial sender)
{
	packetBuffer[0] = 3 + byteMapSize;
	packetBuffer[1] = 0;
	packetBuffer[2] = NETWORK_CMD_FLUSHBYTES;
	for(byte i = 0; i < byteMapSize; i++)
	{
		packetBuffer[i+3] = byteMap[i];
	}
	sender.send(packetBuffer, packetBuffer[0]);
}

void NetworkTable::helloWorld(PacketSerial* sender, byte value)
{
	packetBuffer[0] = 3 + 1;
	packetBuffer[1] = 0;
	packetBuffer[2] = NETWORK_CMD_FLUSHBYTES;
	packetBuffer[3] = value;
	sender->send(packetBuffer, packetBuffer[0]);
}
