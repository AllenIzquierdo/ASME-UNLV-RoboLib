#include "NetworkTable.h"
#define NETWORK_CMD_HELLOWORLD 0 
#define NETWORK_CMD_FLUSHBYTES 1
#define NETWORK_CMD_PS2DATA 2

#define PACKET_COMMAND 2
#define PACKET_HASHKEY 1
#define PACKET_BYTECOUNT 0

#define HMAP_HELLOWORLD 0 // first index of byteMap is reserved for debuggin.

/** \brief Construct NetworkTable.
 *
 * \param byteSize size of NetworkTable byte array.
 * \param floatSize size of NetworkTable float array.
 */
NetworkTable::NetworkTable(int byteSize, int floatSize)
{
	this->byteMap = new byte[byteSize+3];
	byteMapSize = byteSize;
}

/** \brief Function that is binded to PacketSerial.
 *
 * Accepts packets from PacketSerial. Packet is then processed. The algorithem to process packets will likely change in the future.
 * \param sender Reference to serial object that's recieving packets.
 * \param buffer Reference to buffer where packet is stored.
 * \param size Size of packet, in bytes (i.e. 'size = 6' means 6 bytes.
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

/** \brief set a byte's value in NetworkTable bytemap.
 * 
 * This function only sets the value. It does not update Tables on other devices.
 * \param key Defines which table index that should be changed.
 * \param value What value the index should have.
 */
void NetworkTable::setByte(byte key, byte value)
{
	this->byteMap[key] = value;
}

/** \brief Returns the time since NetworkTable last recieved ps2 data.
 *
 * This function is to be used for watchdog protocals.
 * \return Time since last ps2 packet has been recieved, in miliseconds.
 */
unsigned long NetworkTable::getLastPS2PacketTime()
{
	return millis() - time_lastps2packet;
}

/** \brief Retrieve value from byte table.
 *
 * \param key index to retrieve. 
 * \return unsigned char of index.
 */
byte NetworkTable::getByte(byte key)
{
	return this->byteMap[key];
}

/** \brief Sends entire bytemap through the network.
 * \warning Do not send packets concurrently.
 * \param sender Reference to PacketSerial that should be used to send the packet.
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

/** \brief Flush ps2data across network.
 *
 * This function is called on driver station. A good rate to execute this function is 20Hz.
 * \param sender Reference to PacketSerial that should be used to send the packet.
 */ 
void NetworkTable::sendPS2Data(PacketSerial* sender)
{
	packetBuffer[0] = 3 + 21;
	packetBuffer[1] = 0;
	packetBuffer[2] = NETWORK_CMD_PS2DATA;
	for(byte i = 0; i<21; i++)
	{
		packetBuffer[i+3] = ps2x->PS2data[i];
		
		Serial.println(ps2x->PS2data[i], HEX);
	}
	Serial.println();
	sender->send(packetBuffer, packetBuffer[0]);
}

/** \brief Assigns binds a PS2X object to NetworkTable.
 *
 * The when calling sendPS2Data(PacketSerial* sender), the NetworkTable will copy data from assigned ps2 controller and send it over the network.
 * \param ps2x PS2X object that should be used to controll the robot.
 */
void NetworkTable::setPS2(PS2X &ps2x)
{
	this->ps2x = &ps2x;
}
