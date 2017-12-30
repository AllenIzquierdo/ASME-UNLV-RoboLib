#include <MiniMaestroService.h>
MiniMaestroService::MiniMaestroService(Stream &stream, uint8_t resetPin , uint8_t deviceNumber, bool CRCEnabled) : MiniMaestro(stream, resetPin, deviceNumber, CRCEnabled)
{
	
}

void MiniMaestroService::setUpdatePeriod(const uint16_t period)
{
	this->period = period;
}

void MiniMaestroService::queTarget(const uint8_t channel, const uint16_t value)
{
	if(highestOccupiedChannel < channel)
	{
		highestOccupiedChannel = channel;
	}
	values[channel] = value;
}

void MiniMaestroService::sendValues()
{
	setMultiTarget(highestOccupiedChannel + 1, 0, values);
}

void MiniMaestroService::service()
{
	uint16_t time_elasped = millis() - last_update;
	if(time_elasped > period)
	{
		sendValues();
		last_update = millis();
	}
}
