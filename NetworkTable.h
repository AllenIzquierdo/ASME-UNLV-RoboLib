// Network table to share data between multiple MCU(s)
//	Plan:
//		Use a pesuado hashmap using macro constants.
//		Use PacketSerial to encode & decode using COBS.
//	- Use this following Lambda expression to bind PacketSerial handler to the class - 
//	<PacketSerial>.setPacketHandler([](const uint8_t* buffer, size_t size) {
//	<NetworkTable>.processPacketFromSender(<PacketSerial>, buffer, size);
//	});
#include <Arduino.h>
#include <PacketSerial.h>
#include "PS2X_lib.h"

/** \brief Handles data from wixel's radio.
 *
 * NetworkTable is used to process decoded packets from driver station.
 * NetworkTable must be binded to PacketSerial object by Lambda expressions. View examples to see how it's done.
 * \warning Do not send multiple packets concurrently. It doesn't work (yet)!
 */
class NetworkTable {
	public:
		unsigned long getLastPS2PacketTime();
		NetworkTable(int byteSize, int floatSize);
		byte getByte(byte key);
		void setByte(byte key, byte value);
		void processPacketFromSender(const PacketSerial& sender, const uint8_t* buffer, size_t size);
		void networkService();
		// Network Packet Commands
		void flushBytes(PacketSerial* sender);
		void helloWorld(PacketSerial* sender, const byte value);
		void logByte(byte value, PacketSerial* sender);
		void logLong(long value, PacketSerial* sender);
		void logInt(int value, PacketSerial* sender);
		void setPS2(PS2X &ps2X);
		void sendPS2Data(PacketSerial* sender);
	private:
		unsigned long time_lastps2packet;
		byte* byteMap;
		PS2X* ps2x;
		byte byteMapSize;
		byte packetBuffer[100];
};
