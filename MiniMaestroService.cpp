#include <MiniMaestroService.h>
MiniMaestroService::MiniMaestroService(Stream &stream, uint8_t resetPin , uint8_t deviceNumber, bool CRCEnabled) : MiniMaestro(stream, resetPin, deviceNumber, CRCEnabled)
{
	
}

/** \brief Sets update period to update Maestro.
 *
 * Maestro is updated in time intervals, default 20Hz.
 * \param period Sets minimum time elasped before updating maestro output.
 */
void MiniMaestroService::setUpdatePeriod(const uint16_t period)
{
	this->period = period;
}

/** \brief Sets target of Maestro's channel.
 *
 * Channels are configured through Maestro Controll Center Software. If channel is specified as a Output, values above 6000 will drive the signal high. Values below 6000 will drive the signal low.
 * \param channel Selects Maestro's channel.
 * \param value sets target of Maestro's channel, in quarter miliseconds.
 */
void MiniMaestroService::queTarget(const uint8_t channel, const uint16_t value)
{
	if(highestOccupiedChannel < channel)
	{
		highestOccupiedChannel = channel;
	}
	values[channel] = value;
}

/** \brief Starts sending values to maestro.
 * \warning This can be time consuming!
 */
void MiniMaestroService::sendValues()
{
	setMultiTarget(highestOccupiedChannel + 1, 0, values);
}

/** \brief Sends values to maestro periodically.
 *
 * This function is meant to be called in arduino's loop() function.
 * It will update Maestro periodically depending on period set by setUpdatePeriod(). The default period is 50ms.
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
