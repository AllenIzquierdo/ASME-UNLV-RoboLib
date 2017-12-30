#include <HS485.h>

HS485::HS485(const MiniMaestroService &miniMaestro, const unsigned char pwm_channel)
{
	this->pwm_channel = pwm_channel;
	this->miniMaestro = &miniMaestro;
}

unsigned char HS485::getChannel()
{
	return this->pwm_channel;
}

void HS485::setPosition(const float position)
{
	this->position = constrain(position, 0, 180);
	this->output = uint16_t((this->position * 7488) / 180) + 2212; 
	this->miniMaestro->queTarget(this->pwm_channel, this->output);
}
