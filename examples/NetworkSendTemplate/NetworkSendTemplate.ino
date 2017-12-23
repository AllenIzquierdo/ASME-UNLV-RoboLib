#include <NetworkTable.h>
#include <PS2X_lib.h>
#include <PacketSerial.h>

NetworkTable network = NetworkTable(10, 10);
PacketSerial myPacketSerial;
PS2X ps2x;
bool blink_value = 0;
unsigned long last_blink = 0;
unsigned long last_update = 0;

void setup()
{
	pinMode(13, OUTPUT);
	pinMode(7, OUTPUT);
	
	int error = 1;
	while(error!=0)
	{
		delay(50);
		//setup pins and settings:
		error = ps2x.config_gamepad(11, 9, 10, 8, false, false);   
	}

	Serial.begin(115200);
	Serial1.begin(115200);
    	myPacketSerial.setStream(&Serial1);
	myPacketSerial.setPacketHandler([](const uint8_t* buffer, size_t size) {
	network.processPacketFromSender(myPacketSerial, buffer, size);
	});
	network.setPS2(&ps2x);
}

void ledService()
{
	unsigned long temp = millis() - last_blink;
	if(temp > 500)
	{
		blink_value = !blink_value;
		last_blink = millis();
		digitalWrite(13, blink_value);
	}

}

void networkService()
{
	unsigned long temp = millis() - last_update;
	// update controlls 20x a second
	if(temp > 50)
	{
		network.sendPS2Data(&myPacketSerial);
		last_update = millis();
	}
}

void loop() {
	ps2x.read_gamepad();
	ledService();
	myPacketSerial.update();
}



