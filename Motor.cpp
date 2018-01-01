#include <Motor.h>

/** \brief Identification byte to identify the motor.
 * \param id Motor's ID.
 */
Motor::Motor(const unsigned char id)
{
	this->id = id;
}

/** \brief Sets output of motor.
 * 
 * -1 drives the motor full reverse. 1 drives the motor full forward. 0 stops the motor.
 * \param power Float between -1 and 1.
 */
void Motor::setPower(const float power)
{
	this->power = constrain(power, -1, 1);
}

/** \brief Returns target output of motor.
 * \return Power of motor, between -1 and 1.
 */
float Motor::getPower()
{
	return this->power;
}
