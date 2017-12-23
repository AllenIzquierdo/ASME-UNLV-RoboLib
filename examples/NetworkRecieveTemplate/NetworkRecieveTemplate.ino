#include "NetworkTable.h"
#include "PS2X_lib.h"
#include <PacketSerial.h>

NetworkTable network = NetworkTable(10, 10);
PacketSerial myPacketSerial;
PS2X ps2x;
bool blink_value = false;
unsigned long last_blink = 0;
void setup()
{
	pinMode(13, OUTPUT);
	ps2x.config_gamepad();
	ps2x.read_gamepad();
	myPacketSerial.begin(115200);
	myPacketSerial.setPacketHandler([](const uint8_t* buffer, size_t size) {
	network.processPacketFromSender(myPacketSerial, buffer, size);
	});
	network.setPS2(&ps2x);
}

void ledService()
{
	// TODO: change led blink from timer to watchdog
	unsigned long temp = millis() - last_blink;
	if(temp > 500)
	{
		last_blink = millis();
		blink_value = !blink_value;
		digitalWrite(13, blink_value);
	}
}

void loop() {
	myPacketSerial.update();
	ledService();
}



