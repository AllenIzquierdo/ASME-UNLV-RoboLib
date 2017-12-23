
#include "NetworkTable.h"
#define NETWORK_CMD_HELLOWORLD 0 
#define NETWORK_CMD_FLUSHBYTES 1
#define NETWORK_CMD_PS2DATA 2

#define PACKET_COMMAND 2
#define PACKET_HASHKEY 1
#define PACKET_BYTECOUNT 0

#define HMAP_HELLOWORLD 0 // first index of byteMap is reserved for debuggin.

#define HMAP_BUTTONS 5

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
		case NETWORK_CMD_PS2DATA:
			for(byte i = 0; i < 21; i++)
			{
				ps2x->PS2data[i] = buffer[i+3];
				//ps2x->PS2data[i] = 255;
				//Serial.println(buffer[i+3], HEX);
			}
			//Serial.println();
			ps2x->read_gamepad();
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

void NetworkTable::flushBytes(PacketSerial* sender)
{
	packetBuffer[0] = 3 + byteMapSize;
	packetBuffer[1] = 0;
	packetBuffer[2] = NETWORK_CMD_FLUSHBYTES;
	for(byte i = 0; i < byteMapSize; i++)
	{
		packetBuffer[i+3] = byteMap[i];
	}
	sender->send(packetBuffer, packetBuffer[0]);
}

void NetworkTable::helloWorld(PacketSerial* sender, byte value)
{
	packetBuffer[0] = 3 + 1;
	packetBuffer[1] = 0;
	packetBuffer[2] = NETWORK_CMD_HELLOWORLD;
	packetBuffer[3] = value;
	sender->send(packetBuffer, packetBuffer[0]);
}

void NetworkTable::sendPS2Data(PacketSerial* sender)
{
	packetBuffer[0] = 3 + 21;
	packetBuffer[1] = 0;
	packetBuffer[2] = NETWORK_CMD_PS2DATA;
	for(byte i = 0; i<21; i++)
	{
		packetBuffer[i+3] = ps2x->PS2data[i];
		
		//Serial.println(ps2x->PS2data[i], HEX);
	}
	//Serial.println();
	sender->send(packetBuffer, packetBuffer[0]);
}

void NetworkTable::setPS2(PS2X* ps2x)
{
	this->ps2x = ps2x;
}
