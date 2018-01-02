// TODO: Swap Arduino's slow function with registers manipulations
#include <PololuG2.h>
#define MOTOR_ID_POLOLUG2 1

/** \brief Controll PololuG2 motor with Arduino pins.
 *
 * \param enable_pin Digital pin that connects to PololuG2's sleep pin.
 * \param pwm_pin Digital pwm pin that connects to PololuG2's PWM pin.
 * \param dir_pin Digital pin that connects to PololuG2's DIR pin.
 * \warning PWM pins may change between arduino compatible devices. For example, a pwm_pin on an Arduino UNO may not be a pwm_pin on Arduino Leonardo.
 */
PololuG2::PololuG2(const byte enable_pin, const byte pwm_pin, const byte dir_pin) : Motor(MOTOR_ID_POLOLUG2)
{
	this->enable_pin = enable_pin;
	this->pwm_pin = pwm_pin;
	this->dir_pin = dir_pin;
	pinMode(enable_pin, OUTPUT);
	pinMode(pwm_pin, OUTPUT);
	pinMode(dir_pin, OUTPUT);
}

/** \brief Controll PololuG2 motor with MiniMaestroService.
 *
 * This controll method will take up three channels on Maestro.
 * \warning Low MiniMaestroService update periods can make it seem like the speed controller is laggy.
 * \param miniMaestroService MaestroService used to control the device.
 * \param enable_channel Output channel that connects to PololuG2's sleep pin.
 * \param pwm_channel Servo channel that connects to PololuG2's PWM pin.
 * \param dir_channel Output channel that connects to PololuG2's DIR pin.
 * \warning Configure the Output and Servo channels appropiately using Maestro Control Center Software.
 */
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
	// TODO: Find a cleaner way to seperate these two protocals (maestro vs digital pins).
	Motor::setPower(power);
	if(maestroService)
	{
		if(this->getPower() < 0)
		{
			miniMaestroService->queTarget(dir_pin, 0);
		} else {
			miniMaestroService->queTarget(dir_pin, 8000);
		}

		this->maestroOutput = uint16_t(abs(getPower())*16000);
		miniMaestroService->queTarget(this->pwm_pin, maestroOutput);
		return; 
	}

	if(this->getPower() < 0)
	{
		digitalWrite(dir_pin, LOW);
		this->output = byte(-getPower()*255);
	} else {
		digitalWrite(dir_pin, HIGH);
		this->output = byte(getPower()*255);
	}

	analogWrite(this->pwm_pin, this->output);
}
