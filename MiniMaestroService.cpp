#include <MiniMaestroService.h>
MiniMaestroService::MiniMaestroService(Stream &stream, uint8_t resetPin , uint8_t deviceNumber, bool CRCEnabled) : MiniMaestro(stream, resetPin, deviceNumber, CRCEnabled)
{

}

/** \brief This sets the timer for the Maestro update.
 *
 * Maestro is updated in time intervals, the default is set to 20Hz.
 * \param period  - Sets minimum time elapsed before updating maestro output.
 */
void MiniMaestroService::setUpdatePeriod(const uint16_t period)
{
	this->period = period;
}

/** \brief Sets the target of the Maestro's channel.
 *
 * The channels are configured through the Maestro Control Center Software. If the channel is specified as an Output, values above 6000
 * will drive the signal high. Values below 6000 will drive the signal low.
 * \param channel - Selects the Maestro's channel.
 * \param value   - Sets the target of the Maestro's channel, in quarter milliseconds.
 */
void MiniMaestroService::queTarget(const uint8_t channel, const uint16_t value)
{
	if(highestOccupiedChannel < channel)
	{
		highestOccupiedChannel = channel;
	}
	values[channel] = value;
}

/** \brief This starts sending values to the Maestro.
 * \warning This can be time consuming!
 */
void MiniMaestroService::sendValues()
{
	setMultiTarget(highestOccupiedChannel + 1, 0, values);
}

/** \brief This sends values to the Maestro periodically.
 *
 * This function is meant to be called in the Arduino's loop() function.
 * It will update the Maestro periodically, depending on period set by setUpdatePeriod(). The default period is set at 50ms.
 */
void MiniMaestroService::service()
{
	uint16_t time_elasped = millis() - last_update;
	if(time_elasped > period)
	{
		sendValues();
		last_update = millis();
	}
}
