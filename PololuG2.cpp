// TODO: Swap Arduino's slow function with registers manipulations
#include <PololuG2.h>
#define MOTOR_ID_POLOLUG2 1

float PololuG2::last_update;
unsigned char PololuG2::motor_count;
PololuG2* PololuG2::motors[10];


/** \brief Control PololuG2 motor with Arduino pins.
 *
 * \param enable_pin Digital pin that connects to PololuG2's sleep pin.
 * \param pwm_pin Digital pwm pin that connects to PololuG2's PWM pin.
 * \param dir_pin Digital pin that connects to PololuG2's DIR pin.
 * \warning PWM pins may change between arduino compatible devices. For example, a pwm_pin on an Arduino UNO may not be a pwm_pin on Arduino Leonardo.
 */
PololuG2::PololuG2(const byte enable_pin, const byte pwm_pin, const byte dir_pin, const bool linearRamping = false) : Motor(MOTOR_ID_POLOLUG2)
{
	this->enable_pin = enable_pin;
	this->pwm_pin = pwm_pin;
	this->dir_pin = dir_pin;
	pinMode(enable_pin, OUTPUT);
	pinMode(pwm_pin, OUTPUT);
	pinMode(dir_pin, OUTPUT);

	this->linearRamping = linearRamping;
	if(linearRamping)
	{
		addLinearRampingMotor(this);
	}
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
PololuG2::PololuG2(const MiniMaestroService &miniMaestroService, const byte enable_channel, const byte pwm_channel, const byte dir_channel, const bool linearRamping = false) : Motor(MOTOR_ID_POLOLUG2)
{
	this->enable_pin = enable_channel;
	this->pwm_pin = pwm_channel;
	this->dir_pin = dir_channel;
	this->miniMaestroService = &miniMaestroService;

	maestroService = true;
	this->linearRamping = linearRamping;
	if(linearRamping)
	{
		addLinearRampingMotor(this);
	}
}

// TODO: Implement a way to detect if PololuG2 motor controller is powered before sending signals.
void PololuG2::setPower(const float power)
{
	// TODO: Find a cleaner way to seperate these two protocals (maestro vs digital pins).
	// Linear Ramping
	if(linearRamping)
	{
		target = power;
		return;
	} else {
		setOutputPower(power);
	}

	// MiniMaestroService
}


void PololuG2::addLinearRampingMotor(PololuG2* motor)
{
	motors[motor_count] = motor;
	motor_count++;
}

void PololuG2::iterate()
{
	unsigned long time = millis();
	float differential = float(time - last_update) / 1000;
	last_update = time;

	for(unsigned char i = 0; i < motor_count; i++)
	{
		float target = motors[i]->getTarget();
		float power = motors[i]->getPower();
		float velocity = motors[i]->getVelocity();
		float displacement = target - power;
		float projection = velocity * differential;
		if(fabs(displacement) < velocity*differential)
		{
			motors[i]->setOutputPower(target);
		} else {
			if(displacement > 0)
			{
				motors[i]->setOutputPower(power + projection);
			} else {
				motors[i]->setOutputPower(power - projection);
			}
		}
	}
}

void PololuG2::setOutputPower(const float power)
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

		this->maestroOutput = uint16_t(fabs(this->getPower())*16000);
		miniMaestroService->queTarget(this->pwm_pin, maestroOutput);
		return;
	}

	digitalWrite(dir_pin, LOW);
	this->output = byte(fabs(getPower()) * 255);

	analogWrite(this->pwm_pin, this->output);
}

float PololuG2::getTarget()
{
	return this->target;
}

/** \brief This sets the linear ramping velocity.
 * \param velocity   - The units are changed in power per second.
 */
void PololuG2::setVelocity(const float velocity)
{
	this->velocity = velocity;
}

float PololuG2::getVelocity()
{
	return this->velocity;
}
