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

class NetworkTable {
	public:
		NetworkTable(int byteSize, int floatSize);
		byte getByte(byte key);
		void setByte(byte key, byte value);
		void processPacketFromSender(const PacketSerial& sender, const uint8_t* buffer, size_t size);
		void networkService();
		// Network Packet Commands
		void flushBytes(PacketSerial* sender);
		void helloWorld(PacketSerial* sender, const byte value);
		void setPS2Data(byte* ps2data);
		void sendPS2Data(PacketSerial* sender);
	private:
		byte* byteMap;
		byte* ps2Data;
		byte byteMapSize;
		byte packetBuffer[100];
};
