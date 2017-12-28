#include <Motor.h>

Motor::Motor(const unsigned char id)
{
	this->id = id;
}

void Motor::setPower(const float power)
{
	this->power = constrain(power, -1, 1);
}

float Motor::getPower()
{
	return this->power;
}
