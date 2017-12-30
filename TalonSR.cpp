#include <TalonSR.h>

TalonSR::TalonSR(const MiniMaestroService &miniMaestro, const unsigned char pwm_channel) : Motor(MOTOR_ID_TALONSR)
{
	this->pwm_channel = pwm_channel;
	this->miniMaestro = &miniMaestro;
}

unsigned char TalonSR::getChannel()
{
	return this->pwm_channel;
}

void TalonSR::setPower(const float power)
{
	Motor::setPower(power);
	this->output = uint16_t((getPower() + 1) * 2000 )/ 2 + 6000;
	this->miniMaestro->setTarget(this->pwm_channel, this->output);
}


