#ifndef TALONSR_H
#define TALONSR_H
#include <Motor.h>
#include <Arduino.h>
#include <PololuMaestro.h>
#include <MiniMaestroService.h>
#define MOTOR_ID_TALONSR 0
/** \brief Controls TalonSR motor controller.
 *
 * This speed controller is a blast from the past.
 */
class TalonSR: public Motor {
	public:
		TalonSR(const MiniMaestroService &miniMaestro, const unsigned char pwm_channel);

		// Setters and Getters
		unsigned char getChannel();
		void setPower(const float power);
	
	protected:

	private:
		unsigned int output;
		unsigned char pwm_channel;
		MiniMaestroService* miniMaestro;
};

#endif
