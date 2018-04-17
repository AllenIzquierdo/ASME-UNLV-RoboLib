#include "NetworkTable.h"
#define NETWORK_CMD_HELLOWORLD 0
#define NETWORK_CMD_FLUSHBYTES 1
#define NETWORK_CMD_PS2DATA 2

#define PACKET_COMMAND 2
#define PACKET_HASHKEY 1
#define PACKET_BYTECOUNT 0

#define HMAP_HELLOWORLD 0 // first index of byteMap is reserved for debugging.

/** \brief Here we construct the NetworkTable.
 *
 * \param byteSize    - This is the size of the NetworkTable byte array.
 * \param floatSize   - This is the size of the NetworkTable float array.
 */
NetworkTable::NetworkTable(int byteSize, int floatSize)
{
	this->byteMap = new byte[byteSize+3];
	byteMapSize = byteSize;
}

/** \brief This is the function that is binded to 'PacketSerial', it processes the packets.
 *
 * It accepts the packets from PacketSerial, and processes them.
 *
 * \param sender   - This is a Reference to the serial object that's receiving the packets.
 * \param buffer   - This is a Reference to the buffer, where packets are stored.
 * \param size     - This is the Size of a packet, in bytes (i.e. 'size = 6' means 6 bytes).
 */
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
				//Serial.println(buffer[i+3], HEX);
			}
			//Serial.println();
			ps2x->read_gamepad();
			time_lastps2packet = millis();

	}

}

/** \brief This sets a byte's value in the NetworkTable byte-map.
 *
 * This function only sets the value. It does not update the Tables on other devices.
 * \param key     - This defines which table index is the one that should be changed.
 * \param value   - This is the value the index should have.
 */
void NetworkTable::setByte(byte key, byte value)
{
	this->byteMap[key] = value;
}

/** \brief This returns the time since the NetworkTable last received any ps2 data.
 *
 * This function is to be used for watchdog protocols.
 * \return Time   - The amount of time since the last ps2 packet has been received, in milliseconds.
 */
unsigned long NetworkTable::getLastPS2PacketTime()
{
	return millis() - time_lastps2packet;
}

/** \brief Here we retrieve a value from the byte table.
 *
 * \param key   - The index to retrieve.
 * \return  The unsigned char of index.
 */
byte NetworkTable::getByte(byte key)
{
	return this->byteMap[key];
}

/** \brief This sends the entire byte-map through the network.
 * \warning !! Do not send packets concurrently !!
 * \param sender   - This is a reference to the PacketSerial that should be used to send the packet.
 */
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

/** \brief Used to test connectivity.
 */
void NetworkTable::helloWorld(PacketSerial* sender, byte value)
{
	packetBuffer[0] = 3 + 1;
	packetBuffer[1] = 0;
	packetBuffer[2] = NETWORK_CMD_HELLOWORLD;
	packetBuffer[3] = value;
	sender->send(packetBuffer, packetBuffer[0]);
}

/** \brief This pushes the ps2data out across the network.
 *
 * This function is called on by the driver station. A good rate to execute this function is at 20Hz.
 * \param sender   - This is a reference to the PacketSerial that should be used to send the packet.
 */
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

/** \brief This binds a PS2X object to the NetworkTable.
 *
 * When calling sendPS2Data(PacketSerial* sender), the NetworkTable will copy data from the assigned ps2
 * controller and send it over the network.
 *
 * \param ps2x   - A PS2X object that should be used to control the robot. Please refer to the PS2X file for further documentation.
 */
void NetworkTable::setPS2(PS2X &ps2x)
{
	this->ps2x = &ps2x;
}
