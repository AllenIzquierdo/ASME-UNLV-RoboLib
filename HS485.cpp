#include <HS485.h>


/** \brief Constructs HS485 object.
 * \param miniMaestro MaestroService used to control the servo.
 * \param pwm_channel Maestro channel used to control the servo, starting from zero.
 */
HS485::HS485(const MiniMaestroService &miniMaestro, const unsigned char pwm_channel)
{
	this->pwm_channel = pwm_channel;
	this->miniMaestro = &miniMaestro;
}

/** \brief Returns the channel used to construct the HS485 object.
 * \return HS485 channel.
unsigned char HS485::getChannel()
{
	return this->pwm_channel;
}

/** \brief Use to set the position of the servo
 *
 * \warning May behave differently depending on how HS485 motor is configured.
 * \param position Float value between 0 and 180 degrees.
 */
void HS485::setPosition(const float position)
{
	this->position = constrain(position, 0, 180);
	this->output = uint16_t((this->position * 7488) / 180) + 2212; 
	this->miniMaestro->queTarget(this->pwm_channel, this->output);
}
