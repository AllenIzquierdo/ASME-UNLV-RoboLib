
#ifndef MINIMAESTROSERVICE_H
#define MINIMAESTROSERVICE_H

#include <PololuMaestro.h>
#include <SoftwareSerial.h>
#include <Arduino.h>

class MiniMaestroService: public MiniMaestro {
	public:
		MiniMaestroService(Stream &stream, uint8_t resetPin = noResetPin, uint8_t deviceNumber = deviceNumberDefault, bool CRCEnabled = false);
		void queTarget(const uint8_t channel, const uint16_t value);
		void sendValues();
		void service();
		void setUpdatePeriod(const uint16_t period);
		void getQueTarget(const uint8_t channel);
	protected:

	private:
		uint16_t values[18];
		uint8_t highestOccupiedChannel;
		uint16_t period = 50;
		uint16_t last_update = 0;

};

#endif
