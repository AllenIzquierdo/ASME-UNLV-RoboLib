// TODO: Swap Arduino's slow function with registers manipulations
#include <PololuG2.h>
#define MOTOR_ID_POLOLUG2 1
PololuG2::PololuG2(const byte enable_pin, const byte pwm_pin, const byte dir_pin) : Motor(MOTOR_ID_POLOLUG2)
{
	this->enable_pin = enable_pin;
	this->pwm_pin = pwm_pin;
	this->dir_pin = dir_pin;
	pinMode(enable_pin, OUTPUT);
	pinMode(pwm_pin, OUTPUT);
	pinMode(dir_pin, OUTPUT);
}

// TODO: Implement a way to detect if PololuG2 motor controller is powered before sending signals.
void PololuG2::setPower(const float power)
{

	Motor::setPower(power);
	if(this->getPower() < 0)
	{
		digitalWrite(dir_pin, LOW);
	} else {
		digitalWrite(dir_pin, HIGH);
	}
	this->output = byte(abs(this->getPower()) * 255);
	analogWrite(this->pwm_pin, this->output);
}
