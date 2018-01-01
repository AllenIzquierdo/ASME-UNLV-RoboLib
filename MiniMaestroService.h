
#ifndef MINIMAESTROSERVICE_H
#define MINIMAESTROSERVICE_H

#include <PololuMaestro.h>
#include <SoftwareSerial.h>
#include <Arduino.h>

/** \brief Handles Maestro communication.
 * 
 * Extended from MiniMaestro. Used in conjunction with classes that control devices off MiniMaestro, e.g., TalonSR.
 * How to use:
 * 1. Create a serial object (hardware or software) that is connected to Maestro's RX/TX pins.
 * 2. Construct MiniMaestroService with the serial object.
 * 3. Que values to maestro with queTarget().
 * 4. Call service() in Arduino's loop().
 * View example to see how to use this class.
 */
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
