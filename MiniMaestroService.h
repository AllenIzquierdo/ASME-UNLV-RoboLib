
#ifndef MINIMAESTROSERVICE_H
#define MINIMAESTROSERVICE_H

#include <PololuMaestro.h>
#include <SoftwareSerial.h>
#include <Arduino.h>

/** \brief This handles the Maestro communications.
 *
 * This is extended from the MiniMaestro. This is used in conjunction with classes that control devices off of the MiniMaestro, e.g., TalonSR.
 * View the examples to see how to use this class properly.
 *
 * How to use:
 * 1. Create a serial object (hardware or software) that is connected to the Maestro's RX/TX pins.
 * 2. Now construct the MiniMaestroService with the serial object.
 * 3. Que the values to the Maestro with the function queTarget().
 * 4. Finally, call for service() in the Arduino's loop().
 *
 * \warning Please be careful when configuring the Maestro. Some settings can damage the Maestro and devices controlled by it!
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
