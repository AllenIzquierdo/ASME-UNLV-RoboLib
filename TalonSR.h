#ifndef TALONSR_H
#define TALONSR_H
#include <Motor.h>
#include <Arduino.h>
#include <PololuMaestro.h>

#define MOTOR_ID_TALONSR 0
class TalonSR: public Motor {
	public:
		TalonSR(const MiniMaestro miniMaestro, const unsigned char pwm_channel);

		// Setters and Getters
		unsigned char getChannel();
		void setPower(const float power);
	
	protected:

	private:
		unsigned int output;
		unsigned char pwm_channel;
		MiniMaestro* miniMaestro;
};

#endif
