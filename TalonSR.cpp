#include <TalonSR.h>

/** \brief Controlls the TalonSR using a MiniMaestroService.
 * \param miniMaestro MiniMaestroService that used to controll the TalonSR.
 * \param pwm_channel MiniMaestro channel that's connected to TalonSR pwm pin.
 * \warning Check if Servo Voltage on the MiniMaestro is 5v. If it isn't, only connect the output pin to the TalonSR pwm pin. Give TalonSR 5v from some other source.
 * \warning Configure the MiniMaestro channel with Maestro Control Center. Min Pulse is 1 millisecond. Max Pulse is 2 millisecond. Neutral pulse is 1.5 milliseconds.
 */
TalonSR::TalonSR(const MiniMaestroService &miniMaestro, const unsigned char pwm_channel) : Motor(MOTOR_ID_TALONSR)
{
	this->pwm_channel = pwm_channel;
	this->miniMaestro = &miniMaestro;
}

/** \brief Get's assigned channel that TalonSR was constructed with.
 * \return MiniMaestro Channel in use.
 */
unsigned char TalonSR::getChannel()
{
	return this->pwm_channel;
}

void TalonSR::setPower(const float power)
{
	Motor::setPower(power);
	this->output = uint16_t((getPower() + 1) * 4000 )/ 2 + 4000;
	this->miniMaestro->queTarget(this->pwm_channel, this->output);
}
