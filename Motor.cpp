#include <Motor.h>

/** \brief This is the Identification byte to identify the motors.
 * \param id - The motor's ID.
 */
Motor::Motor(const unsigned char id)
{
	this->id = id;
}

/** \brief This sets the output of the motor.
 *
 * -1 drives the motor at full reverse. 1 drives the motor full forward. 0 stops the motor.
 * \param power - a float value between -1 and 1.
 */
void Motor::setPower(const float power)
{
	this->power = constrain(power, -1, 1);
}

/** \brief This returns the target output of the motor.
 * \return Returns the power of motor, a value between -1 and 1.
 */
float Motor::getPower()
{
	return this->power;
}
