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

/** \brief This handles the data sent from the wixel's radio.
 *
 * The NetworkTable is used to process the decoded packets from the driver's station.
 * The NetworkTable must be binded to the PacketSerial object by Lambda expressions. Please view the examples to see how it's done.
 * \warning !! Do not send multiple packets concurrently. It does not work (yet) !!
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
		void logFloat(float value, PacketSerial* sender);
		void logInt(int value, PacketSerial* sender);
		void setPS2(PS2X &ps2X);
		void sendPS2Data(PacketSerial* sender);

		// Network 3
		void putBufferByte(const unsigned char&);
		void putBufferBytes(const unsigned char, const unsigned char*);
		void putBufferInteger(const int&);
		void putBufferFloat(const float&);
		void putBufferPair(const unsigned char&, const float&);
		void putBufferPair(const unsigned char&, const int&);
		void putBufferPair(const unsigned char&, const unsigned char&);
		void sendBuffer(PacketSerial*, const unsigned char);
		void processFloatPairs(unsigned char, const unsigned char*);
		void setFloatMap(float[], const unsigned char);

		void printFloatMap();

	private:
		unsigned long time_lastps2packet;
		byte* byteMap;
		PS2X* ps2x;
		byte byteMapSize;
		byte packetBuffer[100];		// byte Buffer
		unsigned char bufferIndex = 3;	// Index to empty buffer
		unsigned char valuePairs = 0;	// Counts the number of value pairs being sent.
		float* floatMap;
		unsigned char floatMapSize = 0;
};
