#include <HS485.h>


/** \brief This constructs the HS485 object.
 * \param  - The miniMaestro MaestroService used to control the servo.
 * \param  - The pwm_channel Maestro channel used to control the servo, starting from 0, and ending with 17.
 */
HS485::HS485(const MiniMaestroService &miniMaestro, const unsigned char pwm_channel)
{
	this->pwm_channel = pwm_channel;
	this->miniMaestro = &miniMaestro;
}

/** \brief This returns the channel used in constructing the HS485 object.
 * \return This returns the HS485's pwm channel.
 */
unsigned char HS485::getChannel()
{
	return this->pwm_channel;
}

/** \brief Use this to set the position of the servo.
 *
 * \warning This may behave differently depending on how the HS485 motor is configured!
 * \param position - This is a float value between 0 and 180 degrees.
 */
void HS485::setPosition(const float position)
{
	this->position = constrain(position, 0, 180);
	this->output = uint16_t((this->position * 7488) / 180) + 2212;
	this->miniMaestro->queTarget(this->pwm_channel, this->output);
}
