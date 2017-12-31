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

PololuG2::PololuG2(const MiniMaestroService &miniMaestroService, const byte enable_channel, const byte pwm_channel, const byte dir_channel) : Motor(MOTOR_ID_POLOLUG2)
{
	this->enable_pin = enable_channel;
	this->pwm_pin = pwm_channel;
	this->dir_pin = dir_channel;
	this->miniMaestroService = &miniMaestroService;

	maestroService = true;
}

// TODO: Implement a way to detect if PololuG2 motor controller is powered before sending signals.
void PololuG2::setPower(const float power)
{

	Motor::setPower(power);
	if(maestroService)
	{
		if(this->getPower() < 0)
		{
			miniMaestroService->queTarget(dir_pin, 0);
		} else {
			miniMaestroService->queTarget(dir_pin, 8000);
		}

		// abs() don't work on floats
		if(getPower() < 0)
		{
			this->maestroOutput = uint16_t(-getPower() * 12000);
		} else {
			this->maestroOutput = uint16_t(getPower() * 12000);
		}
		miniMaestroService->queTarget(this->pwm_pin, maestroOutput);
		return; // TODO: Find a cleaner way to seperate these two protocals (maestro vs pins).
	}

	if(this->getPower() < 0)
	{
		digitalWrite(dir_pin, LOW);
	} else {
		digitalWrite(dir_pin, HIGH);
	}
	this->output = byte(abs(this->getPower()) * 255);
	analogWrite(this->pwm_pin, this->output);
}
